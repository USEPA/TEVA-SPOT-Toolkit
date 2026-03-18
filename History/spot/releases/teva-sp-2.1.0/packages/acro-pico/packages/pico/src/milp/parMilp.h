/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file parMilp.h
 *
 * Defines the pico::parMILP class.
 */

#ifndef pico_parMilp_h
#define pico_parMilp_h

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <pebbl/parBranching.h>
#include <pebbl/gRandom.h>
#include <pico/milp.h>
#include <pico/milpNode.h>
#include <pico/pCostCast.h>
#include <pico/mipHeuristic.h>
#include <pico/parMilpParams.h>


namespace pico {

  // These are actually the defaults, except for poolInit, 
  // which is 0 by default
  
#ifdef  BRANCHING_CONSTRUCTOR
#undef  BRANCHING_CONSTRUCTOR
#endif
#define BRANCHING_CONSTRUCTOR branching(minimization,relTolerance(),absTolerance())

class parMILPNode;  // Forward reference.


///  Define the master class for parallel MILP.
class parMILP : public pebbl::parallelBranching, virtual public MILP, virtual public parMilpParams
{

  #if !defined(DOXYGEN)
  friend class parMILPNode;
  friend class pCostCastObj;  // Allow access by pCostCast thread.
  #endif

protected:


  // We have our own special thread to broadcast pseudocosts

  pCostCastObj* pCostCaster;

  // A couple of extra data structures for gathering pseudocost data
  // to broadcast

  int numPcostGathered;
  IntVector bcastIndices;
  DoubleVector bcastUp;
  DoubleVector bcastDown;

  // Used during parallel pseudocost initialization during ramp up

 int numUnInit;
 IntVector *fullInitVars;

 // Used to help determine when to cross over from serial ramp up to
 // tree-level parallelism

 double avgNumUnInit;

 // Used in parallel ramp-up heuristic to try to track which
 // variables directly interact.

 adjSetObj adjSet;
  
public:  

// This just echos the value from MILP::useSavedRootSolution.  This is
// necessary because friendship is not inherited and it's not good
// form for the serial code to explicitly declare friendship with
// parallel classes.

 bool useSavedRootSolution()
   {return MILP::useSavedRootSolution;}

  virtual void pack(PackBuffer& outBuffer);
  virtual void unpack(UnPackBuffer& inBuffer);
  virtual int  spPackSize();

  virtual void packSolution(PackBuffer& outBuffer)    
    { 
      outBuffer << incumbent;
    };

  virtual void unpackSolution(UnPackBuffer& inBuffer) 
    {
      inBuffer >> incumbent;
    };

  virtual int solutionBufferSize() 
    {
      int s = sizeof(size_type);
      s += (MIP->numAllVars())*sizeof(double);
      return uMPI::packSlop(s);
    };

  virtual void appCheckpointWrite(PackBuffer& outBuf);
  virtual void appCheckpointRead(UnPackBuffer& inBuf);
  virtual void appMergeGlobalData(UnPackBuffer& inBuf);

  bool mergedAnything;

  void packPseudoCosts(int firstToPack, int numToPack, 
		       IntVector &VarNdx, double *buffer);
  void packIntegralityChanges(int firstToPack, int numToPack, 
			      IntVector &VarNdx, double *buffer);

  ///
  virtual pebbl::parallelBranchSub* blankParallelSub();		// JWB

  ///
  void printSolution(const char* header = "", 
		     const char* footer = "", 
		     std::ostream& outStream = ucout)
    { parallelBranching::printSolution(header,footer,outStream); }

  /// Enhanced statistics printout etc.
  void printAllStatistics(std::ostream& stream = std::cout);

  ///
  void printRampUpHeurStats(std::ostream& stream);

  ///
  void postRampUpAbort(double aggBound)
    {
      printRampUpHeurStats(ucout);
      parallelBranching::postRampUpAbort(aggBound);
    };

  // Incumbent heuristic
  
  void parallelIncumbentHeuristic(double* controlParam);

  // For debugging (ramp up phase) only.  Processor sender will send
  // pseudocost information to processor comparer.  Comparer will
  // signal an error if there is any discrepancy.  For now this uses
  // the packing routine normally used for ramp up, and therefore there are
  // no counts shared.  Add that later if necessary.
  void comparePseudoCosts(int sender, int comparer);

  // This will initialize the MILP object with nothing.  
  // It will be initialized later from a message or getting the MPS file.

  parMILP()
    { 
      lastActivatedId.setEmpty();
      pCostCaster = NULL;
      fullInitVars = NULL;
      avgNumUnInit = 0;
      mergedAnything = false;
    };
      
  virtual ~parMILP();
      
  /// Note: should this include the VB flag?
  void reset(bool VBflag=true)
	{
	  MILP::reset(VBflag);
	  pebbl::parallelBranching::reset(false);
	  pebbl::gRandomReSeed();
	  mergedAnything = false;
	}

  // Only overridden for work cataloging
  virtual void preprocess();

  // Need to override this for parallel pseudocost initialization during ramp up
  // Each processor will take a piece of the work.
  int selectPseudocostInit(int nFrac, IntVector &FracVarNdx, int numFracSets,
			   IntVector &candidateSets, IntVector& initIndices,
			   bool &dontRemoveAllBranchChoices,
			   double tableInitFraction = -1.0);

  // Given a number of (ordered, say numbered) tasks to divide evenly
  // and lexicographically among the processors (meaning the 0th processor gets the first
  // chunk of work, processor 1 gets the second chunk, etc, determine what tasks
  // this processor does.  Specifically determine the task number of the first task, the number
  // of tasks to do, and, since some methods involving message passing need this, the maximum number
  // assigned to any processor and how many have the larger number.

  void divideWorkLexicographically(size_type numTasks, size_type &firstJob, size_type &numberOfJobs,
				   size_type &maxNumberJobs, size_type &numWithMax);

  
  // Used by the pseudocost broadcast thread to insert broadcasted data
  // into the local tables.
  
  void incorporatePCosts(int n,
			 IntVector&    indices,
			 DoubleVector& upValues,
			 DoubleVector& downValues);
  
  // Override default placeTasks method, so that we can add in the
  // pseudocast broadcast thread.
  
  void placeTasks();
  
  // Add initialization of the broadcasting data structures
  void resizePtable(int newSize);
  
  // Send gathered set of pseudocosts
  void sendPcosts();
  
  // Returns true if one of the pseudocosts waiting in the buffer is a
  // first-time pseudocost
  
  int firstPCostPending();
  
  // Check to see if initial broadcasting phase is through.  If so,
  // recoup the space and return true (otherwise return FALSE)
  
  int pcostBcastCleanup();
  
  void resetPcostBcast();
  
  // For debugging
  void printPendingPcostCast();
  
  // Override update of pseudocosts so we can store information for broadcast
  // if appropriate.
  
  void updateUpCostBuffer(int index, double value);
  void updateDownCostBuffer(int index, double value);
  
  void rampUpCleanUp()
    {
      if (fullInitVars != NULL)
	{
	  delete fullInitVars;
	  fullInitVars = NULL;
	}
      adjSet.resize(0);  // Variable interaction info no longer needed
    }
  
  bool continueRampUp()
    {
      if (avgNumUnInit == 0) return(true);
      int numSPs = spCount();
      if (numSPs < rampUpPoolLimit)
	return(true);
      int numWorkers = totalWorkers();
      // First test is also part of the base ramp-up test.  Repeat
      // it hear so we aren't bound to any changes in the base method.
      return((numSPs < rampUpPoolLimitFac* numWorkers) &&
	     (numSPs < numPCCrossFac * avgNumUnInit));
    }
  
  virtual bool hasParallelIncumbentHeuristic()  
    { 
      DEBUGPR(100, ucout << " hasParallelIncumbentMIPHeuristic():"
	      " mipHeuristic=" << mipHeuristic << std::endl);
      return  mipHeuristic;
    }

  pebbl::ThreadObj::ThreadState incumbentHeuristicState()
    {
      if ( mipHeuristicPtr == NULL || mipHeuristicPtr->emptyFlag )
	return pebbl::ThreadObj::ThreadBlocked;
      else
	return pebbl::ThreadObj::ThreadReady;
    };

  bool processParameters(int& argc, 
			 char**& argv, 
			 unsigned int min_num_required_args_=0)
    {
      return MILP::processParameters(argc,argv,min_num_required_args_);
    }

  void register_parameters();

  bool checkParameters(char* progName)
    {
      return parallelBranching::checkParameters(progName) && 
	     MILP::checkParameters(progName);
    }

  bool setupProblem(int& argc,char**& argv)
    {
      return MILP::setupProblem(argc,argv);
    }

  bool setup(int& argc,char**& argv)
    {
      return parallelBranching::setup(argc,argv);
    }

  bool setup(int& argc,char**& argv,MILPProblem& _MIP)
    {
      setMILPProblem(_MIP);
      return setup(argc,argv);
    }

  void solve();

  void getGlobalBound(double &returnBound, bool &boundKnown);
};


class parMILPNode: 
public pebbl::parallelBranchSub, virtual public MILPNode
{
public:

  virtual parMILP*   pmGlobal() const { return globalPtr; };
  pebbl::parallelBranching* pGlobal()  const { return globalPtr; };
  MILP*              mglobal()  const { return globalPtr; };
  pebbl::branching*         bGlobal()  const { return globalPtr; };

  void pack(PackBuffer& outBuffer);
  void packChild(PackBuffer& outBuffer,int whichChild);
  void unpack(UnPackBuffer& inBuffer);


  void parMILPNodeFromParMILP(parMILP* master_, bool initVB = true)
    {
      globalPtr = master_;
      if (initVB)
	MILPNodeFromMILP(master_);
    };
  
  void parMILPNodeAsChildOf(parMILPNode* parent, bool initVB=true)
    {
      globalPtr = parent->pmGlobal();
      if (initVB)
	MILPNodeAsChildOf(parent);
	}
  
  // A form of constructor
  pebbl::branchSub *makeChild(int whichChild)
    {
      return(makeParallelChild(whichChild));
    };
  
  // Need this for derived classes.  They will call the constructors
  // for MILPNode and branchSub.
  
  parMILPNode() {};
  
  ~parMILPNode() 
    { 
      DEBUGPRX(100,pmGlobal(),"parMILPNode destructor for " << this << std::endl);
    };
  
  // This method will always be overridden in derived classes, so
  // no need to worry about controlling initialization of virtual base classes
  pebbl::parallelBranchSub* makeParallelChild(int whichChild)
    {
      parMILPNode *child = new parMILPNode();
#ifdef ACRO_VALIDATING
 if (child == NULL)
   EXCEPTION_MNGR(std::runtime_error, "Failed to allocate a new parMILPNode\n");
#endif
      child->parMILPNodeAsChildOf(this);
      MILPNode::childInit(child, whichChild);
      return(child);
    };
  
  virtual void updateLastID()
  {
    pmGlobal()->updateLastActivated(id);
  }

  // In parallel, we don't want processors to deal with half-infeasible variables
  // as they initial their pseudocosts.  If they did, then FracVarNdx and setCandidateNdx
  // would be in a different order on each processor.  They'd then, as things stand, each
  // processor might make a different branching decision.  We could adjust the variable
  // selection methods in milp to make sure all processors make the same selection, but this
  // will be easier and more efficient (all processors will pseudofix in the same order at the
  // same time after exchanging pseudocost information.

virtual void pseudoCostInit(IntVector &initVars, 
			    int numBranches, 
			    bool dontRemoveAllBranchChoices,
			    bool removeBranchChoiceNow,
			    bool stopOnInfeasible)
  {
    // Only suppress pseudofixing if we're in ramp up.  Also at that time, it
    // makes sense to continue initializing pseudocosts after infeasibility since
    // other processors are likely working on that and this one would otherwise be idle.
  if (rampingUp())
    MILPNode::pseudoCostInit(initVars, numBranches, 
			     dontRemoveAllBranchChoices, false, false);
  else 
    MILPNode::pseudoCostInit(initVars, numBranches, 
			     dontRemoveAllBranchChoices, true, true);
  }

// Computes integrality changes during pseudocost initialization.  
// The MIP heuristic uses this during rampUp

  void postPseudoBranch(int branchChoice, branch_type direction); 

  // Sharing of pseudocost init information during ramp up
  void postPseudoCostInit(IntVector &initVars, bool dontRemoveAllBranchChoices, int &numBranches);


  // For debugging.  Someday make a macro to screen for processor ID
  virtual void print()  { printMILPNode(); }		// JWB
      
  // We can get pseudocost information in splitting (initialization)
  // or bounding, so overide these methods to include a call to broadcast
  // pseudocosts if appropriate.
  void boundComputation(double *controlParam)
    {
      DEBUGPRX(100,pmGlobal(),"From boundComputation\n");
      // Turn this on (and below in splitComputation) for ramp-up inconsistencies
      /*
      if (rampingUp())
	pmGlobal()->comparePseudoCosts(1,0);
      */
      MILPNode::boundComputation(controlParam);
      /*
      if (rampingUp())
	pmGlobal()->comparePseudoCosts(1,0);
      */
      pmGlobal()->sendPcosts();
    }
  
  int splitComputation()
    {
      DEBUGPRX(100,pmGlobal(),"From splitComputation\n");
      /*
      if (rampingUp())
	pmGlobal()->comparePseudoCosts(1,0);
      */
      int temp = MILPNode::splitComputation();
      /*
      if (rampingUp())
	pmGlobal()->comparePseudoCosts(1,0);
      */
      pmGlobal()->sendPcosts();
      return(temp);
    } 
  
  //To disambiguate, though I'm not sure why the compiler is confused.
  
  bool canFathom()
    {
      return(MILPNode::canFathom());
    }

  // Extra info output to validation log file when unpacking

  void valLogUnpackExtra();

  // Disambiguate which destroy print to use, since parallelBranching
  // overrides branching...

  void valLogDestroyPrint() { parallelBranchSub::valLogDestroyPrint(); };

  PicoLPInterface::problemState boundingSolve();

// Verify that the candidate solution is still feasible when integer values
// are set exactly.  During ramp up, only processor 0 does this (to force
// agreement on all decisions that affect the tree.

  virtual PicoLPInterface::problemState recheckSolution();

  void feedToIncumbentThread();
  virtual void rampUpIncumbentHeuristic();

 protected:
  
  parMILP *globalPtr;

};


inline pebbl::parallelBranchSub* parMILP::blankParallelSub()
{
  MEMDEBUG_START_NEW("parMILPNode");
  parMILPNode* tmp = new parMILPNode();
#ifdef ACRO_VALIDATING
 if (tmp == NULL)
   EXCEPTION_MNGR(std::runtime_error, "Failed to allocate a new parMILPNode\n");
#endif
  tmp->parMILPNodeFromParMILP(this);
  MEMDEBUG_END_NEW("parMILPNode");
  return (pebbl::parallelBranchSub *)tmp;
}


inline parMILP::~parMILP()
{
  if (pCostCaster)
    delete pCostCaster;
  if (fullInitVars != NULL)
    {
      delete fullInitVars;
      fullInitVars = NULL;
    }
}


} // namespace pico

#endif
#endif
















