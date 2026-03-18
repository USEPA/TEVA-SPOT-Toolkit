/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// parMilp.cpp
//
// Code for a parallel MIP for PICO.
//
// Cindy Phillips
//


#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <utilib/sort.h> 
#include <utilib/nicePrint.h>
#include <pico/MILPProblem.h>
#include <pico/parMilp.h>
#include <pebbl/gRandom.h>

#if defined(ACRO_USING_AMPL )
#undef NO
extern "C" {
#include "getstub.h"
#include "asl.h"
};
extern ASL* asl;
#endif

#if defined(COUGAR)
extern "C" void heap_summary();
extern "C" void print_links();
#endif

using namespace pebbl;
using namespace utilib;
using namespace std;

namespace pico {

// Pack a description of the base MIP problem.  Assuming this is from an
// initialized instance of MILP, and therefore the LP is loaded.

void parMILP::pack(PackBuffer& outBuffer)
{
  // Info to initialize the LP solver
  lp->write(outBuffer);
  int tint;
  lp->get_parameter("LPdebug",tint);
  if (tint > 0)
     lp->write(ucout,verbose_format);
  // Info to initialize the MIP info
  MIP->packMIP(outBuffer);
}


// When an instance is started from a packed representation of the problem.
// Nothing has been initialized.

void parMILP::unpack(UnPackBuffer& inBuffer)
{

  // JE -- this code was hanging after I unified the start-up procedure
  // I think the first token wasn't being created (I'm not sure how it
  // worked before.  This step is *only* necessary for LP solvers that
  // use a license manager, and perhaps not even in all cases of that.
  // It was being used all the time, even for free LP solvers.  
  // I am disabling it for now; revisit!

//   // LP stuff is first
//   // For parallel versions, wait for your turn to open up the LP environment
//   // Get OK from immediate predecessor, open LP, then send the token on
// #ifndef COUGAR
//   int message;
//   MPI_Status status;
//   int pid = uMPI::rank;
//   if (pid != 0)
//     {
//       // blocking receive
//       uMPI::recv(&message, 1, MPI_INT, pid - 1, 0, &status);
//       DEBUGPR(2, ucout << "unpack: rec'd from " << message << "\n");
//     }
// #endif
//   // Keep this around so we are not constantly opening and closing the LP
//   // solver (e.g. in repeated solutions or qa suite (eventually)

// #ifndef COUGAR
//   DEBUGPR(2, ucout << "size - 1 = " << uMPI::size - 1 << "\n");
//   if (pid < uMPI::size -1)
//     uMPI::send(&pid, 1, MPI_INT, pid + 1, 0);
// #endif

  if (!MIP) {
    EXCEPTION_MNGR(runtime_error, 
		   "MILPProblem must be set before calling unpack");
  }
  MIP->set_parameters(plist,false);
  MIP->lp.set_parameters(plist,false);
  MIP->lp.init();
  lp=MIP->lp();
  MIP->lp()->set_parameters(plist,false);

  MIP->loadProblem(inBuffer);
  MILPInit();

  if (uMPI::size == 1)
     MIP->setIORank(0);
  else
     MIP->setIORank(1);
}


// Pack and unpack subproblems.

  // reducedCosts are used only locally within lockVariables, so don't need
  // to be packed.  If ever used more widely, this could change.
// For now, pack everything.  If we eventually support mulitiple sizes of
// packed subproblems (based on state), then the ones not always packed are
// packed last currently.

void parMILPNode::pack(PackBuffer& outBuffer)
{
  int i;
  // The next 3 are not explicitly sent since they should be available at the
  // unpacking end.  Used here for packing.
  int numInts = mip()->numIntegerVars();
  int numGeneral = mip()->numGeneralIntVars();
  int setCount = mip()->numSets();
  DEBUGPR(70, ucout << "packing subproblem " << id << "\n");
  outBuffer << basis;
  // Make sure this works properly.  I don't want to use a byte for every bit.
  outBuffer << fixed;
  outBuffer << binary_vals;
  for (i = 0; i < numGeneral; i++)
    {
      outBuffer << intLowerBounds[i];
      outBuffer << intUpperBounds[i];
    }
  if (pmGlobal()->useSets && setCount > 0)
    {
    for (i = 0; i < setCount; i++)
      {
	outBuffer << spSetLowerBounds[i];
	outBuffer << spSetUpperBounds[i];
      }
    if (state == boundable || state == beingBounded)
      {
      outBuffer << parentSetBound;
      }
    }
  outBuffer << branchVariable;
  outBuffer << branchValue;
  outBuffer << (int) branchType;
  outBuffer << lpBound;
  outBuffer << recordPCost;
  outBuffer << parentId;

  // Output solution information only if valid
  if (state != boundable && state != beingBounded) // presumably won't try to pack a dead problem
    {
      for (i = 0; i < numInts; i++)
	outBuffer << solution[i];
      outBuffer << nFrac;
      for (i = 0; i < nFrac; i++)
	outBuffer << FracVarNdx[i];
      if (pmGlobal()->useSets && setCount > 0)
	{
	  outBuffer << nSetCandidates;
	  for (i=0; i < nSetCandidates; i++)
	    outBuffer << setCandidateNdx[i];
	  for (i=0; i < setCount; i++)
	    outBuffer << spSetBranchValues[i];
	}
    }
}

// This has the same functionality as creating a parallel child and then
// packing it, but shortcuts the procedure by creating directly into a buffer.

void parMILPNode::packChild(PackBuffer& outBuffer, int whichChild)
{
  MILPProblem *localMipRef = mip();
  int i;
  /*
  DEBUGPR(10, ucout << "packing child subproblem, serial =" 
	  << bGlobal()->probCounter+1 << ", parent = " << id << "\n");
  */

  // Create generic information of the child and pack directly
   packChildGeneric(outBuffer);
  // The next 3 are not explicitly sent since they should be available at the
  // unpacking end.  Used here for packing.
  int numGeneral = mip()->numGeneralIntVars();
  int numInts = mip()->numIntegerVars();
  int setCount = mip()->numSets();
  outBuffer << basis;
  branch_type childBranch;
  //removed previous reference to anyChild
  if (whichChild == 0)
    childBranch = branchType;
  else {
    if (branchType == branch_up)  childBranch = branch_down;
    else if (branchType == branch_down) childBranch = branch_up;
    else EXCEPTION_MNGR(runtime_error, 
		    "Trying to create a child for a parent without "
		    "branching indicator");
  }
  DEBUGPR(10, ucout << "packChild: branchvar = " << branchVariable 
	  << ", dir = " << (int) childBranch << "\n");
  // Make sure this works properly.  I don't want to use a byte for every bit.
  BitArray tempBitArray(fixed.size());
  tempBitArray << fixed;
  if (branchVariable >= numInts) // branching in a set
    {
    BitArray tempbinaryVals(fixed.size());
    tempbinaryVals << binary_vals;
      // Branching on a set.  Set variables in the set outside the indicated index range to 0.
    int setNum = branchVariable - numInts;
    int tempParentSetBound;
    IntVector& thisSet = localMipRef->specialSets[setNum];
    size_type k, setStart, setEnd;
    int thisSetVar;
    // Figure out which binary variables will be forced to 0
    if (childBranch == branch_up)
      {
      setStart = (size_type) spSetLowerBounds[setNum];
      setEnd = (size_type) floor(branchValue);
      tempParentSetBound = (int) setStart;
      // Only one variable allowed to receive the 1, so fix it
      if (setEnd+1 == (size_type)spSetUpperBounds[setNum])
	{
	thisSetVar = thisSet[setEnd + 1];
	tempBitArray.set(thisSetVar);
	tempbinaryVals.set(thisSetVar);
	}
      }
    else
      {
      setStart = (size_type) ceil(branchValue);
      setEnd = (size_type) spSetUpperBounds[setNum];
      tempParentSetBound = (int)setEnd;
      // Only one variable allowed to receive the 1, so fix it
      if (setStart-1 == (size_type)spSetLowerBounds[setNum])
	{
	thisSetVar = thisSet[setStart - 1];
	tempBitArray.set(thisSetVar);
	tempbinaryVals.set(thisSetVar);
	}
      }
    for (k= setStart; k <= setEnd; k++)
      {
      thisSetVar = thisSet[k];
      tempBitArray.set(thisSetVar);
      tempbinaryVals.reset(thisSetVar);
      }
    outBuffer << tempBitArray;
    outBuffer << tempbinaryVals;
    for (i = 0; i < numGeneral; i++)
      {
      outBuffer << intLowerBounds[i];
      outBuffer << intUpperBounds[i];
      }
    for (i = 0; i < setCount; i++)
      {
      if (i == setNum)
	{
        if (childBranch == branch_up)
	  {
	  outBuffer << (int) ceil(branchValue);
	  outBuffer << spSetUpperBounds[i];
	  }
	else
	  {
	  outBuffer << spSetLowerBounds[i];
	  outBuffer << (int) floor(branchValue);
	  }
	}
      else
	{
	outBuffer << spSetLowerBounds[i];
	outBuffer << spSetUpperBounds[i];
	}
      }
    outBuffer << tempParentSetBound;
    }
  else if (mip()->vtype[mip()->integerVarNdx[branchVariable]] == binary) {
    tempBitArray.set(branchVariable);
    outBuffer << tempBitArray;
    DEBUGPR(50, ucout << "parent fixed = " << fixed << 
	    "\nchild fixed =" << tempBitArray << "\n");
    tempBitArray << binary_vals; // This is OK because it's a temp variable
   if (childBranch == branch_up)
      tempBitArray.set(branchVariable);
   else
      tempBitArray.reset(branchVariable);

   outBuffer << tempBitArray;
   DEBUGPR(50, ucout << "parent binary_vals = " << binary_vals << 
	   "\nbinary_vals =" << tempBitArray << "\n");
  for (i = 0; i < numGeneral; i++)
    {
      outBuffer << intLowerBounds[i];
      outBuffer << intUpperBounds[i];
    }
  if (pmGlobal()->useSets && setCount > 0)
    {
    for (i = 0; i < setCount; i++)
      {
	outBuffer << spSetLowerBounds[i];
	outBuffer << spSetUpperBounds[i];
      }
    outBuffer << parentSetBound; // state of child is always boundable
    }
  }
  else {  // general integer variable
    int index = mip()->reverseBoundsNdx[branchVariable];
    if (childBranch == branch_up)
      {
	if ((int)ceil((double)branchValue) == intUpperBounds[index])
	  tempBitArray.set(branchVariable);
      }
    else
      {
	if ((int)floor(branchValue) == intLowerBounds[index])
	  tempBitArray.set(branchVariable);
      }
    outBuffer << tempBitArray;   // fixed array
    outBuffer << binary_vals;
    for (i = 0; i < numGeneral; i++)
      {
	if (i == index)
	  {
	  if (childBranch == branch_up)
	    {
	      outBuffer << (int) ceil((double)branchValue);
	      outBuffer << intUpperBounds[i];
	    }
	  else
	    {
	      outBuffer << intLowerBounds[i];
	      outBuffer << (int) floor(branchValue);
	    }
	  }
	else
	  {
	    outBuffer << intLowerBounds[i];
	    outBuffer << intUpperBounds[i];
	  }
      }
    if (pmGlobal()->useSets && setCount > 0)
      {
      for (i = 0; i < setCount; i++)
	{
	  outBuffer << spSetLowerBounds[i];
	  outBuffer << spSetUpperBounds[i];
	}
      outBuffer << parentSetBound; // state of child is always boundable
      }
  }
  outBuffer << branchVariable;
  outBuffer << branchValue;
  outBuffer << (int) childBranch;
  outBuffer << lpBound;            // child inherits parent's LP bound
  outBuffer << recordPCost;        // set at separation of parent
  outBuffer << id;                 // parentId for the child

  // Solution only valid after the problem has been bounded.  The unpack
  // routine will realize it is invalid from the state.  No need to pack here.

  // nFrac isn't valid, set branching information isn't valid
}

// Compute an upper bound on the size of any MILPNode (subproblem)
// The size is computed incrementally for easy update.

int parMILP::spPackSize()
{
  int packSize;

  DEBUGPR(200,ucout << "Starting to compute pack size, MIP = " 
	  << MIP << '\n');

  int numInts = MIP->numIntegerVars();

  DEBUGPR(200,ucout << "numInts = " << numInts << '\n');

  // From packing the basis.  Assuming size is same as bitArray except for
  // 2 bits/entry (see below).  Verify that.
  packSize = (lp->getNumCols() + lp->getNumRows()) * sizeof(basisState);
  packSize += 5 + (int)((lp->getNumCols() + lp->getNumRows())/4);

  DEBUGPR(200,ucout << "Size of basis: " << packSize << '\n');
  
  // from branchVariable, parentSerial, parentCreatingProc, nFrac, and
  // (worst-case) size of FracVarNdx
  packSize += (numInts + 4) * sizeof(int);

  // From packing the BitArrays fixed and binary_vals.  As of 6/98, the packed
  // size of a BitArray storing x bits is 4 + ceiling(x/8).  Assuming
  // integer division takes the floor.
  packSize += 2 * (5 + (int)(numInts/8));

  // From intLowerBounds, intUpperBounds (#general int variables) and
  // recordPCost

  packSize += (2 * MIP->numGeneralIntVars() + 1) * sizeof(int);

  // From special set lower and upper bounds, worst-case size of
  // setCandidateNdx, nSetCandidates, and parentSetBound

  packSize += (MIP->numSets() * 4 + 2) * sizeof(int);

  // From spSetBranchValues (worst case)

  packSize += (MIP->numSets() * 2) * sizeof(double);

  // from branchValue, lpBound, and solution (size numInts)
  packSize += (numInts + 2) * sizeof(double);

  // From branchType.  Assuming no need to use integer coersion
  packSize += sizeof(branch_type);

  DEBUGPR(200,ucout << "packSize = " << packSize << '\n');

  return(packSize);
}



void parMILPNode::unpack(UnPackBuffer& inBuffer)
{
  int i, temp;
  int numInts = mip()->numIntegerVars();
  int numGeneral = mip()->numGeneralIntVars();
  int setCount = mip()->numSets();
  DEBUGPR(70, ucout << "unpacking subproblem " << id << "\n");
  inBuffer >> basis;
  // Make sure this works properly.
  inBuffer >> fixed;
  inBuffer >> binary_vals;
  for (i = 0; i < numGeneral; i++)
    {
      inBuffer >> intLowerBounds[i];
      inBuffer >> intUpperBounds[i];
    }
  if (pmGlobal()->useSets && setCount > 0)
    {
    for (i = 0; i < setCount; i++)
      {
      inBuffer >> spSetLowerBounds[i];
      inBuffer >> spSetUpperBounds[i];
      }
    if (state == boundable || state == beingBounded)
      {
      inBuffer >> parentSetBound;
      }
    }
  inBuffer >> branchVariable;
  inBuffer >> branchValue;
  // Buffers work with ints, so need temporary to keep compiler happy
  inBuffer >> temp;
  branchType = (branch_type)temp;
  inBuffer >> lpBound;
  inBuffer >> recordPCost;
  inBuffer >> parentId;

// Solution packed only if valid
  if (state != boundable && state != beingBounded)
    {
      for (i = 0; i < numInts; i++)
	inBuffer >> solution[i];
      inBuffer >> nFrac;
      for (i = 0; i < nFrac; i++)
	inBuffer >> FracVarNdx[i];
      if (pmGlobal()->useSets && setCount > 0)
	{
	  inBuffer >> nSetCandidates;
	  for (i=0; i < nSetCandidates; i++)
	    inBuffer >> setCandidateNdx[i];
	  for (i=0; i < setCount; i++)
	    inBuffer >> spSetBranchValues[i];
	}

    }
}

// Pack the pseudocosts indicated in VarNdx into the buffer,
// upCost, then downCost.  An "infinite" cost (MAXDOUBLE) indicates
// infeasibility

void parMILP::packPseudoCosts(int firstToPack, int numToPack, IntVector &VarNdx, double *buffer)
{
int currVar;
for (int i=0; i < numToPack; i++)
  {
  currVar = VarNdx[i + firstToPack];
  buffer[2*i] = upCostBuffer[currVar];
  buffer[2*i + 1] = downCostBuffer[currVar];
  }
}

// Pack integralityChange information.  Pack (up,down) information for a subrange of the branchChoices in
// the branchChoices vector (specifically for numToPack branchChoices starting with firstToPack).

void parMILP::packIntegralityChanges(int firstToPack, int numToPack, IntVector &branchChoices, double *buffer)
{
int currChoice;
for (int i=0; i < numToPack; i++)
  {
  currChoice = branchChoices[i + firstToPack];
  buffer[2*i] = integralityChange[2*currChoice];
  buffer[2*i + 1] = integralityChange[2*currChoice+1];
  }
}


// Write application-dependent data to a buffer during a checkpoint

void parMILP::appCheckpointWrite(PackBuffer& outBuf)
{
  if (iAmWorker())
    {
      outBuf << integralityChange;
      outBuf << downCostBuffer  << upCostBuffer;
      outBuf << downCostCounter << upCostCounter;
    }
}


// Read application-dependent data from a buffer when restarting from
// a checkpoint

void parMILP::appCheckpointRead(UnPackBuffer& inBuf)
{
  if (inBuf.message_length() > 0)
    {
      inBuf >> integralityChange;
      inBuf >> downCostBuffer  >> upCostBuffer;
      inBuf >> downCostCounter >> upCostCounter;
    }
}


// Merge application-dependent data from a buffer when reconfiguring from
// a checkpoint.  This version is really, really dumb.  It just does a read
// on the first non-empty buffer it gets, and otherwise ignores everything.
// This means that on restart, all workers will have the pseudocost state
// of the first worker in the old run.  We may want to improve this later.

void parMILP::appMergeGlobalData(UnPackBuffer& inBuf)
{
  if (!mergedAnything && (inBuf.message_length() > 0))
    {
      mergedAnything = true;
      appCheckpointRead(inBuf);
    }
}


// When the scheduler object is being constructed, this guy makes sure
// that the pseudocost broadcasting thread is created, and incorporated
// into the scheduler for all workers.

void parMILP::placeTasks()
{
  parallelBranching::placeTasks();        // Make all the usual threads.
  pCostCaster = new pCostCastObj(this);
  placeTask(pCostCaster,iAmWorker(),highPriorityGroup);
}

// Incorporate pseudocost information received from other processors during
// the "normal" B&B phase (that is, *not* during the parallel ramp up)

void parMILP::incorporatePCosts(int n,
				IntVector&    indices,
				DoubleVector& upValues,
				DoubleVector& downValues)
{
#ifdef DUMMY
      ucout << "[" << uMPI::rank << "] Incorporating pseudocost broadcast (up,down):\n";
  int j = 0;
  for (int printi = 0; printi < n; printi++)
    {
      ucout << indices[printi] << ": (" << upValues[printi] << ", " << downValues[printi] << ") ";
      if (++j == 5)
	{
	  ucout << "\n";
	  j = 0;
	}
    }
  ucout << "\n";
#endif
  
  for (int i=0; i < n; i++)
    {
      // negative values indicate invalid pseudocosts (i.e. where a value
      // was generated in only one direction. Logically the information is
      // (variable, up, down) triples of which only one of up and down may
      // be valid).  Use MILP version to avoid sending back out again.
    if (upValues[i] >= 0)
      MILP::updateUpCostBuffer(indices[i], upValues[i]);
    if (downValues[i] >= 0)
      MILP::updateDownCostBuffer(indices[i], downValues[i]);
    }
}


void parMILP::preprocess()
{
  // Credit preprocessing as useful work in the event log, but only for 
  // one processor.
  LOG_EVENT_CONDITIONAL(1,veryFirstWorker(),start,workerLogState);
  MILP::preprocess();
  LOG_EVENT_CONDITIONAL(1,veryFirstWorker(),end,workerLogState);

  DEBUGPR(2,ucout << "parMILP::preprocess done, absTolerance="
	  << absTolerance << endl);
  
}


// During ramp up, divide the pseudocost initialization work  evenly among the  processors.
// Keep the same calling args as the call in MILP (so this overrides a virtual function),
// but we don't use the initFracDummy argument during ramp up.

int parMILP::selectPseudocostInit(int nFrac, 
				  IntVector &FracVarNdx, 
				  int numFracSets,
				  IntVector &candidateSets,
				  IntVector& initIndices, 
				  bool &dontRemoveAllBranchChoices,
				  double initFracDummy)
{
if (initFracDummy == -1.0)
   initFracDummy = rampUpTableInitFrac;
  // If we're past the ramp-up stage, just do the normal (serial) thing

  if (!rampingUp())
    return(MILP::selectPseudocostInit(nFrac, 
				      FracVarNdx, 
				      numFracSets, 
				      candidateSets, 
				      initIndices,
				      dontRemoveAllBranchChoices, 
				      initFracDummy));

  // First find the set of variables that need initialization.  If the
  // ramp up lasts for several nodes, some variables may have been
  // initialized earlier.  Also, only initialize highest-priority
  // variables even on ramp up.  Since we're doing this in parallel, we may want to
  // initialize a larger fraction of these variables than when nodes
  // are handled serially, so use the ramp-up parameter for
  // determining what fraction of highest-priority variables to
  // initialize.

  numUnInit = MILP::selectPseudocostInit(nFrac, 
					 FracVarNdx, 
					 numFracSets,
					 candidateSets, 
					 initIndices,
					 dontRemoveAllBranchChoices, 
					 rampUpTableInitFrac);

  // This could be pulled out into a function if we'd like the decision to
  // be more obviously located.  For now, it's only used here

  DEBUGPR(100, ucout << "Uninitialized vbls: (" << numUnInit << ") " 
	  << initIndices << "\n"; ucout.flush());

  if (avgNumUnInit > 0)
  avgNumUnInit = (1 - numPCInitSmoothFactor) * avgNumUnInit +
    numPCInitSmoothFactor * numUnInit;
  else avgNumUnInit = numUnInit;

  // Save out the information on all variables to be initialized.  It'll be
  // needed to interpret the pseudocost messages later
  if (fullInitVars == NULL) fullInitVars = new IntVector(MIP->numIntegerVars() + MIP->numSets());
#ifdef ACRO_VALIDATING
 if (fullInitVars == NULL)
   EXCEPTION_MNGR(runtime_error, "Failed to allocate a new fullInitVars\n");
#endif
  (*fullInitVars) << initIndices;
  size_type firstJob, mysize, biggestSize, numWithMax;
  divideWorkLexicographically(numUnInit, firstJob, mysize, biggestSize, numWithMax);
  for (size_type j = 0; j < mysize; j++)
    {
    initIndices[j] = initIndices[j + firstJob];
    }

  DEBUGPR(100, ucout << "Uninitialized vbls (after move): (" 
	  << numUnInit << ") " 
	  << initIndices << "\n"; ucout.flush());

#ifdef DUMMY
  if (uMPI::rank == 0)
    {
      ucout << "uninitialized variables:\n";
      for (int tempy = 0; tempy < numUnInit; tempy++)
	ucout << initIndices[tempy] << ", ";
      ucout << "\n";
      ucout << Flush;
    }
    ucout << "chosen frac vars:\n";
    int k = 0;
    for (int i = 0; i < mysize; i++)
      {
      ucout << initIndices[i] << ", ";
      if (++k == 5)
	{
	  ucout << "\n";
	  k=0;
	}
      }
 #endif
  return(mysize);
}

// Exchange of pseudocost information after each processor does a piece during
// ramp up.  Also restore initVars and numBranches to the full set (necessary
// for later checks).  Exchange integralityMeasure change information.

void parMILPNode::postPseudoCostInit(IntVector &initVars, 
				     bool dontRemoveAllBranchChoices, 
				     int &numBranches)
{
  if (!rampingUp())
    {
      MILPNode::postPseudoCostInit(initVars, 
				   dontRemoveAllBranchChoices, 
				   numBranches);
      return;
    }

  DEBUGPR(150,for(int ivndx=0; ivndx<mip()->numIntegerVars(); ivndx++)
	  ucout << "Up/down counters (before) for " << ivndx << ": "
	  << pmGlobal()->upCostCounter[ivndx] << ' ' 
	  << pmGlobal()->downCostCounter[ivndx] << endl);

  // Restore list to the full set just initialized to suppress
  // recording of pseudocosts after a branch on a variable just
  // initialized, even if the actual calculation was done on another
  // processor.
  initVars << *pmGlobal()->fullInitVars;
  int pid = uMPI::rank;

  // It might appear that all we need are in the arrays awaiting broadcast, but
  // for this communication, we also need to send information about
  // infeasibilities.  A double infeasibility for a variable means the problem
  // is infeasible (very important case), but this variable would not be
  // represented in the awaiting-broadcast arrays.

  //  Send out the information to everyone.  I think this replicates the
  // behavior in selectPseudocostInit

  int numProcs = uMPI::size;
  int numUnInit = pmGlobal()->numUnInit;
  size_type firstJob, mysize, biggestSize, numWithMax;
  pmGlobal()->divideWorkLexicographically(numUnInit, firstJob, mysize, 
					  biggestSize, numWithMax);
  DoubleVector sendbuf(2 * biggestSize);
  DoubleVector allPseudoInits(2 * numProcs * biggestSize);

  // The branching class handles all the pseudocost data structures
  // If this processor has a light load, the last two entries will not
  // contain real data.  WARNING: this loads values from the regular pseudocost
  // buffers.  This will only work if this is a true initialization (the buffers for
  // these variables were empty before).  This is currently the case through all of ramp
  // up (and whenever doing pseudocost initialization), but if that changes, we'll need 
  // to use some auxilliary buffers as we do to send later pseudocost information.

  pmGlobal()->packPseudoCosts(firstJob, 
			      mysize, 
			      *pmGlobal()->fullInitVars, 
			      sendbuf.data());

  
#ifdef DUMMY
  //if (pid == 0)
  //  {
  int printj = 0;
  int printi;
  ucout << "[" << pid << "]: sendbuf is\n";
      for (printi = 0; printi < 2 * mysize; printi += 2)
	{
        ucout << "(" << sendbuf[printi] << ", " << sendbuf[printi+1] << ")";
	if (++printj == 5)
	  {
	    ucout << "\n";
	    printj = 0;
	  }
	}
      //  }
#endif

  // I'm assuming that in the MIP world, we are still always working with
  // the whole world.  If we do more complex parallelization some day, we'll
  // have to use a more complex comm group.
  // Note:  we could save a little off the size of this message by using
  // MPI_allgatherv, but really this is so close to everyone having the same
  // amount to send, it didn't seem worth the extra effort.

  // JE -- added code here to short-circuit the call if there is only one 
  // processor.  It appears that Sun HPC MPI 4.0 dies if you try to
  // allgather when there is only one processor.

  if (numProcs > 1)
    {
      MPI_Allgather(sendbuf.data(), 2 * biggestSize, 
	            MPI_DOUBLE, allPseudoInits.data(),
		    (2 * biggestSize), MPI_DOUBLE, MPI_COMM_WORLD);
    }
  else
    allPseudoInits << sendbuf;  // If only one processor, just copy buffer

  // JE: keep track of messages sent by the allgather operation.

  pmGlobal()->rampUpMessages += numProcs - 1;

#ifdef DUMMY
  if (pid == 0)
    {
      int printj = 0;
      ucout << "\nAll pseudocosts are "
	    << "(remember zeroes in procs with fewer inits)\n";
      for (int printi = 0; printi < 2 * biggestSize * numProcs; printi += 2)
	{
	  ucout << "(" << allPseudoInits[printi] << ", " 
		<< allPseudoInits[printi+1] << ")";
	  if (++printj == 5)
	    {
	      ucout << "\n";
	      printj = 0;
	    }
	}
    }
#endif

//   // JE -- added this to make sure all processor agree on whether the 
//   // problem is infeasible or not (June 30, 2004).  Also do the shutdown
//   // now.

//   // QUESTION -- would this make more sense above the AllGather?
//   // QUESTION -- or would it make more sense after the communication below?
  
//  {
//    int deadFlag    = (state == dead);
//    int anybodyDead = false;
//    DEBUGPR(100,ucout << "Subproblem is " 
// 	   << (deadFlag ? "dead" : "alive") << endl);
//    uMPI::reduceCast(&deadFlag,&anybodyDead,1,MPI_INT,MPI_LOR);
//    if (anybodyDead)
//      {
//        setState(dead);
//        DEBUGPR(100,ucout << "After state comm, subproblem is dead\n");
//        // There was a "return" here, commented out by JE/DEC04
//      }
//    else
//      DEBUGPR(100,ucout << "After state comm, subproblem is alive\n");
//  }

  int bufferPtr = 0;
  if (dontRemoveAllBranchChoices)  
    {
      // All highest-priority variables were uninitialized
      // check for total half infeasibility
      for (int i = 0; i < numUnInit; i++)
	if (allPseudoInits[bufferPtr++] != MAXDOUBLE &&
	    allPseudoInits[bufferPtr++] != MAXDOUBLE)
	  {
	    // Found a branch choice where both directions are feasible
	    dontRemoveAllBranchChoices = false;
	    break;
	  }
    }

  int currVar, genVarNdx, LPNdx;
  int fracVarNum = 0;
  size_type smallJob;
  bufferPtr = 0; // step through receive buffer
  IntVector & localFullInitVars = *(pmGlobal()->fullInitVars);
  int numInts = mip()->numIntegerVars();
  //int setNum;
  //IntVector *thisSet;
  //size_type setStart, setEnd;
  for (int i = 0; i < numProcs; i++)
    {
      // Each processor used to step past its own information, but now it has
      // waited to narrow bounds and removeBranchChoice (so all will 
      // do this in the same order)
    if (numWithMax != 0 && (size_type)i >= numWithMax)
      smallJob = 1;
    else smallJob = 0;
    for (size_type j=0; j < biggestSize - smallJob; j++)
      {
      currVar = localFullInitVars[fracVarNum++]; // gives integer var# or set#
      if (currVar < numInts)
	{
	  genVarNdx = mip()->reverseBoundsNdx[currVar];
	  LPNdx = mip()->integerVarNdx[currVar];
	}
      if (allPseudoInits[bufferPtr] == MAXDOUBLE) // upward dir not feasible
	{
	  if (allPseudoInits[bufferPtr+1] == MAXDOUBLE) // problem infeasible
	    {
	    setState(dead);
	    // All commented out by JE/DEC044
	    // if (pmGlobal()->veryFirstWorker() && pmGlobal()->validateLog)
	    // 	  valLogSplitPrint();
	    // return;
	    }
	  // If we can't pseudofix all, don't pseudofix the first job
	  else if (!dontRemoveAllBranchChoices || i > 0 || j > 0)
	    {
	      adjustNodeBounds(currVar, branch_down);
	      removeBranchChoice(currVar);
	    }
	  if (pid != i)
	    pmGlobal()->updateFromInfeasible(currVar, branch_up);
	}
      else if (pid != i) 
	pmGlobal()->updateUpCostBuffer(currVar, allPseudoInits[bufferPtr]);
      bufferPtr++;
      if (allPseudoInits[bufferPtr] == MAXDOUBLE) // down direction infeasible
	{
	if (!dontRemoveAllBranchChoices || i > 0 || j > 0)
	  {
	    adjustNodeBounds(currVar, branch_up);
	    removeBranchChoice(currVar);
	  }
	if (pid != i) 
	  pmGlobal()->updateFromInfeasible(currVar, branch_down);
	}
      else 
	if (pid != i) 
	  pmGlobal()->updateDownCostBuffer(currVar, allPseudoInits[bufferPtr]);
      bufferPtr ++;
      }
    bufferPtr += 2 * smallJob; // Step over blank pair for procs w/fewer jobs.
    }

  DEBUGPR(150,for(int ivndx=0; ivndx<numInts; ivndx++)
	  ucout << "Up/down counters (after) for " << ivndx << ": "
	  << pmGlobal()->upCostCounter[ivndx] << ' ' 
	  << pmGlobal()->downCostCounter[ivndx] << endl);

  // Force reload of bounds for all children to reflect bounds changes from
  // this pseudocost initialization

  forceColdStart();

  // Exchange bound information

  double newBound;
  //    ucout << "Sending bound " << bound << "\n";
  //    ucout << Flush;
  if (lround(lp->getObjSense()) == minimize)
    MPI_Allreduce(&bound, &newBound, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  else
    MPI_Allreduce(&bound, &newBound, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
  pmGlobal()->rampUpMessages += uMPI::size - 1;
  bound = newBound;
  //  if (uMPI::rank == 0) ucout << "New bound = " << bound << "\n";

  /*      ucout << "PseudoCosts at end of parallel init are\n";
    pmGlobal()->printPseudoCosts();
    ucout << endl << Flush; */

  // Everything else should be done only if the MIP heuristic is running

  if (pmGlobal()->mipHeuristicPtr == NULL) 
    return;

  pmGlobal()->packIntegralityChanges(firstJob, 
				     mysize, 
				     localFullInitVars, 
				     sendbuf.data());

  // We could reuse allPseudoInits, but let's go for readable code
  DoubleVector allIntegralityChange(2 * numProcs * biggestSize);

  if (numProcs > 1)
    {
      MPI_Allgather(sendbuf.data(), 2 * biggestSize, 
	            MPI_DOUBLE, allIntegralityChange.data(),
		    (2 * biggestSize), MPI_DOUBLE, MPI_COMM_WORLD);
    }
  else
    allIntegralityChange << sendbuf;  // If only one processor, just copy buffer

  // keep track of messages sent by the allgather operation.

  pmGlobal()->rampUpMessages += numProcs - 1;

  bufferPtr = 0;
  int branchChoice = 0;
  int Ndx = 0;
  for (int i = 0; i < numProcs; i++)
    {
      // Each processor used to step past its own information, but now it has
      // waited to narrow bounds and removeBranchChoice 
      // (so all will do this in the same order)
    if (numWithMax != 0 && (size_type)i >= numWithMax)
      smallJob = 1;
    else smallJob = 0;
    for (size_type j=0; j < biggestSize - smallJob; j++)
      {
      Ndx = localFullInitVars[branchChoice++];
      // up and down integrality changes
      pmGlobal()->integralityChange[2*Ndx] = allIntegralityChange[bufferPtr++];
      pmGlobal()->integralityChange[2*Ndx+1] = allIntegralityChange[bufferPtr++];
      }
    bufferPtr += 2 * smallJob; // Step over blank pair for procs w/fewer jobs.
    }
  //pmGlobal()->printIntegralityChange();
}

void parMILP::resizePtable(int newSize)
{
  MILP::resizePtable(newSize);
  numPcostGathered = 0;
  // We're assume the minimum broadcast size is small compared to newSize
  // (which as of 7/00 is only called with the number of integer variables.
  // We can probably make it smaller (max # vars that can be fractional is
  // numrows, though this includes slack variables).
  bcastIndices.resize(newSize);
  bcastUp.resize(newSize);
  bcastDown.resize(newSize);
  for (int i=0; i < newSize; i++)
    {
    bcastUp[i] = -1;
    bcastDown[i] = -1;
    }

}

// Returns true if one of the pseudocosts waiting in the buffer is a
// first-time pseudocost.  Note:  we always initialize both up and down
// directions at the same time if we're doing any initialization at all,
// so it's OK to just check the upward direction.

int parMILP::firstPCostPending()
{
  // Search in reverse, since new ones are more likely to be at the back
  for (int i = numPcostGathered-1; i >= 0; i--)
    {
    if (upCostCounter[bcastIndices[i]] == 1)
      return(TRUE);
    }
  return (FALSE);
}


// Send pseudocosts if any are brand new (first for the variable) or we've
// gathered enough to make the send worthwhile.  As of now, we could have
// the situation where we cannot clean up because there are fewer "interesting"
// pseudocosts left to send than this minimum message size.

void parMILP::sendPcosts()
{
  // After clean up, when no longer sending, this is always the case, so worth
  // this "redundant" check.
  if (numPcostGathered == 0) return;
  if ((numPcostGathered >= pCostMinBcastSize) || firstPCostPending())
    {
    DEBUGPR(100, ucout << "Sending pseudocosts\n");
    DEBUGPR(1000, printPendingPcostCast());
    pCostCaster->startBroadcast(numPcostGathered,
				bcastIndices, 
				bcastUp, 
				bcastDown);
    if (pcostBcastCleanup())  
      return;
    // There may be more to send, so reset
    resetPcostBcast();
    //    ucout << "Verifying reset\n";
    //    printPendingPcostCast();
    }
  else
    {
      DEBUGPR(100, ucout << "Not enough pending to send yet\n");
      DEBUGPR(100, printPendingPcostCast());
    }
}

void parMILP::resetPcostBcast()
{
  for (int i=0; i < numPcostGathered; i++)
    {
    bcastUp[i] = -1;
    bcastDown[i] = -1;
    }
  numPcostGathered = 0;
}

// Check to see if all integral variables have enough pseudocosts shared.
// If so, recoup the space

int parMILP::pcostBcastCleanup()
{
  int bcastPhaseOver = TRUE;
  int numInts = MIP->numIntegerVars();
  for (int i=0; i < numInts; i++)
    {
      if (upCostCounter[i] < pCostShareCutoff ||
	  downCostCounter[i] < pCostShareCutoff)
	{
	bcastPhaseOver = FALSE;
	break;
	}
    }
  if (bcastPhaseOver)
    {
    DEBUGPR(50, ucout << "Through sending pseudocosts, deallocating space\n");
    numPcostGathered = 0;
    bcastIndices.resize(0);
    bcastUp.resize(0);
    bcastDown.resize(0);
    return(TRUE);
    }
  return(FALSE);
}

// Update (upward) pseudocost information.  If this value should be shared,
// put it in the appropriate buffers.  The pseudocast broadcaster sends both
// an upward and downward value for each variable.  It's possible the variable
// is already there with only a downward value (indicated by a -1 in the upward
// buffer). In this case, put the new value with it's downward mate, and
// otherwise open a new (index,up,down) triple [logically; actually it's 3
// vectors and we just add to the end].

void parMILP::updateUpCostBuffer(int index, double value)
{
  MILP::updateUpCostBuffer(index, value);
  if ((pCostShareCutoff == 0) || rampingUp()) return;
  //  ucout << "upCostCounter = " << upCostCounter[index] << " index = " << index << ", cutoff = " << (int)pCostShareCutoff() << "\n";
  if (upCostCounter[index] <= pCostShareCutoff)
    {
      //      ucout << "[" << uMPI::rank << "] scheduling " << index << " for pcost bcast (up)\n";
    int alreadyThere = FALSE;
    if (numPcostGathered > 0)
      {
	for (int j = numPcostGathered -1; j >= 0; j--)
	  {
	  if (bcastIndices[j] == index && bcastUp[j] < 0)
	    {
	    bcastUp[j] = value;
	    alreadyThere = TRUE;
	    break;
	    }
	  }
      }
    if (!alreadyThere)
      {
      bcastIndices[numPcostGathered] = index;
      bcastUp[numPcostGathered] = value;
      numPcostGathered++;
      // Pretty unlikely, but just in case.
      if (numPcostGathered == (int)bcastIndices.size())
	sendPcosts();
      }
    }
}

// See above description of updateUpCostBuffer

void parMILP::updateDownCostBuffer(int index, double value)
{
  MILP::updateDownCostBuffer(index, value);
  if ((pCostShareCutoff == 0) || rampingUp()) return;
  //  ucout << "downCostCounter = " << downCostCounter[index] << " index = " << index << "\n";

  if (downCostCounter[index] <= pCostShareCutoff)
    {
      //      ucout << "[" << uMPI::rank << "] scheduling " << index << " for pcost bcast\n";
    int alreadyThere = FALSE;
    if (numPcostGathered > 0)
      {
      for (int j = numPcostGathered -1; j >= 0; j--)
	{
        if (bcastIndices[j] == index && bcastDown[j] < 0)
	  {
	  bcastDown[j] = value;
	  alreadyThere = TRUE;
	  break;
	  }
	}
      }
    if (!alreadyThere)
      {
      bcastIndices[numPcostGathered] = index;
      bcastDown[numPcostGathered] = value;
      numPcostGathered++;
      // Pretty unlikely, but just in case
      if (numPcostGathered == (int)bcastIndices.size())
	sendPcosts();
      }
    }
}

// using ucout until pucout is working

void parMILP::printPendingPcostCast()
{
  if (bcastIndices.size() == 0)
    {
      DEBUGPR(0,ucout << 
	      "No more pseudocost broadcasts (arrays deallocated)\n");
      return;
    }
  DEBUGPR(0,ucout << "Pending pseudocost broadcasts (up,down):\n");
  int j = 0;
  for (int i = 0; i < numPcostGathered; i++)
    {
      if (j++ == 5)
	{
	  DEBUGPR(0,ucout << "\n");
	  j = 0;
	}
      DEBUGPRP(0,ucout << bcastIndices[i] 
	       << ": (" << bcastUp[i] << ", " << bcastDown[i] << ") ");
    }
}


void parMILPNode::feedToIncumbentThread()
{
  if ((state != bounded) || (pmGlobal()->mipHeuristicPtr == NULL))
    return;
  MIPHeuristic* heur = pmGlobal()->mipHeuristicPtr;
  heur->globalBoundSet(pmGlobal()->workerPool->knowsGlobalBound(),
		       pmGlobal()->workerPool->globalBound());
  heur->feedIn(this);
}


void parMILP::parallelIncumbentHeuristic(double* controlParam)
{
  DEBUGPR( 4, ucout<<"\n---parMILP::parallelIncumbentHeuristic\n");

  int iterations   = 0;
  bool canStillRun = false;

  mipHeuristicPtr->globalBoundSet(workerPool->knowsGlobalBound(),
				  workerPool->globalBound());
		  
  do
    {
      if (mipHeuristicPtr != NULL)
	canStillRun = mipHeuristicPtr->parRun();
    }
  while (canStillRun && (++iterations < *controlParam));

  *controlParam = iterations;
  DEBUGPR( 10, ucout<<"\n====parMILP::parallelIncumbentHeuristic === end\n");
};


//++++++++++++++++++++++++++++++++++++++++++++++++++++

void parMILPNode::rampUpIncumbentHeuristic()
{
  // Prime the queue of incumbent solutions for later...
  if (state == bounded)
    feedToIncumbentThread();
  else if ((state == separated) && pmGlobal()->mipHeuristicPtr)
    {
      // How much presplitting do we want to do?
      int leavesWanted = uMPI::size/(mGlobal()->heurCfg.bbhRampUpMFTarget);
      DEBUGPR(2,ucout << "rampUpIncumbentHeuristic: leavesWanted="
	      << leavesWanted << endl);

      bool wantCanon = mGlobal()->heurCfg.bbhRampUpUseCanonical;

      if (leavesWanted <= 1)
	{
	  DEBUGPR( 2, ucout<< "No presplitting rampUpIncumbentHeuristic" );
	  // Use canonical MF on processor 0 if canonical option set
	  wantCanon &= (uMPI::rank == 0);
	  // Run heuristic with no splitting
	  pmGlobal()->mipHeuristicPtr->rampUpRun(this,no_branch,wantCanon);
	}
      else
	{
	  // OK, we want to presplit.  Make sure we have the information
	  // on variable interactions ready

	  if (pmGlobal()->adjSet.size() == 0)
	    {
	      mip()->buildAdjSets(pmGlobal()->adjSet);
	      DEBUGPR(100,
		      ucout << "Adjacent sets:\n";
		      for(size_type s=0; s<pmGlobal()->adjSet.size(); s++)
	                {
			  ucout << "Adjacent to " << s << ':';
			  for(size_type t=0; 
			      t < pmGlobal()->adjSet.size(); 
			      t++)
			    if (pmGlobal()->adjSet[s].contains(t)) 
			      ucout << ' ' << t;
			  ucout << endl;
			}
		      );
	    }

	  // These arrays hold the information on which variables seem
	  // most attractive to branch on.

	  int numBranchChoices = nFrac + nSetCandidates;
	  DoubleVector splitRating(numBranchChoices);
	  BasicArray<int> pi(numBranchChoices);

	  for(int i=0; i<numBranchChoices; i++)
	    {
	      int j=FracVarNdx[i];
	      double downtemp = pmGlobal()->integralityChange[2*j];
	      double uptemp   = pmGlobal()->integralityChange[2*j+1];
	      splitRating[i] = min(downtemp,uptemp);
	      DEBUGPR(100,ucout << "Fractional " << i << ", integer " << j
		      << " x[" << mglobal()->MIP->integerVarNdx[j] << "]="
		      << solution[j] << ", uptemp=" << uptemp
		      << ", downtemp=" << downtemp 
		      << ", splitRating=" << splitRating[i] << endl);
	    }
	  
	  order(pi,splitRating);
	  
	  // The heap that will hold the presplit tree

	  GenericHeap<preSplitNode> psHeap;

	  // Make a root node (with no presplitting) and throw it in the heap
	  
	  {
	    preSplitNode* psRoot = new preSplitNode(this,pi);
#ifdef ACRO_VALIDATING
	    if (psRoot == NULL)
	      EXCEPTION_MNGR(runtime_error, "Failed to allocate a new preSplitNode\n");
#endif

	    psHeap.add(*psRoot);
	  }

	  // Chop up nodes until we have as many leaf nodes as we want
	  // or there is nothing left to chop up.

	  int leafNodes = 1;

	  while(true)
	    {
	      // Pick best-looking node

	      preSplitNode* top = &(psHeap.top()->key());

	      DEBUGPR(20,top->dump("Top node ",ucout));

	      // If best node is not eligible, stop

	      if (!top->active())
		break;

	      // If we spawn two nodes from this one, how many leaves?
	      // If that would be too many, stop

	      int newLeaves = leafNodes + 2 - top->isLeaf();
	      if (newLeaves > leavesWanted)
		break;

	      // Make two (more?) children from the node.  
	      // The second is identical to the first except for direction

	      adjSetObj& adjSet = pmGlobal()->adjSet;
	      preSplitNode* upChild   = new preSplitNode(top,branch_up,adjSet);
	      preSplitNode* downChild = new preSplitNode(upChild,branch_down);
#ifdef ACRO_VALIDATING
	      if (downChild == NULL)
		EXCEPTION_MNGR(runtime_error, "Failed to allocate a new preSplitNode\n");
#endif

	      DEBUGPR(20,upChild->dump("Up child made: ",ucout));
	      DEBUGPR(20,downChild->dump("Down child made: ",ucout));

	      // Reposition the top of the heap, then insert new nodes
	      // Update count of leaf nodes

	      psHeap.refloatElement(1);
	      psHeap.add(*upChild);
	      psHeap.add(*downChild);
	      leafNodes = newLeaves;
	    }
	  
	  // Figure out which leaf this processor wants, and then find it

	  int leafWanted = uMPI::rank % leafNodes;
	  DEBUGPR(20,ucout << "leafWanted=" << leafWanted << endl);

	  int leafCtr    = -1;
	  int nodeCursor = 1;
	  preSplitNode* thisNode = NULL;
	  do
	    {
	      thisNode = &(psHeap.member(nodeCursor++)->key());
	      if (thisNode->isLeaf())
		leafCtr++;
	    }
	  while (leafCtr < leafWanted);

	  DEBUGPR(20,thisNode->dump("Selected leaf node ",ucout));

	  // Run the ramp-up heuristic on the desired node
	  // If first processor with this leaf and the user wants at
	  // least one canonical merit function, force canonical.

	  wantCanon &= (uMPI::rank == leafWanted);
	  pmGlobal()->mipHeuristicPtr->rampUpRun(this,thisNode,wantCanon);

	  // Release all the memory used by the presplit tree

	  while (psHeap.size() > 0)
	    {
	      bool statusDummy;
	      thisNode = &(psHeap.top()->key());
	      psHeap.remove(*thisNode,statusDummy);
	      delete thisNode;
	    }
	  
	}
      
      pmGlobal()->rampUpIncumbentSync();
      
    }
  
};



// This is called from the parent

void parMILPNode::valLogUnpackExtra()
{
  *vout << ' ' << branchType << ' ' << depth;
}


// For some solvers (e.g. CLP circa 2004), during ramp-up when
// processor actions should be synchronized, different processors
// can compute different optimal solutions or somewhat different
// solution values and bounds for the same problem.  This can lead
// to different tree growth and hanging.  Force the synchronization.

PicoLPInterface::problemState parMILPNode::boundingSolve()
{
  // For now cplex is the only solver we feel doesn't need this.
  // Update as necessary.  If we're pulling a root solution from a file,
  // have a single processor read and propagate (i.e. stay in override).

  if ((!isRoot() || !pmGlobal()->useSavedRootSolution()) &&

    (!rampingUp() || mip()->lp.getLPSolverType() == PicoLPSolver::cplex))
  return(MILPNode::boundingSolve());

  PicoLPInterface::problemState solvedState;
  int tempState;

 if (uMPI::rank == 0)
   {
   if (isRoot() && pmGlobal()->useSavedRootSolution())
     solvedState = retrieveRootSolution();
   else solvedState = runLP();
   tempState = (int) solvedState;
   }
 // Everybody gets the state.
 uMPI::broadcast(&tempState, 1, MPI_INT, 0);
 pGlobal()->rampUpMessages += (uMPI::rank > 0);
 if (uMPI::rank != 0)
   solvedState = (PicoLPInterface::problemState)tempState;
 if (solvedState != PicoLPInterface::solved)
   return(solvedState);
 int broadcastSize;
 if (uMPI::rank == 0)
   {
     // If we retrieved a solution, these are already set
   if (!isRoot() || !pmGlobal()->useSavedRootSolution())
     {
     lpBound = lp->getObjValue();
     getSolution();
     getBasis(basis);
     }
   PackBuffer outBuf;
   outBuf << lpBound << solution << basis;
   // This is simple packing, so we could probably figure out the size, but
   // that would make us vulnerable to changes in the packing operators (and
   // it's not that easy to get basisArray sizes, so just send the size.
   broadcastSize = outBuf.size();
   uMPI::broadcast(&broadcastSize, 1, MPI_INT, 0);

   uMPI::broadcast((void*) outBuf.buf(),
		   broadcastSize, MPI_PACKED, 0);
   }
 else
   {
   uMPI::broadcast(&broadcastSize, 1, MPI_INT, 0);
   pGlobal()->rampUpMessages++;
   char* buf = new char[broadcastSize];          // Make space for it
#ifdef ACRO_VALIDATING
   if (buf == NULL)
     EXCEPTION_MNGR(runtime_error, "Failed to allocate a new buffer to receive bounding info\n");
#endif
   UnPackBuffer inBuf(buf,broadcastSize,1);	 // Give buf to inBuf
   uMPI::broadcast((void *) inBuf.buf(),         // Get the data...
		   broadcastSize, MPI_PACKED, 0);
   pGlobal()->rampUpMessages++;
   inBuf >> lpBound >> solution >> basis;
   }
  // All processors can do this redundantly
 if (lround(lp->getObjSense()) == minimize)
   bound = max(bound, lpBound);
 else bound = min(bound, lpBound);
 nFrac = computeFracVars(); // Also sets FracVarNdx and integralityMeasure
 return(PicoLPInterface::solved);
}

// Verify that the candidate solution is still feasible when integer values
// are set exactly.  During ramp up, only processor 0 does this (to force
// agreement on all decisions that affect the tree.

PicoLPInterface::problemState parMILPNode::recheckSolution()
{
if (!rampingUp() || mip()->lp.getLPSolverType() == PicoLPSolver::cplex)
  return(MILPNode::recheckSolution());

  PicoLPInterface::problemState solvedState;
  int tempState;

  // Proc 0 does the recheck and sends all other processors
  // the resulting state and LP bound.

  if (uMPI::rank == 0)
    {
    lp->setLPMethod(PicoLPInterface::primalSimplex);
    solvedState = runLP();
    tempState = (int) solvedState;
    if (solvedState == PicoLPInterface::solved)
      lpBound = lp->getObjValue();
    }

 uMPI::broadcast(&tempState, 1, MPI_INT, 0);
 pGlobal()->rampUpMessages += (uMPI::rank > 0);
 if (uMPI::rank != 0)
   solvedState = (PicoLPInterface::problemState)tempState;
 if (solvedState != PicoLPInterface::solved)
   return(solvedState);

 // This is a valid solution, so we'll need the LP Bound for incumbent update

 uMPI::broadcast(&lpBound, 1, MPI_DOUBLE, 0);
 pGlobal()->rampUpMessages += (uMPI::rank > 0);

 return(solvedState);
}

// During ramp up, computes integralityMeasure changes after each
// pseudobranch during pseudocost initialization.  The MIP heuristic
// needs this information.  Note: we assume this is called right after
// computing the LP bound on a pseudocost initialization probe.  So all
// the information in the node (e.g. solution) is from the node's original
// bounding operation. We compare the integrality measure of the
// solution to the integralityMeasure stored in the node.

void parMILPNode::postPseudoBranch(int branchChoice, branch_type direction)
{
  // If there's ever a non-empty method in MILPNode, change to call that
  // TODO: also return if the MIP heuristic is off
if (!rampingUp() || pmGlobal()->mipHeuristicPtr == NULL) return;
int numInts = mip()->numIntegerVars();
DoubleVector probeSolution(numInts);

// This is from MILPNode::getSolution(), but we need to use this vector
// rather than the node solution vector
lp->getColSolution(probeSolution.data(), numInts, mip()->integerVarNdx.data());

// Replication of some code in MILPNode::computeFracVars().  It would be wasteful to
// try to use that function directly since it has many side effects on the node we don't want.

double newIntegralityMeasure = 0;
double roundDist, val;
int j;
MILPProblem *localMipRef = mip();
for (int i=0; i < numInts; i++)
  {
  j = localMipRef->integerVarNdx[i]; // get LP var# for this int var #
  val = probeSolution[i];
  if (fixed(i)) continue;
  if (!isInteger(val))
    {
    roundDist = ceil(val) - val;
    newIntegralityMeasure += min(roundDist, 1 - roundDist);
    }
  }
int intChangeNdx = 2 * branchChoice;
if (direction == branch_down) intChangeNdx++;
pmGlobal()->integralityChange[intChangeNdx] = newIntegralityMeasure - integralityMeasure;
// ucout << "Branchchoice = " << branchChoice << ", direction = " << direction << ", (old,new,delta) = (" << integralityMeasure << "," << newIntegralityMeasure << "," << newIntegralityMeasure - integralityMeasure<< ")\n";
}

  // For debugging (ramp up phase) only.  Processor sender will send
  // pseudocost information to processor comparer.  Comparer will
  // signal an error if there is any discrepancy.  For now this uses
  // the packing routine normally used for ramp up, and therefore there are
  // no counts shared.  Add that later if necessary.
void parMILP::comparePseudoCosts(int sender, int comparer)
{
#ifdef ACRO_VALIDATING
  int numInts = MIP->numIntegerVars();
  DoubleVector buffer(size_type(2*numInts));

  if (uMPI::rank == sender)
    {
    IntVector VarNdx(numInts);
    for (int i = 0; i < numInts; i++)
      VarNdx[(size_type)i] = i;
    packPseudoCosts(0, numInts-1, VarNdx, buffer.data());
    // Using 50 as a tag.  Just trying to pick something that isn't used.
    // Let's hope this doesn't run into problems
    uMPI::send(buffer.data(), 2*numInts, MPI_DOUBLE, comparer, 50);
    }
  else if (uMPI::rank==comparer)
    {
      MPI_Status myStatus;
      uMPI::recv(buffer.data(), 2*numInts, MPI_DOUBLE, sender, 50, &myStatus);
      for (size_type i = 0; i < (size_type)numInts; i++)
	if (buffer[2*i] != upCostBuffer[i] || buffer[2*i+1] != downCostBuffer[i])
	  EXCEPTION_MNGR(std::runtime_error, "Pseudocost buffer mismatch in int variable " << i << " in subproblem " << currentSP->id.serial << ". Proc " << sender << " vs " << comparer << " Up: " << buffer[2*i] << " vs " << upCostBuffer[i] << " Down: " << buffer[2*i+1] << " vs " << downCostBuffer[i] << ".\n");
    }
#endif
}


void parMILP::printAllStatistics(ostream &stream)
{
  branching::printAllStatistics(stream);
  printRampUpHeurStats(stream);
}


void parMILP::printRampUpHeurStats(ostream &stream)
{
  if (mipHeuristicPtr && (mipHeuristicPtr->rampUpCalls > 0))
    {
      int taggingWasActive = CommonIO::tagging_active();
      if (taggingWasActive)
	CommonIO::end_tagging();
      int grandSumDepth = uMPI::sum(mipHeuristicPtr->sumDepth);
      int sumSuccesses  = uMPI::sum(mipHeuristicPtr->rampUpSuccesses);
      if (uMPI::rank == uMPI::ioProc)
	{
	  int calls    = mipHeuristicPtr->rampUpCalls;
	  double denom = calls*uMPI::size;
	  stream << calls << " ramp-up call"
		 << plural(calls) << " to heuristic\n";
	  int oldPrecision = stream.precision(2);
	  stream.setf(ios::fixed,ios::floatfield);
	  stream << "Average depth of ramp-up pre-splitting:   "
		 << ((double) grandSumDepth)/denom
		 << endl;
	  stream.precision(0);
	  stream << "Success rate of heuristic during ramp-up: "
		 << 100*((double) sumSuccesses)/denom << '%'
		 << endl << endl;
	  stream.precision(oldPrecision);
	  if (taggingWasActive)
	    CommonIO::begin_tagging();
	}
    }
}


void parMILP::register_parameters()
{
branching::register_parameters();
plist.register_parameters<PicoLPSolver>();
plist.register_parameters<MILPProblem>();
plist.register_parameters<BBHConfig>();
plist.register_parameters<parMILP>();
}


void parMILP::solve()
{
printConfiguration(ucout);

try {

//
// Run the MILP solver
//
double run_start = CPUSeconds();
search();
searchTime = CPUSeconds() - run_start;
ucout << Flush;
CommonIO::end_tagging();
//
// Final I/O
//
 if (!performing_enumeration()) 
   {
     if (uMPI::iDoIO) 
       serialPrintSolValue();
     if (printFullSolution)
       printSolution("","\n",ucout);
   }

parallelBranching::solutionToFile();
printAllStatistics(ucout);

if (uMPI::iDoIO) {
   //
   // WRITE AMPL OUTPUT FROM MILP SOLUTION
   //
   #ifdef ACRO_USING_AMPL 
   if (running_with_ampl) {
      if (asl) {
         char msg[80];
         double ObjVal;
         ObjVal = incumbentValue;
         need_nl = 0;
         sprintf(msg,"PICO Solver: final f = %f\n",ObjVal);
         write_sol("", getIncumbent().data(), NULL, NULL);
         }
      }
   #endif
   }

cancelComm();

}
#if defined(COUGAR)
  MIP_CATCH( heap_summary(); MPI_Abort(MPI_COMM_WORLD,1); )
#else
  MIP_CATCH( MPI_Abort(MPI_COMM_WORLD,1); )
#endif

    }

void parMILP::getGlobalBound(double &returnBound, bool &boundKnown)
{
  returnBound = globalLoad.aggregateBound;
  if (returnBound > -MAXDOUBLE && returnBound < MAXDOUBLE)
    boundKnown = true;
  else boundKnown = false;
}

// Given a number of (ordered, say numbered) tasks to divide evenly
// and lexicographically among the processors (meaning the 0th processor gets the first
// chunk of work, processor 1 gets the second chunk, etc, determine what tasks
// this processor does.  Specifically determine the task number of the first task, the number
// of tasks to do, and, since some methods involving message passing need this, the maximum number
// assigned to any processor.

void parMILP::divideWorkLexicographically(size_type numTasks, size_type &firstJob, size_type &numberOfJobs,
				 size_type &maxNumberJobs, size_type &overage)
{
  int pid = uMPI::rank;
  int numProcs = uMPI::size;
  overage = numTasks % numProcs; // # of processors with higher # jobs
  maxNumberJobs = (int)ceil((double)numTasks/numProcs);
  firstJob = pid * maxNumberJobs;
  numberOfJobs = maxNumberJobs;
  if (overage != 0 && (size_type)pid >= overage)
    {
    numberOfJobs--;
    firstJob -= pid - overage;
    }
}




} // namespace pico

#endif
