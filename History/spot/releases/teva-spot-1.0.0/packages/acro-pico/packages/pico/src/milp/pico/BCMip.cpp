/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// BCMip.cpp
//
// Branch and cut code for MILP in PICO
//

#include <acro_config.h>
#include <utilib/nicePrint.h>
#include <pico/SNLKnapsack.h>
#include <pico/BCMip.h>
#include <pico/cutFinder.h>
#include <pico/cutGen.h>

using namespace std;
using namespace utilib;

namespace pico {

BasicArray<cutFinder*> BCMip::cutGenerators;

BCMip::BCMip() : numFullFindersBounds(0),
	    BCMIP(0), myRNG(0),myUniform(&myRNG), numCoreRows(0)
{
  disableBuiltInCuts = new BitArray(cutFinder::FIRST_APPLICATION_SPECIFIC);
  disableBuiltInCuts->reset(); // make sure all zeros to start
#ifdef ACRO_VALIDATING
  BCMIPTimingInfo.resize(numBCMIPTimingStates);
  for (int i = 0; i < numBCMIPTimingStates; i++)
    BCMIPTimingInfo[i] = 0.0;
#endif
};


void BCMip::reset(bool resetVB)
{

  // These are here for testing till we have a good command-line interface to control
  // which cut generators are on

  //    disableBuiltInCutFinder(cutFinder::CGL_gom);
  //    disableBuiltInCutFinder(cutFinder::CGL_MIR2);
  //  disableBuiltInCutFinder(cutFinder::CGL_2MIR);
  //  disableBuiltInCutFinder(cutFinder::CGL_FC);
  //  disableBuiltInCutFinder(cutFinder::CGL_C);

// These should be off by default

// Documentation says "this is designed for problems with
// few rows and many integer variables where the rhs are <= or == and
// all coefficients and rhs are small integers."  I don't know if the
// finder tests for applicability.  Till we know, turn it off.

    disableBuiltInCutFinder(cutFinder::CGL_DR);


// Returns cuts identical to CGL_MIR2, but uses sparse internal representation
// and therefore seems to run slower.  If space is an issue for a problem, then
// consider using this one instead of MIR2, but never together.

  disableBuiltInCutFinder(cutFinder::CGL_MIR);


// Currently appears to go into an infinite loop
  disableBuiltInCutFinder(cutFinder::SNL_KC);

  // Weird matrix norm assumption, expensive, doesn't really do lift and project (?)
  disableBuiltInCutFinder(cutFinder::CGL_LAP);

  // General belief in the MIP community that simple rounding and odd hole are dominated
  // by other cut finders
  disableBuiltInCutFinder(cutFinder::CGL_oh);
  disableBuiltInCutFinder(cutFinder::CGL_sr);

  // Not finding cuts in problems where it should.  Produces incorrect cuts for p0282.  We'll
  // hope that SNL knapsack cuts and the ones from MINTO can cover for these.
  disableBuiltInCutFinder(cutFinder::CGL_kc);



//
// If using_cuts is false, then disable all cuts
// (though I think they won't be used anyone because of checks elsewhere
//
if (!using_cuts) {
  disableBuiltInCutFinder(cutFinder::CGL_kc);
  disableBuiltInCutFinder(cutFinder::CGL_oh);
  disableBuiltInCutFinder(cutFinder::CGL_sr);
  disableBuiltInCutFinder(cutFinder::CGL_LAP);
  disableBuiltInCutFinder(cutFinder::CGL_P);
  disableBuiltInCutFinder(cutFinder::CGL_gom);
  disableBuiltInCutFinder(cutFinder::CGL_MIR);
  disableBuiltInCutFinder(cutFinder::CGL_MIR2);
  disableBuiltInCutFinder(cutFinder::CGL_2MIR);
  disableBuiltInCutFinder(cutFinder::CGL_FC);
  disableBuiltInCutFinder(cutFinder::CGL_C);
  disableBuiltInCutFinder(cutFinder::CGL_DR);
  disableBuiltInCutFinder(cutFinder::SNL_KC);
}

if (resetVB) 
   MILP::reset();
}



void BCMip::BCMIPInit()
{
loadedPoolMaxSize = (int) (loadedPoolSizeConstant +
  loadedPoolSizeColFactor * lp->getNumCols() +
  loadedPoolSizeRowFactor * lp->getNumRows());

cutBlockSize = (int) (cutBlockSizeConstant +
  cutBlockSizeColFactor * lp->getNumCols() +
  cutBlockSizeRowFactor * lp->getNumRows());

num_queue_objects = 0;
queue_total = 0;
sp_first_solve_time_est = 0.0;
numCoreRows = lp->getNumRows();
cutCounter = 0;
bestFinderQuality = 0.0;
 finderThreshold = 0.0; 
#ifdef ACRO_VALIDATING
// This assumes milp is initialized before BCMip
 numContainedWatchedPoints = 0;
 containedWatchedPointsInd.resize(solutionsToWatch.size());
 if (makeTimingsPseudoRandom)
   myRNG.reseed(TimingSeed);
#endif
}

void BCMipNode::makeActiveLP()
{
if (active)
  {
  MILPNode::makeActiveLP();
  return;
  }
#ifdef ACRO_VALIDATING
if (mGlobal()->checkFathoming())
  recordWatchedPoints();
#endif

// If we ever swap out a problem in the middle of bounding (during
// cuts), then we may need to call restoreLP() after the various calls
// below to reset the solution, etc, for CGL.  For now, a problem is
// activated at the start of bounding only (after branching on a non-root
// node), so it will be solved before any cutting.  Note, however, that
// currently the state is set to beingBounded at the very start of bounding,
// so that will be the state now (i.e. it will be hard to check for calling this
// multiple times in a bounding for the same node).

// Makes exactly this subproblem's cuts active (throws out others) and sets the
// basis using the stored basis for the original variables and rows
// and the stored information about basic slack variables for persistent and
// normal cuts.

// It's possible there's nothing to be done (e.g. if this is called
// from branching initialization that's invoked while trying to
// determine if it's time to branch (the LP will already be set up).
if (replaceCutsToActivate())  // currently if neither this node nor parent ran last
  // This should still work if we are branching on a cut and the parent was the last
  // to run.  In this case, this cut should be added to the LP at the time we make the
  // changes to create the child (presumably from a virtual method in milpNode).
  {
    // If we ever do this more intelligently, may want to pass in two sets of rows
    // directly to replaceRows()
  lp->replaceRows(persistent_cuts);
  lp->addRows(binding, binding.size());

  // Update the cut pools

  bcGlobal()->replaceLoadedCuts(persistent_cuts, binding);
  }
size_type num_cuts = binding.size();
for (size_type i = 0; i < num_cuts; i++)
  binding[i]->decrementRefCount();

num_cuts = persistent_cuts.size();
for (size_type i = 0; i < num_cuts; i++)
  persistent_cuts[i]->decrementRefCount();

  // We'll override the bound-setting and basis-setting routines in here
MILPNode::makeActiveLP();

// Throw away these pointers.  Active problem implicitly needs all loaded cuts.
// It'll acquire pointers to all binding cuts when it goes inactive.  This list was needed
// in the previous call for setting the basis.  We don't want these pointers while the problem
// is active because if it becomes dead and is deleted, it might incorrectly delete these cuts.
// Note: we keep pointers to persistent cuts (we have to identify them when the node goes inactive)
binding.resize(0);
basic_slack_cuts.resize(0);
active = true;
}


// For making a root

void BCMipNode::BCMipNodeFromBCMip(BCMip *master, bool initVB)
{
globalPtr = master;
if (initVB)
  MILPNodeFromMILP(master);
BCMipNodeInit();

// Temporary
/*
 cout << "Making a BCMipNode, globalPtr = " << globalPtr << ", milp is " << mGlobal() << ", branching is " << bGlobal() << "\n";
 cout.flush();
 MILPNode *mNode = this;
 branchSub *bNode = this;
 cout << "BCMipNode is " << this << ", as milpNode: " << mNode << ", as branchSub: " << bNode << "\n";
 cout.flush();
 cout << "bGlobals: " << mNode->bGlobal() << ", " << bNode->bGlobal() << "\n";
 cout.flush();
*/
}

// For making a nonroot

void BCMipNode::BCMipNodeAsChildOf(BCMipNode *parent, bool initVB) 
{
globalPtr = parent->bcGlobal();
if (initVB)
  MILPNodeAsChildOf(parent); // will initialize branchSub too
BCMipNodeInit();
}

void BCMipNode::BCMipNodeInit()
{
  // Only saving values for the variables (not slacks)
  full_solution.resize(lp->getNumCols());
  finderHistory.resize(bcGlobal()->numCutFinders());
  NumLPSolves = 0;
  lastFinder = NULL;
  branchQuality = 0.0;
  active = false;
}

//---------------------------------------------
// Create a child node
// For now the guts can be the same as for a MILPNode
//---------------------------------------------

pebbl::branchSub * BCMipNode::makeChild(int whichChild) 
{
MEMDEBUG_START_NEW("BCMipNode")
BCMipNode *child = new BCMipNode();
#ifdef ACRO_VALIDATING
 if (child == NULL)
   EXCEPTION_MNGR(runtime_error, "Failed to allocate a new BCMipNode\n");
#endif
child->BCMipNodeAsChildOf(this);
MEMDEBUG_END_NEW("BCMipNode")

MEMDEBUG_START_RESIZE("BCMipNode")
childInit(child, whichChild);
MEMDEBUG_END_RESIZE("BCMipNode")
return(child);
}

void BCMipNode::childInit(BCMipNode *child, int whichChild, bool initVB)
{
if (initVB)
  MILPNode::childInit(child, whichChild);
child->binding << binding;
size_type num_cuts = binding.size();
for (size_type i = 0; i < num_cuts; i++)
  binding[i]->incrementRefCount();
child->basic_slack_cuts << basic_slack_cuts;
 child->persistent_cuts << persistent_cuts;
num_cuts = persistent_cuts.size();
for (size_type i = 0; i < num_cuts; i++)
  persistent_cuts[i]->incrementRefCount();
child->persistent_basic_slacks << persistent_basic_slacks;
}

// Update the cut pools to reflect this new set of active cuts (all
// previous ones deleted).  At least for now, this is called when a
// subproblem is made active.  Each of these new cuts should have
// their reference count decremented whether they're moving from the cut
// pool or staying in the loaded pool (previous problem incremented
// this count before deactivating.  The two sets of cuts are
// persistent and regular.  There's some code repetition here.  We
// could get the same end result by doing replaceLoadedCuts(new_cuts1)
// and addLoadedCuts(new_cuts2), but then any already-loaded cuts in
// new_cuts2 would be deleted and then added back.  Easy to change if
// people feel the repetition is too ugly.

void BCMip::replaceLoadedCuts(BasicArray<picoRowCut *> &new_cuts1, BasicArray<picoRowCut *> &new_cuts2)
{
size_type i;
size_type num_new1 = new_cuts1.size();
size_type num_new2 = new_cuts2.size();
bool staying_loaded;

// Step through the loaded cut pool.  Any not on the list of new
// cuts have to move to the cutPool.

picoRowCut *curr_cut;
HashedSet<picoRowCut,false>::iterator stepper = loadedCuts.begin();
HashedSet<picoRowCut,false>::iterator end     = loadedCuts.end();
while (stepper != end)
  {
  curr_cut = &*stepper;
  staying_loaded = false;
  stepper++;
  for (i= 0; i < num_new1; i++)
    {
    if (new_cuts1[i] == curr_cut)
      {
      staying_loaded = true;
      break;
      }
    }
  if (!staying_loaded)
    {
    for (i= 0; i < num_new2; i++)
      {
      if (new_cuts2[i] == curr_cut)
	{
	staying_loaded = true;
	break;
	}
      }
    }
  if (!staying_loaded)
    unloadCut(curr_cut);
  }
// Add to the loaded pool if not already there.
// Remove from the cut pool if there.
// Note: reference count is decremented for this load in
// the method that calls this (for warm starts, this method isn't
// called, but still have to decrement, since active subproblems don't
// have pointers to regular cuts).

for (i = 0; i < num_new1; i++)
  {
  curr_cut = new_cuts1[i];
  if (!curr_cut->loaded())
    {
    curr_cut->setLoaded();
    loadedCuts.insert(*curr_cut);
    }
  if (curr_cut->inPool())
    {
    curr_cut->leavePool();
    cutPool.erase(*curr_cut);
    }
  }
for (i = 0; i < num_new2; i++)
  {
  curr_cut = new_cuts2[i];
  if (!curr_cut->loaded())
    {
    curr_cut->setLoaded();
    loadedCuts.insert(*curr_cut);
    }
  if (curr_cut->inPool())
    {
    curr_cut->leavePool();
    cutPool.erase(*curr_cut);
    }
  }
// for debugging
// ucout << "From replace cuts:\n";
//printCutDataStructures(); 
}

  // Update the loaded cut pool to reflect addition of these cuts
  // Also set the loaded flag in each of these cuts.

void BCMip::addLoadedCuts(BasicArray<picoRowCut *> &new_cuts, int num_cuts)
{
size_type i;
#ifdef ACRO_VALIDATING
size_type num_solutions = solutionsToWatch.size();
size_type j;
if (checkFathomOnSolution)
  {
  for (i=0; i < (size_type) num_cuts; i++)
    {
    if (new_cuts[i]->globallyValid())
      {
      for (j=0; j < num_solutions; j++)
	if (new_cuts[i]->violated(solutionsToWatch[j]))
	  {
	  new_cuts[i]->print();
	  ucout << "violation: " << new_cuts[i]->violation(solutionsToWatch[j]) << "\n";
	  ucout << "Violated point: " << solutionsToWatch[j] << "\n";
	  ucout.flush();
	  EXCEPTION_MNGR(runtime_error,"Adding a cut violated by a watched solution");
	  }
      }
    else if (numContainedWatchedPoints > 0)
      {
      for (j=0; j <  (size_type) numContainedWatchedPoints; j++)
	if (new_cuts[i]->violated(solutionsToWatch[containedWatchedPointsInd[j]]))
	  {
	  new_cuts[i]->print();
	  ucout << "violation: " << new_cuts[i]->violation(solutionsToWatch[containedWatchedPointsInd[j]]) << "\n";
	  ucout << "Violated point: " << solutionsToWatch[containedWatchedPointsInd[j]] << "\n";
	  ucout.flush();
	  EXCEPTION_MNGR(runtime_error,"Adding a cut violated by a watched solution");
	  }
		     
      }
    }
  }
#endif
for (i=0; i < (size_type) num_cuts; i++)
  {
  new_cuts[i]->setLoaded();
  // Assume we never actively add a cut that isn't binding (by itself) at the moment
  new_cuts[i]->nowBinding();
  loadedCuts.insert(*(new_cuts[i]));
  }
}

// Takes an LP solution x.  Scans the cut pool and places all
// cuts violated by x into the array violated_cuts.


void BCMip::scanCutPool(BasicArray<picoRowCut *>& violated_cuts,
			int &num_cuts,
			DoubleVector &x)
{
picoRowCut *curr_cut;
bool redundant;
HashedSet<picoRowCut,false>::iterator stepper = cutPool.begin();
HashedSet<picoRowCut,false>::iterator end     = cutPool.end();
num_cuts = 0;
size_type i;
while (stepper != end)
  {
  curr_cut = &*stepper;
  stepper++;
  if (curr_cut->violated(x))
    {
      // This cut will be loaded, so take it out of the pool of unloaded cuts
    curr_cut->leavePool();
    cutPool.erase(*curr_cut);
    // Check for redundancy.  This can go away if we do redundancy management when cuts enter the pool
    // TODO: remove this if we have other redundancy management
    redundant = false;
    for (i=0; i < (size_type) num_cuts; i++)
      {
      if ((*curr_cut).aligned(*violated_cuts[i]))
	{
	curr_cut->makeBothEquivalentToStrongest(violated_cuts[i]);
	// Coming from the pool. Can't be loaded.  So can delete
	// a redundant cut with no references.
	if (curr_cut->refCount() == 0)
	  delete(curr_cut);
	else if (violated_cuts[i]->refCount() == 0)
	  {
	  delete(violated_cuts[i]);
	  violated_cuts[i] = curr_cut;
	  }
	redundant = true;
	break; // Because we do this as we add each one, can ony have one redundant
	}
      }
    // If both cuts have references, we won't delete either one, but only return one.  The
    // other one is still around for the subproblems that need it.
    if (!redundant)
      {
      if ((int) violated_cuts.size() == num_cuts)
	violated_cuts.resize(num_cuts+1);
      violated_cuts[num_cuts++] = curr_cut;
      }
    }
  }
}


void BCMipNode::noLongerActiveLP()
{
  // basis will be stored in MILPNode routines
  // (we'll only use the ones for the original problem).
  // Store info about degenerate basic slacks for cuts
  // when we get the binding cuts.  Also store basis information
  // for the persistent cuts.
bcGlobal()->getBindingRows(persistent_cuts, persistent_basic_slacks, binding, basic_slack_cuts);
size_type num_cuts = binding.size();  // comes back exactly sized
for (size_type i = 0; i < num_cuts; i++)
  binding[i]->incrementRefCount();
num_cuts = persistent_cuts.size();  // comes back exactly sized
for (size_type i = 0; i < num_cuts; i++)
  persistent_cuts[i]->incrementRefCount();
 basis.resize(initialBasisSize());  //inactive problems don't explicitly represent cut slacks
active = false;
}

  // Remove a cut from the loaded-cuts pool.  Move it to the cut
  // pool if it's poolable, and otherwise, if its reference count is
  // zero, delete it.

void BCMip::unloadCut(picoRowCut* to_unload)
{
  loadedCuts.erase(*to_unload);
  to_unload->unload(); // change flag
  if (to_unload->poolable())
    {
    to_unload->enterPool();
    addToCutPool(to_unload);
    }
  // Non-poolable cuts are pointed to by the subproblems
  // for which they're binding, but they're not in the pool
  else if (to_unload->refCount() == 0)
    delete(to_unload);
}

// Add a cut to the pool.  If there is already a cut in the pool
// parallel to the new one, keep only one in the pool, make sure
// both have the strongest upper bound (rhs), and, if either has
// a zero reference count, delete it.

void BCMip::addToCutPool(picoRowCut *to_pool)
{
  //TODO!!  Handle parallel cuts already in pool.  Need to get
  // iterator for entries with same hash value.  Bill is adding to utilib.
cutPool.insert(*to_pool);
}


// unload a set of cuts (see unloadCut) for effects.  Only for pool
// management

void BCMip::unloadCuts(BasicArray<picoRowCut *> &to_unload)
{
size_type num_to_remove = to_unload.size();
for (size_type i = 0; i < num_to_remove; i++)
  unloadCut(to_unload[i]);
}


// Put all the binding (loaded) cuts into cutlist.  Put the index
// (into cutlist) of all those with degenerate basic slack variables
// into degenerate_list.  Binding cuts have zero slack.  BasicArrays
// are sized exactly afterward (assumed, eg, by the PICO cut
// management piece in the LP interface).  We no longer assume the basis
// in the LP interface is valid (store degeneracy info with the cuts
// immediately after the solve).  Pseudocost initialization, eg, when
// determining branching readiness, made basis invalid sometimes.
// Also record this basis information (slackness) for persistent cuts.

void BCMip::getBindingRows(BasicArray<picoRowCut*>& persistent_cuts,
			   BasicArray<int>& basic_persistent_list,
			   BasicArray<picoRowCut*>& cutlist,
			   BasicArray<int>& degenerate_list)
{
cutlist.resize(loadedCuts.size());
degenerate_list.resize(0);
basic_persistent_list.resize(0);
int num_binding = 0;
int num_degenerate = 0;
size_type num_persistent = persistent_cuts.size();
int num_persistent_basic = 0;
size_type i;

picoRowCut *curr_cut;
HashedSet<picoRowCut,false>::iterator stepper = loadedCuts.begin();
HashedSet<picoRowCut,false>::iterator end     = loadedCuts.end();
while (stepper != end)
  {
  curr_cut = &*stepper;
  stepper++;
  if (curr_cut->isPersistent())
    {
    if (!curr_cut->isBinding() || curr_cut->isDegenerate()) // row slack is basic
      {
      // Find the index in the array of persistent cuts
      for (i=0; i < num_persistent; i++)
	{
	if (persistent_cuts[i] == curr_cut)
	  break;
	}
#ifdef ACRO_VALIDATING
      if (i == num_persistent) // didn't find the cut
	EXCEPTION_MNGR(runtime_error,"Persistent cut for problem going inactive is not in the persistent cut array\n");
#endif
      num_persistent_basic++;
      basic_persistent_list.resize(num_persistent_basic);
      basic_persistent_list[num_persistent_basic-1] = i;
      }
    }
  else if (curr_cut->isBinding())
    {
    if (curr_cut->isDegenerate())
      {
      num_degenerate++;
      degenerate_list.resize(num_degenerate);
      degenerate_list[num_degenerate-1] = num_binding;
      }
    cutlist[num_binding++] = curr_cut;
    }
  }
cutlist.resize(num_binding);
}

// Add these cuts to the array of persistent cuts and mark
// them as persistent.  We assume all cuts are either persistent or
// regular in all subproblems to which they apply.

void BCMipNode::addPersistentCuts(BasicArray<picoRowCut *> newCuts)
{
  size_type num_new = newCuts.size();
  size_type num_old = persistent_cuts.size();
  size_type new_total = num_old + num_new;
  if (num_new == 0) return;
  persistent_cuts.resize(new_total);
  for (size_type i = 0; i < num_new; i++)
    {
    newCuts[i]->makePersistent();
    persistent_cuts[num_old + i] = newCuts[i];  
    }
}


// Create all the cut finders.  Creation methods for candidates
// are stored in the static array available.

void BCMip::preprocess()
{
  min_runtime = timerGranularitySeconds();
  MILP::preprocess();
  size_type num_candidates = (available ? available->size() : 0);
  // candidates plus cut pool plus SNL knapsack cuts
  cutGenerators.resize(num_candidates + 2);
  size_type num_finders = 0;
  if (!(*disableBuiltInCuts)(0))
    {
    cutGenerators[0] = new poolScanner(this);
    (cutGenerators[0])->finderType = POOLSCANNERTYPE;
    num_finders++;
    }
  cutFinder *new_finder;
  for (size_type i = 0; i < num_candidates; i++)
    {
    new_finder = (*((*available)[i]))(this);
    // These are all built-in cuts, so the type will be in the range for cutTypeID
    if (new_finder != NULL && !(*disableBuiltInCuts)((cutFinder::cutTypeID) new_finder->cutType()))
      {
      new_finder->finderType = (int) num_finders;
      cutGenerators[num_finders++] = new_finder;
      }
    }
  // Built-in SNL cuts
  if (!(*disableBuiltInCuts)(cutFinder::SNL_KC))
    {
      SNLKnapsack_finder = new SNLKnapsack(this);
      if (SNLKnapsack_finder->instanceApplies())
	{
	  SNLKnapsack_finder->finderType = (int) num_finders;
	  cutGenerators[num_finders++] = SNLKnapsack_finder;
	}
    }
  // This won't be used again.

  delete disableBuiltInCuts;
  disableBuiltInCuts = NULL;

  // downsize if necessary
  if (num_finders < num_candidates +1)
    cutGenerators.resize(num_finders);
  cutQuality.resize(num_finders);
  for (size_type i = 0; i < num_finders; i++)
    cutQuality[i].newSPAvgQuality = cutQuality[i].AvgQuality = cutGenerators[i]->qualityEstimate();
  lastRunTime.resize(num_finders);
  // May need more or less, but a good place to start
  cutQueue.resize(num_finders);
  // utilib should initialize this to zero everywhere
  finderSchedBias.resize(num_finders);

  // Set up the cut documentation strings for built-in cuts
 if (cutTypeStrings == NULL)
   cutTypeStrings = new BasicArray<char *>(cutFinder::FIRST_APPLICATION_SPECIFIC);
 // Put in the strings we know if "by hand"
  (*cutTypeStrings)[0] = "Any";
  (*cutTypeStrings)[1] = "CglKnapsackCover";
  (*cutTypeStrings)[2] = "CglOddHole";
  (*cutTypeStrings)[3] = "CglSimpleRounding";
  (*cutTypeStrings)[4] = "CglGomory";
  (*cutTypeStrings)[5] = "CglLiftAndProject";
  (*cutTypeStrings)[6] = "CglProbing";

  // Check for the timer granularity one more time, just in case there
  // were coincidental zeros on the first call.  The MILP preprocess
  // should take some time, so this this should be at least a second
  // after the last call.

double timer_check = timerGranularitySeconds();
if (timer_check < min_runtime)
  min_runtime = timer_check;
}


PicoLPInterface::problemState BCMipNode::resolveLP(bool &continue_cutting,
						   double &solution_time)
{
finderHistory.reset();
double start_time = CPUSeconds();
  // This will get the solution, frac vars, update bound
PicoLPInterface::problemState solvedState = boundingSolve();
continue_cutting = true;

// Note: we are not enforcing a minimum run time.  Depending on the
// timer granularity, the solution time could be 0.0
solution_time = CPUSeconds() - start_time;
#ifdef ACRO_VALIDATING
if (bcGlobal()->makeTimingsPseudoRandom)
  solution_time = bcGlobal()->myUniform();
#endif

if (solvedState != PicoLPInterface::solved)
  {
  continue_cutting = false; // infeasible or past cutoff
  return(solvedState);
  }
if (candidateSolution())
  {
  continue_cutting = false;
  // If we're performing enumeration, this node will live on after we
  // record the candidate solution and add an enumeration cut.
  // Therefore, in case the problem goes inactive in the meantime, we
  // do need to record binding cuts, etc.  If we're not enumerating,
  // this node will die very soon.  No need to perform the extra work.

  if (!mGlobal()->performing_enumeration())
    return(PicoLPInterface::solved);  // This region is determined
  }
 if (bcGlobal()->ageCuts())
   {
     // ageCuts removed rows. Restore the LP solver for CGL routines.
     // Make sure the basis is OK.  If restoreLPSolver changes, might
     // need to change here.  This is charged to all the cut generators
     // that contributed this time.
   lp->solve(true);
   getBasis(basis);
   }

 // We should only reach this point if this is a candidate solution and
 // we're enumerating.  If we're not applying cut finders, no need to
 // compute branching measures.
 if (!continue_cutting)
   return(PicoLPInterface::solved);

NumLPSolves++;

// Update quantities used to determine when it's time to branch.  This
// can cause pseudocost initialization, so we suppress this update
// when we're in the run-all-finders start-up phase and this is just
// the first solve on the problem.  The first solve just computes the
// effect of the branch (or is the root solve).  If all cut finders
// will run, we don't need branch quality information to set up the
// scheduler or for branching to compete vs. the finders on a new
// problem.  Since we currently always resolve after a run-all-finders
// phase, we'll be back here before we need branch quality
// information. Because the set of fractional variables may change
// after cutting and subsequent resolve, by avoiding this step, we may
// avoid unnecessary pseudocost initialization.

if (!newSubproblem() || !bcGlobal()->shouldRunAllFinders())
  {
  if (!computeBranchQuality()) // not feasible
    {
    continue_cutting = false;
    return(PicoLPInterface::infeasible);
    }
  if (branchQuality == MAXDOUBLE)
 // We really want to branch.  This happens when there's a fractional variable
    // that has infinite pseudocosts (from infeasibilities) in both up and down directions.
    {
      continue_cutting = false;
      return(PicoLPInterface::solved);
    }
  }
 computeBranchReadiness();

return(PicoLPInterface::solved);
}

  // Called after a resolve during cutting.  Age nonbinding cuts.
  // Remove if they're old enough. Return true if cuts were removed
  // and false otherwise.

bool BCMip::ageCuts()
{
#ifdef ACRO_VALIDATING
  double ACStart = CPUSeconds();
#endif
picoRowCut *curr_cut;
//double denom = solMagnitudeEst();
//denom = max(denom, 1.0);
HashedSet<picoRowCut,false>::iterator stepper = loadedCuts.begin();
HashedSet<picoRowCut,false>::iterator end     = loadedCuts.end();

//double price;
// Will go away when we can delete a single row.
BasicArray<picoRowCut *> to_delete;
int num_to_delete = 0;
while (stepper != end)
  {
  curr_cut = &*stepper;
  stepper++;

  // Old version from using scaled dual, but would allow removal
  // of some binding cuts
  //  price = lp->getRowDual(curr_cut);
  //  price = abs(price) * curr_cut->norm()/denom;
  //  curr_cut->putScaledDual(price);
  //  if (price < BindTolerance())
  
  if (lp->getSlackVal(*curr_cut) > BindTolerance)
    {
    if (!curr_cut->isPersistent())
      curr_cut->incAge();
    curr_cut->nowNotBinding();
    curr_cut->nowNotDegenerate();
    if (curr_cut->getAge() > NonBindLimit)
      {
      to_delete.resize(num_to_delete + 1);
      to_delete[num_to_delete++] = curr_cut;
      }
    }
  else
    {
    curr_cut->nowBinding();  // In case a cut becomes binding again.
    curr_cut->resetAge();
    if (lp->getBasis(*curr_cut) == basic)
      curr_cut->nowDegenerate();
    else curr_cut->nowNotDegenerate();
    }
  }
// Have to delete these from the LP all at the end.  Otherwise,
// we may invalidate the getSlackVal() method needed to determine
// binding rows.
if (num_to_delete > 0)
  {
  lp->deleteRows(to_delete); 

// Can't do this till after removed from LP (because could delete the cut)
  unloadCuts(to_delete);
#ifdef ACRO_VALIDATING
  BCMIPTimingInfo[cutOverhead] += CPUSeconds() - ACStart;
#endif
  return(true);

// for debugging
  //      ucout << "From ageCuts:\n";
  //      printCutDataStructures();
  }
#ifdef ACRO_VALIDATING
  BCMIPTimingInfo[cutOverhead] += CPUSeconds() - ACStart;
#endif
 return(false); // no cuts removed
}

// For each set of cuts in the cut queue, update the
// finder quality measures.  
// Assumes that every cut generator that ran had a nonzero runtime.
// Otherwise, we'll get divide by zero errors.

void BCMip::updateCutFinderQuality(DoubleVector &x, double time_to_add, bool firstSolveForSubproblem)
{
  // The whole guts of the loop can be pulled into a method
  // to update a single cutQObject if necessary
int j;
int num_cuts;
double this_time; // estimate of the cost of resolving the LP for a cut set
double total_cut_effect, this_average_quality;
picoRowCut *curr_cut;
int finderID;
for (int i = 0; i < num_queue_objects; i++)
  {
  finderID = (cutQueue[i].getFinderUsed())->type();
  total_cut_effect = 0.0;
  num_cuts = cutQueue[i].num_cuts;
  if (num_cuts == 0)
    this_time = 0.0;
  else this_time = time_to_add * num_cuts/queue_total;
  this_time += (cutQueue[i]).getTime();
  for (j=0; j < num_cuts; j++)
    {
    curr_cut = ((cutQueue[i]).getCutContainer())[j];
    total_cut_effect += fabs(lp->getRowPrice(*curr_cut)) *
      curr_cut->violation(x);
    }

  this_average_quality = total_cut_effect/this_time;

  // Update summary information
  cutQuality[finderID].numTimesCalled++;
  cutQuality[finderID].numCutsFound += num_cuts;
  cutQuality[finderID].totalObjectiveGain += total_cut_effect;
  cutQuality[finderID].totalTime += this_time;

  recomputeCutFinderQualityAverages(finderID, this_average_quality, firstSolveForSubproblem);
  }
}

// Assumes the number of calls has already been incremented.

void BCMip::recomputeCutFinderQualityAverages(size_type finderID, double new_average,
					      bool newSubproblem)
{
// If the cut generator has never been called and the finder didn't
// override the default 0.0 from initializaiton, only use the new cuts
// (don't weigh in the arbitrary zero from initialization).

  // Set q_f, quality estimate

  double old_average = cutQuality[finderID].AvgQuality;
  if (cutQuality[finderID].numTimesCalled == 1 && old_average == 0.0)
    {
    cutQuality[finderID].AvgQuality = new_average;
    cutQuality[finderID].newSPAvgQuality = new_average;
    }
  else
    {
    cutQuality[finderID].AvgQuality =
      (1 - qualSmoothFactor) * old_average +
      qualSmoothFactor * new_average;

   // update similar measures if this is the first call to the finders for
   // this subproblem.

    if (newSubproblem)
      cutQuality[finderID].newSPAvgQuality =
	(1 - firstQualSmoothFactor) * cutQuality[finderID].newSPAvgQuality +
	firstQualSmoothFactor * new_average;
    }
}

void BCMip::updateFinderQualityThreshold(BCMipNode *curr_node)
{
  bestFinderQuality = 0.0;
  size_type numFinders = cutGenerators.size();
  double this_quality;
  for (size_type i = 0; i < numFinders; i++)
    {
      if (cutGenerators[i] == NULL)  continue;
      this_quality = (cutGenerators[i])->readiness(curr_node) * cutQuality[i].AvgQuality;
      if (this_quality > bestFinderQuality)
	bestFinderQuality = this_quality;
    }
  finderThreshold = max(curr_node->branchingReadyMeasure(),
			bestFinderQuality * finderCompetitiveFactor);
}

  // Update quality measures after a cut finder has determined a subproblem is infeasible

void BCMip::updateCutFinderQualityFromInfeasible(size_type finderID, double this_time,
						 double SPBound, bool newSubproblem)
{
  double cut_quality;
  if (haveIncumbent())
    cut_quality = fabs(incumbentValue - SPBound);
  else
    // No incumbent.  Try to guess a bound movement in the right order
    // of magnitude.  This even makes sense for a maximization with a
    // positive bound where this is acting like the incumbent is 0.
    {
    if (SPBound == 0.0)
      cut_quality = 1.0;
    else cut_quality = SPBound;
    }
  // Extra factor to give a bonus for finding infeasibility beyond full
  // gap closure for the subproblem.
  cut_quality *= infeasibilityDetectionBonus;
  cutQuality[finderID].numTimesCalled++;
  cutQuality[finderID].totalTime += this_time;
  // Count this as an infeasibility detection, but not as a cut
  cutQuality[finderID].numInfeasibilityDetections++;
  // Count the quality in the total, though
  cutQuality[finderID].totalObjectiveGain += cut_quality;
  recomputeCutFinderQualityAverages(finderID, cut_quality/this_time, newSubproblem);
}


// Update the measure of the benefit of branching at this point.
// This can change anytime the LP is solved.  Return false if the
// problem was determined to be infeasible during pseudocost init.
// Otherwise return true.

bool BCMipNode::computeBranchQuality()
{
branch_type dummy;
double minBenefit;
int branchVar = chooseBranchVariable(dummy, minBenefit);
if (branchVar == -1)
  {return(false);}

// This has to be fixed.  The term after minBenefit is to make branchingQuality nonzero,
// so there's a chance of branching even if all the gradients are zero.  The estimate
// of solution magnitude can be zero if there's no incumbent and the global bound is zero
// (occurs with miplib problem enigma).  We may want to rethink that second term.

 if (minBenefit == MAXDOUBLE)
   branchQuality = MAXDOUBLE;
 else branchQuality = (minBenefit +
		 bcGlobal()->branchPossibleFactor * bcGlobal()->solMagnitudeEst() + 1)/
                 (numChildren * bcGlobal()->spFirstSolveTimeEst());
return(true);
}

double BCMip::solMagnitudeEst()
{
double gBound;
bool boundKnown;
getGlobalBound(gBound, boundKnown);
if (!boundKnown) gBound = currentSP->bound; 
if (problemFeasible) // Have a valid incumbent
  return(max(fabs(incumbentValue), fabs(gBound)));
 else return(fabs(gBound));
}

// Add nonredundant cuts from the queue to the LP and loaded pool.  Return the
// number of cuts added.  If multiple finders have found the same cut, removes the redundancy and
// gives credit to the finder that found the tightest cut (if the bounds aren't
// the same).  For ties, credit goes to the fastest finder.  This assumes that the only finder that
// can return cuts with a positive reference count is the cut pool scanner.  That is, we assume all
// other finders are making new cuts.  If a finder generates a cut redundant with the cut pool and
// wins (stronger or faster), then we use the old physical cut, updated with the new rhs, and the
// correct finder gets credit.

int BCMip::incorporateCuts()
{
if (queue_total == 0) return(0);

#ifdef ACRO_VALIDATING
 double ICStart = CPUSeconds();
#endif

// Eliminate redundancy.  Especially in first sweep, multiple cut
// finders could find the same cut.  Redundancy within cut finders
// should already have been eliminated at this point.  There should be
// no redundancy with cuts that are loaded (since otherwise these cuts
// wouldn't be violated).

size_type numNonRedundant = 0;
BasicArray<picoRowCut *> nonRedundant(queue_total);
// The next array matches the nonRedundant array, giving the queue
// object that holds each non-redundant cut.  For example, if
// nonRedundantSourceQueue[i] = 4, then the ith nonredundant cut is
// from 5th cutQueueObject (in a zero-based system). We need this to
// remove the redundancy if a later cut dominates.
BasicArray<size_type> nonRedundantSourceQueue(queue_total);
bool redundant;
size_type i,j, k, num_cuts;
picoRowCut *currCut;
BasicArray<picoRowCut *>  *tmp_cut_container;
int compare_result;

for (i = 0; i < (unsigned) num_queue_objects; i++)
  {
  num_cuts = cutQueue[i].num_cuts;
  tmp_cut_container = &cutQueue[i].new_cuts;
  for (j=0; j < num_cuts; j++)
    {
    redundant = false;
    currCut = (*tmp_cut_container)[j];
    for (k=0; k<numNonRedundant; k++)
      {
      if ((*currCut).aligned(*nonRedundant[k]))
	{
	  // parallel cuts. Find and keep strongest.
	compare_result = (*currCut).compareAligned(*nonRedundant[k]);
	if (compare_result < 0 || (compare_result == 0 &&
				   cutQueue[i].getAverageTime() <
				      cutQueue[nonRedundantSourceQueue[k]].getAverageTime()))
	  {
	    // This cut is better
	    DEBUGPR(10, ucout << "Cut " << j << " from queue " << i << " is replacing a cut from queue " <<  nonRedundantSourceQueue[k] << "\n";);
	    // Cuts from the cut pool could have non-zero reference count.  Cut pool loses credit, but
	    // this cut (with improved rhs) stays.
	  if (cutQueue[nonRedundantSourceQueue[k]].getFinderUsed()->type() == POOLSCANNERTYPE &&
	      nonRedundant[k]->refCount() > 0)
	    {
	    cutQueue[nonRedundantSourceQueue[k]].releaseCut(nonRedundant[k]);
	    nonRedundant[k]->convertTo(currCut);
	    cutQueue[i].replaceCut(j, nonRedundant[k]);
	    delete(currCut);
	    }
	  else
	    {
	    cutQueue[nonRedundantSourceQueue[k]].removeCut(nonRedundant[k]);
	    nonRedundant[k] = currCut;
	    nonRedundantSourceQueue[k] = i;
	    }
	  }
	else
	  {
	  if (cutQueue[i].getFinderUsed()->type() == POOLSCANNERTYPE &&
	      currCut->refCount() > 0)
	    {
	    currCut->convertTo(nonRedundant[k]);
	    cutQueue[i].releaseCut(j);
	    cutQueue[nonRedundantSourceQueue[k]].replaceCut(nonRedundant[k], currCut);
	    delete(nonRedundant[k]);
	    nonRedundant[k] = currCut;
	    }
	  else
	    cutQueue[i].removeCut(j);
	  // This should continue the current loop correctly
	  num_cuts--;
	  j--;
	  }
	redundant = true;
	break;
	}
      }
    if (!redundant)
      {
      nonRedundant[numNonRedundant] = currCut;
      nonRedundantSourceQueue[numNonRedundant++] = i;
      }
    }
  }
queue_total = numNonRedundant;

#ifdef ACRO_VALIDATING
 DEBUGPR(10, ucout << "Found " << queue_total << " cuts.  Incorporating " << numNonRedundant << " nonredundant cuts:\n";);

 if (debug >= 10)
   {
     // Determine how many cuts of each type will be added
     IntVector finderCounts(num_queue_objects);
     for (i=0; i < numNonRedundant; i++)
       finderCounts[nonRedundantSourceQueue[i]]++;
     for (i=0; i < (unsigned) num_queue_objects; i++)
       if (finderCounts[i] > 0)
	 ucout << "\t" << finderCounts[i] << " " << cutQueue[i].getFinderUsed()->typeString() << "\n";
   }
// All cuts must only have nonzero coefficients for permanent variables.
// Verify this is true.

 int numElements;
 const int *thisRowIndices;
 int numPermanentVars = MIP->numIntegerVars() + numCoreRows;

 for (i = 0; i < numNonRedundant; i++)
   {
     numElements = nonRedundant[i]->lpform().getNumElements();
     thisRowIndices = nonRedundant[i]->lpform().getIndices();
     // Should be sorted and zero based. Check the largest index.
     if (thisRowIndices[numElements-1] >= numPermanentVars)
       EXCEPTION_MNGR(runtime_error, "Cut is using nonpermanent variable "
		      << thisRowIndices[numElements-1] << " with " << numPermanentVars
		      << " permanent variables.  Implement substitution\n");
   }

// ucout << "Incorporating " << queue_total << " cuts\n";
#endif
addCuts(nonRedundant, numNonRedundant);
#ifdef ACRO_VALIDATING
 BCMIPTimingInfo[cutOverhead] += CPUSeconds() - ICStart;
#endif
//ucout << "From incorporate cuts\n";
//printCutDataStructures();
return(numNonRedundant);
}

// Removes cut redundancy from a single cut finder (certainly an issue with CGL
// finders, or any finder that doesn't police itself).  This is separate from the
// redundancy elimination from BCMip::incorporateCuts because there's no need to
// assign credit.  Just keep the best.  Run this as we go along to avoid keeping a
// lot of redundant cuts around (a minor efficiency improvement).

void BCMip::removeOneFinderCutRedundancy(int queue_index)
{
  size_type num_cuts = cutQueue[queue_index].num_cuts;
  size_type i,j;
  picoRowCut *currCut;
  if (num_cuts <= 1) return;
  int compare_result;
  BasicArray<picoRowCut *> *returned_cuts = &cutQueue[queue_index].new_cuts;
  for (i = 0; i < num_cuts; i++)
    {
    currCut = (*returned_cuts)[i];
    for (j=i+1; j < num_cuts; j++)
      {
      if ((*currCut).aligned(*(*returned_cuts)[j]))
	{
	  // parallel cuts. Find and keep strongest.
	compare_result = (*currCut).compareAligned(*(*returned_cuts)[j]);
	if (compare_result > 0)
	  {
	    // cut in jth position strictly dominates this one.
	    // swap before deleting
	  (*returned_cuts)[i] = (*returned_cuts)[j];
	  (*returned_cuts)[j] = currCut;
	  currCut = (*returned_cuts)[i];
	  }
	// This will also decrement the cutQueue object's cut count (num_cuts)
	cutQueue[queue_index].removeCut(j);
	// Another cut will take the place of the deleted one (unless that was the last).
	// Make sure we pick up the search from there.
	num_cuts--;
	j--;
	}
      }
    }
}

  // Add cuts from the cut queues to the current problem and resolve the LP.

PicoLPInterface::problemState BCMipNode::incorporateCuts(bool &continue_cutting)
{
  // Adds all waiting cuts, updates pools
int num_added = bcGlobal()->incorporateCuts();
// Keep a copy of the old solution.  Needed to compute cut finder quality
// (to see how much the old solution violated the cuts).  Should be a copy constructor.
DoubleVector old_solution(full_solution);
continue_cutting = true;
double resolve_time = 0.0;
PicoLPInterface::problemState resolveState = PicoLPInterface::broken;
if (num_added > 0)
  {
  resolveState = resolveLP(continue_cutting, resolve_time);
#ifdef ACRO_VALIDATING
 bcGlobal()->BCMIPTimingInfo[resolving] += resolve_time; 
#endif
  }
// Used to return lp->state(), but this can become unsolved
// during pseudocost initialization as part of resolve
else
  {
    // A side-effect of resolve.  Have to make sure this is initialized if no
    // cut finder returns a cut in a sweep.
  if (!computeBranchQuality())
    continue_cutting = false;
  else resolveState = PicoLPInterface::solved;
  }
bcGlobal()->updateCutFinderQuality(old_solution, resolve_time, newSubproblem());
bcGlobal()->updateFinderQualityThreshold(this);
bcGlobal()->resetCutQueue();
return(resolveState);
}

// Adds cuts to the LP and updates the pools. If adding these cuts puts us over
// the loaded-cut limit, remove nonbinding cuts in order of current scaled dual.

void BCMip::addCuts(BasicArray<picoRowCut *> &new_cuts, int num_cuts)
{
// Throw out some nonbinding cuts if necessary (easier to do this all
// up front)
if (loadedCuts.size() + num_cuts > (unsigned) loadedPoolMaxSize)
  trimLoadedPool(loadedCuts.size() + num_cuts - loadedPoolMaxSize);
addLoadedCuts(new_cuts, num_cuts);
lp->addRows(new_cuts, num_cuts);
}

// Remove this many nonbinding cuts if possible.  Otherwise, leave
// only binding cuts active.

void BCMip::trimLoadedPool(int num_to_remove)
{
BasicArray<picoRowCut *>to_trim(num_to_remove);
picoRowCut *curr_cut;
size_type num_found = 0;
HashedSet<picoRowCut,false>::iterator stepper = loadedCuts.begin();
HashedSet<picoRowCut,false>::iterator end     = loadedCuts.end();
while((stepper != end) && num_to_remove > 0)
  {
  curr_cut = &*stepper;
  stepper++;
  if (!curr_cut->isBinding() && !curr_cut->isPersistent())
    {
    to_trim[num_found++] = curr_cut;
    num_to_remove--;
    }
  }
// temporary debugging output.  REMOVE.
// ucout << "Attempting to trim " << num_to_remove << "loaded cuts.  Removing " << num_found << "\n";
if (num_found == 0)  return;
to_trim.resize(num_found);
lp->deleteRows(to_trim); 
// Can't do this till after removed from LP (because could delete the cut)
unloadCuts(to_trim);
}

PicoLPInterface::problemState BCMipNode::boundComputationGuts()
{
  //    ucout << "Bounding problem " << id << "\n";

bool continue_cutting, problemInfeasible;

PicoLPInterface::problemState this_state;

// For now, this method is called only when the problem has just been created
// or just had an enumeration cut added.
// Need to do the first bounding before any cutting.

double resolve_time;
this_state = resolveLP(continue_cutting, resolve_time);

 if (this_state != PicoLPInterface::solved)
   {
   return(this_state);
   }
#ifdef ACRO_VALIDATING
 bcGlobal()->BCMIPTimingInfo[initialBound] += resolve_time;
#endif
 if (isRoot() && onlyRootHeuristic)
   {
     incumbentHeuristic();
     setState(dead);
     return(this_state);
   }
 if (!bcGlobal()->using_cuts)  return(this_state);

// The only time we won't record the time to do this first
// post-branching solve is for the root.  The initial root
// solve can be far more expense than a resolve after a branch.

if (!isRoot())
  bcGlobal()->updateSPFirstSolveTimeEst(resolve_time);

if (!continue_cutting)
  return(this_state);

// Note: finder history is reset in the call to resolveLP(), so
// no need to do it here.

// This could be done much less frequently, but it's also
// not too expensive  TODO: consider doing less frequently
bcGlobal()->resetSchedBias();

//  If this is early enough in the computation, start by running all the cut
//  finders that have positive readiness (later go straight to competition)
 if (bcGlobal()->shouldRunAllFinders())
   {
   bcGlobal()->runAllFinders(this, finderHistory, problemInfeasible);
   bcGlobal()->postRunAllFinders(problemInfeasible);
   if (problemInfeasible) // cut finder found infeasibility
     {
     bcGlobal()->cleanUpAfterInfeasibility(bound, newSubproblem());
     return(PicoLPInterface::infeasible);
     }
   this_state = incorporateCuts(continue_cutting);
   if (!continue_cutting)
     return(this_state);
   }

 // set quality measures for the scheduler based on average quality on
 // new subproblems.

 else
   {

   bcGlobal()->newSPSetCutQual(branchQuality);

   // Reset the information used in competitiveness thresholds after the above resolve.
   bcGlobal()->updateFinderQualityThreshold(this);
   }

// Now let the cut finders compete to run.  If this check returns
// false (and hence we'll stop), then the check incorporates all the
// final cuts.

while (bcGlobal()->canStillCut(this, this_state))
  {
  lastFinder = bcGlobal()->runOneFinder(this, problemInfeasible);
  finderHistory.set(lastFinder->type());
  if (problemInfeasible) // cut finder found infeasibility
    {
    bcGlobal()->cleanUpAfterInfeasibility(bound,newSubproblem());
    return(PicoLPInterface::infeasible);
    }
  // TODO: Do we also want to do this after the pool?
  if (bcGlobal()->cutQueueBig())
    {
    this_state = incorporateCuts(continue_cutting);
    if (!continue_cutting)
      return(this_state);
    }
  }

return(this_state);
}

  // Run all finders that have positive readiness

// We avoid code duplication by using runFinder(...) in the main loop.
// We might get more efficiency by knowing we'll do them all in order
// (we have to chase down pointers to get information in
// runFinder(...)), however, since this code is still evolving and
// getting larger, let's have it all in one place.

void BCMip::runAllFinders(BCMipNode *curr_problem,
			  BitArray &finders_run,
			  bool &problemInfeasible)
{
size_type num_finders;
size_type i;
cutFinder *curr_finder;
#ifdef ACRO_VALIDATING
if (num_queue_objects > 0 || queue_total > 0)
  EXCEPTION_MNGR(runtime_error, "BCMip::runAllFinders expected clean cut queue");
#endif
 num_finders = cutGenerators.size();
 problemInfeasible = false;
 for (i = 0; i < num_finders; i++)
   {
   curr_finder = cutGenerators[i];
   if (curr_finder == NULL) continue;
   // uses a tolerance
   if (isZero(curr_finder->readiness(curr_problem)))  continue;
   runFinder(curr_finder, curr_problem, problemInfeasible);
   if (problemInfeasible)
     return;
   }
}

// Assume the cutFinder passed in has positive readiness
void BCMip::runFinder(cutFinder *curr_finder, BCMipNode *curr_problem, bool &problemInfeasible)
{
if (cutQueue.size() == (size_type)num_queue_objects)
  cutQueue.resize(num_queue_objects + 1);

/*
 ucout << "Calling " << curr_finder->cutName() << " for problem " << curr_problem->id << ". Number of rows is " << lp->getNumRows() << ", Basis is \n";
 curr_problem->printBasis();
 ucout << "\n";
*/

double start_time = CPUSeconds();
curr_finder->generateCuts(*lp, cutQueue[num_queue_objects].getCutContainer(),
			  cutQueue[num_queue_objects].num_cuts, curr_problem->x(),
			  problemInfeasible);
 if (curr_finder->LPNeedsResolve() && !problemInfeasible)
   curr_problem->restoreLPSolver();
 // This charges the finder for the resolve if it's necessary
double end_time = CPUSeconds();
// Temporary for development
/*
 ucout << "Cut Finder " << curr_finder->cutName() << " returned " << cutQueue[num_queue_objects].num_cuts << " cuts before redundancy removal\n";
 cutQueue[num_queue_objects].printCuts();
*/

// Note, if you want the behavior of removeOneFinderCutRedundancy to depend on the finder
// (e.g. warning of redundancy in the pool), you'll have to call this after the finder is
// set in the queue object.
// TODO: this is probably wrong if the pool is disabled!  Fix finder ID!
 if (curr_finder->type() != POOLSCANNERTYPE && !problemInfeasible)
   removeOneFinderCutRedundancy(num_queue_objects);

queue_total += cutQueue[num_queue_objects].num_cuts;
// temporary for debugging
//if (cutQueue[num_queue_objects].num_cuts > 0)
  //  ucout << curr_finder->cutName() << "returned " << cutQueue[num_queue_objects].num_cuts << " nonredundant cuts.\n";
  // else ucout << curr_finder->cutName() << ", ";
// Temporary for debugging

//cutQueue[num_queue_objects].printCuts();

double this_time = end_time - start_time;
if (this_time == 0.0)  this_time = min_runtime;
#ifdef ACRO_VALIDATING
 BCMIPTimingInfo[cutGeneration] += this_time;
if (makeTimingsPseudoRandom)
  this_time = myUniform();
#endif

cutQueue[num_queue_objects++].setCutQObject(this_time,
			 curr_finder, curr_finder->readiness(curr_problem), problemInfeasible);
lastRunTime[curr_finder->type()] = this_time;
}

// Return true if and only if any of the cut finders' readiness
// measure beats the branching readiness and the quality threshold set
// after the last resolve.

bool BCMip::readinessCheck(BCMipNode *curr_problem)
{
size_type num_finders = cutGenerators.size();
for (size_type i = 0; i < num_finders; i++)
  {
    if (finderCompetitive(i, curr_problem))
    return(true);
  }
return(false);
}

bool BCMip::canStillCut(BCMipNode *curr_problem, PicoLPInterface::problemState &curr_state)
{
if (readinessCheck(curr_problem))
  return(true);
if (num_queue_objects == 0)
  return(false);
bool continue_cutting;
curr_state = curr_problem->incorporateCuts(continue_cutting);
if (!continue_cutting)
  return(false);
return(readinessCheck(curr_problem));
}

void BCMipNode::postPseudoCostInit(IntVector &initVars,
				  bool dontRemoveAllBranchChoices,
				  int &numBranches)
{
  // This might happen because this method is called (e.g. for parallel ramp-up pseudocost
  // exchange) even when this processor doesn't have any initialization to do.
  if (numBranches == 0)
    return;
  restoreLPSolver();
// Finish initialization if this is the first root pseudocost
// initialization.  Note that the root will always have uninitialized
// pseudocosts.  During pseudocost init for the root, we set estimate
// to sum of solution times.  Now set to average.  numBranches is the
// original # fractional variables, not effected by half-infeasible
// optimizations during pseudocost computations. See comment in PCInitSolve()
// to understand the second part of this conditional.

  if (isRoot() && NumLPSolves <= 2)
    {
      //      ucout << "Updating first SP solve time est from " << numBranches << " branches\n";
    bcGlobal()->updateSPFirstSolveTimeEst(bcGlobal()->spFirstSolveTimeEst()/(2*numBranches), true);
    }
}


  // Override pseudocost initialization solve call to record timings for
  // initialization of subproblem-first-time-solution time at the root

PicoLPInterface::problemState BCMipNode::PCInitSolve()
{
if (!isRoot())
  return(runLP());

// We only have to set the time estimate the first time we do
// pseudocost initialization.  Since we currently force at least one
// iteration of trying all finders, this could be after 2 LP solves
// (the first root bound, and after incorporating cuts from the
// finders), when we first compute branching quality.  It could also
// be after just 1 if none of the finders are successful at the root.
// But this would be for branching on the root and therefore, the next
// time through here, the node will not be the root.  So this should suffice
// to initialize the timing information precisely once.

if (NumLPSolves > 2)
  return(runLP());
double start_time = CPUSeconds();
double run_time;
 PicoLPInterface::problemState this_state = runLP();
run_time = CPUSeconds() - start_time;
if (run_time == 0.0) run_time = bcGlobal()->min_runtime;
#ifdef ACRO_VALIDATING
if (bcGlobal()->makeTimingsPseudoRandom)
  run_time = bcGlobal()->myUniform();
#endif


// true says we're initializing, so just set the estimate
// rather than really updating it.  This accumulates all the runtimes (later averaged).
bcGlobal()->updateSPFirstSolveTimeEst(run_time +
				    bcGlobal()->spFirstSolveTimeEst(), true);
return(this_state);
}

// During initialization, just replace the estimate.  Otherwise, use
// exponential smoothing
void BCMip::updateSPFirstSolveTimeEst(double new_time, bool initializing)
{
if (initializing)
  {
  sp_first_solve_time_est = new_time;
  return;
  }
sp_first_solve_time_est = (1 - firstSPSolveSmoothFactor) * sp_first_solve_time_est +
  firstSPSolveSmoothFactor * new_time;
}

// Select a finder and run it
cutFinder *BCMip::runOneFinder(BCMipNode *curr_problem, bool &problemInfeasible)
{
  // Select the finder with minimum bias
size_type numFinders = cutGenerators.size();
size_type i;
double minBias = MAXDOUBLE;
double thisBias;
cutFinder *best_finder = NULL;
cutFinder *curr_finder;
problemInfeasible = false;
for (i = 0; i < numFinders; i++)
  {
  curr_finder = cutGenerators[i];
  // TODO check if this can happen; check on disabling
  if (curr_finder == NULL) continue;
  if (finderCompetitive(i, curr_problem))
    {
    thisBias = finderSchedBias[i] +
      lastRunTime[i]/(curr_finder->readiness(curr_problem) * cutQuality[i].AvgQuality);
    if (thisBias < minBias)
      {
      minBias = thisBias;
      best_finder = curr_finder;
      }
    }
  }

#ifdef ACRO_VALIDATING
if (best_finder == NULL)
  EXCEPTION_MNGR(runtime_error, "Expected to find a cut finder to run");
#endif

finderSchedBias[best_finder->type()] = minBias;
runFinder(best_finder, curr_problem, problemInfeasible);
return(best_finder);
}


// Upon starting a new subproblem (never cut before), set the cut
// quality for the scheduler.  Usually this is the average quality for
// this finder on new subproblems, but if that's very low compared to
// the branching quality, add a little so finders still have a chance
// to run occasionally.

void BCMip::newSPSetCutQual(double branchQual)
{
double branchQualComparison = branchQual * branchBias;
double addedQualAmount = branchQualComparison * cutFinderRunPossibleFactor;
size_type num_finders = cutGenerators.size();
size_type i;
int thisPoorQualStreak;
for (i = 0; i < num_finders; i++)
  {
  cutQuality[i].AvgQuality = cutQuality[i].newSPAvgQuality;
  if (cutQuality[i].newSPAvgQuality < branchQualComparison)
    {
    thisPoorQualStreak = cutQuality[i].UnderThresholdStreakSize++;
    cutQuality[i].AvgQuality += thisPoorQualStreak * addedQualAmount;
    }
  else cutQuality[i].UnderThresholdStreakSize = 0;
  }
}

void BCMip::resetSchedBias()
{
size_type num_finders = cutGenerators.size();
size_type i;
double min_bias = MAXDOUBLE;
for (i = 0; i < num_finders; i++)
  {
  if (finderSchedBias[i] < min_bias)
    min_bias = finderSchedBias[i];
  }
for (i = 0; i < num_finders; i++)
  finderSchedBias[i] -= min_bias;
}


  // True iff another cut finder has been called since
  // the last use of this cut finder for this subproblem
inline bool BCMipNode::otherFinderCalled(int finderType)
{return(bcGlobal()->getCutFinder(finderType) != lastFinder);};


  // Keep track of the number of times we've run all finders (early), so we'll know when to stop.
  // If this is the last such round, set cut qualities for the competition-only phase.

void BCMip::postRunAllFinders(bool maybeNotAllTried)
{
  // If the pass through all solvers might have ended prematurely because of infeasibility
  // detection, don't count it as a full pass.  Not fair competition for cut finders later in
  // the list.
  if (maybeNotAllTried)
    return;
  numFullFindersBounds++;
  if (!shouldRunAllFinders()) // Last round where all run
    // This should only run once, just at entry to competition-only
    // scheduling.
    {
      // Find minimum positive quality among all finders.  Remember
      // finder 0 is the cutpool, so don't include that one.
    double min_pos_quality = MAXDOUBLE;
    size_type numFinders = cutGenerators.size();
    size_type i;
    for (i = 1; i < numFinders; i++)
      if (cutQuality[i].newSPAvgQuality < min_pos_quality)
	min_pos_quality = cutQuality[i].newSPAvgQuality;
    if (min_pos_quality == MAXDOUBLE) return;
    min_pos_quality *= finderCompetitionQualInitFactor;
    for (i=1; i < numFinders; i++)
      // If a finder hasn't found cuts on a new problem, it won't have had
      // a chance in the competitive rounds up to now.
      if (cutQuality[i].newSPAvgQuality == 0.0)
	cutQuality[i].newSPAvgQuality = min_pos_quality;
    }
}


void BCMip::printAllStatistics(ostream &stream)
{
  branching::printAllStatistics(stream);

  if (cache.size() > 1) {
     ucout << endl;
     ucout << "----------------------------------------------------------------------------" << endl;
     ucout << "Best Points Found During Search" << endl;
     ucout << "  EnumerationCount:        " << EnumerationCount << endl;
     ucout << "  EnumerationRelTolerance: " << EnumerationRelTolerance << endl;
     ucout << "  EnumerationAbsTolerance: " << EnumerationAbsTolerance << endl;
     ucout << "----------------------------------------------------------------------------" << endl;
     ucout << cache;
     ucout << endl;
     }

  if (using_cuts) {
    size_type num_finders = cutQuality.size();
    IntVector fieldSizes(7);  // for nicely formatted output
    size_type i;
    int longest_name_length = 0;
    int this_name_length;
    int max_num_calls = 0;
    int max_num_cuts = 0;
    int max_num_infeas = 0;
    double max_gain = 0.0;
    double max_time = 0.0;
    double max_qual = 0.0;
    // Can change this here, or make it a parameter if we really want to.
    int floating_precision = 2;

  // Have to do this dynamically, since we don't know which
  // cut finders will be used and there can be user-defined ones.
    for (i=0; i < num_finders; i++)
      {
	this_name_length = strlen(cutGenerators[i]->cutName());
	if (this_name_length > longest_name_length)
	  longest_name_length = this_name_length;
	max_num_calls = max(max_num_calls, (cutQuality[i]).numTimesCalled);
	max_num_cuts = max(max_num_cuts, (cutQuality[i]).numCutsFound);
	max_num_infeas = max(max_num_infeas, (cutQuality[i]).numInfeasibilityDetections);
	max_gain = max(max_gain, (cutQuality[i]).totalObjectiveGain);
	max_time = max(max_time, (cutQuality[i]).totalTime);
	if (cutQuality[i].totalTime > 0)
	  max_qual = max(max_qual, (cutQuality[i]).totalObjectiveGain/(cutQuality[i]).totalTime);
      }

    fieldSizes[0] = longest_name_length;
    fieldSizes[1] = max(digitsNeededFor(max_num_calls), (int)strlen("# calls")) + 1;
    fieldSizes[2] = max(digitsNeededFor(max_num_cuts), (int)strlen("# cuts")) + 1;
    fieldSizes[3] = max(digitsNeededFor(max_num_infeas), (int)strlen("# infeas")) + 1;
    // digitsNeededFor I think is only for the integer part.
    fieldSizes[4] = max(digitsNeededFor(max_gain) + floating_precision, (int)strlen("gain")) + 1;
    fieldSizes[5] = max(digitsNeededFor(max_time) + floating_precision, (int)strlen("time (sec)")) + 1;
    fieldSizes[6] = max(digitsNeededFor(max_qual) + floating_precision, (int)strlen("avg quality")) + 1;

     stream << endl;
     stream << "Cut Generator Summary:\n";
     stream.width(longest_name_length + 1);
     stream.setf(ios::left, ios::adjustfield);
     stream << "Generator ";
     stream.setf(ios::right, ios::adjustfield);
     stream.width(fieldSizes[1]+ 1);
     stream << "# calls ";
     stream.width(fieldSizes[2] + 1);
     stream << "# cuts ";
     stream.width(fieldSizes[3] + 1);
     stream << "# infeas ";
     stream.width(fieldSizes[4]+ 1);
     stream << "gain ";
     stream.width(fieldSizes[5] + 1);
     stream << "time (sec) ";
     stream.width(fieldSizes[6] + 1);
     stream << "avg quality\n";
     // dashes to separate header fields from data fields
     stream.fill('-');
     for (i=0; i < 7; i++)
       {
	 stream.width(fieldSizes[i] + 1);
	 stream << " ";
       }
     stream << "\n";
     stream.fill(' ');
     int oldPrecision = stream.precision(floating_precision);
     stream.setf(ios::fixed,ios::floatfield);

     for (i=0; i < num_finders; i++)
       {
       stream.width(longest_name_length);
       stream.setf(ios::left, ios::adjustfield);
       stream << cutGenerators[i]->cutName() << " ";
       stream.setf(ios::right,ios::adjustfield);
       stream.width(fieldSizes[1]);
       stream << (cutQuality[i]).numTimesCalled << " ";
       stream.width(fieldSizes[2]);
       stream << (cutQuality[i]).numCutsFound  << " ";
       stream.width(fieldSizes[3]);
       stream << (cutQuality[i]).numInfeasibilityDetections  << " ";
       stream.width(fieldSizes[4]);
       stream << (cutQuality[i]).totalObjectiveGain << " ";
       stream.width(fieldSizes[5]);
       stream << (cutQuality[i]).totalTime << " ";
       stream.width(fieldSizes[6]);
       if (cutQuality[i].totalTime > 0)
	 stream << (cutQuality[i]).totalObjectiveGain/(cutQuality[i]).totalTime;
       stream << "\n";
       }
#ifdef ACRO_VALIDATING

     stream << "\nRunning Time breakdown:\n";
     longest_name_length = 0;
     int j;
     for (j=0; j< numMIPTimingStates; j++)
       longest_name_length = max(longest_name_length, (int)strlen(MIPTimingString(j)));
     for (j=0; j< numBCMIPTimingStates; j++)
       longest_name_length = max(longest_name_length, (int)strlen(BCMIPTimingString(j)));
     // All the timings are pieces of what makes up the total search time
     int timeWidth = digitsNeededFor(searchTime) + floating_precision;
     double totalTime = 0.0;
     double this_time;
     for (i=0; i < numMIPTimingStates; i++)
       {
	 stream.width(longest_name_length);
	 stream.setf(ios::left, ios::adjustfield);
	 stream << MIPTimingString(i) << " ";
	 stream.setf(ios::right,ios::adjustfield);
	 stream.width(timeWidth);
	 this_time = MIPTimingInfo[i];
	 stream << this_time << " ";
	 printPercent(stream, this_time, searchTime);
	 // printPercent undoes this
	 stream.setf(ios::fixed,ios::floatfield);
	 stream << "\n";
	 totalTime += this_time;
       }
     for (i=0; i < numBCMIPTimingStates; i++)
       {
	 stream.width(longest_name_length);
	 stream.setf(ios::left, ios::adjustfield);
	 stream << BCMIPTimingString(i) << " ";
	 stream.setf(ios::right,ios::adjustfield);
	 stream.width(timeWidth);
	 this_time = BCMIPTimingInfo[i];
	 // Assumes otherTime is the last state
	 if (i < otherTime)
	   {
	     totalTime += this_time;
	     if (i == otherTime -1)
	       BCMIPTimingInfo[otherTime] = searchTime - totalTime;
	   }
	 stream << this_time << " ";
	 printPercent(stream, this_time, searchTime);
	 // printPercent undoes this
	 stream.setf(ios::fixed,ios::floatfield);
	 stream << "\n";
       }

#endif
     stream.precision(oldPrecision);
     stream.unsetf(ios::floatfield);
  }

  // JE : this is a little gross, but the way the virtual methods
  // have been set up, this code had to be replicated here.
  // It's fixable, but we'd have to add another layer of calls
  // to get rid of it.

  if (abortReason)
    stream << endl << "RUN ABORTED: " << abortReason << endl;

}


// Sets up the heuristic's LP directly from a subproblem.

void BCMipNode::setupLP(__LPSolver_Interface* sol)
{
  DEBUGPRX(10,bGlobal(),"BCMipNode::setupLP setting bound list\n");

  MILPProblem* localMipRef = mip();

  // Set up bounds.  This is actually identical to the non-cuts version

  DoubleVector boundList(2*(localMipRef->numIntegerVars()));
  lpSetupObjCuts::makeBoundList(localMipRef,this,boundList);
  lpSetupObjCuts::setBounds(localMipRef,sol,boundList);

  // Get the binding cuts and which ones are degenerately basic.

  DEBUGPRX(10,bGlobal(),"Finding cuts\n");

  BasicArray<picoRowCut*> persistCuts;
  BasicArray<int>         persistSlack;
  BasicArray<picoRowCut*> addedCuts;
  BasicArray<int>         addedSlack;

  lpSetupObjCuts::findCuts(this,
			   persistCuts,persistSlack,
			   addedCuts,addedSlack);

  // Load in cuts and basis

  lpSetupObjCuts::setBasisAndCuts(lp,
				  persistCuts,persistSlack,
				  addedCuts,addedSlack,
				  basis);
}


int BCMipNode::initialBasisSize()
    {
    return(lp->getNumCols() + bcGlobal()->numCoreRows);
    }
  
BCMipNode::~BCMipNode()
{
  // These pointers to cuts are going away.
size_type num_cuts = binding.size();
picoRowCut *curr_cut;
for (size_type i = 0; i < num_cuts; i++)
  {
  curr_cut = binding[i];
  // Sometimes we can determine a node is dead before de-activation (eg. infeasible LP,
  // candidate solutions when not enumerating).  In these cases, we shortcut the
  // expensive deactivation process.  These cuts reference counts weren't incremented during deactivation
  // so don't decrement them here.  This is not very clean (leaving the active bit set).  Maybe there's
  // a nicer way to signal this.
  if (!active)
    curr_cut->decrementRefCount();
  // If this was the last pointer to this cut, and it can't be used by others, delete it
  if (curr_cut->deletable())
    delete(curr_cut);
  }
num_cuts = persistent_cuts.size();
for (size_type i = 0; i < num_cuts; i++)
  {
  curr_cut = persistent_cuts[i];
  if (!active)
    curr_cut->decrementRefCount();
  // If this was the last pointer to this cut, delete it (don't share persistent cuts)
  if (curr_cut->refCount()==0 && !curr_cut->loaded())
    delete(curr_cut);
  }
}

void BCMip::printCutDataStructures()
{
  ucout << "Cut Pool:\n" << cutPool << "Loaded Cuts (BCMip view):\n" << loadedCuts << "\n";
  /* don't need this for now
  ucout << "Loaded Cuts (LP interface view):\n";
  lp->printRowInfo();
  */
}

void BCMipNode::printMILPNode()
{
MILPNode::printMILPNode();
 size_type num_cuts = persistent_cuts.size();
 size_type i;
if (num_cuts > 0)
  {
  ucout << "Persistent cuts: \n";
  for (i=0; i < num_cuts; i++)
    persistent_cuts[i]->printCut();
  }
else ucout << "No persistent cuts\n";
 num_cuts = binding.size();
if (num_cuts > 0)
  {
  ucout << "binding cuts:\n";
  for (i=0; i < num_cuts; i++)
    binding[i]->printCut();
  }
else ucout << "No binding cuts stored\n";
}

// If we're checking for cutting off watched points, this records which
// watched points are contained in this subproblem

void BCMipNode::recordWatchedPoints()
{
size_type numSolutions = mGlobal()->solutionsToWatch.size();
int num_watched_contained = 0;
for (size_type stepper = 0; stepper < numSolutions; stepper++)
  {
  if (containsOneWatchedPoint(mGlobal()->solutionsToWatch[stepper]))
    {
    bcGlobal()->containedWatchedPointsInd[num_watched_contained++] = stepper;
    }
  }
 bcGlobal()->numContainedWatchedPoints = num_watched_contained;
}

void BCMipNode::computeBranchReadiness()
{
  branchReadiness = bcGlobal()->totalBranchReadyFactor *
    max(0.0, 1 - bcGlobal()->branchWaitFactor * exp(-bcGlobal()->branchExpFactor*NumLPSolves));
}

double BCMipNode::branchingReadyMeasure()
{return(branchQuality * branchReadiness * bcGlobal()->branchBias);};

void BCMip::cleanUpAfterInfeasibility(double SPBound, bool newSubproblem)
{
BasicArray<picoRowCut *>  *tmp_cut_container;
size_type num_cuts,j;
picoRowCut *currCut;

// In serial, the last one should be the one that found the infeasibility, but not necessarily
// true in parallel
for (size_type i = 0; i < (size_type) num_queue_objects; i++)
  {
// No credit for these cuts (dominated by infeasibility, even within
// the finder that found the infeasibility).  Put them in the pool if
// possible.  Otherwise, thrown them away.

  num_cuts = cutQueue[i].num_cuts;
  tmp_cut_container = &cutQueue[i].new_cuts;
  for (j=0; j < num_cuts; j++)
    {
    currCut = (*tmp_cut_container)[j];
    if (currCut->poolable())
      {
      cutQueue[i].releaseCut(j);
      cutPool.insert(*currCut);
      }
    else cutQueue[i].removeCut(j);
    }
  if (cutQueue[i].found_infeasibility)
    {
    updateCutFinderQualityFromInfeasible((cutQueue[i].getFinderUsed())->type(),
					 (cutQueue[i]).getTime(), SPBound, newSubproblem);
    removeCutQObject(i);
    }
  }
resetCutQueue();
}

// Assumes the cuts have already been removed.

void BCMip::removeCutQObject(size_type to_remove)
{
#ifdef ACRO_VALIDATING
  if (to_remove >= (size_type) num_queue_objects)
    EXCEPTION_MNGR(runtime_error, "Trying to remove unused queue object " << to_remove << ". Number used is " << num_queue_objects << "\n");
  if (cutQueue[to_remove].num_cuts > 0)
    EXCEPTION_MNGR(runtime_error, "Removing a cut queue object that still has cuts\n");
#endif
  // Last element in the set.  Should be the normal case.
if (to_remove == (size_type) (num_queue_objects - 1))
  num_queue_objects--;
else
  EXCEPTION_MNGR(runtime_error, "Removing a cut queue object from the middle.  Time to implement the copy.\n");
}


// point is the current feasible integer solution.
// This includes continuous variables.  We assume this has been
// rounded so that integer variables are exact.  If that changes,
// use tolerances in the appropriate tests.

void BCMipNode::enumeration_effect(DoubleVector& point)
{
  // A simple example of a persistent cut that will work for a
  // problem that only has binary variables.  Specifically, if B_1
  // are the binary variables set to 1 in the point and B_0 are the
  // variables set to 0, then add the cut
  // sum_{i in B_1} x_i - sum{i in B_0} x_i <= number of 1's - 1
  // (this comes from
  // sum_{i in B_1} x_i + sum{i in B_0} (1 - x_i) <= number of binary variables -1
  // which simply disallows the current binary settings.

  BasicArray<picoRowCut *> newCuts(1);
  
  MILPProblem* localMipRef = mip();
  if (mip()->numGeneralIntVars() != 0)
    EXCEPTION_MNGR(runtime_error, "PICO cannot yet enumerate problems with general integer variables\n");
  int numOnes = 0; // Number of 1's in the current Solution
  int numInts = localMipRef->numIntegerVars();
  int LPVarNum;
  int *colIndices = new int[numInts];
  double *rowElements = new double[numInts];
#ifdef ACRO_VALIDATING
  // Assumes that all failed attempts to allocate from the heap will give a NULL, so
  // if the first allocation for colIndices fails, the second one for rowElements will to.
   if (rowElements == NULL)
   EXCEPTION_MNGR(runtime_error, "Failed to allocate space for a new enumeration cut\n");
#endif
  for (int i = 0; i < numInts; i++)
    {
      LPVarNum = localMipRef->integerVarNdx[i];
      colIndices[i] = LPVarNum;
      if (point[LPVarNum] == 1.0)
	{
	  rowElements[i] = 1.0;
	  numOnes++;
	}
      else // assume point is 0.0 for this variable.  Add debugging tests if necessary
	rowElements[i] = -1.0;
    }
  OsiRowCut *newOsiCut = new OsiRowCut();
#ifdef ACRO_VALIDATING
 if (newOsiCut == NULL)
   EXCEPTION_MNGR(runtime_error, "Failed to allocate a new OsiRowCut for enumeration\n");
#endif
  newOsiCut->setRow(numInts, colIndices, rowElements);
  newOsiCut->setUb(numOnes - 1); // Lower bound is minus infinity by default

  picoRowCut *newPicoCut = new picoRowCut(bcGlobal(), newOsiCut, cutFinder::ENUM, "enumeration");
#ifdef ACRO_VALIDATING
 if (newPicoCut == NULL)
   EXCEPTION_MNGR(runtime_error, "Failed to allocate a new picoRowCut for enumeration\n");
#endif

  // The subproblem isn't active when we process updateIncumbent and
  // hence process this method (called from the PICO core after
  // boundComputation returns.  So we have to set the reference  counter.
  newPicoCut->incrementRefCount();
  newCuts[0] = newPicoCut;

  // TODO: remove or make conditional on debug level
  /*  ucout << "Adding enumeration cut \n";
      newPicoCut->print(); */

  // Add the cut to the problem specification only.
  addPersistentCuts(newCuts);
  // One would think we don't need to add this cut to the LP environment, but if this problem is
  // immediately selected to run again, we'd warm start and would need it in
  bcGlobal()->addCuts(newCuts, 1);
  setState(beingBounded);
  // Set back the # of LP solves to reduce the branching bias.  This is like a "new" subproblem
  // without the overhead of branching.  We don't want to do all the cataloging for just branching
  // and don't want to overwrite/redo initialization at the root.  I think these values are big
  // enough to not trigger unwanted side effects.
  if (isRoot())
    NumLPSolves = 3;
  else NumLPSolves = 1;
}

// For debugging.  Print information used to schedule cut finders and branching.
void BCMip::printCutSchedulingInfo(BCMipNode *curr_node)
{
    size_type numFinders = cutGenerators.size();
    ucout << "Cut Scheduling information. Branching readiness: " << curr_node->branchingReadyMeasure()
	  << "\nSched threshold: " << finderThreshold << "\n";
  for (size_type i = 0; i < numFinders; i++)
    {
      if (cutGenerators[i] == NULL)  continue;
       ucout << "Finder " << i << " readiness: " << cutGenerators[i]->readiness(curr_node)
	     << " avg qual " << cutQuality[i].AvgQuality << "\n";
    }
}

} // namespace pico


