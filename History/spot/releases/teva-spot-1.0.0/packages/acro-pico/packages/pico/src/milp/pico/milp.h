/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file milp.h
 *
 * Defines the pico::MILP class.
 */

#ifndef pico_MILP_h
#define pico_MILP_h

#include <acro_config.h>
#include <utilib/BasicArray.h>
#include <pebbl/BestIncumbentRepository.h>
#include <pebbl/branching.h>
#include <pico/MILPProblem.h>
#include <pico/mipHeuristic.h>
#include <pico/lpSetupObj.h>
#include <pico/mipBase.h>


#if defined(ACRO_USING_COIN)

#define MIP_CATCH(cmd)\
   STD_CATCH(cmd)\
\
  catch (const CoinError& err) { cerr << "ERROR! Caught COIN exception object: " << err.className() << " : " << err.methodName() << " : " << err.message() << std::endl << std::flush; }

#else
#define MIP_CATCH(cmd) STD_CATCH(cmd)
#endif


namespace pico {

class MILPNode;  // Forward declaration.
class MIPHeuristic; 


/// Mixed-integer programming solver
class MILP : virtual public pebbl::branching, public serialMilpParams, public mipBase
{
public:
  // Temporary for experiments on basis run length and parent/child
  // differences.  This can go away afterward.

 std::ostream *basisOutfile;

  // May want to have a barrier method eventually
  enum LPMethodType
    {
      primal = 0,
      dual = 1
    };

  void setName(const char* cname);

  // For keeping track of timing information.  Gather some information
  // about where we're spending time for MILP
  DoubleVector MIPTimingInfo;

protected:

  friend class MILPNode;
  friend class MIPHeuristic;
  friend class parMILPNode;

  // We always branch on a fractional variable of highest priority.
  // Private to enforce positive priorities.  Array has an entry for each integer
  // variable.

private:

  IntVector branchingPriority;

protected:

  // Next 5 lines are for the pseudocost table

  DoubleVector downCostBuffer, upCostBuffer;
  IntVector downCostCounter, upCostCounter;

  // Used by the MIP heuristic during ramp up.  Stores change
  // in integrality for the first up/down branch for each branch
  // choice during ramp up.  The first (# integer vars) PAIRS of
  // entries are for regular single-variable branches: up then down.
  // The next (# sets) pairs are for special set branches: up then down.

  DoubleVector integralityChange;

  // Used to choose branching variable

  bool pCostUseDistances; // Do we care how big the up/down move is?
  double nearIntSplitFrac; // Weight on how close a variable is to int
  double objBestSplitFrac; // Weight on min penalty (value) up/down
  double objWorstSplitFrac; // As above, max
  // Next two used only to set direction preference once the variable is chosen
  double upSplitFrac; // Weighting for general preference to branch up
  double downSplitFrac; // Weighting for general preference to branch down
 // Fraction of noninitialized fractional variables to initialize through branching.
  double tableInitFrac;
 // Weighting for pseudocost updates from infeasible nodes or those beyond cutoff.
  double infeaseFactor;

  // When selecting a variable to branch on, we sequentially score each
  // candidate fractional variable.  A newly-scored variable will become the
  // candidate of choice only if the score beats the current best by this
  // relative factor (or if it is the first one of course)
  double relVarSelectTol;

  // Keep a set of known solutions in the file "pico-sol-collection.bin".  
  // If the file already exists and we are also checking fathoming 
  // (below), append to the file only if the solution is new.
  // This is a special debugging feature for pathological problems.
  bool maintainSolCollection; 

  // For debugging:
  // Read in a set of solutions from the file PICO-solution, and exit with an
  // error if a any subproblem containing that point is ever fathomed while
  // it is still a candidate (i.e. it's objective value is at least as good
  // as the incumbent.
  bool checkFathomOnSolution; 

  bool preprocessIP;

  // Temporary.  Here while we're doing testing to determine our options for basis
  // compression

  bool recordBasisInfo;
  int  earlyStopBasisInfoCount; // used to stop the computation after collecting a sufficient number of basis samples

  DoubleVector incumbent;


  pebbl::BestIncumbentRepository<DoubleVector> cache;

  // LP was last set up for this problem.  Used for warm starting.
  pebbl::branchSubId lastActivatedId;

  bool ForceColdStart;

  bool problemFeasible;


  // Heuristic-related things

 public:

  ///
  bool performing_enumeration()
        {
        return (EnumerationCount > 1) || (EnumerationAbsTolerance > 0.0) ||
                (EnumerationRelTolerance > 0.0);
        }

  ///
  unsigned int enumeration_count() { return cache.size(); }

  ///
  double fathomValue() {return cache.worst_value();}

  BBHConfig     heurCfg;
  // If false, disable all general PICO MIP heuristics
  bool          PICOMIPHeuristics;

  // If false, disable Nediak/Eckstin MIP heuristic
  bool bhhMIPHeuristic;
  MIPHeuristic* mipHeuristicPtr;

  // Parameters for simple randomized rounding

  int maxRRDepth;  // Apply randomized rounding up through this depth of the search tree

// Number of randomized rounding trials per call.  I say "per call" rather than "per node"
// because one might want to try this multiple times on a node when there is significant cutting.

  int numRRTrialsPerCall; 

  // Seed for the randomized rounding random number generator.
  int RRSeed;

  // Random number generator for the randomized rounding heuristic.  This generates numbers between
  // 0 and 1 pseudorandomly (with the goal of a uniform distribution)

  utilib::PM_LCG  RR_RNG;
  utilib::Uniform RRUniform;

  // For debugging (check for fathoming of solutions we should keep)
  BasicArray<DoubleVector> solutionsToWatch;
  double toWatchVal;

//  Determine when to complain if a watched point is about to be
//  fathomed.  Don't complain if the incumbent value
// is at least as good as the value of the watched points (within the
  // current relative tolerance).  In the latter
// case, it is OK to fathom the watched point because there are multiple
// optima, and bombing unnecessarily makes debugging harder.

  bool checkFathoming();

  // Simplex methods and basis reloading

  ///
  void printAllStatistics(std::ostream& stream = std::cout)
        {
        pebbl::branching::printAllStatistics(ucout);
        if (cache.size() > 1) {
           ucout << std::endl;
           ucout << "----------------------------------------------------------------------------" << std::endl;
           ucout << "Best Points Found During Search" << std::endl;
           ucout << "  EnumerationCount:        " << EnumerationCount << std::endl;
           ucout << "  EnumerationRelTolerance: " << EnumerationRelTolerance << std::endl;
           ucout << "  EnumerationAbsTolerance: " << EnumerationAbsTolerance << std::endl;
           ucout << "----------------------------------------------------------------------------" << std::endl;
           ucout << cache;
           ucout << std::endl;
           }
        }


protected:
  ///
  std::string rootSimplexStr;

  ///
  LPMethodType rootSimplexMethod;

  ///
  std::string warmSimplexStr;

  ///
  LPMethodType warmSimplexMethod;

  ///
  std::string nonwarmSimplexStr;

  ///
  LPMethodType nonwarmSimplexMethod;

  /// Force a basis reload for each pseudocost initialization
  /// (prevent some numerical instability)
  bool forcePCInitBasisReload;

      
public:

  MILPProblem* MIP;
  __LPSolver_Interface* lp;

  ///
  virtual void setMILPProblem(MILPProblem& _MIP);

  ///
  virtual bool setup(int& argc, char**& argv, MILPProblem& _MIP)
    {
      setMILPProblem(_MIP);
      return branching::setup(argc,argv);
    }

  // Note:  Does _NOT_ assume ownership of inputMIP
  virtual void MILPInit(bool initVB=true);

  // This is used to create an empty instance on processors that will be
  // initialized off a message.  Everything will have to be done when the
  // message arrives.
  MILP();

  virtual ~MILP();

  void MILPInitialize();
  // resetVB is flag for whether to reset virtual base classes.
  void reset(bool resetVB = true)
		{resetExec(false,resetVB);}
  void resetExec(bool keepPseudocosts=false, bool resetVB = true);
  void MILPResetGuts();

  virtual void preprocess();
  virtual pebbl::branchSub* blankSub();
  void readIn(int argc, char** argv);
  // WEH - I don't see how this is ever used or defined
  // branchSub* newSub();

// Use an approximation algorithm or heuristic to set the first incumbent
  int initialSolution();


  // We have an incumbent heuristic as long as we have not disabled all general PICO MIP
  // heuristics (with the first parameter).  We must also have one individually enabled.
  // Currently there are two choices: the Nediak/Eckstein heuristic and a simple randomized
  // rounding heuristic.

  virtual bool haveIncumbentHeuristic()
    { return (PICOMIPHeuristics && (bhhMIPHeuristic ||
				    (maxRRDepth != 0 && numRRTrialsPerCall >= 1))); } 

  virtual void updateIncumbent(DoubleVector& newSolution, double solutionValue);

  virtual void newIncumbentEffect(double new_value);

  virtual void serialPrintSolution(const char* header = "",
			     const char* footer = "",
			     std::ostream& outStream = ucout);

  virtual void serialPrintSolValue(std::ostream& stream = std::cout);
  
  /// For pseudocost table
  virtual void resizePtable(int numIntegers);  // resize all associated arrays

  ///
  virtual void updateUpCostBuffer(int index, double value);

  ///
  virtual void updateDownCostBuffer(int index, double value);
  
  // Determine which variables to initialize gradients for
  virtual int selectPseudocostInit(int nFrac, IntVector& FracVarNdx, int numFracSets,
				   IntVector &candidateSets, IntVector& initIndices,
				   bool &dontPseudoFixAll,
				   double tableInitFraction = -1.0);
  int selectVariable(DoubleVector& solution, 
		     int nFrac, 
		     IntVector& FracVarNdx, 
		     branch_type& branch_flag,
		     double &minBenefit,
		     DoubleVector *spSetBranchValues = NULL,
		     int numSetCandidates = 0,
		     IntVector *setCandidateList = NULL);

  ///
  void updateFromInfeasible(int branchingVar, branch_type direction);

  ///
  void updateFromCutoff(int branchingVar, branch_type direction,
			double branchingValue, double previousBound,
			double cutoff);

  ///
  void printPseudoCosts();

  ///
 void printIntegralityChange();

  ///
  int branchPriority(int intVarNum)
{return(branchingPriority[intVarNum]);}

  // Various ways to set user-defined priorities.  Have to control this to enforce
  // non-negative values.

  void setBranchPriority(int index, int newPriority)
    {
    if (newPriority < 0)
      EXCEPTION_MNGR(std::runtime_error, "Trying to set branching priority for variable " << index << " to negative value " << newPriority);
    branchingPriority[index] = newPriority;
    }

  void setBranchingPriority(IntVector &newPriorities);

  virtual void initializeBranchingPriorities();

// The serial number of the node whose bound was most recently computed.
// The LP solver should have state from this computation

  virtual void updateLastActivated(pebbl::branchSubId& id);
  pebbl::branchSubId& lastActivated() { return lastActivatedId; }

  // This used to set the lastActivatedId to empty, but that had bad side effects
  // in B&C.  Now manage an extra bool for this.
  void forceColdStart() {ForceColdStart = true; };
  void allowWarmStart() {ForceColdStart = false;};
  bool mustColdStart() {return(ForceColdStart);};


bool problemIsFeasible()
  {return(problemFeasible);}

// Use, e.g., if a driver code needs to do something with the final
// solution besides file output.  Be sure to use it before the milp is destroyed.
DoubleVector &getIncumbent()
  {return(incumbent);}

  bool running_with_ampl;
  CharString input_file1;
  CharString input_file2;
  bool lp_solve;
  BasicArray<int> listOfInts;

  /// Instead of bounding the root, read in all relevant bounding information
  /// from a solution saved in the file root-bounding-info.bin.
  bool useSavedRootSolution;

  bool processParameters(int& argc, 
			 char**& argv, 
			 unsigned int min_num_required_args_=0);

  void register_parameters();

  bool checkParameters(char* progName);

  bool setupProblem(int& argc, char**& argv);

  void solve();

  void writeCommandUsage(char* progName,std::ostream& os) const;

  virtual void getGlobalBound(double &returnBound, bool &boundKnown)
    {
      if (pool->knowsGlobalBound())
	{
	  returnBound = pool->globalBound();
	  boundKnown = true;
	}
      else boundKnown = false;
    }
};


} // namespace pico

#endif
