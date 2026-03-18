/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file milpNode.h
 *
 * Defines the pico::MILPNode class.
 */

#ifndef pico_milpNode_h
#define pico_milpNode_h

#include <acro_config.h>
#include <utilib/DoubleVector.h>
#include <utilib/BitArray.h>
#include <utilib/memdebug.h>
#include <pico/BasisArray.h>
#include <pico/PicoLPInterface.h>
#include <pico/milp.h>

namespace pico {

///
/// MILPNode defines the objects that are nodes in the branch-and-bound tree.
/// Basic serial code for MILP solving in PICO (subproblem level code).
///
class MILPNode : virtual public pebbl::branchSub, public mipBase
{

  friend class MIPHeuristic;
  friend class lpSetupObj;
  friend class preSplitNode;
  
public:

  // This used to set the lastActivatedId to empty, but that had bad side effects
  // in B&C.  Now manage an extra bool for this.
  void forceColdStart() {mGlobal()->forceColdStart();};
  void allowWarmStart() {mGlobal()->allowWarmStart();};

protected:

  __LPSolver_Interface* lp;

  //MSN: Next line is heuristic stuff
  double mfValue; //Merit function value

  enum warmType
  {
    notWarm = 0,
    warm = 1
  };


// This will hold parent's basis until the problem is bounded.  Then it
// holds the basis for this subproblem.

  //  BasicArray(basisState) basis;  // Old implementation
  BasisArray basis;

// This holds values only for the integer variables, used (for instance)
// in pseudocost computations.
  DoubleVector solution;

  // For branching on sets, this plays a role similar to that of
  // solution for single-variable branches.  This is of size NumSets.
  // Indexed by set number.  See documentation for for the branchValue
  // member.
  DoubleVector spSetBranchValues;

BitArray binary_vals;  // Stores values of binary variables once fixed
		 // so if fixed[i] == 1, then binary_vals[i] gives value
               // Indexed integer variable number, so length is numIntegerVars()


// Indexed by integer variable number, so length is numIntegerVars()
  BitArray fixed;  // 1 if the integer variable is now fixed, 0 otherwise

  IntVector intLowerBounds, intUpperBounds;// node-specific reductions in
     // bounds from original, length numGeneralIntVars() since binary vals
     // done separately with bit arrays.

 // node-specific reductions in the set of binary variables that can have a
  // value of one for each special set, length numSets().  If spSetLowerBounds[i] = l
  // and spSetUpperBounds[i] = u, then in set i, the 0th through (l-1)st variables
  // must be 0, exactly one variable among the lth to the uth is 1, and all the
  // remaining variables (u+1 and higher) are 0, for this node.  Length is # sets.
  IntVector spSetLowerBounds, spSetUpperBounds;

  // To warm start a node created by branching on a set, need to know the old parent bound
  // that was replaced in the branch.  For example, if the parent bounds on the set were
  // L and U, and the child is an up branch with bounds L_c and U, then parentSetBound
  // holds L (if the child is a down child, this hold the old upper bound).  If we didn't
  // store this, a warm start would have to zero out all binary variables outside this node's
  // range.  A good deal of that could be redundant.
  int parentSetBound;

// Number of integer variables that are fractional in the solution
  int nFrac;

  // of size numIntegerVars(), gives integer var # of fractional vars
  IntVector FracVarNdx;  // Only valid after a problem has been bounded,
              // since it will differ from parent in incidentally-integral vars

  /// Number of special sets that are candidates for branching (there are fractional variables
  /// in the set).

int nSetCandidates;

/// Set numbers of the candidate sets.  Length numSets().  Only valid after bounding.

IntVector setCandidateNdx;

// Only the reduced costs for integer variables (used for locking).  Since only
// used locally in one routine is is *not packed* for now.
  DoubleVector reducedCosts;

// Which (currently fractional) integer variable to branch on.  Before bounding
// this is parent's value (used to reset bounds in LP on warmstart).  After
// bounding, this is the branchvariable for this problem.
  // If we're using special sets.  If branchVariable > # integer vars, it indicates
  // the number of the set to branch on (set # = branchVariable - #integerVars)
  int    branchVariable;  // gives integer var # (eg. index into IntegerVarNdx)

  // fractional value of the variable being branched on.  If we're using sets, and
  // we want to partition (L,u) to (L,u_1) (u_1 + 1, u), then this is u_1 + x, where
  // x is the sum of the LP values of set elements L to u_1.  Thus taking floor and
  // ceiling give indices into the vector in specialSets and deviation from integrality
  // up and down gives a measure consistent with that used to compute pseudocosts for
  // branches on single variables.
  double branchValue;
  branch_type branchType; // branch type for the next child to be generated

  int recordPCost; // This will be true unless the pseudocost information
  // for this subproblem was already recorded in the parent's separation
  // (from the pseudocost initialization)

  double lpBound; // This could be worse than the subproblem bound, but we
  // need to store them so we get accurate pseudocosts.  A subproblem could
  // inherit a strong bound from a parent's pseudocosts initializations.
  // Though the branch could make quite a difference in LP value compared to
  // the parent's initial LP value, this will not be reflected by comparing
  // to the subproblem bound.

// We'll use this to determine whether we have a warm start.

  pebbl::branchSubId parentId;

  virtual bool isWarmStart() 
  {
    return(parentWasLast() && !mGlobal()->mustColdStart());
  };
  

  virtual bool IwasLast() {return (id == mGlobal()->lastActivated());}
  virtual bool parentWasLast() {return (parentId == mGlobal()->lastActivated());}
  bool isRoot() {return (parentId.isEmpty());}

  //  to set primal vs. dual method in the LP
void  setSolverMethod(MILP::LPMethodType lp_method);

  void MILPNodeInit();  // Used for the guts of initializing both root and child
  void childInit(MILPNode *child, int whichChild);

  // Separated out so that child init routines of derived classes have access
  branch_type childBranchType(MILPNode *child, int whichChild);

public:

  static int numChildren;

  MILP* mGlobal()  	     { return globalPtr; };
  pebbl::branching* bGlobal() const { return globalPtr; };
  MILPProblem* mip()	     { return mGlobal()->MIP; }

  // For making the root. No need to ask about initializing virtual
  // base classes because this needs no extra initialization
  void MILPNodeFromMILP(MILP* master);

  void MILPNodeAsChildOf(MILPNode* parent, bool initVB=true); // For making a child
  // Use only and empty constructor because of derived classes.  With multiple
  // inheritance of a virtual base (VB) class, the VB class must have a constructor
  // without arguments.  The most derived subclass then calls all initialization routines.

  //MSN: Add initialization of mfValue: 
  //-1 indicates it is not in the population
  MILPNode() : mfValue(-1.0) { }

  ///
  ~MILPNode();

  //MSN: we need to override this method to delete mfValue
  //of this subproblems from the population
  void recycle();

  virtual void boundComputation(double* controlParam);
  virtual void makeActiveLP();
  virtual void noLongerActiveLP(){};

  // Pulled out so that derived classes can override this
  virtual void getSolution();
  // Derived classes need to override this
  virtual void getBasis(BasisArray& basisBuffer) const
    {lp->getBasis(basisBuffer);}
  PicoLPInterface::problemState runLP();
  virtual PicoLPInterface::problemState PCInitSolve()
    {return(runLP());}
  // Warning: changes in this will have to be propagated to
  // parMILPNode::boundingSolve();
  virtual PicoLPInterface::problemState boundingSolve();
  virtual bool useSavedSolutionNow()
    {
      return(isRoot() && mGlobal()->useSavedRootSolution);
    }
  virtual PicoLPInterface::problemState boundComputationGuts()
    {return(boundingSolve());}
  virtual bool updatePCFromBoundingSolve()
    {return(recordPCost);};
  // Get the root solution from a file (e.g. when the root subproblem has been solved by
  // another solver, perhaps on another platform).  This is particularly useful for
  // parallel runs when the root solve is much more expensive than a re-solve.
  PicoLPInterface::problemState retrieveRootSolution();
  int boundAndRecord(branch_type branchDirection, int branchngVar);
  void removeBranchChoice(int variableNum);
  virtual int        splitComputation();
// Turn a node into the root
  virtual void setRootComputation();
  virtual pebbl::branchSub* makeChild(int whichChild = anyChild);
// The state variable is inherited from branchSub
  virtual bool        candidateSolution()
  {
    //    if (state == bounded && nFrac == 0)
    //      {
    //      ucout << "Candidate Solution:\n";
    //      printMILPNode();
    //      }
    return ((state == bounded) && (nFrac == 0));
  }

  virtual void 	incumbentHeuristic();
  void BBHincumbentHeuristic(); // Nediak/Eckstein heuristic

  // A randomized rounding heuristic.  Round based on the LP
  // relaxation.  For a binary variable with fractional value f, round
  // to 1 with probability f and to 0 with probability (1-f).  For a
  // general integer variable with non-integer value v.f (f is the
  // fractional part), round to v+1 with probability f and to v with
  // probability (1-f).  If there are continuous variables, set them
  // with the LP if possible after all integer variables are set.
  // Apply to every node up to depth maxRRDepth.  At each invocation, do
  // numRRTrialsPerCall trials.  Default is trying once at the root
  // for every LP solve.  TODO: consider whether we only want to do once per
  // node, or at start/end, etc.
    // This will give an immediate incumbent for IPs where every setting
  // of the integer variables is feasible.

  void RRheuristic();

  // Used by the randomized rounding heuristic, and possibly useful
  // for user-defined incumbent heuristics.  Takes a partial solution
  // where all integer variables have been set to viable integer
  // values and sets the continuous variables in the solution.
  // Returns true if the method succeeds in setting the continuous
  // variables.  Sets solnKnownFeasible to true if the resulting
  // solution vector is known to be feasible.  The default behavior is
  // to use an LP to set the continuous variables.  In this case,
  // succeeding in setting the variables and knowing the solution is
  // feasible is the same test (LP solve).  More generally, in an
  // application with more structure, a user might know of a way to
  // set the continous variables from the integer variables without
  // using an LP.  If the user has a problem-specific feasibility
  // test, they can either set solnKnownFeasible (if it succeeds) or
  // return false if it fails.

  virtual bool setContinuousVars(DoubleVector &partialSolution, bool &solnKnownFeasible);

  double     	improveIncumbent();
  virtual void  updateIncumbent();
  virtual void  enumeration_effect(DoubleVector& ) {};

// Verify that the candidate solution is still feasible when integer values
// are set exactly.  Pulled out and made virtual to allow a parallel override.

  virtual PicoLPInterface::problemState recheckSolution();

  int urandomIncumbentHeuristic(DoubleVector& vec, double& value);

  void updatePseudoCosts(const double newBound, const double parent_bound,
	   const branch_type branchDirection, const int branchingVar, 
	   const double branchingVal);
  virtual void updateLastID()
  {
    mGlobal()->updateLastActivated(id);
  }

  void lockVariables();
  virtual int chooseBranchVariable(branch_type& branch_flag,
				   double& minBoundMvmt);

// Do some branching to inialize pseudocosts. 
// See .cpp file for a longer description.

  virtual void pseudoCostInit(IntVector &initVars, 
			      int numBranches, 
			      bool dontRemoveAllBranchChoices,
			      bool removeBranchChoiceNow = true,
			      bool stopOnInfeasible = true);

  // Actions to take after doing a pseudobranch during initialization.  
  // We override this in the parallel code.

  virtual void postPseudoBranch(int branchChoice, branch_type direction)
    {};


// Actions to take after (local, uniprocessor) pseudocost initialization
// computations.  We override this in various derived classes.


  virtual void postPseudoCostInit(IntVector &initVars, 
				  bool dontRemoveAllBranchChoices, 
				  int &numBranches)
    { };


  virtual void setBasis()
    {lp->setBasis(basis);};

// Set the bounds in the LP
  virtual void setBounds();
  // When all the bounds are reset "from scratch"
  void setAllLPBounds(BitArray& fixed, IntVector& intLowerBounds, 
			IntVector& intUpperBounds, BitArray& binary_vals);

// Adjust the bounds within a MILPNode to implement a branch.  Used to
// create children, in locking variables, and when infeasible branches
// are discovered during the computation of pseudocosts. This
// functionality is duplicated in packChild (changes should be
// propagated).

  void adjustNodeBounds(const int branchVariable, branch_type branch_flag);

// Adjust some bounds in the LP associated with a single branch choice
// (one variable or one set). This is used during pseudocost
// initialization, so we assume the node has the full ranges reflected
// in its variable/set bounds. branchChoice is encoded the way the
// MILP class encodes branching choices: the first # integers values
// are for variables, the next # sets values are for the sets.  If
// reset is true, then this actually *undoes* a branch (e.g. after
// "fake" branching for pseudocost initialization).

//  JE -- added ability to set bounds for a different LP
//  If the LP is omitted, we use MILPNode::lp

  void adjustLPBounds(int branchChoice, 
		      branch_type direction, 
		      bool reset=false)
    {
      adjustLPBounds(lp,branchChoice,direction,reset);
    };

  // Derived classes sometimes have to do this differently
  virtual int initialBasisSize()
    {return(lp->getNumCols() + lp->getNumRows());}

 public:

  // This used to be in the BC world only, since it was added for CGL
  // (the cut generators get all problem and solution information from
  // the OsiSolverInterface).  But move down to the MILP-only world to
  // allow starting from a saved root solution to work for (e.g.) LP-based
  // heuristics that expect to get information from the LP object.
  // Make sure the solver is in the state it
  // was when this subproblem was solved.
  void restoreLPSolver();

  void printBasis()
    {ucout << basis;}

  void adjustLPBounds(OsiSolverInterface* whichLP,
		      int branchChoice, 
		      branch_type direction, 
		      bool reset=false);

  // Routines that set up an arbitrary LP to conform to the present node
  // This can be done immediately, or stored in an lpSetupObj object.

 public:

  virtual lpSetupObj* makeLPSetupObj()
    {
      return new lpSetupObj(this);
    };
  
  virtual void setupLP(__LPSolver_Interface* sol);

  // End of lpSetup stuff.

 protected: 

  void MtoS_pack(PackBuffer& ) {ucout << "Using this?" << std::endl;}
  void MtoS_unpack(UnPackBuffer& ) {ucout << "Using this?" << std::endl;}

  void StoM_pack(PackBuffer& ) {ucout << "Using this?" << std::endl;}
  void StoM_unpack(UnPackBuffer& ) {ucout << "Using this?" << std::endl;}

  // implement this if we need it
  int numFixedVars()
  {
    EXCEPTION_MNGR(std::runtime_error, "Haven't implemented counting number of fixed variables");
    return(0);
  };
  int numFracVars() {return(nFrac);}


  int    branchVar() {return(branchVariable);}
  double branchVal() {return(branchValue);} 
  branch_type branchDir(){return(branchType);}

// Override the branchsub one so we can check for improper fathoming
  bool canFathom();

  bool containsWatchedPoint();
  bool containsOneWatchedPoint(DoubleVector &point_to_check);

 // Give more output for creating visual output of the tree
  void valLogSplitExtra();
  void valLogCreateExtra(pebbl::branchSub *parent);

// Compute the number and location of fractional variables which should be ints
  int computeFracVars();

  // For debugging

  virtual void printMILPNode();
  virtual void printIntegerVariableSettings();
  void checkLPBounds();

  // Global Pointer

protected:

  ///
  MILP* globalPtr;

};

// Note: If this changes, propagate changes to serialMIP driver code section where
// we save a root solution to a file.  We only have an LP solver there, so can't
// access this method directly.

 inline void MILPNode::getSolution()
   {
     lp->getColSolution(solution.data(), (int)solution.size(),
			mip()->integerVarNdx.data());
     DEBUGPR(300,ucout << "MILPNode::getSolution: " << std::endl
	     << solution << std::endl);
   }

// This is used by the handlers (search engines) to create the root.
// As an inline, it will generate a call to MILPNode with the MIP (branching
// class) pointer as an argument.

inline pebbl::branchSub* MILP::blankSub()
{

MEMDEBUG_START_NEW("MILPNode")
MILPNode *tmp = new MILPNode;
#ifdef ACRO_VALIDATING
 if (tmp == NULL)
   EXCEPTION_MNGR(std::runtime_error, "Failed to allocate a new MILPNode\n");
#endif
tmp->MILPNodeFromMILP((MILP *)this);
MEMDEBUG_END_NEW("MILPNode")
return (pebbl::branchSub *) tmp;
};

} // namespace pico

#endif
