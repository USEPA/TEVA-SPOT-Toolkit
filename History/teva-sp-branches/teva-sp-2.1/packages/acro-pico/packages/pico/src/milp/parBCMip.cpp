/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

//
// parBCMip.cpp
//
// Code for a parallel branch and cut MIP for PICO
//

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <pico/parBCMip.h>

using namespace std;

namespace pico {

int parBCMip::spPackSize()
{
int size_guess = parMILP::spPackSize();
// We'll send the full solution if the problem is bounded
 size_guess += MIP->numAllVars() * sizeof(double);
 // Otherwise, we'll just assume no cuts for now and expect the buffers to
 // resize dynamically as required.
return size_guess;
}

void parBCMip::preprocess()
{
BCMip::preprocess();

  // If an application-specific cut type created the makeNewCut array, it'll
  // have set elements at or later than cutFinder::FIRST_APPLICATION_SPECIFIC
if (makeNewCut==NULL)
  makeNewCut = new BasicArray<cutConstructor>(cutFinder::FIRST_APPLICATION_SPECIFIC);
  // All the built-in cut types are just regular picoRowCuts
for (int i=0; i < cutFinder::FIRST_APPLICATION_SPECIFIC; i++)
  (*makeNewCut)[i] = (cutConstructor) &picoRowCut::makeNew;

parMILP::preprocess();
}

// This is used by parBCMipNode instances for packing binding cuts,
// and by the cut server thread.  num_to_pack == -1 means to use the
// length of the array.
// This starts by packing the number of cuts.  It makes unpacking more
// efficient.  We may eventually want a version that just packs cuts and
// terminates with a negative number (e.g. another argument to this method)

void parBCMip::packCuts(BasicArray<picoRowCut *> &to_pack, 
			PackBuffer &outBuffer,
			int num_to_pack)
{
if (num_to_pack < 0)
  num_to_pack = to_pack.size();
#ifdef ACRO_VALIDATING
else if (num_to_pack > (int) to_pack.size())
  EXCEPTION_MNGR(runtime_error, "packCuts: request to pack " << num_to_pack << " cuts from an array of length " << to_pack.size());
#endif
outBuffer << num_to_pack;
for (int i=0; i < num_to_pack; i++)
  {
  outBuffer << to_pack[i]->type();
  to_pack[i]->pack(outBuffer);
  }
}

int parBCMip::unPackCuts(BasicArray<picoRowCut *> &unpack_array, UnPackBuffer &inBuffer)
{
  size_type num_cuts;
int cut_type;
inBuffer >> num_cuts;
if (num_cuts > unpack_array.size())
  unpack_array.resize(num_cuts);
for (size_type i = 0; i < num_cuts; i++)
  {
  inBuffer >> cut_type;
  // Call a "constructor" to get a new (blank) cut of this type
  //  EXCEPTION_MNGR(runtime_error, "WEH - I commented out the next line.  This contains an implicit coercion, I think.");
  unpack_array[i] = (picoRowCut *)((*makeNewCut)[cut_type])(BCMIP);
  unpack_array[i]->unpack(inBuffer, cut_type);
  }
return((int)num_cuts);
}

void parBCMipNode::pack(PackBuffer& outBuffer)
{
parMILPNode::pack(outBuffer);
if (state != boundable)
  outBuffer << full_solution;
pbcGlobal()->packCuts(persistent_cuts, outBuffer);
outBuffer << persistent_basic_slacks;
pbcGlobal()->packCuts(binding , outBuffer);
outBuffer << basic_slack_cuts;
if (state == beingBounded)
  {
  outBuffer << finderHistory;
  outBuffer << NumLPSolves;
  // Have to pass a way to locate the last cut finder used.  We cannot
  // assume the cut finder types will be the same on all processors (depends
  // on static initalization ordering), so pass the type string, which is
  // assumed to be unique.  TODO: is there a better way?  
  outBuffer << lastFinder->typeString();
  outBuffer << branchQuality;
  outBuffer << branchReadiness;
  }
}

void parBCMipNode::packChild(PackBuffer &outBuffer, int whichChild)
{
parMILPNode::packChild(outBuffer, whichChild);
pbcGlobal()->packCuts(persistent_cuts, outBuffer);
outBuffer << persistent_basic_slacks;
pbcGlobal()->packCuts(binding , outBuffer);
outBuffer << basic_slack_cuts;
}

void parBCMipNode::unpack(UnPackBuffer &inBuffer)
{
  size_type num_cuts;
parMILPNode::unpack(inBuffer);
if (state != boundable)
  inBuffer >> full_solution;
pbcGlobal()->unPackCuts(persistent_cuts, inBuffer);
inBuffer >> persistent_basic_slacks;
num_cuts = persistent_cuts.size();
for (size_type i=0; i < num_cuts; i++)
  {
  persistent_cuts[i]->makePersistent();
  persistent_cuts[i]->incrementRefCount();
  }
pbcGlobal()->unPackCuts(binding, inBuffer);
inBuffer >> basic_slack_cuts;
num_cuts = binding.size();
for (size_type i=0; i < num_cuts; i++)
  {
  binding[i]->incrementRefCount();
  }
if (state == beingBounded)
  {
  inBuffer >> finderHistory;
  inBuffer >> NumLPSolves;
  // TODO deal with string
  inBuffer >> branchQuality;
  inBuffer >> branchReadiness;
  }
}

PicoLPInterface::problemState parBCMipNode::boundComputationGuts()
{
  if (!rampingUp() || !bcGlobal()->using_cuts)
    return(BCMipNode::boundComputationGuts());

  // Resolve from the branching that created this node (or solve the root).
  // Since we're in ramp up, processor 0 will do this resolve and
  // send out relevant information.  We won't keep track of time since it's
  // not an accurate time estimate (we'll get some of that from pseudocost inits).
  PicoLPInterface::problemState this_state = boundingSolve();


  // In ramp up, parallelize the cut generation and make sure processors
  // stay synchronized (e.g. by allowing processor 0 to do all resolves)

  pbcGlobal()->rampUpCutting(this);

  // TODO finish this as necessary.

  return(this_state);
}

void parBCMip::rampUpCutting(parBCMipNode *curr_problem)
{
  size_type numFinders = cutGenerators.size();
  size_type numEligibleFinders = 0;
  size_type firstFinder, lastFinder, numFindersToRun, maxJobs, numWithMax, i;
  int pid = uMPI::rank;
  int numProcs = uMPI::size;
  size_type numProcs_st = (size_type) numProcs;
  int my_size = 0; // used in communicating cuts.  Initialized to 0 because of processor 0
  int total_size;
  IntVector sendSizes(pid==0 ? numProcs:0); // Only used by processor 0
  IntVector displacements(pid==0 ? numProcs:0); // Only used by processor 0
  bool problemInfeasible;
  PackBuffer cutInformation; // For processors that will be sending cuts
  UnPackBuffer sentCutInformation; // For processor 0
  BasicArray<cutFinder *> eligibleFinders(numFinders);
  getEligibleFinders(eligibleFinders, curr_problem, numEligibleFinders);
  divideWorkLexicographically(numEligibleFinders, firstFinder,
			      numFindersToRun, maxJobs, numWithMax);
  lastFinder = firstFinder + numFindersToRun;
  for (i = firstFinder; i < lastFinder; i++)
    {
    runFinder(eligibleFinders[i], curr_problem, problemInfeasible);
    if (problemInfeasible)
      break;
    }
  if (pid != 0)
    {
      packCutQueue(cutInformation);
      my_size = (int) cutInformation.size();
    }
  
    // Processor 0 gets the size it must receive from each processor
  MPI_Gather(&my_size, 1, MPI_INT, sendSizes.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
  if (pid == 0)
    {
      // Counted at receiving end
      rampUpMessages += numProcs -1;

      // Allocate enough space to receive the cut information from all processors
    total_size = 0;
    displacements[0] = 0;
      // Processor 0 won't send anything to itself, so start with 1
    for (i=1; i < numProcs_st; i++)
      {
      displacements[i] = total_size;
      total_size += sendSizes[i];
      }
    sentCutInformation.resize(total_size);
    }
  // Send packed cut information to processor 0
  MPI_Gatherv((void *)cutInformation.buf(), my_size, MPI_PACKED, (void *)sentCutInformation.buf(),
	      sendSizes.data(), displacements.data(), MPI_PACKED, 0, MPI_COMM_WORLD);
  if (pid == 0)
    {
    rampUpMessages += numProcs -1;
    unpackCutQueue(sentCutInformation);
    // Find out if any finder determined infeasibility
    if (!problemInfeasible)
      for (i=0; i < (size_type) num_queue_objects; i++)
	{
	  if (cutQueue[i].getInfeasibility())
	    {
	    problemInfeasible = true;
	    break;
	    }
	}
    if (problemInfeasible)
      {
	cleanUpAfterInfeasibility(curr_problem->bound, curr_problem->newSubproblem());
      }
    else
      {
      }
    }
  else  // pid != 0
    {
    dropCutQueue(); // any cuts that survive will be sent back
    }
  // TODO: finish
}

// Pack the whole cut queue.  It might be nice to have the cutQObjects
// pack themselves, but the packCuts method is only available in the
// parallel code, so it would be incomplete anyway.

void parBCMip::packCutQueue(PackBuffer &outBuf)
{
outBuf << num_queue_objects;
cutQObject *this_info;
 size_type num_queue_objects_st = (size_type) num_queue_objects;
for (size_type i=0; i < num_queue_objects_st; i++)
  {
    // This could become a method to pack a single cutQObject if necessary
   this_info = &(cutQueue[i]);
   packCuts(this_info->getCutContainer(), outBuf, this_info->num_cuts);
   outBuf << this_info->getTime() << this_info->getFinderUsed()->type()
	  << this_info->getFinderReadiness() << this_info->getInfeasibility();
  }

}
// Unpack a set of cutQObjects.  Append to whatever we currently have. 

void parBCMip::unpackCutQueue(UnPackBuffer &inBuf)
{
int num_to_unpack;
 int this_num_cuts;
 double packedTime, packedReadiness;
 int packedFinderType;
 bool packedInfeasibility; // TODO make sure bools pack and unpack properly
 
inBuf >> num_to_unpack;
 if (cutQueue.size() < (size_type)(num_queue_objects + num_to_unpack))
   cutQueue.resize(num_queue_objects + num_to_unpack);

cutQObject *this_info;
for (int i=0; i < num_to_unpack; i++)
  {
    // This could become a method to unpack a single cutQObject if necessary
   this_info = &(cutQueue[num_queue_objects++]);
   this_num_cuts = unPackCuts(this_info->getCutContainer(), inBuf);
   queue_total += this_num_cuts;
   this_info->num_cuts = this_num_cuts;
   inBuf >> packedTime >> packedFinderType >> packedReadiness >> packedInfeasibility;
   this_info->setCutQObject(packedTime, cutGenerators[packedFinderType], packedReadiness, packedInfeasibility);
  }
}

// Throw away all cuts and clean up.  Used in parallel ramp up for processors other than 0.

void parBCMip::dropCutQueue()
{
  size_type j, num_to_drop;
  cutQObject *this_queue_object;
  for (int i= num_queue_objects; i >= 0; i--)
    {
    this_queue_object = &cutQueue[i];
    // TODO:  cuts from the pool should not be deleted.  They
    // should be put back in the pool.
    num_to_drop = this_queue_object->num_cuts;
    for (j = 0; j < num_to_drop; j++)
      {
	// Assume there are no cut reference issues
	this_queue_object->removeCut(j);
      }
 // Does some checks in debugging mode.  This is also why we're going in reverse order
    removeCutQObject(i);
    }
  resetCutQueue();
}


void parBCMip::getEligibleFinders(BasicArray<cutFinder *> eligibleFinders, parBCMipNode *curr_problem,
				  size_type &numEligibleFinders)
{
  cutFinder *curr_finder;
  size_type i;
  size_type numFinders = cutGenerators.size();
  if (shouldRunAllFinders())
    {
    // gather all that would be run by BCMip::runAllFinders().
    // Currently, that's all with nonzero readiness
    for (i=0; i < numFinders; i++)
      {
      curr_finder = cutGenerators[i];
      // TODO: check if this can happen (after new system)
      if (curr_finder == NULL) continue;
      if (!isZero(curr_finder->readiness(curr_problem)))
	eligibleFinders[numEligibleFinders++] = curr_finder;
      }
    return;
    }
  // Gather all that would compete in the competitive scheduling phase of serial
  // branch and cut.
  for (i=0; i < numFinders; i++)
    {
      if (cutGenerators[i] == NULL) continue;
      if (finderCompetitive(i, curr_problem))
	eligibleFinders[numEligibleFinders++] = curr_finder;
    }
}

} // namespace pico

#endif
