/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// milpNode.cpp
//
// Basic serial code for MILP solving in PICO (subproblem level code).
//
// Cindy Phillips
//

#include <acro_config.h>
#include <utilib/_math.h>
#include <utilib/DUniform.h>
#include <pebbl/gRandom.h>
#include <pebbl/fundamentals.h>
#include <pico/milpNode.h>
#include <pico/BasisArray.h>
#include <pico/mipHeuristic.h>

using namespace std;
using namespace utilib;

/*  CAP: I hope this just goes away and the new intialization will work.  Keep this through the
    next attempt to run on Janus
#if defined(COUGAR) 

namespace utilib {

template<>
int        EnumBitArray<1,pico::basisState>::enum_count    = 4;

template<>
char*      EnumBitArray<1,pico::basisState>::enum_labels = "oLBU";

template<>
pico::basisState EnumBitArray<1,pico::basisState>::enum_vals[]   = { pico::other, pico::atLower, pico::basic, pico::atUpper };

}
#endif

*/

namespace pico {


int MILPNode::numChildren = 2;


// For making the root

void MILPNode::MILPNodeFromMILP(MILP* master)
{
  lp = master->lp;
  globalPtr = master;
  bound = -(mGlobal()->sense)*MAXDOUBLE;
  MILPNodeInit();

  // For heuristic -- mfValue unknown, use HUGE_VAL and inform the heuristic
  if (mGlobal()->mipHeuristicPtr)
    {
      mfValue = HUGE_VAL;
      mGlobal()->mipHeuristicPtr->inform(this);
    }
}

// For making a nonroot node

void MILPNode::MILPNodeAsChildOf(MILPNode *parent, bool initVB) 
{
  if (initVB)
    branchSubAsChildOf(parent);
  lp = parent->mGlobal()->lp;
  globalPtr = parent->mGlobal();

  // Use mfValue from the parent, until this
  // subproblem has been bounded
  if (mGlobal()->mipHeuristicPtr)
    {
      mfValue = parent->mfValue;
      mGlobal()->mipHeuristicPtr->inform(this);
    }

  MILPNodeInit();
}

void MILPNode::MILPNodeInit()
{
  //
  // Initialize the local LP solver
  //
  // Try doing this directly in the constructor to avoid exessive make/deletes
  //lp &= mGlobal()->lp;

  //DEBUGPR(4, ucout << "Entering MILPNode::MILPNode" << endl;);
  
  // get local reference to lp and mip()
  MILPProblem* localMipRef = mip();
 
  // We use this a lot for this routine, so keep local.
  int numInts = localMipRef->numIntegerVars();
  basis.resize(initialBasisSize());
  solution.resize(numInts);  // This is enlarged later for terminal nodes...
  fixed.resize(numInts);     // ... otherwise we don't care about cts variables 
  if (localMipRef->numBinaryVars() > 0)
  binary_vals.resize(numInts);

  intLowerBounds.resize(localMipRef->numGeneralIntVars());
  intUpperBounds.resize(localMipRef->numGeneralIntVars());

  size_type num_sets = (size_type)localMipRef->numSets();
  spSetLowerBounds.resize(num_sets);
  spSetUpperBounds.resize(num_sets);
  setCandidateNdx.resize(num_sets);
  spSetBranchValues.resize(num_sets);

  // nFrac is not valid until the problem is solved
  nFrac = localMipRef->numIntegerVars() + 1;
  nSetCandidates = 0; // also not valid, but if we aren't using sets, we'll want a reasonable number.
  FracVarNdx.resize(numInts);
  reducedCosts.resize(numInts);

  branchType = no_branch;
  branchValue = -999.0;		// This should never be used, but...
  lpBound = MAXDOUBLE * globalPtr->sense;
}

 
// This is to make the root node (no parent).  It assumes the LP is loaded.
 
void MILPNode::setRootComputation()
{
  // Note that basis is not valid till the problem is solved.  

  binary_vals.reset();

  // Root starts with just the bounds from the original problem.
  if (!mip()->getbounds(fixed,intLowerBounds,intUpperBounds,binary_vals, spSetLowerBounds, spSetUpperBounds))
    setState(dead);

  // Root has no parent.  We can make this value a named constant
  // if it makes sense.

  parentId.setEmpty();

  // branchVariable also not valid initially
  branchVariable = -1;  // valid entries should be 0 to numInts - 1
  recordPCost = FALSE;  // no parent value to compare to
  
  DEBUGPR(10, ucout << "MILPNode::setRootComputation() invoked.\n");
};



//---------------------------------------------
// Create a child node
//---------------------------------------------

pebbl::branchSub * MILPNode::makeChild(int whichChild) 
{
  MEMDEBUG_START_NEW("MILPNode")
    MILPNode *child = new MILPNode();
#ifdef ACRO_VALIDATING
 if (child == NULL)
   EXCEPTION_MNGR(runtime_error, "Failed to allocate new MILPNode\n");
#endif
    child->MILPNodeAsChildOf(this);
  MEMDEBUG_END_NEW("MILPNode")
    
  MEMDEBUG_START_RESIZE("MILPNode")
    childInit(child, whichChild);
  MEMDEBUG_END_RESIZE("MILPNode")

  return(child);
}

// Branches in preferred direction for child 0 and in opposite direction
// for child 1.  If a higher number is given, this will duplicate child
// 1.  Assume there are checks elsewhere for generation of too many children.

// Remember, if you add a field to the milp node (e.g. initialized here),
// you also have to initialize in packChild, and add to pack, unpack, and sp
// (subproblem) sizing routine (the one that estimates the packed size of
// a subproblem.

void MILPNode::childInit(MILPNode *child, int whichChild)
{
  child->lpBound = lpBound;
  child->recordPCost = recordPCost;
  child->basis << basis;  // This should be a copy, not a link

  child->fixed << fixed;
  child->binary_vals << binary_vals;

  // This is true for starters.  At least one more will be restricted later.
  child->intLowerBounds << intLowerBounds;
  child->intUpperBounds << intUpperBounds;
  if (mGlobal()->useSets)
    {
    child->spSetLowerBounds << spSetLowerBounds;
    child->spSetUpperBounds << spSetUpperBounds;
    }

  child->branchVariable = branchVariable;
  child->branchValue = branchValue;
  child->branchType = childBranchType(child, whichChild);

  // Update upper/lower bounds 

  child->adjustNodeBounds(branchVariable, child->branchType);

  if (mGlobal()->debug >= 10) {
    ucout << "parent= " << id << ", child = " << child->id 
	  << ", branchVar = " << branchVariable << ", type = " 
	  << (int)(child->branchType) << "\n";
  }

  child->parentId = id;

  // verbosity tag
  DEBUGPR(10, ucout << "Created child of " << this << '\n');
}


branch_type MILPNode::childBranchType(MILPNode *child, int whichChild)
{
  // branchType indicates preferred branch direction, which is defined to
  // be child 0.
  if (whichChild == 0)
    return(branchType);
  else {
    if (branchType == branch_up)  return(branch_down);
    else if (branchType == branch_down) return(branch_up);
    else EXCEPTION_MNGR(runtime_error,"Trying to create a child for a parent without branching indicator");
    return(no_branch); // to keep compiler happy
  }
}



// Presumably all the arrays are automatically collected, since weren't
// allocated with new() (done with vector mechanism).
MILPNode::~MILPNode() 
{
}

// For heuristic, delete mfValue of this subproblem from the
// population, then call the original method

void MILPNode::recycle()
{
  if (mGlobal()->mipHeuristicPtr)
    mGlobal()->mipHeuristicPtr->remove(this);
 
  branchSub::recycle();
}

//
// Use LP to compute the lower bound.  Has to work for root too with
// current search framework.
//
void MILPNode::boundComputation(double* controlParam) 
{

  DEBUGPR(5, ucout << "Bounding problem " 
	  << this << " branchVariable=" << branchVariable 
	  << " type=" << branchType << "\n");

#ifdef ACRO_VALIDATING
  DEBUGPR(2,if (mGlobal()->mipHeuristicPtr) 
	  mGlobal()->mipHeuristicPtr->checkLP("boundComputation"));
#endif

  // Added by JE.  Says we're doing one subproblem.  Later, we may
  // change this to be the number of pivots executed (set after calling solver)
  *controlParam = 1;

  makeActiveLP();

  // TODO: temp
  //  cout << "In boundComputation: \n";
  //  printIntegerVariableSettings();

  double parent_bound = lpBound;
 
  PicoLPInterface::problemState solvedState = boundComputationGuts();
 
  if (solvedState == PicoLPInterface::solved) {

    setState(bounded);

    // Do some post-LP calculations and lock
    // variables.  Don't bother if this solution
    // will be thrown away

    // As long as we're not enumerating (in which case the node lives on),
    //  I think it's OK to not deactivate this, since it will be eliminated
    // as soon as the candidate is recorded (no children.  No need to store cuts,
    // increment reference counts, etc, just to undo that).  If noLongerActiveLP
    // gains new functionality, consider changes here.

    if (candidateSolution())
      {
	if (mGlobal()->performing_enumeration())
	  noLongerActiveLP();
	else 
	  return;
      }

    else if (!canFathom()) 
      {

	// Need the solution for the integer variables to see how many integer
	// variables are fractional

	lockVariables();
	noLongerActiveLP();

	if (mGlobal()->mipHeuristicPtr)
	  mGlobal()->mipHeuristicPtr->update(this);
      }
    
    return;
  }

  // Serious problems would have been detected in boundComputationGuts().
  // Otherwise the problem is infeasible or past the cutoff, both of which
  // are valid reasons to fathom the problem (i.e. make it dead without error)
  setState(dead);
  if (!recordPCost) {
    return;
  }
  if (solvedState == PicoLPInterface::infeasible) {
    mGlobal()->updateFromInfeasible(branchVariable, branchType);
  }
  // hit cutoff; for update use the LP cutoff, which may not correspond
  // to the incumbent at this time
  else
    mGlobal()->updateFromCutoff(branchVariable, 
				branchType, 
				branchValue, 
				parent_bound,
				lp->getDualCutoff());
  return;
}

// Set the LP solver environment to reflect this subproblem

void MILPNode::makeActiveLP()
{
  // Cut off set to incumbent value in the updateIncumbent routine of MILP
  if (isRoot()) {
    setSolverMethod(mGlobal()->rootSimplexMethod);
    allowWarmStart();
    updateLastID();
    return;
  }
  setBounds();
  if (isWarmStart()) {
    setSolverMethod(mGlobal()->warmSimplexMethod);
    // I don't think we want to set the basis on a warm start (this is the point)
    //  setBasis();
  } else {
    setSolverMethod(mGlobal()->nonwarmSimplexMethod);
    setBasis();
  }
  // Have to do this at the end to make sure we update bounds, etc, correctly.
  allowWarmStart();
  updateLastID();
}

// Set LP bounds to reflect this subproblem.  Bounds in this subproblem already reflect
// any differences from the parent.

void MILPNode::setBounds() {
  if (!isWarmStart()) {
    setAllLPBounds(fixed, intLowerBounds, intUpperBounds, binary_vals);
    return;
  }
  // For now, a child differs from its parent in only one variable.
  DEBUGPR(150, ucout << "warm start\n");

  MILPProblem* localMipRef = mip();
  if (mGlobal()->useSets && branchVariable >= localMipRef->numIntegerVars())
    {
      // Branching on a set.  Set variables in the set outside the indicated index range to 0.
    int setNum = branchVariable - localMipRef->numIntegerVars();
    IntVector& thisSet = localMipRef->specialSets[setNum];
    size_type k;
    size_type setStart, setEnd;  // range of variables (indices into set) to be adjusted
    if (branchType == branch_up)
      {
      setStart = (size_type) parentSetBound;
      setEnd = (size_type) floor(branchValue);
      }
    else
      {
      setStart = (size_type) ceil(branchValue);
      setEnd = (size_type) parentSetBound;
      }
      // Only one variable allowed to receive the 1, so set it
    if (spSetLowerBounds[setNum] == spSetUpperBounds[setNum])
      lp->setColLower(localMipRef->integerVarNdx[thisSet[spSetUpperBounds[setNum]]], 1.0);

    for (k= setStart; k <= setEnd; k++)
      lp->setColUpper(localMipRef->integerVarNdx[thisSet[k]], 0.0);
    return;
    }
  // get LP variable number
  int thisVarIndex = localMipRef->integerVarNdx[branchVariable];
  if (localMipRef->vtype[thisVarIndex] == generalInteger) {
    if (branchType == branch_up)
      lp->setColLower(thisVarIndex,
			      intLowerBounds[localMipRef->reverseBoundsNdx[branchVariable]]);
    else 
      lp->setColUpper(thisVarIndex,
			      intUpperBounds[localMipRef->reverseBoundsNdx[branchVariable]]);
  } else if (localMipRef->vtype[thisVarIndex] == binary) {
    if (branchType == branch_up)
      lp->setColLower(thisVarIndex, (double) 1);
    else 
      lp->setColUpper(thisVarIndex, (double) 0);
  }
  else EXCEPTION_MNGR(runtime_error,"*** Trying to branch on noninteger variable");
}

// Adjust some bounds in the LP associated with a single branch choice
// (one variable or one set). This is used during pseudocost
// initialization, so we assume the node has the full ranges reflected
// in its variable/set bounds. branchChoice is encoded the way the
// MILP class encodes branching choices: the first # integers values
// are for variables, the next # sets values are for the sets.  If
// reset is true, then this actually *undoes* a branch (e.g. after
// "fake" branching for pseudocost initialization).

// Note -- JE -- added ability to modify an arbitrary LP instead of the 
// usual one.

void MILPNode::adjustLPBounds(OsiSolverInterface* whichLP,
			      int                 branchChoice, 
			      branch_type         direction, 
			      bool                reset) 
{
  DEBUGPR(200,ucout << "MILPNode::adjustLPBounds invoked"
	  << (reset ? " (reset flag)" : "") << endl);
  
  MILPProblem* localMipRef = mip();

  if (mGlobal()->useSets && branchChoice >= localMipRef->numIntegerVars())
    {
      // Branching on a set.  Set variables in the set 
      // outside the indicated index range to 0.

      DEBUGPR(200,ucout << "SOS set " 
			<< branchChoice - localMipRef->numIntegerVars()<<endl);
 
      int setNum = branchChoice - localMipRef->numIntegerVars();
      IntVector& thisSet = localMipRef->specialSets[setNum];
      size_type k;
      size_type setStart;       // Range of variables (indices into set)
      size_type setEnd;         // to be adjusted

      if (direction == branch_up)
	{
	  setStart = (size_type) spSetLowerBounds[setNum];
	  setEnd = (size_type) floor(spSetBranchValues[setNum]);
	}
      else
	{
	  setStart = (size_type) ceil(spSetBranchValues[setNum]);
	  setEnd = (size_type) spSetUpperBounds[setNum];
	}
#ifdef ACRO_VALIDATING
      if (setEnd > 10000)
		cout << "milpNode.cpp:471: heres a bad set: " << setNum<<endl;
#endif
      int intVarNum;
      for (k=setStart; k<=setEnd; k++)
	{
	  intVarNum = thisSet[k];
	  // Some of the variables in a special set range 
	  // could already be fixed (in this case to 0)
	  if (!fixed(intVarNum))
	    {
	      whichLP->setColUpper(localMipRef->integerVarNdx[intVarNum], 
				   (reset ? 1.0 : 0.0));
	      DEBUGPR(200,ucout << "Set upper bound of variable "
		      << intVarNum << '/'
		      << localMipRef->integerVarNdx[intVarNum]
		      << " to " << (reset ? 1.0 : 0.0) << endl);
	    }
	}
      return;
    }

  // Regular variable.  Get LP variable number

  int thisLPVarIndex = localMipRef->integerVarNdx[branchChoice];

  if (localMipRef->vtype[thisLPVarIndex] == generalInteger) 
    {
      int genIntNum = localMipRef->reverseBoundsNdx[branchChoice];
      int newVal;

      // Could do this with ? : syntax too, but it would make 
      // it a bit harder to read

      if (direction == branch_up)
	{
	  if (reset)
	    newVal = intLowerBounds[genIntNum];
	  else newVal = (int) ceil(solution[branchChoice]);
	  whichLP->setColLower(thisLPVarIndex, newVal);
	  DEBUGPR(200,ucout << "Set lower bound of general variable "
		  << branchChoice << '/' << thisLPVarIndex
		  << " to " << newVal << ", solution[" << branchChoice
		  << "]=" << solution[branchChoice] << endl);
	}
      else 
	{
	  if (reset)
	    newVal = intUpperBounds[genIntNum];
	  else newVal = (int) floor(solution[branchChoice]);
	  whichLP->setColUpper(thisLPVarIndex, newVal);
	  DEBUGPR(100,ucout << "Set upper bound of general variable "
		  << branchChoice << '/' << thisLPVarIndex
		  << " to " << newVal << "solution[" << branchChoice
		  << "]=" << solution[branchChoice] << endl);
	}
    }
  else  //binary
    {
      if (direction == branch_up)
	{
	  whichLP->setColLower(thisLPVarIndex, reset ? 0.0: 1.0);
	  DEBUGPR(100,ucout << "Set lower bound of binary variable "
		  << branchChoice << '/' << thisLPVarIndex
		  << " to " << (reset ? 0.0: 1.0) << endl);
	}
      else 
	{
	  whichLP->setColUpper(thisLPVarIndex, reset ? 1.0 : 0.0);
	  DEBUGPR(100,ucout << "Set upper bound of binary variable "
		  << branchChoice << '/' << thisLPVarIndex
		  << " to " << (reset ? 1.0 : 0.0) << endl);
	}
    }
}


// Runs the LP.  If the problem has serious problems, signal an error, which
// is assumed to abort the computation.  Otherwise, return the state (either
// solved, infeasible, or past cut-off).

PicoLPInterface::problemState MILPNode::runLP()
{
  if (!isRoot())
    lp->solve(true); // know the primal is bounded
  else  // doing pseudocost init on the root if the state is bounded (hence primal bounded)
     lp->solve(state==beingSeparated);
 
  PicoLPInterface::problemState solvedState = lp->state();
 
  if (solvedState == PicoLPInterface::unsolved)
    EXCEPTION_MNGR(runtime_error, "LP Returned Unsolved State");
  if (solvedState == PicoLPInterface::unbounded)
    {
    EXCEPTION_MNGR(runtime_error, "LP Instance Unbounded");
    }
  if (solvedState == PicoLPInterface::broken) {
    //    printMILPNode();
    //    lp->testLP();
    // DELETE THIS FIRST ONE
    lp->write("bad.mps", mps_format);
    lp->printBrokenInfo();
    checkLPBounds();
    EXCEPTION_MNGR(runtime_error, "LP Instance Broken");
  }

  return(solvedState);
}

// Warning: changes here should be propagated to
// parMILPNode::boundingSolve().  TODO: can we clean
// these methods up and avoid code duplication?

PicoLPInterface::problemState MILPNode::boundingSolve()
{
  PicoLPInterface::problemState solvedState;
  double parent_bound = lpBound;
  if (useSavedSolutionNow())
    {
    solvedState = retrieveRootSolution();
    if (solvedState != PicoLPInterface::solved)
      return(solvedState);
    // Make the LP solver look as though it has just finished this bounding
    restoreLPSolver();
    }
  else
    {
    solvedState = runLP();
    if (solvedState != PicoLPInterface::solved)
      return(solvedState);
    lpBound = lp->getObjValue();
    getSolution();
    //    cout << "Ran LP for problem " << id << ". New solution is: " << solution << "\n";
    getBasis(basis);
    // Temporary for basis experiments
    if (mGlobal()->recordBasisInfo)
      {
      *(mGlobal()->basisOutfile) << basis << "\n";
    //    cout << "New basis is: " << basis << "\n";
      // use serial numbers produced as a surrogate for # of bases output.  Most problems are
      // bounded before they're stored I think.
      if (mGlobal()->probCounter > mGlobal()->earlyStopBasisInfoCount)
	EXCEPTION_MNGR(runtime_error, "Hit the limit for basis recording.  Stopping computation\n");
      }
    }
    // LP value could improve the bound
  if (lround(lp->getObjSense()) == pebbl::minimize)
    bound = max(bound, lpBound);
  else bound = min(bound, lpBound);
  nFrac = computeFracVars(); // Also sets FracVarNdx, and computes set candidates

  // This should be true except for the root (which has no parent for
  // comparison) and cases where pseudocosts were recorded during a
  // pseudocost initialization phase and then the parent is separated
  // using one of these newly-initialized variables (i.e. this check
  // prevents a double recording).

  if (updatePCFromBoundingSolve())
    updatePseudoCosts(lpBound, parent_bound, branchType,  
		      branchVariable, branchValue);

  return(PicoLPInterface::solved);
}

  // Get the root solution from a file (e.g. when the root subproblem has been solved by
  // another solver, perhaps on another platform).  This is particularly useful for
  // parallel runs when the root solve is much more expensive than a re-solve.

PicoLPInterface::problemState MILPNode::retrieveRootSolution()
{
  PicoLPInterface::problemState solvedState;
  int tempState;
  // Just to be sure...
  forceColdStart();
  ifstream savedSolution("root-bounding-info.bin");
  if (savedSolution.bad())
    EXCEPTION_MNGR(runtime_error, "Can't open the saved root solution");
  savedSolution >> tempState;
  solvedState = (PicoLPInterface::problemState) tempState;
  if (solvedState != PicoLPInterface::solved)
    return(solvedState);
  savedSolution >> lpBound;
  savedSolution >> solution;
  savedSolution >> basis;
  return(PicoLPInterface::solved);
}

void MILPNode::setSolverMethod(MILP::LPMethodType method)
{
  if (method == MILP::dual)
    lp->setLPMethod(PicoLPInterface::dualSimplex);
  else if (method == MILP::primal)
    lp->setLPMethod(PicoLPInterface::primalSimplex);
  // Shouldn't happen if we really have an LPMethodType passed in
  else EXCEPTION_MNGR(runtime_error, "Illegal LP method: %d\n");
}


//---------------------------------------------
// Decide how many children to compute
// Also, decide how children are generated
//---------------------------------------------

int MILPNode::splitComputation()
{
  // get local reference to mip()
  MILPProblem* localMipRef = mip();

  // This compiles, but doesn't actually appear to turn on debugging.
  //if (id.serial==5) localMipRef->setDebug(100);

  int numInts = localMipRef->numIntegerVars();
 
  double minBenefit; // dummy for this (used to track quality of branching)
  // Note:  branchtype passed by reference; will be filled in
  branchVariable = chooseBranchVariable(branchType, minBenefit);
  if (state == dead) return(0);

  if (mGlobal()->useSets && (branchVariable >= numInts))
    branchValue = spSetBranchValues[branchVariable-numInts];
  else branchValue = solution[branchVariable];
  // For now, we are assuming only two children, even for general integer vars
  // childrenLeft is set in branchSub::splitProblem
  setState(separated);   // Added by JE when beingSeparated state introduced.
  return 2;
}

// Initialize some of the pseudocosts.  numBranches is the number of
// variables/sets to initialize.  It should never be 0, but the code
// will work even if it is. initVars has indices into the full set of
// branching choices.  If the entry is < # integer variables, it's a
// variable.  Otherwise it's a set. If dontRemoveAllBranchChoices is true, then
// even if all the choices are half infeasible, make sure that one
// still has its original bounds so we can legitimately branch on a
// choice with highest priority (at least for now, all these variables
// have the highest priority among all branching
// choices). removeBranchChoiceNow=false suppresses actions on half-infeasible
// branching choices (i.e. bounds narrowing and removal from branching
// consideration).  This is only false for derived parallel methods.
// The boolean parameter stopOnInfeasible set to true means that as soon as
// this method detects subproblem infeasibility (both branches of a branch choice
// are LP infeasible), then it stops computing any more pseudocosts.  This is
// default.  The only time it is false is in parallel ramp up when the processor
// would otherwise be idle.


void MILPNode::pseudoCostInit(IntVector &initVars, int numBranches, bool dontRemoveAllBranchChoices,
			      bool removeBranchChoiceNow, bool stopOnInfeasible)
{
  double upBound, downBound, bestBound;
  /* ucout << "pseudoCostInit, " << numBranches << " to init. id = " id << serial << " Vars are: \n";
     for (int tempry = 0; tempry < numBranches; tempry++)
     ucout << initVars[tempry] << ", ";
     ucout << "\n"; */

  int branchChoice;
  bool upRunInfeasible, downRunInfeasible;

#ifdef ACRO_VALIDATING
  double PCStart = CPUSeconds();
#endif

  makeActiveLP();

  do {
    branchChoice = initVars[--numBranches];
    // Reset to this subproblem's basis for each run.  For some problems, we were
    // getting numerical problems on subsequent down branching.  On all iterations after
    // the first, the up branch will also have two variable changes.  See if this improves
    // stability.
    setBasis();

    adjustLPBounds(branchChoice, branch_up);

    upRunInfeasible = boundAndRecord(branch_up, branchChoice);
    postPseudoBranch(branchChoice, branch_up);
    upBound = lp->getObjValue();

    // Restore bounds in LP
    adjustLPBounds(branchChoice, branch_up, true);

    // Set variable bounds for the down branch. 
    adjustLPBounds(branchChoice, branch_down);
      
    setSolverMethod((MILP::LPMethodType)(mGlobal()->warmSimplexMethod));
      // Reset to this subproblem's basis for each run.  For some problems, we were
      // getting numerical problems on subsequent down branching.
    setBasis();
    downRunInfeasible = boundAndRecord(branch_down, branchChoice);
    postPseudoBranch(branchChoice, branch_down);
    downBound = lp->getObjValue();

    // restore LP bounds
    adjustLPBounds(branchChoice, branch_down, true);

    if (upRunInfeasible)
      {
	upBound = bGlobal()->sense * MAXDOUBLE;
	mGlobal()->updateFromInfeasible(branchChoice, branch_up);

	if (downRunInfeasible) // subproblem can be fathomed
	  {
	    setState(dead);
	    downBound = bGlobal()->sense * MAXDOUBLE;
	    mGlobal()->updateFromInfeasible(branchChoice, branch_down);
	    if (stopOnInfeasible)
	      break;
	  }
	  // Take advantage of the half infeasibility by narrowing ranges and removing from consideration
	  // for branching.  We cannot do this if this is the last initialization
	  // and all previous ones have been half infeasible too.
	else if ((!dontRemoveAllBranchChoices || numBranches > 0) && removeBranchChoiceNow)
	  {
	  removeBranchChoice(branchChoice);
  // This is necessary for B&C, and no harm for straight B&B
	  adjustLPBounds(branchChoice, branch_down);
	  // Have to adjust LP bounds first since that relies on node bounds
	  adjustNodeBounds(branchChoice, branch_down);
	  }
      }

    else if (downRunInfeasible)
      {
	  // See above similar case for explanation
	if ((!dontRemoveAllBranchChoices || numBranches > 0) && removeBranchChoiceNow)
	  {
	    removeBranchChoice(branchChoice);
	    adjustLPBounds(branchChoice, branch_up);
	    // Have to adjust LP bounds first since that relies on node bounds
	    adjustNodeBounds(branchChoice, branch_up);
	  }
	downBound = bGlobal()->sense * MAXDOUBLE;
	mGlobal()->updateFromInfeasible(branchChoice, branch_down);
      }
    //  ucout << "upBound = " << upBound << ", downBound = " << downBound << ", bound = " << bound;
    if (lround(lp->getObjSense()) == pebbl::minimize)
      {
	bestBound = min(upBound, downBound); // valid bound for the subproblem
	bound = max(bestBound, bound); // could have seen a better one in other
	// subproblems
      }
    else
      {
	bestBound = max(upBound, downBound);
	bound = min(bestBound, bound);
      }
    //  ucout << ", new bound = " << bound << "\n";
  } while (numBranches > 0);
  if (state == beingSeparated)	// in branch & cut world, might still be active
      noLongerActiveLP();
  /*     mGlobal()->printPseudoCosts();
	 ucout << Flush; */
#ifdef ACRO_VALIDATING
  mGlobal()->MIPTimingInfo[PCInit] += CPUSeconds() - PCStart;
#endif
}

// Used in initializing pseudocosts.  The LP has been set already.  Returns
// 0 if the problem solved and 1 if the problem is infeasible or otherwise
// fathomable.  Major errors will result in an abort at the time of the solve.

int MILPNode::boundAndRecord(branch_type branchDirection, int branchingVar)
{
  PicoLPInterface::problemState solvedState = PCInitSolve();

  if (solvedState == PicoLPInterface::solved) {
    //   ucout << "BAR calling UPC\n";
    int numInts = mip()->numIntegerVars();
    double branchingVal;
    if (branchingVar >= numInts) // a set
      {
      branchingVal = spSetBranchValues[branchingVar-numInts];
      // the integer part encodes a place in the set list.  The
      // real branching part is just the fractional piece.
      branchingVal -= floor(branchingVal);
      }
    else branchingVal = solution[branchingVar];
      
    updatePseudoCosts(lp->getObjValue(), lpBound, branchDirection,
		      branchingVar, branchingVal);
    return(0);
  }
  else return(1);
}
  
// Adjust the bounds within a MILPNode to implement a branch.  Used to
// create children, in locking variables, and when infeasible branches
// are discovered during the computation of pseudocosts. This
// functionality is duplicated in packChild (changes should be
// propagated).

void MILPNode::adjustNodeBounds(const int thisBranchVariable, branch_type branch_flag)
{
  MILPProblem* localMipRef = mip();
  double thisBranchVal;

  // branching on a special set
  if (mGlobal()->useSets && thisBranchVariable >= localMipRef->numIntegerVars())
    {
    int setNum = thisBranchVariable - localMipRef->numIntegerVars();
    if (state == boundable)
      thisBranchVal = branchValue;
    else  thisBranchVal = spSetBranchValues[setNum];
      // Branching on a set.  Set variables in the set outside the indicated index range to 0.
    IntVector& thisSet = localMipRef->specialSets[setNum];
    size_type k, setStart, setEnd;
    int thisSetVar;
    // Figure out which binary variables will be forced to 0.
    // This are delimited by setStart and setEnd.
    if (branch_flag == branch_up)
      {
      setStart = (size_type) spSetLowerBounds[setNum];
      // Next step not necessary if this is for infeasibility during
      // pseudocost computation, but in that case, this value won't be
      // used anymore.
      parentSetBound = setStart;
      setEnd = (size_type) floor(thisBranchVal);
      spSetLowerBounds[setNum] = setEnd+1;
#ifdef ACRO_VALIDATING
      if ((size_type) spSetUpperBounds[setNum] < setEnd+1)
	EXCEPTION_MNGR(runtime_error,"Inconsistent set bounds for set" << setNum << " (" << setEnd+1 << "," << spSetUpperBounds[setNum] << "), branching up. setStart = " << setStart << ", branchValue = " << thisBranchVal << ". Node " << id << ". parent: " << parentId << "\n");
#endif
      // Only one variable allowed to receive the 1, so fix it
      if (spSetUpperBounds[setNum] == (int) setEnd+1)
	{
	thisSetVar = thisSet[setEnd+1];
	fixed.set(thisSetVar);
	binary_vals.set(thisSetVar);
	}
      }
    else // branching down
      {
      setStart = (size_type) ceil(thisBranchVal);
      setEnd = (size_type) spSetUpperBounds[setNum];
      parentSetBound = setEnd;
      spSetUpperBounds[setNum] = setStart - 1;
#ifdef ACRO_VALIDATING
      if ((size_type) spSetLowerBounds[setNum] > setStart-1)
	EXCEPTION_MNGR(runtime_error,"Inconsistent set bounds for set" << setNum << " (" << spSetLowerBounds[setNum] << "," << setStart-1 << ").  branching down. setEnd = " << setEnd << "branch value = " << thisBranchVal << "\n");
#endif

      // Only one variable allowed to receive the 1, so fix it
      if (spSetLowerBounds[setNum] == (int) setStart-1)
	{
	thisSetVar = thisSet[setStart-1];
	fixed.set(thisSetVar);
	binary_vals.set(thisSetVar);
	}
      }
    for (k= setStart; k <= setEnd; k++)
      {
      thisSetVar = thisSet[k];
#ifdef ACRO_VALIDATING
      if (fixed(thisSetVar) && binary_vals(thisSetVar))
	EXCEPTION_MNGR(runtime_error, "MilpNode::fix:  branching on set" << thisBranchVariable - localMipRef->numIntegerVars() << " when member var " << thisSetVar << " already fixed to 1\n");
#endif

      // It's OK to redo this if already fixed to 0 (this is possible)
      fixed.set(thisSetVar);
      binary_vals.reset(thisSetVar);
      }
    return;
    }

  //
  // branching on a variable
  //
  if (localMipRef->vtype[localMipRef->integerVarNdx[thisBranchVariable]] == binary) {
    fixed.set(thisBranchVariable);
    if (branch_flag == branch_up)
      binary_vals.set(thisBranchVariable);
    else
      binary_vals.reset(thisBranchVariable);
  }
  else {
    if (state == boundable)
      thisBranchVal = branchValue;
    else  thisBranchVal = solution[thisBranchVariable];
    int genIntVarNumber = localMipRef->reverseBoundsNdx[thisBranchVariable];
    if (branch_flag == branch_up)
      intLowerBounds[genIntVarNumber] = (int) ceil(thisBranchVal);
    else
      intUpperBounds[genIntVarNumber] = (int) floor(thisBranchVal);
    if (intLowerBounds[genIntVarNumber] == intUpperBounds[genIntVarNumber])
      fixed.set(thisBranchVariable);
#ifdef ACRO_VALIDATING
    if (intLowerBounds[genIntVarNumber] > intUpperBounds[genIntVarNumber])
      EXCEPTION_MNGR(runtime_error, "MilpNode::fix:  general variable with infeasible bounds.  Integer Variable: " << genIntVarNumber << ", (" << intLowerBounds[genIntVarNumber] << "," << intUpperBounds[genIntVarNumber] << ")");
#endif
  }
}

// Called when the pseudocost initialization routine finds that a branch
// can be fathomed.  Since the range will be narrowed, don't allow branching
// on this variable (remove from fractional variable list).

// TODO: also have to remove individual variables as choices when a set is removed
// (at least one side; that may need another argument).

void MILPNode::removeBranchChoice(int branchChoice)
{
  int i;
  if (branchChoice >= mip()->numIntegerVars())
    {
#ifdef ACRO_VALIDATING
    bool found = false;
#endif
    int setNum = branchChoice - mip()->numIntegerVars();
    for (i = 0; i < nSetCandidates; i++)
      if (setCandidateNdx[i] == setNum)
	{
	setCandidateNdx[i] = setCandidateNdx[nSetCandidates-1];
#ifdef ACRO_VALIDATING
	found = true;
#endif	
	break;
	}
#ifdef ACRO_VALIDATING
    if (!found)
      EXCEPTION_MNGR(runtime_error, "Trying to remove branchChoice " << branchChoice << " (set " << setNum << "). Not found\n");
#endif
    nSetCandidates--;
    return;
    }
  for (i = 0; i < nFrac; i++) {
    if (FracVarNdx[i] == branchChoice) {
      // Note:  if only 1 left, its index will still be in FracVarNdx[0]
      FracVarNdx[i] = FracVarNdx[nFrac-1];
      break;
    }
  }
  nFrac--;
}


double MILPNode::improveIncumbent() 
{
  // verbosity tag
  DEBUGPR(10, ucout << "Trying to improve incumbent...\n");

  if (state == beingBounded)
    return incumbentValue();

  if (nFrac == 0)
    EXCEPTION_MNGR(runtime_error, "Trying to improve terminal solution");

  // A heuristic could go here, but not in the present approach

  return incumbentValue();
};


// Not called unless this is a candidate solution (e.g.  no fractional
// variables), in which case solution should be valid.  But with a
// high integer tolerance, this may not be the case, so we check.
// Also set the integer variables to their exact integer values.
// Among other things, this makes it easier to compare solutions
// using, eg, standard utilib ==.  Node solution only contains integer
// variables, but the incumbent contains them all, so need to pull in
// the full solution now.

void MILPNode::updateIncumbent() 
{
  MILPProblem* localMipRef = mip();
 
  DEBUGPR(300,ucout << "MILPNode::updateIncumbent" << endl);
  DoubleVector fullSolution(mip()->numAllVars());
  lp->getColSolution(fullSolution.data());
  DEBUGPR(300,ucout << "Got solution" << endl);
  int j;
  int intVal;
  int numInts = localMipRef->numIntegerVars();
  // round all the integer variables (already tested for integrality)
  for (int i=0; i < numInts; i++) {
    j = localMipRef->integerVarNdx[i]; // get LP var# for this int var #
    intVal = lround(fullSolution[j]);
    fullSolution[j] = intVal;
    // Fix lower and upper bounds (doesn't matter if general or binary)
    // bounds of continuous variables are never changed   
    lp->setColLower(j, intVal);
    lp->setColUpper(j, intVal);
  }

  PicoLPInterface::problemState solvedState = recheckSolution();

  if ((solvedState == PicoLPInterface::solved) && 
      mGlobal()->performing_enumeration())
     {
     DEBUGPR(300,ucout << "MILPNode::updateIncumbent - enumeration effect" << endl);
     enumeration_effect(fullSolution);
     // Have to reopen bounds in the LP that were fixed to the integer values for the check
     setAllLPBounds(fixed, intLowerBounds, intUpperBounds, binary_vals);
     }
  else
     {
     DEBUGPR(300,ucout << "MILPNode::updateIncumbent - setting state dead" << endl);
     setState(dead);
     }

  if (solvedState == PicoLPInterface::solved) 
    {
    // could get a better lpBound from the resolve too
    DEBUGPR(300,ucout << "MILPNode::updateIncumbent - updating incumbent" << endl);
    mGlobal()->updateIncumbent(fullSolution, lpBound);
    if (state == beingBounded)
       mGlobal()->needPruning = false;
    }
  else {
    if (!(mGlobal()->suppressWarnings)) 
      ucout << "Warning: Not recording infeasible incumbent candidate,"
	    << " id: " << id << "\n";
  }
}

// Verify that the candidate solution is still feasible when integer values
// are set exactly.  Also update lpBound (if the solution is valid).
// TODO: replace the LP-based check with a matrix-vector multiply
// using the Osi routines

PicoLPInterface::problemState MILPNode::recheckSolution()
{
DEBUGPR(300,ucout << "Integer variables rounded.\n" << Flush);
  lp->setLPMethod(PicoLPInterface::primalSimplex);
  DEBUGPR(300,ucout << "Method set" << endl << Flush);

  // Setting the basis at this point could cause some problems with
  // the B&C derived class (don't know or care what cuts are loaded),
  // so let's not set the basis for now.  Hopefully won't be a problem
  // because all the variables are fixed.

  //  setBasis();  // presumably the final basis of the solve.
  //  DEBUGPR(300,ucout << "Basis set" << endl << Flush);
  // if this is infeasible, hope the LP solver won't bomb.

  PicoLPInterface::problemState recheckState = runLP();
  if (recheckState == PicoLPInterface::solved)
    lpBound = lp->getObjValue();

  return(recheckState);
}


// Apply the incumbent heuristic.  This may be overridden!

void MILPNode::incumbentHeuristic()
{
  DEBUGPR(100,ucout << "MILPNode::incumbentHeuristic():" << endl);

  // Bail out if the subproblem is not bounded
  if (state != bounded)
    return;

  MIPHeuristic* heur = mGlobal()->mipHeuristicPtr;

  if (heur == NULL) 
    { 
      //Heuristic is switched off
      DEBUGPR(10, ucout << "No heuristic present." << endl);
      return;
    }

  // If we have a heuristic, just let it decide if it wants to run
#ifdef ACRO_VALIDATING
  double MHStart = CPUSeconds();
#endif
  heur->globalBoundSet(mGlobal()->pool->knowsGlobalBound(),
		       mGlobal()->pool->globalBound());
  heur->possibleRun(this);
#ifdef ACRO_VALIDATING
  mGlobal()->MIPTimingInfo[MIPHeur] += CPUSeconds() - MHStart;
#endif
}


// This is called after a problem is bounded to determine how many
// integer variables are fractional in the optimal solution.
// It assumes the solution has been pulled from the LP already.
// It also sets FracVarNdx, the array containing the indices of the
// fractional variables, and if we're using special sets, it computes
// similar numbers and indices for the special sets.

int MILPNode::computeFracVars()
{
  // get local reference to mip()
  MILPProblem* localMipRef = mip();
  // used inside loop
 
nFrac = 0;
int numInts = localMipRef->numIntegerVars();
double roundDist;
integralityMeasure = 0;
double val;
int i;
for (i=0; i < numInts; i++) {
  val      = solution[i]; // Solution only has entries for int vars
#ifdef ACRO_VALIDATING
     int j = localMipRef->integerVarNdx[i]; // get LP var# for this int var #
     double lowerBound, upperBound;
     if (localMipRef->vtype[j] == binary) {
       if (fixed(i))
	 lowerBound = upperBound = (double)binary_vals(i);
       else
	 {
	 lowerBound = 0.0;
	 upperBound = 1.0;
	 }
       // TODO: Is this the right tolerance?
       if (val < lowerBound - integerTolerance || val > upperBound + integerTolerance)
	 {
	 printMILPNode();
	 ucout << "LP solver bounds are: " << lp->getColLower(j) << ", " << lp->getColUpper(j) << "\n";
	 //	 ucout << "Basis state for problem variable is:" << lp->getBasis(j) << "\n";

	 EXCEPTION_MNGR(runtime_error, "MILPNode::computeFracVars - binary variable " << i << " (name "
			<< (&(lp->getVariableName(j)[0]))
		       << "),(LP var " << j << ") = "
			<< val
			<< " supposed to be in range ["
			<< lowerBound 
			<< ","
			<< upperBound 
			<< "] on processor " << bGlobal()->rank());
        }
     else if ((localMipRef->vtype[j] == generalInteger) &&
               ((val < intLowerBounds[localMipRef->reverseBoundsNdx[i]] - integerTolerance) ||
                (val > intUpperBounds[localMipRef->reverseBoundsNdx[i]] + integerTolerance))) {
        lowerBound = lp->getColLower(j);
        upperBound = lp->getColUpper(j);
        EXCEPTION_MNGR(runtime_error,"MILPNode::computeFracVars - "
			<< (&(lp->getVariableName(j)[0]))
			<< "="
			<< val
			<< " ("
			<< localMipRef->reverseBoundsNdx[i]
			<< ") supposed to be in ["
			<< intLowerBounds[localMipRef->reverseBoundsNdx[i]]
			<< ","
                	<< intUpperBounds[localMipRef->reverseBoundsNdx[i]]
			<< "] ["
			<< lowerBound
			<< ","
			<< upperBound
			<< "] on pn" << bGlobal()->rank());
     }
     }
#endif
  if (fixed(i)) continue;

  if (!isInteger(val)) {
      FracVarNdx[nFrac++] = i;
      roundDist = ceil(val) - val;
      integralityMeasure += min(roundDist, 1 - roundDist);
      DEBUGPRX(200,bGlobal(),"Fractional variable " << nFrac - 1
	       << " idx=" << i << " val=" << val << endl);
      }
}
// Now compute sets that are candidates for branching
int setCount = localMipRef->numSets();
if (mGlobal()->useSets && setCount > 0)
  {
  nSetCandidates = 0;
  BasicArray<bool> setRecorded(setCount);
  // To be sure
  for(i=0; i<setCount; i++)
    setRecorded[i] = false;
  IntVector *setsForThisVar;
  size_type thisVarSetSize, k;
  int thisSet, thisSetLowEnd;
  for (i=0; i < nFrac; i++)
    {
    setsForThisVar = &(localMipRef->setReverseNdx[FracVarNdx[i]]);
    thisVarSetSize = (*setsForThisVar).size();
    for (k=0; k < thisVarSetSize; k++)
      {
      thisSet = (*setsForThisVar)[k];
      if (!setRecorded[thisSet])
	{
        setRecorded[thisSet] = true;
	setCandidateNdx[nSetCandidates++] = thisSet;
	}
      }
    }

  // Compute the branch values for special sets.  These play a role
  // similar to that played by solution for single-variable branches.
  // For solution, we can just save the appropriate LP values, but
  // here we must compute them.  We'll split based on LP value as close
  // to evenly (.5 out of 1) as possible.

  IntVector *varsForThisSet;
  double totalWeight; // accumulates LP values for variables in a special set
  double currWeight;
  int lastAdded; // set index for the last (positive) variable added to the running sum.
  int setID, idx;
  for (i=0; i < nSetCandidates; i++)
    {
    setID = setCandidateNdx[i];
    varsForThisSet = &(localMipRef->specialSets[setID]);
    thisVarSetSize = (*varsForThisSet).size();
    totalWeight = 0;
    lastAdded = -1;
    thisSetLowEnd = spSetLowerBounds[setID];
#ifdef ACRO_VALIDATING
    // Verify that all the weight (exactly 1) is distributed among the variables
    // in the appropriate range (range can be narrowed by branching).
    for (idx=spSetUpperBounds[setID]; idx >= thisSetLowEnd; idx--)
      totalWeight += solution[(*varsForThisSet)[(size_type)idx]];
    if (totalWeight < 1.0 - integerTolerance || totalWeight > 1.0 + integerTolerance)
      {
      printMILPNode();
      EXCEPTION_MNGR(runtime_error, "Special Set " << setID << " in node " << id << " has value " << totalWeight << " in set member range " << thisSetLowEnd << " to " << spSetUpperBounds[setID] << ". Should be 1.0.\n");
      }
    totalWeight = 0;
#endif
  // The fractional value reflects the weight in the upper end of the
  // range.  This will make branching movement (used for gradients)
  // consistent with the way they are computed for variables.  That
  // is, a branch up sets the weight in the upper end to 1 and a
  // branch down sets the weight in the upper end to 0, so the current
  // weight is used for comparison.  So, compute running sums from the top.

    // Note: idx can't be a size_type (unsigned int) directly because if the
    // set lower end is 0, this index may have to go negative to detect loop termination.

    for (idx = spSetUpperBounds[setID]; idx >= thisSetLowEnd; idx--)
      {
      currWeight = solution[(*varsForThisSet)[(size_type)idx]];
      if (currWeight > 0.0)
	{
	totalWeight += currWeight;
	if (totalWeight > 0.5)
	  {
	    // compare deviation from .5 of this set (which is the first over .5)
	    // with the last set (last under .5)
	    // Test is whether totalWeight - 0.5 > 0.5 - (totalWeight - currWeight),
	    // algebraically simplified
	  if (2 * totalWeight > 1.0 + currWeight) // deviation from .5 was better before crossing .5
	    {
	      // lastAdded will not be -1 (i.e. we will not be in this case if this is the first positive-valued
	      // variable and its value is over .5 because that elements weight (totalWeight in this case) will
	      // be < 1.
	      spSetBranchValues[setID] = lastAdded -1 + totalWeight - currWeight;
#ifdef ACRO_VALIDATING
	      if (spSetBranchValues[setID] > spSetUpperBounds[setID] ||
		  spSetBranchValues[setID] < spSetLowerBounds[setID])
		EXCEPTION_MNGR(runtime_error, "Special Set " << setID << " has branching value " << spSetBranchValues[setID] << ". Should be in [" << spSetLowerBounds[setID] << "," << spSetUpperBounds[setID] << "].\n");
#endif
	    }
	  else
	    {
	    spSetBranchValues[setID] = idx -1 + totalWeight;
#ifdef ACRO_VALIDATING
	      if (spSetBranchValues[setID] > spSetUpperBounds[setID] ||
		  spSetBranchValues[setID] < spSetLowerBounds[setID])
		EXCEPTION_MNGR(runtime_error, "Special Set " << setID << " has branching value " << spSetBranchValues[setID] << ". Should be in [" << spSetLowerBounds[setID] << "," << spSetUpperBounds[setID] << "].\n");
#endif
	    }
	  break;
	  }
	lastAdded = idx;
	}
      }
    }

 // For debugging
  /*
  ucout << "Computing frac set for node " << id << "with branch variable " << branchVariable << "\nSolution: " << solution << "\n";
  ucout << "Sets eligible for branching: " << setCandidateNdx << "\n";
  ucout << "Set branch values: " << spSetBranchValues << "\n"; */
  }
 DEBUGPRX(200,bGlobal(),nFrac << " fractional variables:\n");
// This is used to set nFrac in the milpNode (not returning the # of branch choices)
 return(nFrac);
}


void MILPNode::updatePseudoCosts(const double newBound, 
				 const double parent_bound, 
				 const branch_type branchDirection, 
				 const int branchingVar, 
				 const double branchingVal)
{
#ifdef ACRO_VALIDATING
if (ceil(branchingVal) == branchingVal) {
   if (!(mGlobal()->suppressWarnings))
     ucerr << "MILPNode::updatePseudoCosts -- called with near integral branching value " << branchingVal;
   return;
   }
#endif
/*
 ucout << "UPC: parent bound = " << parent_bound << ", new bound = " << newBound << ", variable: " << branchingVar << ", branchingVal: " << branchingVal << "\n";*/
double pCostDatum;
if (lround(lp->getObjSense()) == pebbl::minimize)
   pCostDatum = (newBound - parent_bound);
else 
   pCostDatum = parent_bound - newBound;

// This will presumably be a tiny negative number (a slight degradation
// in the bound) caused by solution tolerance of the LP.
if (pCostDatum < 0) pCostDatum = 0;
if (branchDirection == branch_up)
   pCostDatum /= (ceil(branchingVal) - branchingVal);
else
   pCostDatum /= (branchingVal - floor(branchingVal));

if ((pCostDatum < MAXDOUBLE) && (branchingVar >= 0)) {
   if (branchDirection == branch_up) 
      mGlobal()->updateUpCostBuffer(branchingVar, pCostDatum);
   else
      mGlobal()->updateDownCostBuffer(branchingVar, pCostDatum);
   }
}


int MILPNode::chooseBranchVariable(branch_type& thisBranchType, double& minBoundMvmt)
{
IntVector initVars(nFrac + nSetCandidates);
bool dontRemoveAllBranchChoices;
int numBranches = mGlobal()->selectPseudocostInit(nFrac, FracVarNdx, nSetCandidates, setCandidateNdx, initVars, dontRemoveAllBranchChoices);
if (numBranches > 0)
  {
    DEBUGPR(9, ucout << "Initializing " << numBranches << " branch choices out of " << nFrac + nSetCandidates << " candidates for " << id << endl;);
  pseudoCostInit(initVars, numBranches, dontRemoveAllBranchChoices);
  DEBUGPR(9, ucout << "Back from pseudocost init for " << id << endl;);
  }

// In parallel, this will be overridden to exchange pseudocost initialization.
// Have to pull this out of the previous conditional because during ramp up, we
// might run into the case where some processors have no initialization work.  However,
// they must participate in the pseudocost exchange to receive the info and to avoid deadlock.
// In parallel, initVars is restored to the full set of variables just initialized (not just
// the ones initialized locally) so the next check is correct.
postPseudoCostInit(initVars, dontRemoveAllBranchChoices, numBranches);

if (state == dead) return(-1);
int chosenVar = mGlobal()->selectVariable(solution, nFrac, FracVarNdx, thisBranchType, minBoundMvmt, &spSetBranchValues, nSetCandidates, &setCandidateNdx);
recordPCost = TRUE;
for (int i = 0; i < numBranches; i++)
  {
  if (initVars[i] == chosenVar)
    {
    recordPCost = FALSE;
    break;
    }
  }
return(chosenVar);
}


void MILPNode::lockVariables()
{
// get local reference to mip()
 MILPProblem* localMipRef = mip();
 
  lp->getReducedCost(reducedCosts.data(), nFrac, FracVarNdx.data());

  for (int i=0; i<nFrac; i++) {
    //  if (fixed(FracVarNdx[i])) continue; // checked in computeFracVars
    int ii = FracVarNdx[i];  // integer variable number

    int j = localMipRef->integerVarNdx[ii];  // lp variable number
    if (localMipRef->vtype[j] == binary) {
      if ((solution[ii] <= integerTolerance) && 
          mGlobal()->canFathom(bound+reducedCosts[i]))
        adjustNodeBounds(i,branch_down);
        //fix(i,branch_down);
      else if ((solution[ii] >= (1.0 - integerTolerance)) && 
	       mGlobal()->canFathom(bound-reducedCosts[i]))
	{
        adjustNodeBounds(i,branch_up);
	}
    }

    else if (mGlobal()->canFathom(bound+fabs(reducedCosts[i]))) {
      if ((reducedCosts[i] > 0) && 
	  (solution[ii] <= intLowerBounds[localMipRef->reverseBoundsNdx[ii]]
	                   + integerTolerance)) 
	{
	  adjustNodeBounds(i,branch_down);
	}
      else if ((reducedCosts[i] < 0) && 
	       (solution[ii] >= intUpperBounds[localMipRef->reverseBoundsNdx[ii]]
		                - integerTolerance)) 
	{
	  adjustNodeBounds(i,branch_up); 
	}
    }
  }
}

// This routine is used to completely reset the integer upper and lower bounds
// in the LP whenever a node is run as a non-warm-start.

// The arrays are called by reference for efficiency.  They are not changed.

// Bounds introduced by branching on sets should be reflected in the variable bounds.

void MILPNode::setAllLPBounds(BitArray& thisFixed, IntVector& theseIntLowerBounds, 
		    IntVector& theseIntUpperBounds, BitArray& theseBinary_vals)
{
   // get local reference to mip()
   MILPProblem* localMipRef = mip();

  int numInts = localMipRef->numIntegerVars(); // avoid calling this in loop body
  DoubleVector LB(numInts), UB(numInts);
  variableType thisVarType;
  int thisVarIndex;
  for (int i = 0; i <numInts; i++)
  {
    thisVarIndex = localMipRef->integerVarNdx[i]; //index into LP
    thisVarType = localMipRef->vtype[thisVarIndex];
    if (thisVarType == binary)
      {
      if (thisFixed(i))
	{
	LB[i] = (double)theseBinary_vals(i);
	UB[i] = (double)theseBinary_vals(i);
	}
      else
	{
	LB[i] = 0.0;
	UB[i] = 1.0;
	}
      }
    else if (thisVarType == generalInteger)
      {
      LB[i] = (double)theseIntLowerBounds[(localMipRef->reverseBoundsNdx)[i]];
      UB[i] = (double)theseIntUpperBounds[(localMipRef->reverseBoundsNdx)[i]];
      }
    // The continuous (rational) variables will not have any changes in
    // bounds (assume set at initialization).
  }
  //printMILPNode();
  //    cout << "Setting bounds, Lower: " << LB << "\nUpper: " << UB << "\n";
lp->setColLower(LB.data(), localMipRef->integerVarNdx.data(), numInts);
lp->setColUpper(UB.data(), localMipRef->integerVarNdx.data(), numInts);
// for debugging, remove
//lp->getColLower(LB.data(), localMipRef->integerVarNdx.data(), numInts);
// cout << "Lower bounds back from LP: " << LB << "\n";
}

// Check for fathoming of optimal solutions

bool MILPNode::canFathom()
{
#ifdef ACRO_VALIDATING
  bool canChop = coreSPInfo::canFathom();  // regular check
  if (canChop && mGlobal()->checkFathoming())
    {
    if (containsWatchedPoint())
      {
      // Saved solution is inside the region wrt all integer variables
      printMILPNode();
      ucout << mGlobal()->cache << endl;
      EXCEPTION_MNGR(runtime_error, "About to fathom the saved solution!");
      }
    }
  return(canChop);
#else
return branchSub::canFathom();
#endif
}

// Return TRUE if the region represented by this milpNode contains one
// of the watched points. 

bool MILPNode::containsWatchedPoint()
{
size_type numSolutions = mGlobal()->solutionsToWatch.size();
for (size_type stepper = 0; stepper < numSolutions; stepper++)
  {
  if (containsOneWatchedPoint(mGlobal()->solutionsToWatch[stepper]))
    return(true);
  }
return(false); 
}

bool MILPNode::containsOneWatchedPoint(DoubleVector &point_to_check)
{
// get local reference to mip()
 MILPProblem* localMipRef = mip();
 
 int numints = localMipRef->numIntegerVars();
 variableType thisVtype;
 int genVarnum;
 int lpVarNum;
 bool pointContained = true;
 for (int i = 0; i < numints; i++)
   {
   lpVarNum = localMipRef->integerVarNdx[i]; // get LP var# for this int var #
   thisVtype = localMipRef->vtype[localMipRef->integerVarNdx[i]];
   if (thisVtype == binary)
     {
     if (fixed(i) == 1 &&
	 point_to_check[lpVarNum] != binary_vals(i))
       {
 // this watched solution disagrees on this fixed value
       pointContained = false;
       break;
       }
     }
   else if (thisVtype == generalInteger)
     {
     genVarnum = localMipRef->reverseBoundsNdx[i];
     if (point_to_check[lpVarNum] > intUpperBounds[genVarnum] ||
	 point_to_check[lpVarNum] < intLowerBounds[genVarnum])
       {
 // this watched solution disagrees on this gen var
       pointContained = false;
       break;
       }
     }
// If not tested above, this is a continuous variable.
// PICO will not impose additional bounds
   }
 return(pointContained);
}


// For debugging.  Comment out irrelevant portions at any given time.

void MILPNode::printMILPNode()
{
// get local reference to mip()
 MILPProblem* localMipRef = mip();
 
  int i, j;
  int numInts = localMipRef->numIntegerVars();
  int numGeneral = localMipRef->numGeneralIntVars();
  ucout << "printing subproblem " << id << "\n";
  ucout << stateString(state) << '\n';   // Big switch here replaced by JE
  ucout << "bound: " << bound << "\n";
  ucout << "LP bound: " << lpBound << "\n";
  ucout << "incumbent value " << incumbentValue() << "\n";
  ucout << "Integer variables\n";
  ucout << localMipRef->integerVarNdx;
  ucout << "\nGeneral Integer numbering (-1 is binary)\n";
  ucout << localMipRef->reverseBoundsNdx;

  if (mGlobal()->useSets)
    {
      ucout << "\nSpecial Set Bounds:\n";
    j = 0;
    for (i=0; i< localMipRef->numSets(); i++)
      {
      ucout << i << ": (" << spSetLowerBounds[i] << ", " << spSetUpperBounds[i] << ") ";
      if (++j == 10)
	{
	ucout << "\n";
	j = 0;
	}
      }
    }
  
  ucout << "\nFixed Binary Variables: (" << localMipRef->numBinaryVars() << ")\n";
  // Recall indexed by integer variable number, so not all entries valid
  j = 0;
  for (i = 0; i < numInts; i++)
    {

      // Only print out fixed binary variables
    if (localMipRef->vtype[localMipRef->integerVarNdx[i]] == binary && fixed(i) == 1)
      {
	ucout << i << ": " << binary_vals(i) << "   ";
	if (++j == 5)
	  {
	    ucout << "\n";
	    j = 0;
	  }
      }
    }
  ucout << "\n";
    ucout << "General Integer Variables: (" << numGeneral << ")\n";
  j = 0;
  for (i = 0; i < numGeneral; i++)
    {
      ucout << i << ": (" << intLowerBounds[i] << ", " << intUpperBounds[i] << ")   ";
      if (++j == 5)
	{
	  ucout << "\n";
	  j = 0;
	}
      ucout << "\n";
    }
  // Mixed integer.  Give mapping of general integer variables [FILL IN]
  if (localMipRef->numAllVars() > localMipRef->numIntegerVars())
    {
    }
  ucout << "branchVariable: " << branchVariable << "\n";
  ucout << "branchValue: " << branchValue << "\n";
  switch (branchType) {
  case branch_up:
    ucout << "branch up\n";
    break;
  case branch_down:
    ucout << "branch down\n";
    break;
  case no_branch:
    ucout << "no branch\n";
    break;
  default:
    ucout << "invalid branch type\n";
  }
  ucout << "parentId: " << parentId << "\n";

  // Output only if valid
  if (state == bounded || state == separated)
    {
      ucout << "solution:\n";
      j = 0;
      for (i = 0; i < numInts; i++)
	{
	ucout << i << ": " << solution[i] << "   ";
	if (++j == 5)
	  {
	    ucout << "\n";
	    j = 0;
	  }
	}
      ucout << "\n";
    }
  if (nFrac < numInts + 1)  // only output if it's been computed.
    {
      j = 0;
      ucout << "nFrac  " << nFrac << "\n";
      for (i = 0; i < nFrac; i++)
	{
	ucout << i << ": " << FracVarNdx[i] << "   ";
	if (++j == 5)
	  {
	    ucout << "\n";
	    j = 0;
	  }
	}
      ucout << "\n";
    }
  ucout << Flush;
}

// For debugging.  Print variable settings (binary and general) as
// the node has it recorded and minimal identifying
// information.  Verify that the LP settings agree with bounds.

void MILPNode::printIntegerVariableSettings()
{
 MILPProblem* localMipRef = mip();
 
  int numInts = localMipRef->numIntegerVars();

  ucout << "subproblem: " << id << "\n";
  ucout << "branchVariable: " << branchVariable << "\n";
  ucout << "branchValue: " << branchValue << "\n";
  switch (branchType) {
  case branch_up:
    ucout << "branch up\n";
    break;
  case branch_down:
    ucout << "branch down\n";
    break;
  case no_branch:
    ucout << "no branch\n";
    break;
  default:
    ucout << "invalid branch type\n";
  }
  ucout << "parentId: " << parentId << "\n";
  ucout << "bound (possibly from parent):" << bound << "\n";

  int j = 0;
  int linearVarNum, genIntVarNum;
  variableType thisVarType;
  for (int i = 0; i < numInts; i++)
    {
    thisVarType = localMipRef->vtype[localMipRef->integerVarNdx[i]];
    if (thisVarType == continuous || thisVarType == initiallyFixed)
      continue;
    linearVarNum = localMipRef->integerVarNdx[i];
    if (thisVarType == generalInteger)
      {
      genIntVarNum = localMipRef->reverseBoundsNdx[i];
      ucout << i << "(" << genIntVarNum << "): (" << intLowerBounds[genIntVarNum] << ", " << intUpperBounds[genIntVarNum] << ")   ";
      if (++j == 3)
	{
	  ucout << "\n";
	  j = 0;
	}
      if (lp->getColLower (linearVarNum) < intLowerBounds[genIntVarNum] - integerTolerance)
	    EXCEPTION_MNGR(runtime_error,"Variable " << i << " ("
			    << linearVarNum
			   << ") has LP lower bound value "
			   << lp->getColLower(linearVarNum) << "\n");

      if (lp->getColUpper(linearVarNum) > intUpperBounds[genIntVarNum] + integerTolerance)
	    EXCEPTION_MNGR(runtime_error,"Variable " << i << " ("
			    << linearVarNum
			   << ") has LP upper bound value "
			   << lp->getColLower(linearVarNum) << "\n");
      continue;
      }
    // Must be binary.
      // Only print out fixed binary variables
    if  (fixed(i) == 1)
      {
	ucout << i << ": " << binary_vals(i) << "   ";
	if (++j == 5)
	  {
	    ucout << "\n";
	    j = 0;
	  }
	if (binary_vals(i))
	  {
	  if (lp->getColLower(linearVarNum) != 1)
	    EXCEPTION_MNGR(runtime_error,"Variable " << i << " ("
			    << linearVarNum
			    << ") fixed on, but lower bound "
			    << lp->getColLower(linearVarNum));
	  if (lp->getColUpper(linearVarNum) != 1)
	    EXCEPTION_MNGR(runtime_error,"Variable " << i << " ("
			    << linearVarNum
			    << ") fixed on, but lower bound "
			    << lp->getColUpper(linearVarNum));
	  }
	else
	  {
	  if (lp->getColLower(linearVarNum) != 0)
	    EXCEPTION_MNGR(runtime_error,"Variable " << i << " ("
			    << linearVarNum
			    << ") fixed off, but lower bound "
			    << lp->getColLower(linearVarNum));
	  if (lp->getColUpper(linearVarNum) != 0)
	    EXCEPTION_MNGR(runtime_error,"Variable " << i << " ("
			    << linearVarNum
			    << ") fixed off, but lower bound "
			    << lp->getColUpper(linearVarNum));
	  }
      }
    else // not fixed
      if (lp->getColUpper(linearVarNum) != 1 ||
	  lp->getColLower(linearVarNum) != 0)
	    EXCEPTION_MNGR(runtime_error,"Variable " << i << " ("
			    << linearVarNum << ") not fixed, but bounds ("
			    << lp->getColLower(linearVarNum)
			    << ", "
			    << lp->getColUpper(linearVarNum)
			    << ")");
    }
}

int MILPNode::urandomIncumbentHeuristic(DoubleVector& vec, double& value)
{
RNG* rng = pebbl::gRandomRNG();
DUniform dunif(rng);

int flag=0;
int ctr=0;

vec << solution;
while (!flag && (ctr < 10)) {
  //
  // Iterate through the fractional variables, generating a random 
  // integer variable for that value.
  //
  for (int i=0; i<nFrac; i++) {
    if (fixed(FracVarNdx[i])) continue;

    //int ii = FracVarNdx[i];  // integer variable number
    //int j = mip()->integerVarNdx[ii];  // lp variable number
    if (dunif(0,1))
       vec[i] = ceil(vec[i]);
    else
       vec[i] = floor(vec[i]);
    }
  // 
  // Check if 'vec' is feasible
  //

  ctr++;
  }

if (flag)
   value = lp->evalVector(vec);

return flag;
}

// Output extra information to the log files for creation of a graphic
// representation of the tree

void MILPNode::valLogSplitExtra()
{
  *vout << ' ' << branchVariable << ' ' << branchValue;
}

void MILPNode::valLogCreateExtra(branchSub *parent)
{
  *vout << ' ' << branchType << ' ' << depth;
}

//  Compare this node's variable bounds to those currently in the LP solver.
// Report any discrepancies for integer variables.  I think we use the LP to
// store the bounds for continuous variables, since we don't change them.  Therefore,
// no current way to compare them.

// Should this print to ucout??  That doesn't seem to work on Janus.
void MILPNode::checkLPBounds()
{
  MILPProblem* localMipRef = mip();
  int numVars = lp->getNumCols();
  int numIntVars = localMipRef->numIntegerVars();
  int i;
  BasicArray<Ereal<double> > lowerBounds(numVars);
  BasicArray<Ereal<double> > upperBounds(numVars);
  lp->getColLower(lowerBounds);
  lp->getColUpper(upperBounds);
  variableType currentType;
  double currentLower, currentUpper, lpLower, lpUpper;
  cout << "Checking for LP bounds discrepancies for subproblem " << id.creatingProcessor << ": " << id.serial << ":\n";
  for (i=0; i < numIntVars; i++)
    {
    currentType = localMipRef->vtype[localMipRef->integerVarNdx[i]];
    if (currentType == continuous || currentType == initiallyFixed)
      EXCEPTION_MNGR(runtime_error,"Integer variable " << i << ", has incorrect type");
    if (currentType == binary)
      {
      if (fixed(i))
	{
	currentLower = binary_vals(i);
	currentUpper = currentLower;
	}
      else
	{
        currentLower = 0.0;
	currentUpper = 1.0;
	}
      }
    else // general integer variable
      {
      currentLower = (double)intLowerBounds[localMipRef->reverseBoundsNdx[i]];
      currentUpper = (double) intUpperBounds[localMipRef->reverseBoundsNdx[i]];
      }
    lpLower = lowerBounds[localMipRef->integerVarNdx[i]];
    lpUpper = upperBounds[localMipRef->integerVarNdx[i]];

    if (currentLower != lpLower || currentUpper != lpUpper)
      cout << "Integer variable " << i << "(LP var " << localMipRef->integerVarNdx[i] << ") MIP: (" << currentLower << ", " << currentUpper << "), LP: (" << lpLower << ", " << lpUpper << ")\n";
    if (lpLower > lpUpper)
      cout << "Inconsistent LP bounds, variable " << i << ", (" << lpLower << ", " << lpUpper << ")\n";
    }
cout.flush();
}


void MILPNode::setupLP(__LPSolver_Interface* sol)
{
  MILPProblem* localMipRef = mip();
  DEBUGPRX(10,bGlobal(),"MILPNode::setupLP setting bound list\n");
  DoubleVector boundList(2*(localMipRef->numIntegerVars()));
  lpSetupObj::makeBoundList(localMipRef,this,boundList);
  lpSetupObj::setBounds(localMipRef,sol,boundList);
  DEBUGPRX(10,bGlobal(),"MILPNode::setupLP setting basis\n");
  DEBUGPRX(160,bGlobal(),basis << endl);
  sol->setBasis(basis);
  DEBUGPRX(10,bGlobal(),"MILPNode::setupLP done\n");
}


  // This used to be in the BC world only, since it was added for CGL
  // (the cut generators get all problem and solution information from
  // the OsiSolverInterface).  But move down to the MILP-only world to
  // allow starting from a saved root solution to work for (e.g.) LP-based
  // heuristics that expect to get information from the LP object.
//   Make sure the solver is in the state it was when this subproblem
// was solved.  The safest way to do this is to actually "resolve" the
// problem.  However, we have the optimal basis.  If we set that
// before the call, the LP solver should just recognize the optimality
// and reset it's state (solution, dual solution, slacks, etc, etc).

void MILPNode::restoreLPSolver()
{
  // TODO temporary
  //  double starttime = CPUSeconds();
  // No need to worry about solver method, since we're giving the
  // solver the optimal basis.  Just reset all bounds, since we presumably
  // don't know how the bounds were changed.
  setAllLPBounds(fixed, intLowerBounds, intUpperBounds, binary_vals);
  setBasis();
// We may want to put in some checks (or even use runLP()), but do this for now.
  lp->solve(true);
// double endTime = CPUSeconds();
 // ucout << "Restoring LP Solver took " << endTime - starttime << " seconds.\n";
}

} // namespace pico
