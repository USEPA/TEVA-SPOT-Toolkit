/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// milp.cpp
//
// Basic serial code for MILP solving in PICO.
//
// Cindy Phillips
//

#include <acro_config.h>
//#include "CoinShallowPackedVector.hpp"
#include <utilib/seconds.h>
#include <utilib/SparseMatrix.h>
//#include <utilib/paramTable.h>
#ifdef ACRO_HAVE_MPI
#include <utilib/mpiUtil.h>
#endif
#include <pebbl/fundamentals.h>
#include <pebbl/branchingGblParams.h>
#include <pico/milp.h>
#include <pico/milpNode.h>
#include <pico/mipHeuristic.h>

#if defined(COUGAR)
extern "C" void heap_summary();
extern "C" void print_links();
#endif

using namespace std;
using namespace utilib;


#if defined(ACRO_USING_AMPL )
#undef NO
extern "C" {
#include "getstub.h"
#include "asl.h"
};
extern ASL* asl;

namespace pico {
void asl_init();
}


static char xxxvers[] = "AMPL/PICO\0\nAMPL/PICO 1.0 Driver Version 20050101\n";
static char* usage_msg[] = { "  TODO " };
static Option_Info Oinfo = {
        "PICO", "PICO Solver Library 1.0",
        "PICO_options", 0, 0, 0, xxxvers,
        usage_msg, 0, 0, 0, 0, 20050101
        };


void get_ampl_parameters(char*** argv, utilib::ParameterList& plist)
{
pico::asl_init();
getstub(argv,&Oinfo);
printf("\n");

char* s = getenv("pico_options");
if (!s)
   s = getenv("PICO_OPTIONS");
if (!s)
   s = getenv("PICO_options");
if (s) {
   ucout << "Processing AMPL Options" << endl;
   //// Eliminate white space before and after '=' characters
   int slen = strlen(s);
   int offset=0;
   for (int i=0; i<slen; i++) {
     if (s[i] == '=') {
        int j = i-1;
        while ((j >= 0) && (s[j] == ' ')) { offset++; j--; }
        s[i-offset] = s[i];
        i++;
        while ((i<slen) && (s[i] == ' ')) { offset++; i++; }
        s[i-offset] = s[i];
        }
     else {
        s[i-offset] = s[i];
        }
     }
   s[slen-offset] = '\000';
   //// Process the options
   istringstream isstr(s);
   string str;
   while (isstr >> str) {
     const char* param = str.c_str();
     char* tmp = const_cast<char*>(param);
     unsigned int i=0;
     while ((i<str.size()) && (*tmp != '=')) {tmp++; i++;}
     if (i == str.size())
        plist.add_parameter(str,"true");
     else {
        *tmp = '\000';
        tmp++;
        ucout << "  OPTION: " << param << " " << tmp << endl;
        plist.add_parameter(param,tmp);
        }
     }
   ucout << endl;
   }
}
#endif



namespace pico {

MILP::MILP()
 : running_with_ampl(false)
{
version_info += ", PICO 1.0";
min_num_required_args=1;
MIP = NULL;
lp = NULL;
//temporary
basisOutfile = NULL;

#ifdef ACRO_VALIDATING
 MIPTimingInfo.resize(numMIPTimingStates); 
 for (int i=0; i < numMIPTimingStates; i++)
   MIPTimingInfo[i] = 0.0;
#endif

mipHeuristicPtr=NULL;

pCostUseDistances=true;
ParameterSet::create_parameter("pCostUseDistances",pCostUseDistances,
	"<bool>","true",
	"If true, use pseudo-cost distances.");

nearIntSplitFrac=0.0;
ParameterSet::create_parameter("nearIntSplitFrac",nearIntSplitFrac,
	"<double>","0.0",
	"Weight on how close a variable is to integer",
	utilib::ParameterLowerBound<double>(0.0));

objBestSplitFrac=10.0;
ParameterSet::create_parameter("objBestSplitFrac",objBestSplitFrac,
	"<double>","10.0",
	"Weight on min penalty (value) up/down",
	utilib::ParameterNonnegative<double>());

objWorstSplitFrac=0.0;
ParameterSet::create_parameter("objWorstSplitFrac",objWorstSplitFrac,
	"<double>","0.0",
	"Weight on max penalty (value) up/down",	
	utilib::ParameterLowerBound<double>(0.0));

upSplitFrac=5.0;
ParameterSet::create_parameter("upSplitFrac",upSplitFrac,
	"<double>","5.0",
"Weighting for general preference to branch up (once a variable is\n\t\
chosen)",
	utilib::ParameterLowerBound<double>(0.0));

downSplitFrac=0.0;
ParameterSet::create_parameter("downSplitFrac",downSplitFrac,
	"<double>","0.0",
"Weighting for general preference to branch down (once a variable is\n\t\
chosen)",
	utilib::ParameterLowerBound<double>(0.0));

tableInitFrac=1.0;
ParameterSet::create_parameter("tableInitFrac",tableInitFrac,
	"<double>","1.0",
"Fraction of noninitialized fractional variables to initialize through\n\t\
branching",
	utilib::ParameterBounds<double>(0.0,1.0));

infeaseFactor=10.0;
ParameterSet::create_parameter("infeaseFactor",infeaseFactor,
	"<double>","10.0",
"Weighting for pseudocost updates from infeasible nodes or those beyond\n\t\
cutoff",
	utilib::ParameterLowerBound<double>(1.0));

// This had a default of 1e-5, but turning it off to chase node counts
relVarSelectTol=0.0;
ParameterSet::create_parameter("relVarSelectTol",relVarSelectTol,
	"<double>","0.0",
"When selecting a variable to branch on, we sequentially score each\n\t\
candidate fractional variable.  A newly-scored variable will become the\n\t\
candidate of choice only if the score beats the current best by this\n\t\
relative factor (or if it is the first one of course).",
	utilib::ParameterLowerBound<double>(0.0));

 mipHeuristic=false;
 ParameterSet::create_parameter("mipHeuristic",mipHeuristic,
 "<bool>","false",
 "Use Nediak/Eckstein heuristic to find feasible MIP solutions.");
 
preprocessIP=true;
ParameterSet::create_parameter("preprocessIP",preprocessIP,
	"<bool>","true",
	"If true, then perform PICO's IP preprocessing");

maintainSolCollection=false;
ParameterSet::create_parameter("maintainSolCollection",maintainSolCollection,
	"<bool>","false",
"Keep a set of known solutions in the file \"pico-sol-collection.bin\".\n\t\
If the filealready exists and we are also checking fathoming\n\t\
(below), append to the file only if the solution is new.\n\t\
This is a special debugging feature for pathological problems.");

checkFathomOnSolution=false;
ParameterSet::create_parameter("checkFathomOnSolution",checkFathomOnSolution,
	"<bool>","false",
	"Read a set of solutions from the file pico-sol-collection.bin,\n\t"
        "and exit with an error if a any subproblem containing that point\n\t"
        "is ever fathomed while it is still a candidate (i.e. its\n\t"
        "objective value is at least as good as the incumbent.");

rootSimplexStr = "primal";
ParameterSet::create_parameter("rootSimplexMethod",rootSimplexStr,
	"<string>","primal",
        "Simplex method for the root (either primal or dual)");

warmSimplexStr = "dual";
ParameterSet::create_parameter("warmSimplexMethod",warmSimplexStr,
        "<string>","dual",
        "Simplex method for a warm start (a small change from previous solve),\n\teither primal or dual\n");

nonwarmSimplexStr = "dual";
ParameterSet::create_parameter("nonwarmSimplexMethod",nonwarmSimplexStr,
        "<string>","dual",
        "Simplex method when cold starting (from new basis/problem),\n\teither primal or dual");

forcePCInitBasisReload=false;
ParameterSet::create_parameter("forcePCInitBasisReload",forcePCInitBasisReload,
        "<bool>","false",
        "Force a basis reload for each pseudocost initialization\n\t"
        "(prevent some numerical instability)");

useSavedRootSolution=false;
ParameterSet::create_parameter("useSavedRootSolution",useSavedRootSolution,
        "<bool>","false",
        "Instead of bounding the root, read in all relevant\n\t"
	"bounding information from a solution saved in the file\n\t"
        "root-bounding-info.bin .");

recordBasisInfo = false;
ParameterSet::create_parameter("recordBasisInfo",recordBasisInfo,"<bool>","false",
		"If true, output basis information in <problemName>BasisInfo.txt.\n\tTemporary.");

earlyStopBasisInfoCount = 50000;
ParameterSet::create_parameter("earlyStopBasisInfoCount",earlyStopBasisInfoCount,"<int>","50000",
		"If recordBasisInfo is true, stop the computation after this many basis\n\toutputs (just computing to collect this). Temporary.");
}

void MILP::setMILPProblem(MILPProblem& _MIP)
{
MIP=&_MIP;
#if 0
// Temporary for experiments on basis runlength and parent-child
// relationships. Name of data file depends on the problem.
// [name]BasisInfo.txt
if (recordBasisInfo)
  {
  int fileNameLength = lp->name().size() + 13;
  char *basisFileName = new char[fileNameLength];
  strcpy(basisFileName, lp->name().data());
  strcpy(basisFileName + fileNameLength - 13, "BasisInfo.txt");
  basisOutfile = new ofstream(basisFileName , ios::out);
  }
#endif
}

MILP::~MILP()
{ 
  if ( mipHeuristicPtr != NULL ) 
    delete mipHeuristicPtr;
  // Temporary for exploring basis structure
if (basisOutfile != NULL)
  {
  basisOutfile->flush();
  delete basisOutfile;
  } 
}


//
// This is how to intialize a parallel instance that has I/O capabilities
void MILP::readIn(int argc, char** argv)
{
  // JE TEMPORARILY DISABLE THIS CODE TO GET THINGS COMPILING
// #ifdef ACRO_HAVE_MPI
//   // For parallel versions, wait for your turn to open up the LP environment
//   // Get OK from immediate predecessor, open LP, then send the token on
// #ifndef COUGAR
//   int message;
//   MPI_Status status;
//   int pid = uMPI::rank;
//   if (pid != 0)
//     {     
//     // blocking receive
//       uMPI::recv(&message, 1, MPI_INT, pid - 1, 0, &status);
//       DEBUGPR(2, ucout << "readIn: rec'd from " << message << "\n");
//     }
// #endif

//   #endif
//   // Keep this around to avoid excessive opening and closing of the LP
//   // environment (e.g. in repeat loops, or running from a file)
//   // Testing, trying to remove seg faults
// #ifdef ACRO_HAVE_MPI
// #ifndef COUGAR
//   if (pid < uMPI::size -1)
//     uMPI::send(&pid, 1, MPI_INT, pid + 1, 0);
// #endif
// #endif
//   char* fileName1;
//   char* fileName2;
//   inputFile(argc,argv,fileName1,fileName2);
//   if (!fileName1)
//      EXCEPTION_MNGR(runtime_error, "An input file must be specified");

//   if (fileName2)
//      MIP->loadProblem(fileName1,fileName2);
//   else
//      MIP->loadProblem(fileName1);

//   // CAP: TODO: verify which is correct

//   //  branchingInit((optimType)(lp->getObjSense()), relTolerance,absTolerance);

//   branchingInit( (lp->getObjSense() == -1.0 ? maximization : minimization), 
// 			relTolerance,absTolerance);

//   // Assuming there is a pool there from defaults
//   //  MILPInitialize(); 
//   MILPInit(true);
}

void MILP::MILPInit(bool initVB)
{
if (initVB)
  branchingInit( (lp->getObjSense() == -1.0 ? maximization : minimization),
		relTolerance,absTolerance);
MILPInitialize();
}

void MILP::resetExec(bool keepPseudocosts, bool resetVB)
{
if (MIP)
   lp=MIP->lp();
if (resetVB)
  branching::reset();
if (!keepPseudocosts)
  {
  int numIntegers = MIP->numIntegerVars();
  for (int i=0; i < numIntegers; i++)
    {
    downCostBuffer[i] = 0;
    downCostCounter[i] = 0;
    upCostCounter[i] = 0;
    upCostBuffer[i] = 0;
    integralityChange[i] = 0;
    }
  }
MILPResetGuts();
MIP->reset();
}

void MILP::MILPResetGuts()
{
cache.sense = sense;
if (EnumerationRelTolerance >= 0.0)
   cache.EnumRelTolerance = EnumerationRelTolerance;
if (EnumerationAbsTolerance >= 0.0)
   cache.EnumAbsTolerance = EnumerationAbsTolerance;
if (EnumerationCount >= 1)
   cache.EnumLimit = EnumerationCount;
else
   cache.EnumLimit = 1;

if (rootSimplexStr == "primal")
   rootSimplexMethod = primal;
else
   rootSimplexMethod = dual;

if (warmSimplexStr == "primal")
   warmSimplexMethod = primal;
else
   warmSimplexMethod = dual;

if (nonwarmSimplexStr == "primal")
   nonwarmSimplexMethod = primal;
else
   nonwarmSimplexMethod = dual;

// CAP: now done when LP object is allocated in its init(type) method
//lp->initialize();
lp->setDualCutoff(incumbentValue);
lp->setPrimalCutoff(-sense*MAXDOUBLE);
lastActivatedId.setEmpty();
allowWarmStart();
problemFeasible = false;
}

// The guts of initialization.  Must have MIP initialized first.
void MILP::MILPInitialize()
{

MILPResetGuts();

resizePtable(MIP->numIntegerVars() + MIP->numSets());

incumbent.resize(MIP->numAllVars());
#ifdef ACRO_VALIDATING
if (checkFathomOnSolution)
  {
    int numSolutions;
    ifstream savedSolution("pico-sol-collection.bin");
    if (savedSolution.bad())
      EXCEPTION_MNGR(runtime_error, "Can't open the saved solution");
    savedSolution >> numSolutions;
    solutionsToWatch.resize(numSolutions);
    for (int stepper = 0; stepper < numSolutions; stepper++)
      {
      savedSolution >> solutionsToWatch[stepper];
      if (stepper == 0)
	toWatchVal = lp->evalVector(solutionsToWatch[stepper]);
      else if (lp->evalVector(solutionsToWatch[stepper]) != toWatchVal)
	EXCEPTION_MNGR(runtime_error, "Solutions to watch have different objective values");
      }
  }
 else toWatchVal = 0.0; // Not used
#endif
// ucout << "Initializing MILP, absTolerance = " << absTolerance << "\n";
 if (absTolerance < 1)
   {
   bool all_int = true;
   double objCoeff;
   variableType thisVType;
   for (int i = 0; i < MIP->numAllVars(); i++)
     {
     objCoeff = lp->getObjCoefficient(i);
     if (objCoeff != 0)
       {
       thisVType = MIP->vtype[i];
	 // check to see if variable with non-zero objective coef is integer
       if (thisVType == continuous)
	 {
	 all_int = false;
	 break;
	 }
       if (thisVType == initiallyFixed)
	 {
	 if (!(pebblBase::isInteger(objCoeff * lp->getColLower(i))))
	   {
	   all_int = false;
	   break;
	   }
	 else continue;
	 }
       // Variable is integer.  If coefficient is *not* integer, then we
       // cannot guarantee an integer objective.
       if (!pebblBase::isInteger(objCoeff))
	 {
	 all_int = false;
	 break;
	 }
       }
     }

   // 1 would probably be correct if we had exact arithmetic.  I suspect the small
   // backoff is to protect against bounds/incumbents that are just a little off.
   // Perhaps a hard value of 1 would lead to declaring a solution optimal when it
   // is really 1 off.  This used to subtract relTolerance, but we were seeing the
   // off-by-one problem, so try integerTolerance (slightly larger) instead.

   if (all_int && (absTolerance < 1 - relTolerance)) 
     absTolerance = 1 - integerTolerance;
   }
 
 DEBUGPR(2,ucout << "Initialized MILP, absTolerance = " 
	 << absTolerance << endl);

}

//  /*  Will go away I think
//  // For initialization off a message.  Assumes MIP is a valid pointer to
//  // a MILPProblem

//  void MILP::Initialize()
//  {

//    // Assumed that parameters have been read in
//    optimType direction = (optimType)(lp->sense());
//    // Taken from the branching constructor
//    branching::standardInit(direction * MAXDOUBLE, direction,1e-7,1);
//    MILPInitialize();
//  }
//  */


// This is the place to put virtual initialization methods that
// must be overridden by derived classes (they can't be overridden
// in methods called during construction.

void MILP::preprocess() 
{
  //
  // Preprocess the LP matrix
  //

  // TODO: verify that this is still a problem.  If not, remove.
  // Because of a diamond inheritance pattern, parBCMip will call
  // this twice.  Just return the second time.
  static bool already_preprocessed_mip = false;
  if (preprocessIP && !already_preprocessed_mip) {
#ifdef ACRO_VALIDATING
    double PPStart = CPUSeconds();
#endif
    MIP->preprocess();
    already_preprocessed_mip = true;
#ifdef ACRO_VALIDATING
    MIPTimingInfo[preprocessTime] += CPUSeconds() - PPStart;
#endif
  }

  initializeBranchingPriorities();

  if (mipHeuristic)
    mipHeuristicPtr = new MIPHeuristic(this, heurCfg.bbhCanonicalOnly, debug);

}


void MILP::resizePtable(int numBranchChoices)
{
branchingPriority.resize(numBranchChoices);
downCostBuffer.resize(numBranchChoices);
upCostBuffer.resize(numBranchChoices);
downCostCounter.resize(numBranchChoices);
upCostCounter.resize(numBranchChoices);
integralityChange.resize(2 * numBranchChoices);
// Need to explicitly initialize
for (int i=0; i < numBranchChoices; i++)
  {
  downCostBuffer[i] = 0;
  downCostCounter[i] = 0;
  upCostCounter[i] = 0;
  upCostBuffer[i] = 0;
  integralityChange[i] = 0;
  }
}


// When a subproblem is infeasible, that's evidence it's a good selection
// for branching.  Right now, pretend this branch had a change
// infeaseFactor greater than the current average change.  If the pseudocost
// isn't initialized in this direction, set it to "infinity".  This
// update will count as infeaseFactor times the first finite pseudocost that
// comes in (when it arrives).

void MILP::updateUpCostBuffer(int ndx, double val)
{
if (upCostCounter[ndx] == 0)
  {
  upCostBuffer[ndx] = val;
  upCostCounter[ndx] = 1;
  return;
  }
if (val == MAXDOUBLE) // "infinite"
  {
    // if what's there is already infinite.  It will stay that way, and we'll
    // just increment the count (later)
  if (upCostBuffer[ndx] != MAXDOUBLE)
    upCostBuffer[ndx] += infeaseFactor * upCostBuffer[ndx]/upCostCounter[ndx];
  }
else if (upCostBuffer[ndx] == MAXDOUBLE)
  {
  upCostBuffer[ndx] = val * (infeaseFactor * upCostCounter[ndx] + 1);
  }
// both finite
else upCostBuffer[ndx] += val;
upCostCounter[ndx]++;
}

// See documentation above for updateUpCostBuffer

void MILP::updateDownCostBuffer(int ndx, double val)
{
if (downCostCounter[ndx] == 0)
  {
  downCostBuffer[ndx] = val;
  downCostCounter[ndx] = 1;
  return;
  }
if (val == MAXDOUBLE) // "infinite"
  {
    // if what's there is already infinite.  It will stay that way, and we'll
    // just increment the count (later)
  if (downCostBuffer[ndx] != MAXDOUBLE)
    downCostBuffer[ndx] += infeaseFactor * downCostBuffer[ndx]/downCostCounter[ndx];
  }
else if (downCostBuffer[ndx] == MAXDOUBLE)
  {
  downCostBuffer[ndx] = val * (infeaseFactor * downCostCounter[ndx] + 1);
  }
// both finite
else downCostBuffer[ndx] += val;
downCostCounter[ndx]++;
}


// Return the integer variable number of the variable to branch on,
// or, if we're using sets and choose to branch on a special set,
// return setID + #integer variables (this is the location of the set
// in the list of possible branching choices).  Set branch type using
// call-by-reference.  Set minBoundMvmt (expected minimum bound
// movement) also by reference.  Only consider variables with highest
// user-supplied priority (among those that are fractional candidates
// for branching).  Then subselect based on pseudocost (gradient)
// information, closeness to integrality, etc.

//TODO: should we just access the problem arrays via the current problem
// pointer?  This structure is from before it was available.  Would
// we every want to select a variable for a problem other than the current one?
// maybe take a pointer to a problem with current as default.  Note that we'd have
// to deal with the same kind of casting issues we have with the various globalPtr
// versions (avoid backcasting) and MILP would have to become a friend of MILPNode.
int MILP::selectVariable(DoubleVector& solution, 
	int nFrac, IntVector& FracVarNdx, branch_type& branch_flag,
	double &minBoundMvmt, DoubleVector *spSetBranchValues, int nSetCandidates,
			 IntVector *setCandidateNdx)

{
  //  ucout << "In selectVariable.  nfrac = " << nFrac << "\n";
  // If nFrac = 0, then all fractional variables were uninitialized and each
  // were half-fathomed.  The first entry in FracVarNdx is one such variable.
  // Use that.
if (nFrac == 0 && nSetCandidates == 0)
  {
// doesn't really matter what we choose, but must set it
  branch_flag = branch_up;
  return(FracVarNdx[0]);
  }
double bestScore = 0.0;
double scoreGap, currentScore;
int branchVariable = -99;
int i,j;
double downDist, upDist, downPenalty, upPenalty, bestPenalty, worstPenalty;
bool variableSelected = false;

 int highestPriority = 0;
 int numInts = MIP->numIntegerVars();
 for (i = 0; i < nFrac; i++)
   if (branchingPriority[FracVarNdx[i]] > highestPriority)
     highestPriority = branchingPriority[FracVarNdx[i]];
 for (i=0; i < nSetCandidates; i++)
   if (branchingPriority[numInts + (*setCandidateNdx)[i]] > highestPriority)
     highestPriority = branchingPriority[numInts + (*setCandidateNdx)[i]];

 // ucout << "Highest priority is " << highestPriority << "\n" << Flush;

 int numChecks = nFrac + nSetCandidates;
double val;
for (j=0; j < numChecks; j++) {
  if (j < nFrac)
    {
    i = FracVarNdx[j];
    val = solution[i];
    }
  else
    {
    i = numInts + (*setCandidateNdx)[j - nFrac];
    val = (*spSetBranchValues)[j - nFrac];
    }
  if (branchingPriority[i] != highestPriority)
    {
      DEBUGPR(50, ucout << "Frac var " << i << " not considered b/c lower priority\n");
    continue;
    }
  // Ignore zero entries
  downDist = 1.0;
  upDist = 1.0;
  if (pCostUseDistances) {
     downDist = val - floor(val);
     upDist   = 1.0 - downDist;
     }
  if (downCostCounter[i] == 0) downPenalty = 0.0;
  else if (downCostBuffer[i] == MAXDOUBLE)  downPenalty = MAXDOUBLE;
  else downPenalty  = downCostBuffer[i]*downDist / downCostCounter[i];
  if (upCostCounter[i] == 0)  upPenalty = 0.0;
  else if (upCostBuffer[i] == MAXDOUBLE)  upPenalty = MAXDOUBLE;
  else upPenalty    =   upCostBuffer[i]*  upDist / upCostCounter[i];

  bestPenalty = max(upPenalty, downPenalty);
  worstPenalty = min(upPenalty, downPenalty);

  // Not clear how or if to add up and down preferences to variable choice
  currentScore =   nearIntSplitFrac * min(upDist,downDist)
                   + objBestSplitFrac * bestPenalty 
                   + objWorstSplitFrac * worstPenalty
                   + downSplitFrac * upDist;
  DEBUGPR(50, ucout << "Frac var " << i << " upcostbuf= " << upCostBuffer[i] << " upcount=" << upCostCounter[i] << " downcostbuf= " << downCostBuffer[i] << " downcount= " << downCostCounter[i] << " updist=" << upDist << " downdist=" << downDist << " score = " << currentScore << "\n");


  scoreGap = currentScore - bestScore;
  if (bestScore != 0) scoreGap = scoreGap/bestScore;

  // If currentScore is worse than bestScore, scoreGap will be negative
  // so the first condition will fail

  if ((scoreGap > relVarSelectTol) || !variableSelected) {
    variableSelected = true;
     bestScore = currentScore;
     branchVariable = i; //Note:  this used to return index into FracVarNdx (j)
     if (downSplitFrac > 0) downDist = downDist/downSplitFrac;
     if (upSplitFrac > 0) upDist = upDist/upSplitFrac;
     if (downDist < upDist) branch_flag = branch_down;
     else branch_flag = branch_up;
     minBoundMvmt = worstPenalty;
     }
  }

return branchVariable;
}

// Select a subset of variables (or special sets) that need gradient
// initialization.  Only initialize variables with the highest
// priority.  The parameter tableInitFraction tells the fraction of
// uninitialized highest-priority fractional variables to initialize
// (default all). If this is < 1.0, return the appropriate fraction
// from the front of the list.  Eventually we could make this a random
// selection.  Return the number of variables/sets to be initialized.
// Return variable numbers or set numbers of those sets/variables to be
// initialized in initIndices.  The bool dontPseudoFixAll should be true
// if all highest-priority branching choices (sets or vars) are uninitialized.
// Then in later initialization of these gradients, we cannot pretend they are
// all fixed if they are all half-infeasible (a rare, but possible, case).

int MILP::selectPseudocostInit(int nFrac, 
			       IntVector &FracVarNdx, 
			       int numFracSets, 
			       IntVector &candidateSets, 
			       IntVector& initIndices, 
			       bool &dontPseudoFixAll, 
			       double tableInitFraction)
{
DEBUGPRP(100,ucout << "MILP::selectPseudocostInit: nfrac=" << nFrac << endl
	 << "FracVarNdx=")
DEBUGPRP(100, for(int fndx=0; fndx<nFrac; fndx++) 
	 ucout << ' ' << FracVarNdx[fndx]);
DEBUGPR(100,ucout << endl << "numFracSets=" << numFracSets 
	<< " candidateSets " << candidateSets << endl);
dontPseudoFixAll = true;
if (tableInitFraction == -1.0) tableInitFraction=tableInitFrac;
if (tableInitFraction == 0.0)  return(0);
int numUnInit = 0;
int i,j;
int highestPriority = 0;
int numInts = MIP->numIntegerVars();
for (i = 0; i < nFrac; i++)
  if (branchingPriority[FracVarNdx[i]] > highestPriority)
    highestPriority = branchingPriority[FracVarNdx[i]];
for (i=0; i < numFracSets; i++)
  {
  if (branchingPriority[numInts + candidateSets[i]] > highestPriority)
    highestPriority = branchingPriority[numInts + candidateSets[i]];
  }
 int numChecks = nFrac + numFracSets;
for (j=0; j<numChecks; j++) {
  if (j < nFrac)
    i = FracVarNdx[j];
  else i = numInts + candidateSets[j-nFrac];
  if (branchingPriority[i] != highestPriority)
    continue;
 // Could have only one initialized if previous testing had one branch fathomed
  if (downCostCounter[i] == 0 || upCostCounter[i] == 0) 
    initIndices[numUnInit++] = i;
  else  // There's an already-initialized highest-priority option
    dontPseudoFixAll = false;
  }
    
if (tableInitFraction == 1.0)
  return(numUnInit);

// Only a fraction of the uninitialized variables will be initialized
// (to control computational cost).  For now just return first, but
// later include randomization maybe.
return((int)ceil(tableInitFraction * numUnInit));
}


// Print solution.  Do we want the basis too?

void MILP::serialPrintSolution(const char* header,
			       const char* footer,
			       ostream& outStream)
{
  int i;

  if (problemFeasible)
    {
      branching::serialPrintSolution(header,"",outStream);  // prints obj value
    }
  else 
    outStream << "Problem integer infeasible\n";

  // Nodes created printout deleted by JE; is inaccurate for parallel runs.
  // print nonzero variables from the solution (no slacks)

  for (i = 0; i < lp->getNumCols(); i++)
    {
      if (incumbent[i] != 0)
	outStream << "variable " << i << "( = " 
		  << lp->getVariableName(i) 
		  << "): " << incumbent[i] << "\n"; 
    }

  if (maintainSolCollection)
    {
      if (checkFathomOnSolution)
	{
	  // Check to see if this is one we're already watching.  If so, do
	  // nothing.  Otherwise, add this on at the end.  Start with the
	  // number of solutions.
	  bool new_solution = true;
	  size_type stepper;
	  size_type num_solutions = solutionsToWatch.size();
	  for (stepper = 0; stepper < num_solutions; stepper++)
	    {
	      if (solutionsToWatch[stepper] == incumbent)
		{
		  new_solution = false;
		  break;
		}
	    }
	  if (new_solution)
	    {
	      ofstream solFile("pico-sol-collection.bin");
	      if (solFile.bad())
		EXCEPTION_MNGR(runtime_error, 
			       "Error opening file for solution output");
	      // We may want to make this a parameter, 
              // but for now, probably not worth it
	      // This is set to be about integerTolerance 
              // (what we use to decide if a
	      // variable is an integer value)
	      solFile.precision(6);

	      solFile << solutionsToWatch.size() + 1;
	      solFile << "\n";
	      for (stepper = 0; stepper < num_solutions; stepper++)
		solFile << solutionsToWatch[stepper];
	      solFile << incumbent;
	    }
	}
      else
	{
	  ofstream solFile("pico-sol-collection.bin");
	  if (solFile.bad())
	    EXCEPTION_MNGR(runtime_error, 
			   "Error opening file for solution output");
	  // We may want to make this a parameter, but for now, 
          // probably not worth it.  This is set to be about 
          // integerTolerance (what we use to decide if a
	  // variable is an integer value)
	  solFile.precision(6);
	  solFile << 1;
	  // add whitespace so the size of the following vector
	  // doesn't run together with the number of vectors
	  solFile << "\n";
	  solFile << incumbent;
	}
    }
  outStream << footer;
}


void MILP::updateIncumbent(DoubleVector& newSolution, double solutionValue)
{
  DEBUGPR(100,ucout << "In MILP::updateIncumbent" << endl << Flush);
  cache.insert(solutionValue,newSolution);
  if (sense *(incumbentValue - solutionValue) > 0)
    {
      incumbentValue = solutionValue;
      incumbent << newSolution;
    }

  newIncumbentEffect(solutionValue);
  signalIncumbent();
  DEBUGPR(5, serialPrintSolution("New Incumbent ", "", ucout));
}

void MILP::updateLastActivated(pebbl::branchSubId& id) {lastActivatedId = id;}


// Treat gradient as infinite on infeasible branch.  The routines to update
// the buffers will implement the actual policy.  If this changes, it will
// effect the way infeasibility is detected in parallel root initialization.
// Perhaps try to encapsulate that better later.

void MILP::updateFromInfeasible(int branchingVar, branch_type direction)
{
switch (direction) {
  case branch_up:
    updateUpCostBuffer(branchingVar, MAXDOUBLE);
    break;

  case branch_down:
    updateDownCostBuffer(branchingVar, MAXDOUBLE);
    break;

  default:	// no_branch
    break;
  };
}

// If the cutoff was predicted by the pseudocosts, don't do anything.
// Otherwise, act as though the cutoff bound was the final bound, and
// weight by infeaseFactor

void MILP::updateFromCutoff(int branchingVar, branch_type direction,
		double branchingValue, double previousBound, double cutoff)
{
double deltaObj, pseudoGrad;
if (lround(lp->getObjSense()) == pebbl::maximize)
  deltaObj = previousBound - cutoff;
else deltaObj = cutoff - previousBound;
if (direction == branch_up)
  {
    pseudoGrad = deltaObj/(ceil(branchingValue) - branchingValue);

    // This requires the compiler to stop evaluation of the OR condition
    // as soon as it detects a true (otherwise, we'll have a divide by 0).
    if (upCostCounter[branchingVar] == 0 ||
	pseudoGrad >= upCostBuffer[branchingVar]/upCostCounter[branchingVar])
      updateUpCostBuffer(branchingVar, infeaseFactor * pseudoGrad);
  }
else
  {
    pseudoGrad = deltaObj/(branchingValue - floor(branchingValue));
    // As above with compiler
    if (downCostCounter[branchingVar] == 0 ||
	pseudoGrad >= downCostBuffer[branchingVar]/downCostCounter[branchingVar])
      updateDownCostBuffer(branchingVar, infeaseFactor * pseudoGrad);
  }
}

void MILP::printPseudoCosts()
{
  DEBUGPR(0,ucout << "Pseudocosts "
	  "(UPtotal, UPcount, DOWNtotal, DOWNcount), nonzero only:\n");
  int numBranchChoices = MIP->numIntegerVars() + MIP->numSets();
  //int j = 0;
  for (int i = 0; i < numBranchChoices; i++)
    {
      /*
      if (j++ == 5)
	{
	  DEBUGPR(0,ucout << "\n");
	  j = 0;
	  } */
      if (downCostCounter[i] == 0 && upCostCounter[i] == 0) 
	continue;
      // Newline after every pseudocost for now (makes automatic comparisons
      // easier)
      DEBUGPRP(0,ucout << i << ": (" 
	       << upCostBuffer[i] << ", " << upCostCounter[i] << ", " 
	       << downCostBuffer[i] << ", " << downCostCounter[i] << ")\n");
      ucout.flush();
    }
  cout.flush();
}

void MILP::printIntegralityChange()
{
  DEBUGPR(0,ucout << "IntegralityChange (up,down), nonzero only:\n");
  int numBranchChoices = MIP->numIntegerVars() + MIP->numSets();
  //int j = 0;
  for (int i = 0; i < numBranchChoices; i++)
    {
      /*
      if (j++ == 5)
	{
	  DEBUGPR(0,ucout << "\n");
	  j = 0;
	  } */
      if (integralityChange[2*i] == 0 && integralityChange[2*i+1] == 0) 
	continue;
      // Newline after every one for now (makes automatic comparisons
      // easier)
      DEBUGPRP(0,ucout << i << ": (" 
	       << integralityChange[2*i] << ", " << integralityChange[2*i+1] << ")\n");
      ucout.flush();
    }
  cout.flush();
}

// Determines if we should check for improper fathoming.  This is true
// if the user has asked for fathoming checks and if the watched point
// has a objective value better than the fathomValue, which is the
// worst value in the solution cache (i.e. it would indeed be an
// incorrect fathoming).

bool MILP::checkFathoming()
{
  if (!checkFathomOnSolution) return (false);
  double checker = fathomValue() - toWatchVal;
  if (toWatchVal != 0) checker = checker/toWatchVal;
  return (checker * sense > relTolerance);
}


void MILP::setBranchingPriority(IntVector &newPriorities)
{
size_type length = newPriorities.size();
if (length != branchingPriority.size())
  EXCEPTION_MNGR(runtime_error, "Branching Priority vector size mismatch");

for (size_type i=0; i < length; i++)
  if (newPriorities[i] < 0)
    EXCEPTION_MNGR(runtime_error, "Trying to set branching priority for variable " << i << " to negative value " << newPriorities[i]);
 branchingPriority << newPriorities;
}


void MILP::initializeBranchingPriorities()
{
  if (useSets && giveSetsPriority)
    {
    int i;
    int numInts = MIP->numIntegerVars();
    int numBranchChoices = numInts + MIP->numSets();
    for (i=0; i < numInts; i++)
      branchingPriority[i] = 0;
    for (i=numInts; i < numBranchChoices; i++)
      branchingPriority[i] = 1;
    }
  else  branchingPriority << 0;
}


void MILP::setName(const char* cname)
{
  // First do the usual setName fromthe file name

  branching::setName(cname);

  // Next take of .mps or .MPS

  size_type n = problemName.length();
  if (n < 4) return; // can't end with .mps and still have a real name
  string endOfName(problemName,n-4,4);
  if ((endOfName == ".mps") || (endOfName == ".MPS"))
    problemName.erase(n-4,4);
}


typedef char* char_ptr;


bool MILP::processParameters(int& argc, 
			     char**& argv, 
			     unsigned int min_num_required_args_)
{
  //
  // Set the 'help' flag if no arguments were passed to the solver.
  //
  if (argc == 1) 
     help_parameter=true;

#ifdef ACRO_USING_AMPL 
  //
  // AMPL Passes in a -AMPL flag.  Verify that we get this, and
  // set a flag to ensure that ampl parameters are read later.
  // PICO recognized *.nl files and sets up the AMPL ASL file.
  // This is read in in the pico/src/tools/lp/read_ampl.cpp routines.
  //
  // TODO
  //   1. How handle exceptions w.r.t. AMPL?
  //   2. How read in AMPL parameters?
  //   3. How report results back to AMPL?
  //

  if ((argc == 3) && (strcmp(argv[2],"-AMPL") == 0)) {
    running_with_ampl=true;
  }
#endif

  // --version is now handled by branching::processParameters

  if (!branching::processParameters(argc,argv,min_num_required_args_))
    return false;

  input_file1 = argv[1];
  if (!((argc == 2) || running_with_ampl))
    input_file2 = argv[2];
  else
    input_file2 = "";

#if defined(ACRO_USING_AMPL )
  if (running_with_ampl)
    get_ampl_parameters(&argv,plist);
#endif

  return true;
}


void MILP::register_parameters()
{
branching::register_parameters();
plist.register_parameters<PicoLPSolver>();
plist.register_parameters<BBHConfig>();
plist.register_parameters<MILPProblem>();
plist.register_parameters<MILP>();
}


bool MILP::checkParameters(char* progName)
{
  if (saveRootBoundingInfo && !onlyRootLP)
    {
      ucout << "The saveRootBoundingInfo option can "
	    << "only be used in conjunction with onlyRootLP.\n";
      return false;
    }
  bool tmp = debug_solver_params;
  debug_solver_params=false;
  bool ans = branching::checkParameters(progName);
  debug_solver_params=tmp;
}


bool MILP::setupProblem(int& argc, char**& argv)
{
  if (!MIP) {
    EXCEPTION_MNGR(runtime_error, 
		   "MILPProblem must be set before calling setup");
  }
  MIP->set_parameters(plist,false);
  MIP->lp.set_parameters(plist,false);
  if (debug_solver_params) {
    ucout << "---- PicoLPSolver Parameters ----" << endl;
    MIP->lp.write_parameter_values(ucout);
    ucout << endl << Flush;
  }
  MIP->lp.init();
  lp=MIP->lp();
  MIP->lp()->set_parameters(plist,false);
  if (debug_solver_params) {
    ucout << "---- PicoLPInterface Parameters ----" << endl;
    MIP->lp()->write_parameter_values(ucout);
    ucout << endl << Flush;
  }
  //
  // Load Problem
  //
  if (input_file2 == "")
    MIP->lp()->loadProblem(input_file1, listOfInts);
  else
    MIP->lp()->loadProblem(input_file1, input_file2, listOfInts);
  if (debug >= 300) {
    MIP->lp()->write(ucout,verbose_format);
    ucout << Flush;
  }
  lp_solve = ((listOfInts.size() == 0) || (onlyRootLP == 1));
  if (!lp_solve) {
    //
    // Setup the MILP
    //
    MIP->initialize(listOfInts, debug);
    set_parameters(plist,false);
    heurCfg.set_parameters(plist,false);
    if (debug_solver_params) {
      ucout << "---- MILPProblem Parameters ----" << endl;
      MIP->write_parameter_values(ucout);
      ucout << endl << Flush;
      ucout << "---- MILP Parameters ----" << endl;
      write_parameter_values(ucout);
      ucout << endl << Flush;
    }
    MILPInit();
    setDebug(debug);
  }
  return true;
}



void MILP::solve()
{
try {

if (preprocessOnly) {
   MIP->preprocess();
   return;
   }
//
//
// Perform LP Solve
//
//
if (lp_solve) {
   ucout << "No integer variables... solving the LP normally." << endl;

   if (preprocessIP) {
      MIP->preprocess();
      }
   double run_start = CPUSeconds();
   MIP->lp()->solve();
   PicoLPInterface::problemState solvedState = MIP->lp()->state();
   double run_time = CPUSeconds() - run_start;

   //
   // Save information to the file 'root-bounding-info.bin'
   //
   if (saveRootBoundingInfo) {
      ofstream savedSolution("root-bounding-info.bin");
      if (savedSolution.bad())
         EXCEPTION_MNGR(runtime_error, "Can't open file to save root solution");

      // May want to adjust precision here!
      savedSolution << (int)solvedState;
      // Don't let numbers run together.  This isn't a packbuffer.
      savedSolution << " ";

      if (solvedState == PicoLPInterface::solved) {
         savedSolution << MIP->lp()->getObjValue() << " ";
         //
         // Any changes to MILPNode::getSolution() should propagate here
         //
         DoubleVector solution(listOfInts.size());
         MIP->lp()->getColSolution(solution.data(),
                                   (int)solution.size(), listOfInts.data());
         savedSolution << solution << " ";
         //
         // Any changes to the way basis information is
         // stored/retrieved should propagate here.
         //
         BasisArray basis(MIP->lp()->getNumCols() + MIP->lp()->getNumRows());
         MIP->lp()->getBasis(basis);
         savedSolution << basis;
         }
      }
   //
   // Misc I/O about final solution
   //
   if (solvedState == PicoLPInterface::unbounded) {
      ucout << endl << "Problem is unbounded\n";
      }
   else if (solvedState == PicoLPInterface::infeasible) {
      ucout << endl << "Problem is infeasible\n";
      }
   else if (solvedState == PicoLPInterface::solved) {
      ucout.precision(12);
      ucout << "\tLP value= " << MIP->lp()->getObjValue() << endl;
      ucout << "\tTotal Runtime= " << run_time << endl;
      ucout << "\tTotal Time= " << (CPUSeconds() - startTime) << endl;
      #if defined(ACRO_VALIDATING)
      if (debug > 0) {
         DoubleVector fullSolution(MIP->lp()->getNumCols());
         MIP->lp()->getColSolution(fullSolution.data());
         for (int j = 0; j < MIP->lp()->getNumCols(); j++) {
           if (fullSolution[j] != 0)
              ucout << "variable " << j << "(= " <<
                                        MIP->lp()->getVariableName(j) <<
                                        "): " << fullSolution[j] << "\n";
           }
         }
      #endif
      }
   //
   // WRITE AMPL OUTPUT FROM LP SOLUTION
   //
   #ifdef ACRO_USING_AMPL 
   if (running_with_ampl) {
      if (asl) {
         double ObjVal;
         ObjVal = MIP->lp()->getObjValue();
         char msg[80];
	 need_nl=0;
         sprintf(msg,"PICO Solver: final f = %f\n",ObjVal);
         write_sol(msg, 
		const_cast<double*>((MIP->lp()->getColSolution())),
                const_cast<double*>((MIP->lp()->getRowPrice())), NULL);
         }
      }
   #endif
   }
//
//
// Perform MIP Solve
//
//
else {
   #ifdef ACRO_VALIDATING
   // This seems like only a debugging feature.
   // Not very efficient either.
   if (rootLP) {
      if (preprocessIP)
         MIP->preprocess();
         MIP->lp()->solve();
         ucout << "Root LP value= " << MIP->lp()->getObjValue() << endl;
         MIP->lp()->reset();
      }
   #endif
   //
   // Run the MILP solver
   //
   search();
   //
   // Final I/O
   //
   if (!performing_enumeration()) {
     serialPrintSolValue(ucout);
     if (printFullSolution)
       serialPrintSolution("","\n",ucout);
   }
   solutionToFile();
   printAllStatistics();
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
         write_sol(msg, getIncumbent().data(), NULL, NULL);
         }
      }
   #endif
   }
}

#if defined(COUGAR)
  MIP_CATCH( heap_summary(); )
#else
  MIP_CATCH( ; )
#endif

}


void MILP::serialPrintSolValue(std::ostream& stream)
{
  if (problemIsFeasible()) {
     int oldPrecision = stream.precision(20);
     stream << endl << "Final Solution:  Value = " << incumbentValue;
     stream.precision(oldPrecision);
     }
  else
     stream << endl << "Integer Infeasible";
  stream << endl << endl;
}


void MILP::writeCommandUsage(char* progName,ostream& os) const
{
  os << "Usage: " << progName 
     << " {--parameter=value ...} <MPS-file>\n";
  os << "Usage: " << progName 
     << " {--parameter=value ...} <LP-file>\n";
  os << "Usage: " << progName 
     << " {--parameter=value ...} <AMPL-model-file> <AMPL-data-file> \n";
  os << "Usage: " << progName 
     << " <AMPL-nl-file> -AMPL\n";
  os.flush();
}


void MILP::newIncumbentEffect(double new_value)
{
  DEBUGPR(100,ucout << "MILP::newIncumbentEffect called.\n");
  problemFeasible = true;
  // If we're enumerating with no enumeration tolerances, don't set a cutoff till we have
  // found enough solutions.  Then set the cut off based on the worst solution found so far
    if (EnumerationCount > 0 && EnumerationRelTolerance < 0.0 && EnumerationAbsTolerance < 0.0)
      {
        if (enumeration_count() < (unsigned int) EnumerationCount)
	  return;
	else new_value = fathomValue();
      }

  if (EnumerationRelTolerance > 0.0 || EnumerationAbsTolerance > 0.0)
    // Make the LP solution cutoff sensitive to enumeration tolerances.  This is kind of like
    // pretending the incumbent isn't quite as good as it really is for the sake of setting the
    // cutoffs.  If both tolerances are nontrivially set, use the tighter of the two for this
    // instance.
    {
      if (lp->getObjSense() == -1.0) // maximization
	{
	 if (EnumerationRelTolerance > 0.0)
	    {
	      if (EnumerationAbsTolerance > 0.0)
		{
		new_value = max((1 - EnumerationRelTolerance) * new_value,
				new_value - EnumerationAbsTolerance);
		}
	      else  new_value = (1 - EnumerationRelTolerance) * new_value;
	    }
	  else
	    new_value = new_value - EnumerationAbsTolerance;
	} 
      else // minimization
	{
	  if (EnumerationRelTolerance > 0.0)
	    {
	      if (EnumerationAbsTolerance > 0.0)
		{
		new_value = min((1 + EnumerationRelTolerance) * new_value,
				EnumerationAbsTolerance + new_value);
		}
	      else  new_value = (1 + EnumerationRelTolerance) * new_value;
	    }
	  else
	    new_value = EnumerationAbsTolerance + new_value;
	}
    }
  lp->setDualCutoff(new_value);
}

} // namespace pico
