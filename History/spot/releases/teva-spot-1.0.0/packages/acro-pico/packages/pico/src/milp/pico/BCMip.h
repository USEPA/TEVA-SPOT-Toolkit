/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file BCMip.h
 *
 * Classes for a branch-and-cut search: BCMip, BCMipProblem, BCMipNode
 */

#ifndef pico_BCMip_h
#define pico_BCMip_h

#include <acro_config.h>
#include <utilib/HashedSet.h>
#include <utilib/_math.h>
#include <utilib/seconds.h>
#include <pico/cutFinder.h>
#include <pico/milpNode.h>
#include <pico/milp.h>
#include <pico/MILPProblem.h>
#include <pico/BCMipBase.h>
#include <pico/picoRowCut.h>
#include <pico/PicoLPInterface.h>
#include <pico/lpSetupObjCuts.h>

namespace pico {

  class SNLKnapsack;

using utilib::HashedSet;

// An index into the set of cut finders.  For now there's
// always a pool scanner and it's always first.
#define POOLSCANNERTYPE 0

class cutFinder;

// Do we need to define debug as a global parameter here?

class BCMip;
class BCMipProblem;
class BCMipNode;

// Used to manage the cutting schedule

class cutQObject
{
friend class BCMip; 
protected:
// If this ever changes, be sure to propagate effects to parBCMip::(un)packCutQueue
  BasicArray<picoRowCut *>new_cuts;
  double time_to_find;
  cutFinder *finder_used;
  double finder_readiness;
  bool found_infeasibility;
public:

  // This is an int rather than a size_type because it's passed as an argument
  // (by reference) to generateCuts(...).  We want that method, which could be
  // used by other PICO users, to have more generic types whenever possible.
  int num_cuts;
  cutQObject()
    {
    time_to_find = 0.0;
    finder_used = NULL;
    finder_readiness = 0.0;
    found_infeasibility = false;
    };

BasicArray<picoRowCut *>& getCutContainer()
  {return(new_cuts);};

double getTime()
  {return(time_to_find);};

 double getAverageTime()
   {return(time_to_find/num_cuts);};

cutFinder *getFinderUsed()
  {return(finder_used);};

double getFinderReadiness()
  {return(finder_readiness);};

bool getInfeasibility()
  {return(found_infeasibility);};

void setCutQObject(double new_time, cutFinder *new_finder,
		   double new_readiness, bool new_infeas)
  {
  time_to_find = new_time;
  finder_used = new_finder;
  finder_readiness = new_readiness;
  found_infeasibility = new_infeas;
  }

 void removeCut(size_type to_remove)
   {
   delete new_cuts[to_remove];   
   new_cuts[to_remove] = new_cuts[--num_cuts];
   }

 void removeCut(picoRowCut *to_remove)
   {
     for (int i = 0; i < num_cuts; i++)
       {
	 if (new_cuts[i] == to_remove)
	   {
	   removeCut(i);
	   return;
	   }
       }
     EXCEPTION_MNGR(std::runtime_error, "Trying to remove a cut from a queue that doesn't contain it\n");
   }

 // Like removeCut, but no deletion

 void releaseCut(size_type to_remove)
   {
   new_cuts[to_remove] = new_cuts[--num_cuts];
   }

 void releaseCut(picoRowCut *to_remove)
   {
     for (int i = 0; i < num_cuts; i++)
       {
	 if (new_cuts[i] == to_remove)
	   {
	   releaseCut(i);
	   return;
	   }
       }
     EXCEPTION_MNGR(std::runtime_error, "Trying to release a cut from a queue that doesn't contain it\n");
   }


 // Replace cut, don't delete the old one

 void replaceCut(size_type to_replace, picoRowCut *new_cut)
   {
   new_cuts[to_replace] = new_cut;
   }

 void replaceCut(picoRowCut *to_replace, picoRowCut *new_cut)
   {
     for (int i = 0; i < num_cuts; i++)
       {
	 if (new_cuts[i] == to_replace)
	   {
	   replaceCut(i, new_cut);
	   return;
	   }
       }
     EXCEPTION_MNGR(std::runtime_error, "Trying to replace a cut in a queue that doesn't contain it\n");
   }


void printCuts()
  {
  for (int i=0; i < num_cuts; i++)
    (new_cuts[i])->print();
  }

};


class BCMipProblem: public MILPProblem, public BCMipBase
{
 public:
  // If this returns true, when a BCMipNode is made active, the LP
  // solver state is restored to exactly what it was when the current
  // LP solution was created.  For example, CGL cut generators get
  // their problem information this way.  The knapsack cut generator
  // at least uses the matrix directly.  If there is a derived class
  // that uses only cut generators that do not use the
  // OsiSolverInterface for problem data (e.g. applications within
  // PICO that use the BCMipProblem-derived class), then setting this
  // to false, means we can leave old (globally valid) cuts in the LP
  // if we want.  Probably means a more efficient swap.

  // Can this be static in derived classes?
   virtual bool resetNodeState()
     {return(true);};

  //// Make an empty MIP, for parallel initialization
  BCMipProblem() {};

};

class BCMip: virtual public MILP, virtual public BCMipBase
{

  // For tracking performance
public:
  DoubleVector BCMIPTimingInfo;

protected:

  // The SNL knapsack cut finder.  This will probably move into a finder manager once that's
  // implemented

  SNLKnapsack *SNLKnapsack_finder;

  // Gives cuts serial numbers similar to subproblem serial numbers.  Mostly
  // used for debugging.
  int cutCounter;

  // TODO: make sure this is reasonably efficient.  May be necessary to use
  // picoRowCut here rather than picoRowCut * for correct operation of the hash table.

  // Cuts currently loaded into the LP
  HashedSet<picoRowCut,false> loadedCuts;

  // Globally valid, possibly useful cuts *not* loaded

  HashedSet<picoRowCut,false> cutPool;

  // Maximum number of cuts loaded at any given time (unless all cuts are
  // binding)

  int loadedPoolMaxSize;

  // Usually generate this many cuts before incorporating them

  int cutBlockSize;

  // Number of subproblems for which all cut Finders have been run.
  // This is used to control the cutting process.  Early on, when
  // BCMip hasn't learned anything about the power of the various
  // cutFinders on this problem, run all finders once on a subproblem
  // before starting the Finder competition.

  int numFullFindersBounds;

	///
  static BasicArray<cutFinder*> cutGenerators;

  void BCMIPInit();

  BasicArray<cutQObject> cutQueue;
  int num_queue_objects;
  int queue_total;

  // Quality-tracking information associated with each cut
  // finder in cutGenerators

  BasicArray<finderQualityObject> cutQuality;

  // Used for scheduling cut generators

  DoubleVector finderSchedBias;
  DoubleVector lastRunTime;
  
  // Estimate of the amount of time needed to solve the
  // first LP for a subproblem.
  double sp_first_solve_time_est;

  // If bit i is on, then disable the cutFinder of type i during set
  // up in preprocessing.  There may be a better way to do this, but
  // we can't prevent the cutter anouncing its availability (that's
  // done in static initialization). That would require editing the
  // cut finder class and we'd like the users to be able to disable
  // with a call in the driver.  We can't disable after the array of
  // cut generators is made, because that's done after the search
  // starts (and that's a monolithic process in the driver).  So we
  // set a bit before the search starts and don't enter this cut
  // finder into the array of cut finders.  We'll use a pointer so we
  // can ditch this after it's used.
  BitArray *disableBuiltInCuts;

public:

  ///
  BCMipProblem *BCMIP;

  // Highest quality (weighted by readiness) among all finders just after the last resolve

  double bestFinderQuality;
  // finders with effective quality (i.e. quality weighted by readiness) at least this high
  // can compete to run during a competitive phase.
  double finderThreshold;

  void updateFinderQualityThreshold(BCMipNode *curr_node);

  virtual bool finderCompetitive(size_type finderID, BCMipNode *curr_node)
    {
      return(cutGenerators[finderID]->readiness(curr_node) * cutQuality[finderID].AvgQuality >= finderThreshold);
    }

  ///
  bool setup(int& argc, char**& argv, BCMipProblem& _MIP)
        {
	  setBCMipProblem(_MIP);
	  return MILP::setup(argc,argv,_MIP);
        }

  // If a cut finder runs, it must run for a positive time.  This is
  // the smallest unit of time the timer can distinguish from zero.
  double min_runtime;

  // Used for debugging.  Make behavior deterministic (from a seed), but give lots
  // of choices for runtime sequences.  Just doing what Jonathan did in the heuristic.
  utilib::PM_LCG  myRNG;
  utilib::Uniform myUniform;

  // Used for debugging.  Print information used to schedule cut finders and branching

  void printCutSchedulingInfo(BCMipNode *curr_node);

  // Number of rows in the original (core) problem.  Inactive subproblems only
  // store slack variables for these rows in the basis.

  int numCoreRows;

  // For debugging.  Indices of solutions in the solutionsToWatch array (from milp)
  // contained by the currently active problem, and the number of such solutions.
  // Used to check for cuts that cut off known feasible (optimal) solutions.

  IntVector containedWatchedPointsInd;
  int numContainedWatchedPoints;

  // This used to set the MIP pointer too, but that's called elsewhere
  // (through the setup methods

  void setBCMipProblem(BCMipProblem& inputMIP)
	{
	BCMIP = &inputMIP;
	}

  // Note:  Does NOT assume ownership of inputMIP
  // This no longer attempts to initialize the hash table names, since
  // currently the hash tables in utilib ignore this name.  If that changes,
  // initialize it with a direct hash-table method from here.
  void MILPInit(bool initVB=true)
    {
    if (initVB)
       MILP::MILPInit();
    BCMIPInit();
    }

  /* This has gone away at the MILP level at least for now
  //For parallel versions that have I/O capabilities
  BCMip(int argc,char** argv): 
    MILP(argc, argv), loadedCuts("LoadedCuts"), cutPool("CutPool") 
    {
    MCMIPInit();
    }; */

  BCMip();

  void reset(bool resetVB = true);

  virtual pebbl::branchSub* blankSub();

  int getCutSerial()
    {return(cutCounter++);}

  int poolSize()
    {
    return (cutPool.size());
    }

  // For debugging.

  void printCutDataStructures();

  // Update the cut pools to reflect this new set of active cuts
  // (and all previous ones deleted) At least for now, this is called
// when a subproblem is made active.  Each of these new cuts should
// have their reference count decremented whether they're moving from the
// cut pool or staying in the loaded pool (previous problem incremented this
// count before deactivating.  The two sets of cuts are persistent and regular.

  void replaceLoadedCuts(BasicArray<picoRowCut *> &new_cuts1,BasicArray<picoRowCut *> &new_cuts2);

  // Update the loaded cut pool to reflect addition of these cuts
  // Also set the loaded flag in each of these cuts.

  void addLoadedCuts(BasicArray<picoRowCut *> &new_cuts, int num_cuts);

  // Adds cuts to the LP and updates the pools.   If adding these cuts puts us over
  // the loaded-cut limit, remove nonbinding cuts in order of current scaled dual.

  void addCuts(BasicArray<picoRowCut *> &new_cuts, int num_cuts);

  // Remove a cut from the loaded-cuts pool.  Move it to the cut
  // pool if it's poolable, and otherwise, if its reference count is
  // zero, delete it

  void unloadCut(picoRowCut *to_unload);

  // Add a cut to the pool.  If there is already a cut in the pool
  // parallel to the new one, keep only one in the pool, make sure
  // both have the strongest upper bound (rhs), and, if either has
  // a zero reference count, delete it.

  void addToCutPool(picoRowCut *to_add);

  // As above for a set of cuts
  void unloadCuts(BasicArray<picoRowCut *> &to_unload);


// Put all the binding cuts into cutlist.  Put the index (into cutlist)
// of all those with degenerate basic slack variables into degenerate_list.
// Binding cuts have zero slack.  BasicArrays
// are sized exactly afterward (assumed, eg, by the PICO cut
// management piece in the LP interface).  We no longer assume the basis
// in the LP interface is valid (store degeneracy info with the cuts
// immediately after the solve).  Pseudocost initialization, eg, when
// determining branching readiness, made basis invalid sometimes.
// Also record this basis information (slackness) for persistent cuts.


  void getBindingRows(BasicArray<picoRowCut*>& persistent_cuts,
		      BasicArray<int>& slack_persistent_list,
		      BasicArray<picoRowCut*>& cutlist,
		      BasicArray<int>& degenerate_list);

  // Called after a resolve during cutting.  Age nonbinding cuts.
  // Remove if they're old enough.  Return true if cuts were removed
  // and false otherwise.

  bool ageCuts();

  // Add the cuts in the queue into the LP and loaded pool.  Return
  // the number added. If multiple finders have found the same cut,
  // removes the redundancy and gives credit to the finder that found
  // the tightest cut (if the bounds aren't the same).  For ties,
  // credit goes to the fastest finder.  This assumes that the only
  // finder that can return cuts with a positive reference count is the
  // cut pool scanner.  That is, we assume all
// other finders are making new cuts.  If a finder generates a cut redundant with the cut pool and
// wins (stronger or faster), then we use the strongest cut, and the correct finder gets credit.

  int incorporateCuts();

  // Removes cut redundancy from a single cut finder (certainly an issue with CGL
  // finders, or any finder that doesn't police itself).  This is separate from the
  // redundancy elimination from BCMip::incorporateCuts because there's no need to
  // assign credit.  Just keep the best.  Run this as we go along to avoid keeping a
  // lot of redundant cuts around (a minor efficiency improvement).

void removeOneFinderCutRedundancy(int queue_index);

// Clean up the cut management objects if a cut finder determines a subproblem is infeasible

void cleanUpAfterInfeasibility(double SPBound, bool newSubproblem);

void trimLoadedPool(int num_to_remove);

  // Estimate of the magnitude of the (optimal) objective function

  double solMagnitudeEst();

  // Takes an LP solution x.  Scans the cut pool.  Removes all cuts
  // that are violated by x and returns them.

  void scanCutPool(BasicArray<picoRowCut *>& violated_cuts, int &num_cuts, DoubleVector &x);

  // Should we run all cut Finders on this subproblem?  Currently yes if this is one of the first
  // suproblems we've bound.  If this method changes, consider effect on postRunAllFinders().

  bool shouldRunAllFinders()
    {return(numFullFindersBounds <= tryAllFindersLimit);}

  // Keep track of the number of times we've run all finders (early), so we'll know when to stop.
  // If this is the last such round, set cut qualities for the competition-only phase.  If the
  // pass might have terminated early because of infeasibility detection, don't count the pass.
  void postRunAllFinders(bool maybeNotAllRun);

  // Run all finders that have positive readiness

  void runAllFinders(BCMipNode *curr_problem,
		     BitArray &finders_run,
		     bool &problemInfeasible);

  // Assumes the cutFinder passed in has positive readiness
  void runFinder(cutFinder *curr_finder, BCMipNode *curr_problem, bool &problemInfeasible);

  // Select a finder and run it
  cutFinder  *runOneFinder(BCMipNode *curr_problem, bool &problemInfeasible);

    // Return true if and only if any of the cut finders' readiness
    // measure beats the branching readiness
  bool readinessCheck(BCMipNode *curr_problem);

  bool canStillCut(BCMipNode *curr_problem,
		   PicoLPInterface::problemState &this_state);

  cutFinder *getCutFinder(int finderType)
    	{return(cutGenerators[finderType]);};

  int numCutFinders()
    	{ return((int)cutGenerators.size()); }

  // For each set of cuts in the cut queue, update the
  // finder quality measures. 

  void updateCutFinderQuality(DoubleVector &x, double time_to_add, bool firstSolveForSubproblem);

  void recomputeCutFinderQualityAverages(size_type finderID, double new_average,
					 bool newSubproblem);

  // Update quality measures after a cut finder has determined a subproblem is infeasible

  void updateCutFinderQualityFromInfeasible(size_type finderID, double this_time,
					    double SPBound, bool newSubproblem);

  // Upon starting a new subproblem (never cut before), set the cut
  // quality the scheduler.  Usually this is the average quality for
  // this finder on new subproblems, but if that's very low compared
  // to the branching quality, add a little so finders still have a
  // chance to run occasionally.

  void newSPSetCutQual(double branchQual);

  // Decrease all cut schedule bias values by the min, to insure numerical
  // stability (e.g. no wrap)
  void resetSchedBias();

  virtual void preprocess();

  double spFirstSolveTimeEst()
    {return(sp_first_solve_time_est);};

  void updateSPFirstSolveTimeEst(double new_time, bool initializing = false);

  // All cut queue objects should have no cuts (handled before this call)
  void resetCutQueue()
    {
    num_queue_objects = 0;
    queue_total = 0;
    };

  void removeCutQObject(size_type to_remove);

  bool cutQueueBig()
    {
    if (queue_total >= cutBlockSize)
      return(true);
    return(false);
    }
  virtual void printAllStatistics(std::ostream& stream = std::cout);

// This should be called from the driver before search starts.  Indicate a
// built-in cut finder shouldn't be added to the list of cut finders in
// BCMip preprocessing.

void disableBuiltInCutFinder(cutFinder::cutTypeID thisFinder)
  {disableBuiltInCuts->set(thisFinder);};
};


// Note: don't have node classes share parameter classes (like BCMipBase)
// with branching classes and problem classes (parameters would need to be
// reset on construction of each node).
class BCMipNode: virtual public MILPNode, virtual public BCMipBase
{
  friend class lpSetupObjCuts;
  
protected:

  // Mostly for safety
  bool active;

  DoubleVector full_solution;

  // These are passed into routines in the LP interface class (set from there)

  // List of binding cuts (needed, eg, to get this LP solution)
  BasicArray<picoRowCut *> binding;
  // The indices (into binding) of the cuts with basic slack variables
  IntVector basic_slack_cuts;

  // Cuts explicitly enforced as long as the node is alive.  For example, these
  // are used for branching on constraints.

  BasicArray<picoRowCut *> persistent_cuts;

  // The indices (into persistent_cuts) of the persistent cuts with basic slack
  // variables.  Since these are always enforced, they can go slack and will therefore
  // have basic slacks more often.  Thus this will be a higher percentage of the persistent
  // cuts than the percentage of binding in basic_slack_cuts, but there probably won't be
  // a lot of persistent cuts.  If that changes (there are a lot of persistent cuts), consider
  // representing this as a BasisArray.

  IntVector persistent_basic_slacks;

  // Bit i is 1 iff cut finder i has been invoked since the last
  // time this LP was solved

  BitArray finderHistory;

  int NumLPSolves;
  
  // The last cut finder used for this subproblem
  cutFinder *lastFinder;

  // A measure of the quality of branching
  double branchQuality;

  // How ready are we to branch? (between 0 and 1)

  double branchReadiness;

public:

  bool useSavedSolutionNow()
    {
      return(MILPNode::useSavedSolutionNow() && NumLPSolves == 0);
    }

// Add these cuts to the array of persistent cuts and mark
// them as persistent.  We assume all cuts are either persistent or
// regular in all subproblems to which they apply.
// We'll likely only add one at a time, but we'll use this more
// general form for now.

  void addPersistentCuts(BasicArray<picoRowCut *> newCuts);

  // The current LP relaxation solution
  DoubleVector &x(){return(full_solution);};

  virtual int initialBasisSize();

  void makeActiveLP();

  // If we're checking for cutting off watched points, this records which
  // watched points are contained in this subproblem

  void recordWatchedPoints();

  void noLongerActiveLP();

  PicoLPInterface::problemState boundComputationGuts();

  void  enumeration_effect(DoubleVector& point);
  
  void setBasis()
    {
      // For activating problems.  Cuts must already be in the LP.
      if (!active && (binding.size() > 0 || persistent_cuts.size() > 0))
      lp->setBasisWithCuts(persistent_cuts, 
			   persistent_basic_slacks, 
			   binding, 
			   basic_slack_cuts, 
			   basis);
    // For active problems with full basis stored locally
    else 
      MILPNode::setBasis();
    }

  void setBounds()
    {
    if (!IwasLast())
      MILPNode::setBounds();
    }
    
  void getSolution()
    {
    MILPNode::getSolution(); // for integer vars
    lp->getColSolution(full_solution.data());  
    }

  void getBasis(BasisArray& basisBuffer) const
    {
      // TODO: This could be inefficient if it causes a lot of allocation/deallocation.
      // but we have to make sure the size() function is correct (used heavily in the PicoLPInterface).
      // Check that at least resizes that don't require a new allocation (e.g. ones that move only
      // within the last allocated byte) don't do allocation.
      basisBuffer.resize(lp->getNumCols() + lp->getNumRows());
      lp->getBasis(basisBuffer);
    }

  // Pseudocost initialization uses the LP solver.  Make sure the LP is back in
  // the state it was after solving the node initially.  Otherwise, the cut generators
  // could see the wrong point (solution).  Also, if this is the root, use average
  // pseudocost resolve time to set the initial subproblem solve time estimate.

  virtual void postPseudoCostInit(IntVector &initVars,
				  bool dontRemoveAllBranchChoices,
				  int &numBranches);

  PicoLPInterface::problemState resolveLP(bool &continue_cutting, double &resolve_time);

  // Override pseudocost initialization solve call to record timings for
  // initialization of subproblem-first-time-solution time at the root
  PicoLPInterface::problemState PCInitSolve();

  // Add cuts from the cut queues to the current problem and resolve the LP.

  PicoLPInterface::problemState incorporateCuts(bool &continue_cutting);


  // Used by cut finders to determine readiness

  // True iff the LP has been resolved since the last
  // time this finder was invoked for this subproblem.
  // Vacuously true if this finder has never been invoked.

  bool LPResolved(int finderType)
    {return(finderHistory(finderType)==0);};

  // True iff another cut finder has been called since
  // the last use of this cut finder for this subproblem
  bool otherFinderCalled(int finderType);

  // True iff this is a new subproblem for which no cutting has taken
  // place yet.  (other than the addition of cuts from the pool, for example).

  bool newSubproblem()
    {return (NumLPSolves <= 1);};

  // Record the pseudocost effects from a branch only on the LP solve immediately
  // after the branch (not on subsequent resolves for cut incorporation)
  bool updatePCFromBoundingSolve()
    {return(NumLPSolves == 0 && MILPNode::updatePCFromBoundingSolve());};

  bool isWarmStart()
    {return(MILPNode::isWarmStart() || MILPNode::IwasLast());};

  bool replaceCutsToActivate()
    {return(!IwasLast() && !parentWasLast());}

  // MILPNode required the state be bounded, but we will want to
  // recognize this as a possible reason for stopping cutting (before
  // we declare the state bounded).
  bool candidateSolution()
  {
    if (nFrac == 0)
      return true;
    return false;
  }


  // Update the measure of the benefit of branching at this point.
  // This can change anytime the LP is solved.

  bool computeBranchQuality();

  // Update the measure of readiness to branch.  Should probably be
  // called with the above update of branching quality

  void computeBranchReadiness();

  double branchingReadyMeasure();

  virtual branchSub *makeChild(int whichChild);
  void childInit(BCMipNode *child, int whichChild, bool initVB = true);

  virtual BCMip *bcGlobal() {return globalPtr;};
  MILP *mGlobal() {return (MILP *)globalPtr;};
  pebbl::branching *bGlobal() const {return (pebbl::branching *)globalPtr;};

  void BCMipNodeFromBCMip(BCMip* master, bool initVB=true);  // For making the root
  void BCMipNodeAsChildOf(BCMipNode* parent, bool initVB=true); // For making a child
  // For use with derived classes that will call constructors for MILPNode and branchSub
  BCMipNode()
    { }

  ~BCMipNode();

  void BCMipNodeInit();

  virtual void printMILPNode();

  // Stuff for setting up heuristic LP's.  Override the similar methods
  // in MILPNode.

  // Make an object that will set up an LP later

  lpSetupObj* makeLPSetupObj()
    {
      return new lpSetupObjCuts(this);
    };

  // Set up an LP right now (code in .cpp file)
  
  void setupLP(__LPSolver_Interface* sol);

  // End of heuristic LP setup stuff.
    
 protected:
  BCMip *globalPtr;
};



// This is used by the handlers (search engines) to create the root.
// As an inline, it will generate a call to BCMipNode with the BCMip (branching
// class) pointer as an argument.

inline pebbl::branchSub* BCMip::blankSub()
{
MEMDEBUG_START_NEW("BCMipNode")
BCMipNode *tmp = new BCMipNode;
#ifdef ACRO_VALIDATING
 if (tmp == NULL)
   EXCEPTION_MNGR(std::runtime_error, "Failed to allocate a new BCMipNode\n");
#endif
tmp->BCMipNodeFromBCMip((BCMip *)this);
MEMDEBUG_END_NEW("BCMipNode")
return (pebbl::branchSub *)tmp;
};

} // namespace pico

#endif
