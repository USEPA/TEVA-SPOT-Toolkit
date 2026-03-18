/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
// 
// MILPProblem.cpp
//

#include <acro_config.h>
#include <utilib/_math.h>
#include <utilib/exception_mngr.h>
#include <utilib/mpiUtil.h>
#include <pico/MILPProblem.h>
#include <pico/BasisArray.h>
#include <pico/MILPSymbInfo.h>
#include <CoinPackedMatrix.hpp>

using namespace std;
using namespace utilib;

namespace pico {


#define PHASE1
#define PHASE2
#define PHASE3
#define SMALLINF 1.0E+10
#define MAXK 10

///
///
/// Constants and Type Definitions
///
///


#define MAX_ITER 25 //number of preprocessor iterations

#define EPSILON (1e-6)
#define epsilon_positive(x)\
	 ((x) > EPSILON)
#define epsilon_negative(x)\
	 ((x) < -EPSILON)
#define epsilon_equal(x,y)\
	 (((x) < (y) + EPSILON) && ((x) > (y) - EPSILON))

enum CColStat
{
  BAD_COL_STAT,
  COL_FIX_AT_LB,
  COL_FIX_AT_UB,
  COL_OK
};


//#define DBG_RED //uncomment this if you want preprocessor to log debugging info


///
///
/// Definition of MILPProblem
///
///

ostream* MILPProblem::rout = &(cout); //added for Inferential Reduction logging

void MILPProblem::initialize(BasicArray<int> & listOfIntegers, int _debug)
{
integerVarNdx = listOfIntegers;
reverseBoundsNdx.resize(listOfIntegers.size());
debug = _debug;
constructGuts();
if (_debug > 0)
   print_stats(ucout);
}


void MILPProblem::initialize(char* MPSFile, int _debug)
{
debug = _debug;
lp()->loadProblem(MPSFile, integerVarNdx);
constructGuts();
if (_debug > 0)
   print_stats(ucout);
}


// An alternative form of "construction" when initialized from packed info

void MILPProblem::initialize(UnPackBuffer& inBuffer)
{
#ifdef ACRO_HAVE_MPI
if (!uMPI::iDoIO)
   CommonIO::setIORank(-2);
#endif

int numInts;
inBuffer >> numInts;
reverseBoundsNdx.resize(numInts);
integerVarNdx.resize(numInts);
for (int i = 0; i < numInts; i++)
  inBuffer >> integerVarNdx[i];
constructGuts();
}


MILPProblem::~MILPProblem()
{
// Note: LP deleted externally since it was constructed externally!!
}

// To rerun with a closely-related problem now specified in the same LP object.
// It's possible integer-variable bounds have changed so that ones that were 
// fixed
// in the previous problem are not fixed now.  Thus we need to reset the integer
// variable
// information.

void MILPProblem::reset()
{
reverseBoundsNdx.resize(integerVarCopy.size());
integerVarNdx = integerVarCopy;
computeTypes();
if (useSets)
  computeSets();
}

void MILPProblem::restoreVarBounds()
{
lp()->setColLower(originalLowerBounds.data());
lp()->setColUpper(originalUpperBounds.data());
}


MILPProblem::MILPProblem()
{
  // Set up the static data structures used for interpreting the
  // basis.  This is really an LP issue, but we only have one
  // MILPProblem object per computation (where we could have multiple
  // LP solvers) and this seems a safe and consistent place to
  // initialize
  // Warning: If the order changes, look for effects in PicoCpxLP.h (e.g. in
  // getBasis()

  char *labelArray = new char[4];
  labelArray[0] = 'N';
  labelArray[1] = 'B';
  labelArray[2] = 'U';
  labelArray[3] = 'L';
  basisState *enumValues = new basisState[4];
  enumValues[0] = nonbasic;
  enumValues[1] = basic;
  enumValues[2] = atUpper;
  enumValues[3] = atLower;

  BasisArray initializer;

  initializer.setEnumInformation(4,labelArray, enumValues);

reduceLog=false;
create_parameter("reduceLog",reduceLog,
	"<bool>","false",
	"If true, log preprocessor reductions to a log file");

debugReduceLog=false;
create_parameter("debugReduceLog",debugReduceLog,
	"<bool>","false",
	"If true, log preprocessor debug messages to a log file.  If this is on, the reduceLog *must* be on as well.");

allowInfiniteIntegerVarBounds = false;
create_parameter("allowInfiniteIntegerVarBounds",allowInfiniteIntegerVarBounds,
	"<bool>","false",
	"If true, allow integer variables to have infinite bounds.  PICO does not assume finite bounds by default, so users must specify finite bounds or use this option.");
}



// Some work common to all types of initialization.  lp has to be valid.
void MILPProblem::constructGuts()
{
integerVarCopy = integerVarNdx;
int numVars = lp()->getNumCols();
originalLowerBounds.resize(numVars);
lp()->getColLower(originalLowerBounds.data());
originalUpperBounds.resize(numVars);
lp()->getColUpper(originalUpperBounds.data());
//lp()->getInfo(objx,colmajor,senx,rhsx,LowerBound,UpperBound);
computeTypes();
NumSets=0;
if (useSets)
  computeSets();
}



//  From the list of which variables are integer, figure out 
//     a type for each variable: cts, binary, general integer.
//    or initially fixed.  Remove initially-fixed integer variables
// from the set of integer variables. Also, count the number of binary variables.

void MILPProblem::computeTypes() 
{
  int n = lp()->getNumCols();
  vtype.resize(n);
  LPtoIntegerVar.resize(n);
  int j;
  for(j=0; j<n; j++)
    {
    vtype[j] = continuous;
    LPtoIntegerVar[j] = -1;
    }
  int i = 0;
  NumBinaryVars = 0;
  int numFinalInts = 0;
  int int_l, int_u;
  DEBUGPR(50,  ucout << "Num ints before removing fixed is " << numIntegerVars()<< endl;);
  for(int k=0; k< numIntegerVars(); k++)
    {
      j = integerVarNdx[k];
      double l = lp()->getColLower(j);
      double u = lp()->getColUpper(j);
      double infty = lp()->getInfinity();

      if (l == infty)
	EXCEPTION_MNGR(runtime_error, "**** Integer Variable " << j << " has lower bound of positive infinity\n");
      if (l == -infty)
         int_l = -MAXINT;
      else if (isInteger(l))
	int_l = lround(l);
      else
	int_l = (int) ceil(l);

      if (u == -infty)
	EXCEPTION_MNGR(runtime_error, "**** Integer Variable " << j << " has upper bound of negative infinity\n");
      if (u == infty)
         int_u = MAXINT;
      else if (isInteger(u))
	 int_u = lround(u);
      else 
         int_u = (int) floor(u);

      if (int_l < int_u) 
	{
	  // This will be filled in behind (or at the same spot as) 
	  // the loop counter, i.e. numFinalInts <= k
	 integerVarNdx[numFinalInts] = j;
	 LPtoIntegerVar[j] = numFinalInts;

	  if ((int_l == 0) && (int_u == 1)) 
	    {
	      vtype[j] = binary;
	      NumBinaryVars++;
	      reverseBoundsNdx[numFinalInts++] = -1;
	    }
	  else
	    {
	    vtype[j] = generalInteger;
	    reverseBoundsNdx[numFinalInts++] = i++;
	    }
	}
      else if (int_l == int_u)
	{
	  // Don't record this as an integer variable
	  vtype[j] = initiallyFixed;
	  lp()->setColLower(j, int_l);
	  lp()->setColUpper(j, int_u);
	}
      else EXCEPTION_MNGR(runtime_error, "**** Variable " << j << " has lower bound greater than upper bound");
    }
  integerVarNdx.resize(numFinalInts);
  reverseBoundsNdx.resize(numFinalInts);
  DEBUGPR(100, printMIP())
  DEBUGPR(9, ucout <<  "Number of binary variables: " << NumBinaryVars << "\n" <<
	    "Number of general integer variables: " << numGeneralIntVars() << "\n" <<
	    "Total vars:  " << numAllVars() << "\n");

  DEBUGPR(11,
        ucout << "Initial VariableTypes= ";
	for (int i=0; i<n; i++)
          switch (vtype[i]) {
             case binary:         ucout << 'b'; break;
             case generalInteger: ucout << 'i'; break;
             case initiallyFixed: ucout << 'f'; break;
             case continuous:     ucout << 'c'; break;
             };
        ucout << endl;
        )
    lp()->setVariableTypes(vtype);
}

// From the constraint matrix, compute sets of binary variables constrained to
// sum to exactly 1.

void MILPProblem::computeSets()
{
  // If this is called after preprocessing, we need to reset this.
NumSets = 0;
if (NumBinaryVars == 0) return; // No special sets
const CoinPackedMatrix *A = lp()->getMatrixByRow();
int numRows = A->getNumRows();
const double *AElements = A->getElements();
const int *AIndices = A->getIndices();
const CoinBigIndex *AVectorStarts = A->getVectorStarts();

CharString rowType(numRows);
lp()->getRowSense(rowType.data());

// To record the row number of the special rows we find
BasicArray<CoinBigIndex> specialRows(numRows);
int currRowSize;
CoinBigIndex rowStepper;

int i,j;
bool isSpecial;
 int varNum, varCount;
 // Identify rows that represent special sets
for (i=0; i<numRows; i++)
  {
  if (rowType[(size_type)i] != 'E') continue;
  if (lp()->getRightHandSide(i) != 1.0) continue;
  varCount = 0;
  currRowSize = A->getVectorSize(i);
  rowStepper = AVectorStarts[i];
  isSpecial = true;
  for (j=0; j < currRowSize; j++)
    {
    varNum = AIndices[rowStepper++];
    if (vtype[varNum] == initiallyFixed)
      {
      if (lp()->getColLower(varNum) == 0.0)
	continue; // OK to have these in SOS
      else  // Not OK if fixed to anything other than 0
	{
        isSpecial = false;
	break;
	}
      }
    if (AElements[rowStepper] != 1.0 || vtype[varNum] != binary)
      {
      isSpecial = false;
      break;
      }
    // A set participating included in the potential SOS constraint
    else varCount++;
    }
  // Don't count constraints that just set a single variable to 1 (we've seen these)
  if (isSpecial && varCount >= 2)
    specialRows[NumSets++] = i;
  }
if (NumSets == 0) return;
int realSetSize;
specialSets.resize(NumSets);
for (i=0; i < NumSets; i++)
  {
  realSetSize = 0;
  currRowSize = A->getVectorSize(specialRows[i]);
  (specialSets[(size_type)i]).resize(currRowSize);
  rowStepper = AVectorStarts[specialRows[i]];
  for (j=0; j < currRowSize; j++)
    {
    varNum = AIndices[rowStepper++];
 // Special sets can also have variables fixed to 0.  Don't record those.
    if (vtype[varNum] == binary)
      {
      specialSets[(size_type)i][(size_type)realSetSize] = LPtoIntegerVar[(size_type)varNum];
      realSetSize++;
      }
    }
  (specialSets[(size_type)i]).resize(realSetSize);
  }

 int numInts = numIntegerVars();
setReverseNdx.resize(numInts);
// For safety.  This can be called a second time after preprocess
 for (i=0; i < numInts; i++)
   setReverseNdx[i].resize(0);
size_type row_size,elem,currSize;
for (size_type k=0; k < (size_type)NumSets; k++)
  {
  row_size = specialSets[k].size();
  for (elem=0; elem < row_size; elem++)
    {
    varNum = specialSets[k][elem];
    currSize = setReverseNdx[varNum].size();
    setReverseNdx[varNum].resize(currSize+1);
    setReverseNdx[varNum][currSize] = k;
    }
  }
// for debugging
// printSets();
}

void MILPProblem::printSets()
{
  ucout << "Special Sets (integer var #/LP var #):\n";
  size_type row_size,j;
  for (size_type i = 0; i < (size_type)NumSets; i++)
    {
    ucout << "\nSet " << i << ":";
    row_size = specialSets[i].size();
    for (j=0; j < row_size; j++)
      {
	size_type ivarnum = (size_type)specialSets[i][j];
	ucout << ' ' << ivarnum << '/' << integerVarNdx[ivarnum];
      if (j != 0 && j % 10 == 0) 
	ucout << endl;
      }
    }
  ucout << "\nReverse sets (integer variable to sets), "
	<< "sets are cardinality:members\n";
  int NumInts = numIntegerVars();
  for (size_type i = 0; i < (size_type) NumInts; i++)
    {
      if ((setReverseNdx[i]).size() > 0)
	{
	  ucout << "var " << i << '/' << integerVarNdx[i] << " in Sets count=" 
		<< setReverseNdx[i] << "\n";
        }
    }
}


// This is for initializing the root problem with integer lower and upper
// bounds, including those that are fixed initially.  Also initialize variables
// that can contain a 1 in a special set.  Returns false if we determine the problem is infeasible.

bool MILPProblem::getbounds(BitArray& fixed, IntVector& intLowerBounds,
		     IntVector& intUpperBounds, BitArray& binary_vals,
		     IntVector& spSetLowerBounds, IntVector &spSetUpperBounds)
{
  // Just to be sure.  If this is 0 for a binary variable, then it
  // has full range (0 to 1).
 fixed.reset();
 int j;
 for(int k=0; k< numIntegerVars(); k++)
    {
      j = integerVarNdx[k];
      // It would be nice to use Ereal<double> for these bounds, but
      // we have to handle different notions of infinity from different
      // solvers.  Even resetting infinity in the Ereal<double> class will
      // not work if we have multiple solvers.
      double l,u;
      double infty = lp()->getInfinity();
      l = lp()->getColLower(j);
      u = lp()->getColUpper(j);

      if (u == -infty || l == infty)
	EXCEPTION_MNGR(runtime_error, "**** Integer Variable " << j << "has bounds that force an infinite value.\n");
      if ((l == -infty || u == infty)
	  && !allowInfiniteIntegerVarBounds)
	EXCEPTION_MNGR(runtime_error, "**** Variable " << j << " is an integer variable with at least one infinite bound.  If this is what you intended, rerun PICO with the option --allowInfiniteIntegerVarBounds=true");

      int int_l, int_u;
      // These bound should be finite unless the user has explicitly
      // allowed infinite bounds
      if (l != -infty) // +infty should be ruled out
	{
	  if (isInteger(l))
	  // If within integer tolerance, round
	    int_l = lround(l);
	  else
	    int_l = (int) ceil(l);
	}
      if (u != infty) // -infty should be ruled out
	{
	  if (isInteger(u))
	  // If within integer tolerance, round
	    int_u = lround(u);
	  else 
	    int_u = (int) floor(u);
	}
       
      if (vtype[j] == generalInteger)
	{
	if ((l == -infty) || (l <= (double)INT_MIN))
	  intLowerBounds[reverseBoundsNdx[k]] = INT_MIN;
	else
	  intLowerBounds[reverseBoundsNdx[k]] = int_l;
	if ((u == infty) || (u >= (double)INT_MAX))
	  intUpperBounds[reverseBoundsNdx[k]] = INT_MAX;
	else
	  intUpperBounds[reverseBoundsNdx[k]] = int_u;
	}

      // This could happen when the value range narrows by taking floors and ceilings of
      // fractional bounds.  However, I expect this would have already been taken care of
      // during computeTypes().

      if (vtype[j] == generalInteger &&
	  intUpperBounds[reverseBoundsNdx[k]] < intLowerBounds[reverseBoundsNdx[k]])
	    return(false);

      // This should now never happen on the root, but leave the logic
      // in in case we ever need to use it for something else

      if (u == infty || l == -infty)
	continue;
      if (int_l == int_u)
	{
	  fixed.set(k);
	  if (vtype[j] == binary)
	    if (int_l == 0)  binary_vals.reset(k);
	    else binary_vals.set(k);
	}
    }
 // Initial special sets.  Initially the whole range (0 to size of set) can contain the single 1
 for (int idx = 0; idx < NumSets; idx++)
   {
   spSetLowerBounds[idx] = 0;
   spSetUpperBounds[idx] = (int) specialSets[idx].size()-1;
   }
 return(true);
}

// Only need enough to initialize, which is location of integer variables
void MILPProblem::packMIP(PackBuffer& outBuffer)
{
  int numInts = numIntegerVars();
  outBuffer << numInts;
  for (int i = 0; i < numInts; i++)
    outBuffer << integerVarNdx[i];
}


// For debugging

void MILPProblem::printMIP()
{
  int i,j;
  ucout << "Number of binary variables:          " << NumBinaryVars << "\n";
  ucout << "Number of general integer variables: " << numGeneralIntVars() << "\n";
  ucout << "Total ints:  " << numIntegerVars() << ", Total vars:  "
       << numAllVars() << "\n";
  ucout << "Types:\n";
  j = 0;
  for (i = 0; i < numAllVars(); i++)
    {
      ucout << "i:  ";
      switch (vtype[i]) {
	case binary:
	  ucout << "binary,  ";
	  break;
      case generalInteger:
	ucout << "general int,  ";
	break;
      case continuous:
	ucout << "continuous,  ";
	break;
      default:  ucout << "fixed,  ";
      }
      if (++j == 5)
	{
	  j = 0;
	  ucout << "\n";
	}
    }
  if (numIntegerVars() > 0) {
     ucout << "\nIndices of integer variables:\n";
     j = 0;
     for (i = 0; i < numIntegerVars(); i++)
       {
         ucout << i <<  ":  " << integerVarNdx[i] << "   ";
         if (++j == 5)
	   {
	     j = 0;
	     ucout << "\n";
	   }
       }
     }
  if (numGeneralIntVars() > 0)
    {
      ucout << "\nGeneral integer numbering (var index, gen int index):\n";
      j = 0;
      int k = 0;
      for (size_type st_i = 0; st_i < reverseBoundsNdx.size(); st_i++)
	{
	  ucout << "(" << st_i << ":  " << reverseBoundsNdx[st_i] << ")   ";
	  k += (reverseBoundsNdx[st_i] >= 0);
	  if (++j == 5)
	    {
	      j = 0;
	      ucout << endl;
	    }
	}
      ucout << endl << "Number >= 0: " << k << endl;
    }
  ucout << endl;
  // Added by JE/temporary?
  ucout << "Constraint matrix:\n";
  lp()->write(ucout,verbose_format);
  ucout << endl << endl << "Done dumping MIP\n\n";
}

/******** Inferential Reduction code ***************/

int MILPProblem::preprocess( void ) {
  lp()->getInfo(objx,colmajor,senx,LowerBound,UpperBound,lhsx,rhsx);

  /* BDP - There is a lot of code in the preprocessor that is commented out
   * with a message saying that dual tests are not currently used in the
   * preprocessor.  All of this code works correctly, but it interferes with
   * enumeration.  Since the payoff from dual tests is limited, the dual tests
   * have been commented out in favor of making enumeration work.
   */

  // BDP - There are an awful lot of calls to get_nrows and get_ncols,
  // including being called in loop control comparisons.  We could probably
  // speed this up by eliminating the overhead of all those function calls.
  // At the very we should probably replace the function calls used for loop
  // control.
  
  // BDP - These variables are declared in MILPProblem.h, but could be
  // declared here instead.  They would have to be passed to preprocess_rows,
  // preprocess_columns_one, and preprocess_columns_two as parameters which is
  // easy enough, and if it avoids memory reallocation then it is probably worth
  // doing.
  LowerBoundY.resize(get_nrows());
  UpperBoundY.resize(get_nrows());
  /* dual tests are not currently used in the preprocessor
   * LowerBoundP.resize(get_nrows());
   * UpperBoundP.resize(get_nrows());
   */
  LowerRangeY.resize(get_nrows());
  UpperRangeY.resize(get_nrows());
  Degree.resize(get_nrows());
  Marker.resize(get_nrows());

  // int nrows = get_nrows();
  // int ncols = get_ncols();

  //mark all rows as non-forcing
  Marker << 0;
  
  //set up the output file.
  rout = redLogFile();
  redLogInit();
 
#ifdef ACRO_VALIDATING
  if (IShouldWrite() && debugReduceLog) {
    *rout << "Debug Mode" << endl;
    *rout << endl;
    *rout << "VariableTypes= " << endl;
    for (int i=0; i<get_ncols(); i++) {
      switch (vtype[i]) {
        case binary:         *rout << 'b'; break;
        case generalInteger: *rout << 'i'; break;
        case initiallyFixed: *rout << 'f'; break;
        case continuous:     *rout << 'c'; break;
      };
    *rout << endl;
    }
  *rout << endl;
  }
 #endif

  //
  //initialize row bounds, row ranges, price bounds, and degree of row
  //

  if (IShouldWrite())
    *rout << "Initialize:" << endl;
  
  //initialize row bounds
  BasicArray<Ereal<double> > originalLowerBound;
  BasicArray<Ereal<double> > originalUpperBound;
  originalLowerBound.resize(get_ncols());
  originalUpperBound.resize(get_ncols());
  originalLowerBound << LowerBound;
  originalUpperBound << UpperBound;
  LowerBoundY << lhsx;
  UpperBoundY << rhsx;

  for (int i=0; i < get_nrows(); i++) {
    // BDP - sanity checks.  I think we need to add a check to make 
    // sure lhsx <= rhsx, since the lp would be infeasible if that were not true
    //TODO add check for NaN
    if ((rhsx[i] == Ereal<double>::positive_infinity) && (lhsx[i] == Ereal<double>::positive_infinity))
      EXCEPTION_MNGR(runtime_error, "Both bounds at positive infinity");

    if ((rhsx[i] == Ereal<double>::negative_infinity) && (lhsx[i] == Ereal<double>::negative_infinity))
      EXCEPTION_MNGR(runtime_error, "Both bounds at negative infinity");

/* dual tests are not currently used in the preprocessor
 *  //initialize dual prices from associated y bounds.
 *  if ((int) lp()->getObjSense() == minimize) {
 *    //minimization problem
 *    if (lhsx[i] == Ereal<double>::negative_infinity) {
 *      if (rhsx[i] == Ereal<double>::positive_infinity) {
 *        LowerBoundP[i] = 0;
 *        UpperBoundP[i] = 0;
 *      }
 *      else {
 *        LowerBoundP[i] = Ereal<double>::negative_infinity;
 *        UpperBoundP[i] = 0;
 *      }
 *    }
 *    else {
 *      if (rhsx[i] == Ereal<double>::positive_infinity) {
 *        LowerBoundP[i] = 0;
 *        UpperBoundP[i] = Ereal<double>::positive_infinity;
 *      }
 *      else {
 *        LowerBoundP[i] = Ereal<double>::negative_infinity;
 *        UpperBoundP[i] = Ereal<double>::positive_infinity;
 *      }
 *    }
 *  }
 *  else {
 *    //maximazation problem
 *    if (lhsx[i] == Ereal<double>::negative_infinity) {
 *      if (rhsx[i] == Ereal<double>::positive_infinity) {
 *        LowerBoundP[i] = 0;
 *        UpperBoundP[i] = 0;
 *      }
 *      else {
 *        LowerBoundP[i] = 0;
 *        UpperBoundP[i] = Ereal<double>::positive_infinity;
 *      }
 *    }
 *    else {
 *      if (rhsx[i] == Ereal<double>::positive_infinity) {
 *        LowerBoundP[i] = Ereal<double>::negative_infinity;
 *        UpperBoundP[i] = 0;
 *      }
 *      else {
 *        LowerBoundP[i] = Ereal<double>::negative_infinity;
 *        UpperBoundP[i] = Ereal<double>::positive_infinity;
 *      }
 *    }
 *  }
 */

#ifdef ACRO_VALIDATING
    if (IShouldWrite() && debugReduceLog) {
      // **** logging
      *rout << "contstraint " << i << endl;
      *rout << " sense= " << senx[i] << endl;
      *rout << " LowerBoundY= " << LowerBoundY[i] << endl;
      *rout << " UpperBoundY= " << UpperBoundY[i] << endl;
      
      /* dual tests are not currently used in the preprocessor
       * *rout << " LowerBoundP= " << LowerBoundP[i] << endl;
       * *rout << " UpperBoundP= " << UpperBoundP[i] << endl;
       */
    }
#endif

  } //end for

  // the following arrays are used Summary of Reduction in reduce log
  // BDP - These are declared in MILPProblem.h, but could be reduced in scope.
  // They are only used in the preprocessor.
  freed.resize(get_nrows());
  freed << 0;

  cfixed.resize(get_ncols());
  cfixed << 0;
  
  rreduced.resize(get_nrows());
  rreduced << 0;
  
  creduced.resize(get_ncols());
  creduced << 0;
  
  rforced.resize(get_nrows());
  rforced << 0;
  
  cforced.resize(get_ncols());
  cforced << 0;
 
  weakened.resize(get_nrows());
  weakened << 0;

  // end reduction summary arrays
  
  // BDP - these should move down to where they are being used,
  // about 150 lines from here.
  /// infFlag is returned by each member function in the preprocessor
  /// if the returned value is > than 0, the problem has been found to
  /// infeasible.  1 = primal infeasible, 2 = dual infeasible
  int infFlag = 0, tmpFlag;  

  BasicArray<Ereal<double> > Coef; //DoubleVector Coef;
  Coef.resize(get_nrows());
  IntVector Colidx;
  Colidx.resize(get_nrows());

  // BDP - This is initializing the row degrees and the coefficients used below
  // to make bound adjustments.
  // It's not immediately clear (to me at least) that this iterates through the
  // matrix in a memory friendly way.  I think it is, but need to look at it more
  // TO DO:
  // 1) Get detail of how the matrix is stored.  Is the column major form using
  //    an array like [row_num][value] with pointers to the beginning of columns,
  //    is it a hash by columns with (row, value) pairs?
  // 2) look at this more.
  //    - After staring at this more, this is correct.  It just needs to be
  //    - documented better
  for (int j=0; j < get_ncols(); j++) {
    int rcnt = colmajor.get_matcnt()[j];      
    int* rind =  colmajor.get_matind().data() + colmajor.get_matbeg()[j];
    Ereal<double>* coefp = colmajor.get_matval().data() + colmajor.get_matbeg()[j];
       
    for (int i=0; i < rcnt; i++) {
      Degree[rind[i]] += 1;
      Coef[rind[i]] = coefp[i];
      Colidx[rind[i]] = j;
    }
  }

#ifdef ACRO_VALIDATING
  if (IShouldWrite() && debugReduceLog) {
    *rout << "Degree, Coef, Colidx " << endl;
    for (int i=0; i < get_nrows(); i++) {
      *rout << Degree[i] << ", " << Coef[i] << ", " << Colidx[i] << endl;
    }
    *rout << endl;
  }
#endif

  for (int i=0; i < get_nrows(); i++) {
    Ereal<double> lbhat = 0.0;  //implied lower row bound
    Ereal<double> ubhat = 0.0;  //implied upper row bound
    
    // return infeasible if row degree == 0 but lower or upper bound forces
    // a nonzero value
    if(Degree[i] == 0) {
      if(LowerBoundY[i] > 0.0 || UpperBoundY[i] < 0.0) {
        redLogInfeasible('-',433);
        return 1; //infeasible
      }
    }
    // if row degree == 1, replace the constraint with a bound, or fix
    // the variable
    else if(Degree[i] == 1) {
      // Define implied bounds.
      if(Coef[i] > 0) {
        // if coef > 0, then lbhat == (lower bound / coef) <= x and 
        //                   ubhat == (upper bound / coef) >= x 
        lbhat = LowerBoundY[i]/Coef[i];
        ubhat = UpperBoundY[i]/Coef[i];
      } else {
        // if coef < 0, then lbhat == (upper bound / coef) <= x and 
        //                   ubhat == (lower bound / coef) >= x 
        lbhat = UpperBoundY[i]/Coef[i];
        ubhat = LowerBoundY[i]/Coef[i];
      }
    } // end else if
 
    //-check if variable is integer, make appropriate integrality adjustment to lbhat and ubhat
    // if the variable type is integer, tighten the upper and lower bounds to
    // make them integers as well
    //
    // BDP - 1 == binary variable, 2 == general integer.  These are defined
    // in an enum if mipEnum.h.  That header is already included, so these
    // should just use the labels from that enum instead of hard coded ints.
    if((vtype[Colidx[i]] == 1) || (vtype[Colidx[i]] == 2)) {
      ubhat = floor(ubhat);
      lbhat = ceil(lbhat);
    }
    //-bool temp = true;
    //-temp = compare(0, ubhat, 4, 5);
      
    // return infeasible if lower bound > adjusted upper bound
    if (compare(ubhat, LowerBound[Colidx[i]], 2, 3)) {
      redLogInfeasible('-',456); //?
      return 1; //infeasible
    } 

    // return infeasible if upper bound < adjusted lower bound
    if (compare(lbhat, UpperBound[Colidx[i]], 4, 3)) {
      redLogInfeasible('-',459); //?
      return 1; //infeasible
    }

    // Set the lower bound to max{current lower bound, adjusted lower bound}
    //if lbhat > LowerBound[Colidx[i]]
    if (compare(lbhat, LowerBound[Colidx[i]], 4, 1)) {
      LowerBound[Colidx[i]] = lbhat;
      //BDP shouldn't this stuff be written out before changing the value of LowerBound[Colidx[i]] ?
      if (IShouldWrite()) {
        *rout << "LowerBound[" << Colidx[i] << "] updated from " << LowerBound[Colidx[i]] << " to " << lbhat << endl;
        *rout << "This frees row " << lp()->getRowName(i) << " (column " << lp()->getVariableName(Colidx[i]) << ")" << endl;
      }
      freed[Colidx[i]] = 1;
    }

    // Set the upper bound to min{current upper bound, adjusted upper bound}
    //if(ubhat < UpperBound[Colidx[i]])
    if (compare(ubhat, UpperBound[Colidx[i]], 2, 1)) {
      if (IShouldWrite()) {
        *rout << "UpperBound[" << Colidx[i] << "] updated from " << UpperBound[Colidx[i]] << " to " << ubhat << endl;
        *rout << "This frees row " << lp()->getRowName(i) << " (column " << lp()->getVariableName(Colidx[i]) << ")" << endl;
      }
      UpperBound[Colidx[i]] = ubhat;
      freed[Colidx[i]] = 1;
    }

    // check if UpperBound and LowerBound are equal within tolerance
    // if they are, the variable can be fixed.
    //
    // if upper bound == lower bound within tolerance
    if (compare(UpperBound[Colidx[i]], LowerBound[Colidx[i]], 1, 5)) {
      // if the lower bound did not change, set upper bound = lower bound
      if (LowerBound[Colidx[i]] == originalLowerBound[Colidx[i]]) {
        if (IShouldWrite()) {
          *rout << "UpperBound[" << Colidx[i] << "] updated from " << UpperBound[Colidx[i]]
                << " to " << LowerBound[Colidx[i]] << endl;
        }
        UpperBound[Colidx[i]] = LowerBound[Colidx[i]];
      // if the upper bound did not change, set lower bound = upper bound
      } else if (UpperBound[Colidx[i]] == originalUpperBound[Colidx[i]]) {
        if (IShouldWrite()) {
          *rout << "LowerBound[" << Colidx[i] << "] updated from " << LowerBound[Colidx[i]]
                << " to " << UpperBound[Colidx[i]] << endl;
        }
        LowerBound[Colidx[i]] = UpperBound[Colidx[i]];
      // if both bounds changed, but they are close to the same integer, set
      // both bounds equal to that integer.
      } else if (ceil(LowerBound[Colidx[i]])==floor(UpperBound[Colidx[i]])) {
        if (IShouldWrite()) {
          *rout << "LowerBound[" << Colidx[i] << "] and UpperBound[" << Colidx[i]
                << "] updated from " << LowerBound[Colidx[i]] << " and "
                << UpperBound[Colidx[i]] << " to " << ceil(LowerBound[Colidx[i]]) << endl;
        }
        LowerBound[Colidx[i]]=UpperBound[Colidx[i]]=ceil(LowerBound[Colidx[i]]);
      // otherwise, set both bounds to the average of the bounds
      } else {
        if (IShouldWrite()) {
          *rout << "LowerBound[" << Colidx[i] << "] and UpperBound[" << Colidx[i]
                << "] updated from " << LowerBound[Colidx[i]] << " and "
                << UpperBound[Colidx[i]] << " to "
                << (LowerBound[Colidx[i]]+UpperBound[Colidx[i]])/2.0 << endl;
        }
        LowerBound[Colidx[i]] = UpperBound[Colidx[i]] = (LowerBound[Colidx[i]]+UpperBound[Colidx[i]])/2.0;
      }
        
      // mark the column as fixed
      if (IShouldWrite()) {
        *rout << "This fixes column " << lp()->getVariableName(Colidx[i]) << endl;
      }
      cfixed[Colidx[i]] = 1;
    } //end if
  } //end for

  //Convert to a minimization problem if it isn't one already.
  //BDP - Is this necessary?  If so, it should be done before dual prices are set.
  if ((int) lp()->getObjSense() != pebbl::minimize) {
    for (int j=0; j < get_ncols(); j++) {
      objx[j] = (Ereal<double>)-(double)objx[j];
    }
  }
  
  //begin main preprocessing
  int count = MAX_ITER;
  while(count > 0) {
    // initialize row range values
    // the reduce document states that this initialization actually takes
    // place above and after the row loop
   
#ifdef ACRO_VALIDATING
    if (IShouldWrite() && debugReduceLog) {
      *rout << "Inititalizing LowerRange and UpperRange to 0:" << endl;
      *rout << endl;
    }
#endif
    LowerRangeY << static_cast<Ereal<double> >(0.0);
    UpperRangeY << static_cast<Ereal<double> >(0.0);
    if (IShouldWrite()) {
      *rout << endl;
      *rout << "REDUCE PASS  " << (MAX_ITER - count + 1) << endl;
      *rout << "===================" << endl;
      *rout << endl;
    }
    infFlag = preprocess_columns_one();
    if (infFlag > 0) return infFlag;

    tmpFlag = preprocess_rows();
    if (tmpFlag > 0) return tmpFlag;

    infFlag += tmpFlag;
    tmpFlag = preprocess_columns_two();
    if (tmpFlag > 0) return tmpFlag;

    infFlag += tmpFlag;
    if (infFlag == 0) break;
      
    count -= 1;
  } //end while

  // Summary of reduction events
  int ifreed = 0;
  int ifixed = 0;
  int icreduced = 0;
  int irreduced = 0;
  int icforced = 0;
  int irforced = 0;
  int iweakened = 0;
      
  for (int j = 0; j < get_ncols(); j++) {
    ifixed += cfixed[j];
    icreduced += creduced[j];
    icforced += cforced[j];
  }
      
  for (int i = 0; i < get_nrows(); i++) {
    ifreed += freed[i];
    irreduced += rreduced[i];
    irforced += rforced[i];
    iweakened += weakened[i];
  }
      
  if (IShouldWrite()) {
    *rout << "Summary of Reductions" << endl;
    *rout << "=====================" << endl;
    *rout << "Rows:" << endl;
    *rout << " Freed: " << ifreed << endl;
    *rout << " Forced: " << irforced << endl;
    *rout << " Reduced: " << irreduced << endl;
    *rout << " Weakened: " << iweakened << endl;
    *rout << "Columns:" << endl;
    *rout << " Fixed: " << ifixed << endl;
    *rout << " Forced: " << icforced << endl;
    *rout << " Reduced: " << icreduced << endl;
    //end summary logging    
  }

  // Update at the MILP level to reflect variables that are fixed
  // initially either in the initial problem statement (mps) or in
  // this preprocessing.  They should not be considered integer variables
  // for the search.

  // TODO: see if we can use computeTypes(), perhaps with arguments, to avoid
  // this duplication.

  int numInitialInts = numIntegerVars();
  int n = numAllVars();
  int numFinalInts = 0;
  int numFinalGeneral = 0;
  // recount this
  NumBinaryVars = 0;
  int LPVarNum;
  variableType thisVarType;
  size_type k;
  
  for (k = 0; k < (size_type) n; k++)
    LPtoIntegerVar[k] = -1;

  for (k = 0; k < (size_type) numInitialInts; k++) {
    LPVarNum = integerVarNdx[k];
    thisVarType = vtype[LPVarNum];
    if (thisVarType == continuous)  continue;

#ifdef ACRO_VALIDATING
    if (!isInteger(LowerBound[LPVarNum]) || !isInteger(UpperBound[LPVarNum])) {
      EXCEPTION_MNGR(runtime_error,"Variable " << LPVarNum
        << " is integer but final preprocessor bounds of ("
        << (double) LowerBound[LPVarNum] << "," << (double) UpperBound[LPVarNum]
        << ") are not within integer tolerance");
    }
#endif

    // Treat initiallyFixed as if it's continuous, except make sure the
    // bounds set in the LP (once and for all) are true integers
    if (thisVarType == initiallyFixed) {
      LowerBound[LPVarNum] = lround(LowerBound[LPVarNum]);
      UpperBound[LPVarNum] = lround(UpperBound[LPVarNum]);
      continue;
    }

    // Forced by the preprocessor.  Consider continuous.
    if (UpperBound[LPVarNum] - LowerBound[LPVarNum] <= integerTolerance) {
      DEBUGPR(10,  ucout << "Preprocessor fixing variable " << LPVarNum << " to " << LowerBound[LPVarNum] << "\n");
      vtype[LPVarNum] = initiallyFixed;
      continue;
    }

    // Integer variables with real ranges
    integerVarNdx[numFinalInts] = LPVarNum;
    LPtoIntegerVar[LPVarNum] = numFinalInts;
    if (vtype[LPVarNum] == generalInteger) {
      reverseBoundsNdx[numFinalInts++] = numFinalGeneral++;
    } else { // binary
      reverseBoundsNdx[numFinalInts++] = -1;
      NumBinaryVars++;
    }
  } //end for

  if (numInitialInts > numFinalInts) {
    integerVarNdx.resize(numFinalInts);
    reverseBoundsNdx.resize(numFinalInts);
    if (useSets)
      computeSets();
  }

  lp()->updateProblem( LowerBound, UpperBound, lhsx, rhsx);


#ifdef ACRO_VALIDATING

  DEBUGPR(10, ucout << "VariableTypes after preprocess = ";
    int n = lp()->getNumCols();
    for (int i=0; i<n; i++)
      switch (vtype[i]) {
        case binary:         ucout << 'b'; break;
        case generalInteger: ucout << 'i'; break;
        case initiallyFixed: ucout << 'f'; break;
        case continuous:     ucout << 'c'; break;
      };
      ucout << endl;
  )
#endif

return OK;
}

//first preprocessing pass through columns
//
int MILPProblem::preprocess_columns_one() {
  int infFlag = 0;

  for (int j = 0; j < get_ncols(); j++) {
      /* dual tests are not currently used in the preprocessor
       * Ereal<double> dmin = objx[j];
       * Ereal<double> dmax = objx[j];
       */

#ifdef ACRO_VALIDATING
    if (IShouldWrite() && debugReduceLog) {
      *rout << "Variable " << j << endl;
      *rout << endl;
    }
#endif

    //set_extremes
    //
    int rcnt = colmajor.get_matcnt()[j];
    int* rind =  colmajor.get_matind().data() + colmajor.get_matbeg()[j];
    Ereal<double>* rval = colmajor.get_matval().data() + colmajor.get_matbeg()[j];

#ifdef ACRO_VALIDATING
    if (IShouldWrite() && debugReduceLog) {
      *rout <<"Set Extremes:" << endl;
      //*rout << "LowerRangeY" << ", " << "UpperRangeY" << endl;
    }
#endif

    for (int i = 0; i < rcnt; i++) {
      if (rval[i] > 0){
        LowerRangeY[rind[i]] += rval[i] * LowerBound[j];
        UpperRangeY[rind[i]] += rval[i] * UpperBound[j];
          
        /* dual tests are not currently used in the preprocessor
         * dmin = dmin - rval[i] * UpperBoundP[rind[i]];
         * dmax = dmax - rval[i] * LowerBoundP[rind[i]];
         */
      } else {
        LowerRangeY[rind[i]] += rval[i] * UpperBound[j];
        UpperRangeY[rind[i]] += rval[i] * LowerBound[j];
  
        /* dual tests are not currently used in the preprocessor
         * dmin = dmin - rval[i] * LowerBoundP[rind[i]];
         * dmax = dmax - rval[i] * UpperBoundP[rind[i]];
         */
      }
    }

#ifdef ACRO_VALIDATING
    /* dual tests are not currently used in the preprocessor
     * if (IShouldWrite() && debugReduceLog) {
     *   // logging
     *   // *rout << "Variable " << j << ": " << LowerRangeY[j] << "," << UpperRangeY[j] << endl; 
     *   *rout << "dmin= " << dmin << endl;
     *   *rout << "dmax= " << dmax << endl;
     *   *rout << endl;
     *   // end logging
     * }
     */
#endif

    //set_forced
    //
 
#ifdef ACRO_VALIDATING
    if (IShouldWrite() && debugReduceLog) {
      *rout << "Set Forced:" << endl;
    }
#endif
      
    if (LowerBound[j] == UpperBound[j]) {
      // skip following code, row was already forced
    } else { 
      for (int i = 0; i < rcnt; i++) {
        if (Marker[rind[i]]>0 && UpperBound[j]>LowerBoundY[rind[i]]*1.0e-2) {

#ifdef ACRO_VALIDATING
          if (IShouldWrite() && debugReduceLog) {
            *rout << "Marker[" << rind[i] << "] = " << Marker[rind[i]] << endl;
          }
#endif
          if (IShouldWrite()) {
            *rout << "Set Forced:";
            *rout << " Variable " << j << " ";
          }

          if (rval[i] > 0) {
            if (IShouldWrite()) {
              *rout << " LowerBound set to UpperBound = " << UpperBound[j] << endl;
              *rout << " Column " << lp()->getVariableName(j) << " fixed at upper bound (by row "
                    << lp()->getRowName(rind[i]) << ")" << endl;
            }
            LowerBound[j] = UpperBound[j];
          } else {
            if (IShouldWrite()) {
              *rout << " UpperBound set to LowerBound = " << LowerBound[j] << endl;
              *rout << " Column " << lp()->getVariableName(j) << " fixed at lower bound (by row "
                    << lp()->getRowName(rind[i]) << ")" << endl;
            }
            UpperBound[j] = LowerBound[j];
          }
          cfixed[j] = 1;
          --infFlag;
          break;
        } else if (Marker[rind[i]] < 0) {

#ifdef ACRO_VALIDATING
          if (IShouldWrite() && debugReduceLog) {
            *rout << "Marker[" << rind[i] << "] = " << Marker[rind[i]] << endl;
          }
#endif
          if (IShouldWrite()) {
            *rout << "Set Forced:";
            *rout << " Variable " << j << " ";
          }

          if (rval[i] > 0) {
            if (IShouldWrite()) {
              *rout << " UpperBound set to LowerBound = " << LowerBound[j] << endl;
              *rout << " Column " << lp()->getVariableName(j) << " fixed at lower bound (by row "
                    << lp()->getRowName(rind[i]) << ")" << endl;
            }
            UpperBound[j] = LowerBound[j];
          } else {
            if (IShouldWrite()) {
              *rout << " LowerBound set to UpperBound = " << UpperBound[j] << endl;
              *rout << " Column " << lp()->getVariableName(j) << " fixed at Upper bound (by row "
                    << lp()->getRowName(rind[i]) << ")" << endl;
            }
            LowerBound[j] = UpperBound[j];
          }
          cfixed[j] = 1;
          --infFlag;
          break;
        }
      }
    
      if (LowerBound[j] == UpperBound[j]) {
        for (int i = 0; i < rcnt; i++) {
          Degree[rind[i]] -= 1;
        }
      }
    }
  
      //test_drange
      //
      //

/* dual tests are not currently used in the preprocessor
 *      Ereal<double> LowerBoundD = 0;
 *      Ereal<double> UpperBoundD = 0;
 *
 *#ifdef ACRO_VALIDATING
 *      if (IShouldWrite() && debugReduceLog)
 *  {
 *        *rout << "Test d Range:" << endl;
 *  }
 *#endif
 *
 *      if((LowerBound[j] != Ereal<double>::negative_infinity) &&
 *   (UpperBound[j] == Ereal<double>::positive_infinity))
 *        UpperBoundD = Ereal<double>::positive_infinity;
 *
 *      else if ((UpperBound[j] != Ereal<double>::positive_infinity) &&
 *         (LowerBound[j] == Ereal<double>::negative_infinity))
 *  LowerBoundD = Ereal<double>::negative_infinity;
 *
 *      else if ((LowerBound[j] != Ereal<double>::negative_infinity) &&
 *         (UpperBound[j] != Ereal<double>::positive_infinity)) {
 *        LowerBoundD = Ereal<double>::negative_infinity;
 *        UpperBoundD = Ereal<double>::positive_infinity;
 *      }
 *    
 *#ifdef ACRO_VALIDATING
 *      if (IShouldWrite() && debugReduceLog)
 *  {
 *        *rout << "LowerBoundD= " << LowerBoundD << endl;
 *        *rout << "UpperBoundD= " << UpperBoundD << endl;
 *        }
 *#endif
 *      
 *      //if ((dmax < LowerBoundD) || (dmin > UpperBoundD))
 *      if (compare(dmax, LowerBoundD, 2, 4) || compare(dmin, UpperBoundD, 4, 4))
 *      { 
 *  redLogInfeasible('-',591);
 *  return 2; //dual infeasible
 *      }
 *      //if (dmax == LowerBoundD)
 *      else if (compare(dmax, LowerBoundD, 1, 5)) {
 *  for (int i = 0; i < rcnt; i++)
 *    if (LowerBoundP[rind[i]] != UpperBoundP[rind[i]]) {
 *          if (rval[i] > 0) {
 *            UpperBoundP[rind[i]] = LowerBoundP[rind[i]];
 *      if (IShouldWrite())
 *        {
 *              *rout << "Test d Range:";
 *        *rout << " Row " << rind[i] << " ";
 *        *rout << " UpperBoundP set to LowerBoundP= " << LowerBoundP[rind[i]] << endl;
 *        *rout << " Row " << lp()->getRowName(rind[i]) << " duals forced to lower bound (by column " << lp()->getVariableName(j) << ")" << endl;
 *        }
 *      rforced[rind[i]] = 1;
 *      --infFlag;
 *          }
 *          else {
 *            LowerBoundP[rind[i]] = UpperBoundP[rind[i]];
 *      if (IShouldWrite())
 *        {
 *        *rout << "Test d Range:";
 *        *rout << " Row " << rind[i] << " ";
 *        *rout << " LowerBoundP set to UpperBoundP= " << UpperBoundP[rind[i]] << endl;
 *        *rout << " Row " << lp()->getRowName(rind[i]) << " duals forced to upper bound (by column " << lp()->getVariableName(j) << ")" << endl;
 *        }
 *      rforced[rind[i]] = 1;
 *      --infFlag;
 *          }
 *        }
 *      }
 *      //if (dmin == UpperBoundD) {
 *      else if (compare(dmin, UpperBoundD, 1, 5)) {
 *        for (int i = 0; i < rcnt; i++)
 *    if (LowerBoundP[rind[i]] != UpperBoundP[rind[i]]) {
 *          if (rval[i] > 0) {
 *            LowerBoundP[rind[i]] = UpperBoundP[rind[i]];
 *      if (IShouldWrite())
 *        {
 *              *rout << "Test d Range:";
 *        *rout << " Row " << rind[i] << " ";
 *        *rout << " LowerBoundP set to UpperBoundP= " << UpperBoundP[rind[i]] << endl;
 *        *rout << " Row " << lp()->getRowName(rind[i]) << " duals forced to upper bound (by column " << lp()->getVariableName(j) << ")" << endl;
 *        }
 *      rforced[rind[i]] = 1;
 *      --infFlag;
 *          }
 *          else {
 *            UpperBoundP[rind[i]] = LowerBoundP[rind[i]];
 *      if (IShouldWrite())
 *        {
 *        *rout << "Test d Range:";
 *        *rout << " Row " << rind[i] << " ";
 *        *rout << " UpperBoundP set to LowerBoundP= " << LowerBoundP[rind[i]] << endl;
 *        *rout << "Row " << lp()->getRowName(rind[i]) << " duals forced to lower bound (by column " << lp()->getVariableName(j) << ")" << endl;
 *        }
 *      rforced[rind[i]] = 1;
 *      --infFlag;
 *          }
 *        }
 *      }
 */

      //test_bound
      //

/* dual tests are not currently used in the preprocessor
 *#ifdef ACRO_VALIDATING
 *      if (IShouldWrite() && debugReduceLog)
 *  {
 *        *rout << "Test Bound:" << endl;
 *  }
 *#endif
 *
 *      //if (dmax < 0) {
 *      if (compare(dmax, 0, 2, 7)) {
 *  if (UpperBound[j] == Ereal<double>::positive_infinity) {
 *    redLogInfeasible('-',618);
 *    return 2; //dual infeasible
 *  }
 *  else if (LowerBound[j] != UpperBound[j]) {
 *    LowerBound[j] = UpperBound[j];
 *    if (IShouldWrite())
 *      {
 *            *rout << "Test Bound:";
 *      *rout << " Variable " << j << " ";
 *      *rout << " LowerBound set to UpperBound = " << UpperBound[j] << endl;
 *      *rout << " Column " << lp()->getVariableName(j) << " fixed at upper bound (by reduced cost bound)" << endl;
 *      }
 *          cfixed[j] = 1;
 *    --infFlag;
 *  }
 *      }
 *      //if (dmin > 0) {
 *      if (compare(dmin, 0, 4, 7)) {
 *  if (LowerBound[j] == Ereal<double>::negative_infinity) {
 *    redLogInfeasible('-',627);
 *    return 2; //dual infeasible
 *  }
 *  else if (UpperBound[j] != LowerBound[j]) {
 *    UpperBound[j] = LowerBound[j];
 *    if (IShouldWrite())
 *      {
 *            *rout << "Test Bound:";
 *      *rout << " Variable " << j << " ";
 *      *rout << " UpperBound set to LowerBound = " << LowerBound[j] << endl;
 *      *rout << " Column " << lp()->getVariableName(j) << " fixed at lower bound (by reduced cost bound)" << endl;
 *      }
 *    cfixed[j] = 1;
 *    --infFlag;
 *  }
 *      }
 */     
      //test_dual
      //
/* dual tests are not currently used in the preprocessor
 *
 *#ifdef ACRO_VALIDATING
 *      if (IShouldWrite() && debugReduceLog)
 *  {
 *        *rout << "Test Dual:" << endl;
 *        }
 *#endif
 *
 *      Ereal<double> lhat;
 *      Ereal<double> uhat;
 *
 *      if ((LowerBoundD == Ereal<double>::negative_infinity) && 
 *    (UpperBoundD == Ereal<double>::positive_infinity)) {
 *  // code will be skipped
 *      }
 *      else {
 *      for (int i = 0; i < rcnt; i++) {
 *  if (rval[i] > 0) {
 *    if (LowerBoundD == 0.0) {
 *      uhat = dmax / rval[i] + LowerBoundP[rind[i]];
 *      if (isind(uhat))
 *        uhat =  Ereal<double>::positive_infinity;
 *    } else
 *      uhat = Ereal<double>::positive_infinity;
 *    if (UpperBoundD == 0.0) {
 *      lhat = dmin / rval[i] + UpperBoundP[rind[i]];
 *      if (isind(lhat))
 *        lhat =  Ereal<double>::negative_infinity;
 *    } else
 *      lhat = Ereal<double>::negative_infinity;
 *  }
 *  else if (rval[i] < 0.0) {
 *    if (LowerBoundD == 0.0) {
 *      lhat = dmax / rval[i] + LowerBoundP[rind[i]];
 *      if (isind(lhat))
 *        lhat =  Ereal<double>::negative_infinity;
 *    } else
 *      lhat = Ereal<double>::negative_infinity;
 *    if (UpperBoundD == 0.0) {
 *      uhat = dmin / rval[i] + UpperBoundP[rind[i]];
 *      if (isind(uhat))
 *        uhat =  Ereal<double>::positive_infinity;
 *    } else
 *      uhat = Ereal<double>::positive_infinity;
 *  }
 *  //if (LowerBoundP[rind[i]] < lhat) 
 *  if (compare(lhat, LowerBoundP[rind[i]], 4, 2)) {
 *    LowerBoundP[rind[i]] = lhat;
 *    if (IShouldWrite())
 *      {
 *      *rout << "Test Dual:";
 *      *rout << " Row " << rind[i] << " ";
 *      *rout << " LowerBoundP set to lhat = " << lhat << endl;
 *      *rout << " Row " << lp()->getRowName(rind[i]) << " dual lower bound tightened (reduced)" << endl;
 *      }
 *    rreduced[rind[i]] = 1;
 *    --infFlag;
 *  }
 *  //if (UpperBoundP[rind[i]] > uhat) 
 *  if (compare(uhat, UpperBoundP[rind[i]], 2, 2)) {
 *    UpperBoundP[rind[i]] = uhat;
 *    if (IShouldWrite())
 *      {
 *            *rout << "Test Dual:";
 *      *rout << " Row " << rind[i] << " ";
 *      *rout << " UpperBoundP set to uhat = " << uhat << endl;
 *      *rout << " Row " << lp()->getRowName(rind[i]) << " dual upper bound tightened (reduced)" << endl;
 *      }
 *    rreduced[rind[i]] = 1;
 *    --infFlag;
 *  }
 *       }
 *      }
 */
  } //end for

#ifdef ACRO_VALIDATING
    if (IShouldWrite() && debugReduceLog) {
    // logging
      for (int i = 0; i < get_nrows(); i++) {
        *rout << "LowerRangeY[" << i << "]=" << LowerRangeY[i] << endl;
        *rout << "UpperRangeY[" << i << "]=" << UpperRangeY[i] << endl;
      }
      *rout << endl;
      // end logging
    }
#endif

return infFlag;
}

//preprocessing pass through rows
//
int MILPProblem::preprocess_rows() {
  int infFlag = 0;

  for (int i = 0; i < get_nrows(); i++) {

#ifdef ACRO_VALIDATING
    if (IShouldWrite() && debugReduceLog) {
      *rout << "Row " << i << endl;

      //clear_marker

      *rout << "Clear Marker:" << endl;
    }
#endif

    if(Marker[i] == 0) {
#ifdef ACRO_VALIDATING
      if (IShouldWrite() && debugReduceLog)
        *rout << "Marker[" << i << "]= " << 0 << endl;
#endif 
    } //skip code
    else {
#ifdef ACRO_VALIDATING
      if (IShouldWrite() && debugReduceLog) {
        *rout << "Clear Marker:" << endl;
        *rout << " Marker[" << i << "]= " << Marker[i] << endl;
        *rout << " Row " << lp()->getRowName(i) << " was freed" << endl;
        *rout << "  Initializing YBounds to +- infinity" << endl;
        /* dual tests are not currently used in the preprocessor
         * *rout << "  Initializing PBounds to 0" << endl;
         */
      }
#endif
      if (IShouldWrite())
        *rout << " Row " << lp()->getRowName(i) << " was freed" << endl;
    
      freed[i] = 1;
      Marker[i] = 0;
      LowerBoundY[i] = Ereal<double>::negative_infinity;
      UpperBoundY[i] = Ereal<double>::positive_infinity;
      /* dual tests are not currently used in the preprocessor
       * LowerBoundP[i] = 0;
       * UpperBoundP[i] = 0;
       */
      continue;
    }

      //row_range
      //

#ifdef ACRO_VALIDATING
    if (IShouldWrite() && debugReduceLog)
      *rout << "Row Range:" << endl;
#endif

    //if ((LowerRangeY[i] > UpperBoundY[i]) || (UpperRangeY[i] < LowerBoundY[i])) 
    if (compare(LowerRangeY[i], UpperBoundY[i], 4, 3) || compare(UpperRangeY[i], LowerBoundY[i], 2, 3)) {
      redLogInfeasible('-',695);
      // *rout << LowerRangeY[i] << "," << UpperBoundY[i] << "," << UpperRangeY[i] << "," << LowerBoundY[i] << endl; 
      return 1; //primal infeasible
    }
    //if (LowerRangeY[i] == UpperBoundY[i]) 
    else if (compare(LowerRangeY[i], UpperBoundY[i], 1, 5)) {
#ifdef ACRO_VALIDATING
      if (IShouldWrite() && debugReduceLog) {
        *rout << " LowerRangeY[" << i << "] = UpperBoundY = " << UpperBoundY[i] << endl;
        *rout << " Marker being set to -1" << endl;
      }
#endif
      Marker[i] = -1;
      --infFlag;
      continue;
    }
    //if (UpperRangeY[i] == LowerBoundY[i]) {
    else if (compare(UpperRangeY[i], LowerBoundY[i], 1, 5)) {
#ifdef ACRO_VALIDATING
      if (IShouldWrite() && debugReduceLog) {
        *rout << " UpperRangeY[" << i << "] = LowerBoundY = " << LowerBoundY[i] << endl; 
        *rout << " Marker being set to 1" << endl;
      }
#endif
      Marker[i] =  1;
      --infFlag;
      continue;
    }

/* dual tests are not currently used in the preprocessor
 *    //if (UpperRangeY[i] <= UpperBoundY[i]) 
 *    if (compare(UpperRangeY[i], UpperBoundY[i], 3, 1)) {
 *#ifdef ACRO_VALIDATING
 *      if (IShouldWrite() && debugReduceLog)
 *        *rout << "UpperRangeY <= UpperBoundY" << endl;
 *#endif
 *      //if (UpperBoundP[i] < 0) 
 *      if (compare(UpperBoundP[i], 0, 2, 4)) {
 *        redLogInfeasible('-',708);
 *        return 2; //dual infeasible
 *      }
 *      else if (UpperBoundY[i] != Ereal<double>::positive_infinity || LowerBoundP[i] != 0) {
 *        if (freed[i] == 0) {
 *          if (IShouldWrite()) {
 *            *rout << "Row Range:";
 *            *rout << " Upper Bound of row " << lp()->getRowName(i) << " is never binding" << endl;
 *            *rout << " resetting UpperBoundY = INF and LowerBoundP = 0" << endl;
 *          }
 *          UpperBoundY[i] = Ereal<double>::positive_infinity;
 *          LowerBoundP[i] = 0;
 *          freed[i] = 1;
 *        }
 *      }
 *    }
 *
 *    //if (LowerRangeY[i] >= LowerBoundY[i]) 
 *    if (compare(LowerRangeY[i], LowerBoundY[i], 5, 1)) {
 *#ifdef ACRO_VALIDATING
 *      if (IShouldWrite() && debugReduceLog)
 *      *rout << "LowerRangeY >= LowerBoundY" << endl;
 *#endif
 *      //if (LowerBoundP[i] > 0)
 *      if (compare(LowerBoundP[i], 0, 4, 4)) {
 *        redLogInfeasible('-',718);
 *        return 2; //dual infeasible
 *      }
 *      else if (LowerBoundY[i] != Ereal<double>::negative_infinity || UpperBoundP[i] != 0) {
 *        if (freed[i] == 0) {
 *          if (IShouldWrite()) {
 *            *rout << "Row Range:";
 *            *rout << " Lower Bound of row " << lp()->getRowName(i) << " is never binding" << endl;
 *            *rout << " resetting LowerBoundY = -INF and UpperBoundP = 0" << endl;
 *          }
 *          LowerBoundY[i] = Ereal<double>::negative_infinity;
 *          UpperBoundP[i] = 0;
 *          freed[i] = 1;
 *        }
 *      }
 *    }
 */

  }
  
      //row_degree
      //

#ifdef ACRO_VALIDATING
  if (IShouldWrite() && debugReduceLog)
    *rout << "Row Degree:" << endl;
#endif

  Ereal<double> lhat;
  Ereal<double> uhat;

  for (int j = 0; j < get_ncols(); j++) {
    int rcnt = colmajor.get_matcnt()[j];      
    int* rind =  colmajor.get_matind().data() + colmajor.get_matbeg()[j];
    Ereal<double>* cval = colmajor.get_matval().data() + colmajor.get_matbeg()[j];
    
    for (int i = 0; i < rcnt; i++) {
      if (Degree[rind[i]] == 1) {
#ifdef ACRO_VALIDATING
        if (IShouldWrite() && debugReduceLog)
          *rout << "Row " << rind[i] << "Degree = " << 1 << endl;
#endif
        if (cval[i] > 0) {
          lhat = LowerBoundY[rind[i]] / cval[i];
          uhat = UpperBoundY[rind[i]] / cval[i];
        }
        else {
          lhat = UpperBoundY[rind[i]] / cval[i];
          uhat = LowerBoundY[rind[i]] / cval[i];
        }
        //find if variable j is integer, adjust integrality
        if((vtype[j] == 1) || (vtype[j] == 2)){
          uhat = floor(uhat);
          lhat = ceil(lhat);
        }
        //if (lhat > LowerBound[j]) 
        if (compare(lhat, LowerBound[j], 4, 1)) {
          if (IShouldWrite()) {
            *rout << "Row Degree:";
            *rout << " Column " << lp()->getVariableName(j) << " reduced by row " << lp()->getRowName(rind[i]) << endl;
            *rout << " Increasing LowerBound to " << lhat << endl;
          }
          LowerBound[j] = lhat;
          creduced[j] = 1;
          --infFlag;
        }
        //if (uhat < UpperBound[j]) 
        if (compare(uhat, UpperBound[j], 2, 1)) { 
          if (IShouldWrite()) {
            *rout << "Row Degree:";
            *rout << " Column " << lp()->getVariableName(j) << " reduced by row " << lp()->getRowName(rind[i]) << endl;
            *rout << " Decreasing UpperBound to " << uhat << endl;
          }
          UpperBound[j] = uhat;
          creduced[j] = 1;
          --infFlag;
        }
        //if (LowerBound[j] == UpperBound[j]) 
        if (compare(UpperBound[j], LowerBound[j], 1, 5)) {
#ifdef ACRO_VALIDATING
          if (IShouldWrite() && debugReduceLog)
            *rout << "LowerBound = UpperBound, setting degree = 0" << endl;
#endif
          if (IShouldWrite())
            *rout << " Column " << lp()->getVariableName(j) << " was fixed at " << LowerBound[j] << " causing row " << lp()->getRowName(rind[i]) << " to be freed" << endl;
        
          cfixed[j] = 1;
          freed[rind[i]] = 1;
          Degree[rind[i]] = 0; 
          LowerBoundY[rind[i]] = Ereal<double>::negative_infinity;
          UpperBoundY[rind[i]] = Ereal<double>::positive_infinity;
          /* dual tests are not currently used in the preprocessor
           * LowerBoundP[rind[i]] = 0;
           * UpperBoundP[rind[i]] = 0;
           */
        }
      }
    }
  }
  
  return infFlag;
}

//second preprocessing pass through columns
//
int MILPProblem::preprocess_columns_two() {
  int infFlag = 0;
#ifdef ACRO_VALIDATING
  if (IShouldWrite() && debugReduceLog)
    *rout << "Second Column Loop:" << endl;
#endif

  Ereal<double> lhat;
  Ereal<double> uhat;
     
  for (int j = 0; j < get_ncols(); j++) {
    //if (LowerBound[j] < UpperBound[j]) 
    if (compare(UpperBound[j], LowerBound[j], 4, 1)) {

#ifdef ACRO_VALIDATING
      if (IShouldWrite() && debugReduceLog) {
        *rout << "Variable " << j << endl;
        *rout << " LowerBound < UpperBound" << endl;
      }
#endif

      int rcnt = colmajor.get_matcnt()[j];
      int* rind =  colmajor.get_matind().data() + colmajor.get_matbeg()[j];
      Ereal<double>* rval = colmajor.get_matval().data() + colmajor.get_matbeg()[j];
  
      for (int i = 0; i < rcnt; i++) {
#ifdef ACRO_VALIDATING
        if (IShouldWrite() && debugReduceLog)
          *rout << "Row " << rind[i] << endl;
#endif 
        if (rval[i] > 0) {
      
          lhat = (LowerBoundY[rind[i]] - UpperRangeY[rind[i]]) / rval[i] + UpperBound[j];
          if (isind(lhat))
            lhat =  Ereal<double>::negative_infinity;
      
          uhat = (UpperBoundY[rind[i]] - LowerRangeY[rind[i]]) / rval[i] + LowerBound[j];
          if (isind(uhat))
            uhat =  Ereal<double>::positive_infinity;
        } else {
          lhat = (UpperBoundY[rind[i]] - LowerRangeY[rind[i]]) / rval[i] + UpperBound[j];
          if (isind(lhat))
            lhat =  Ereal<double>::negative_infinity;
      
          uhat = (LowerBoundY[rind[i]] - UpperRangeY[rind[i]]) / rval[i] + LowerBound[j];
          if (isind(uhat))
            uhat =  Ereal<double>::positive_infinity;
        }
        //find if variable j is integer, adjust integrality
        if((vtype[j] == 1) || (vtype[j] == 2)) {
          uhat = floor(uhat);
          lhat = ceil(lhat);
        }
#ifdef ACRO_VALIDATING
        if (IShouldWrite() && debugReduceLog) {
          *rout << "lhat=" << lhat << endl;
          *rout << "uhat=" << uhat << endl;
        }
#endif
        //if (lhat > LowerBound[j]) 
        if (compare(lhat, LowerBound[j], 4, 1)) {
          if (IShouldWrite())
            *rout << "Second Column Loop:";
#ifdef ACRO_VALIDATING
          if (IShouldWrite() && debugReduceLog) {
            *rout << endl;
            *rout << "lhat=" << lhat << ">" << "LowerBound[" << j << "]=" << LowerBound[j] << endl;
          }
#endif
          if (IShouldWrite()) {
            *rout << " Variable " << lp()->getVariableName(j) << " ";
            *rout << " Increasing LowerBound to " << lhat << endl;
          }
          LowerBound[j] = lhat;
          creduced[j] = 1;
          --infFlag;
        }
        //if (uhat < UpperBound[j]) 
        if (compare(uhat, UpperBound[j], 2, 1)) {
          if (IShouldWrite())
            *rout << "Second Column Loop:";
#ifdef ACRO_VALIDATING
          if (IShouldWrite() && debugReduceLog) {
            *rout << endl;
            *rout << "uhat=" << uhat << "<" << "UpperBound[" << j << "]=" << UpperBound[j] << endl;
          }
#endif
          if (IShouldWrite()) {
            *rout << " Variable " << lp()->getVariableName(j) << " ";
            *rout << " Decreasing UpperBound to " << uhat << endl;
          }
          UpperBound[j] = uhat;
          creduced[j] = 1;
          --infFlag;
        }
      }
      //if (LowerBound[j] == UpperBound[j]) 
      if (compare(LowerBound[j], UpperBound[j], 1, 5)) {
        for (int i = 0; i < rcnt; i++) {
#ifdef ACRO_VALIDATING
          if (IShouldWrite() && debugReduceLog)
            *rout << "LowerBound = UpperBound, decrementing degree" << endl;
#endif 
          Degree[rind[i]] -= 1;
        }
        cfixed[j] = 1;
      }
    }
  }
  return infFlag;
}

//  compare - a tolerance function used in preprocessor when comparing two values
//   returns true or false depending on comparison within a tolerance gap  
//  number is what is being compared
//  referent is the value that the number is being compared to
//  Valid compareTypes:
//   1- equivalence (e.g. ==)
//   2- less than (e.g. <)
//   3- less than or equal to (e.g. <=)
//   4- greater than (e.g. >)
//   5- greater than or equal to (e.g. >=)
//  Decision is the purpose of the comparison
//  Valid decision types include:
//   1- primal comparison 
//   2- dual comparison 
//   3- primal infeasibility
//   4- dual infeasibility
//   5- fix value
//   6- coefficient nonzero
//   7- alpha nonzero
//   8- pivot acceptable

bool MILPProblem::compare(Ereal<double> number, Ereal<double> referent, int compareType, int decision) {
  if (finite(number) && finite(referent)) {
   
    Ereal<double> tolRelative = 1.0e-15;
    Ereal<double> tolAbsolute = 1.0e-15;

    Ereal<double> diff = 0;
    Ereal<double> tol = 0;

    switch (decision) {
      case 1: tolAbsolute = 1.0e-6;   //primal comparison
              tolRelative = 1.0e-3;
              break;
      case 2: tolAbsolute = 1.0e-6;   //dual comparison
              tolRelative = 1.0e-3;
              break;
      case 3: tolAbsolute = 1.0e-5;   //primal infeasibility
              tolRelative = 1.0e-2;
              break;
      case 4: tolAbsolute = 1.0e-5;   //dual infeasibility
              tolRelative = 1.0e-2;
              //tolRelative = 0;
              break;
      case 5: tolAbsolute = 1.0e-6;   //fix value
              tolRelative = 1.0e-2;
              break;
      case 6: tolAbsolute = 1.0e-10;  //coefficient nonzero
              tolRelative = 0;
              break;
      case 7: tolAbsolute = 1.0e-14;  //alpha nonzero
              tolRelative = 0;
              break;
      case 8: tolAbsolute = 1.0e-12;  //pivot acceptable
              tolRelative = 0;
    }

    tol = tolRelative*fabs(referent) + tolAbsolute;
    
    diff = number - referent;
    switch (compareType) {
      case 1: //equivalence
              if (fabs(diff) <= tol) return true; // |number - referent| <= tol
              else return false;
      case 2: //less than
              if (diff < tol) return true; // number < referent + tol
              else return false;
      case 3: //less than or equal
              if (diff <= tol) return true; // number <= referent + tol
              else return false;
      case 4: //greater than
              if (diff > -tol) return true; // number > referent - tol
              else return false;
      case 5: //greater than or equal
              if (diff >= -tol) return true; // number >= referent - tol
              else return false;
    }
  } else {
    //handle cases where number and/or referent are not finite
    switch (compareType) {
      case 1:
              if (number == referent) return true;
              else return false;
      case 2:
              if (number < referent) return true;
              else return false;
      case 3:
              if (number <= referent) return true;
              else return false;
      case 4:
              if (number > referent) return true;
              else return false;
      case 5:
              if (number >= referent) return true;
              else return false;
    }
  }

//this should do something besides returning false.  Theoretically, the code
//should never get here, but if it does, it shouldn't return an acceptable
//value
return false;
}


// You might think redLogInit() would create a new output stream, but you'd be
// wrong.  Not clear why, but it's done here instead.
// redLogFile() and redLogInit() could be combined.  It's not clear why they are
// separate.
ostream* MILPProblem::redLogFile()
{
  if (reduceLog)
    return new ofstream("red.log",ios::out);
  else
    return 0;
}

// Must call redLogFile() before calling redLogInit().
// redLogFile() and redLogInit() could be combined.  It's not clear why they are
// separate.
void MILPProblem::redLogInit()
{
  if (!rout)
    return;
  rout->setf(ios::scientific,ios::floatfield);  // Output bounds at very
  rout->precision(20);                          // high precision.
}

void MILPProblem::redLogInfeasible(char separator, int line)
{
  if (IShouldWrite())
	*rout << "Infeasible" << separator << "line " << line << endl;
}


/******** End Inferential Reduction code ***************/




/*
int MILPProblem::preprocess( void )
{
#ifdef ACRO_HAVE_MPI
if (!uMPI::iDoIO)
   CommonIO::setIORank(-2);
#endif

lp()->getInfo(objx,colmajor,senx,LowerBound,UpperBound,lhsx, rhsx);

DEBUGPR(1,
	ucout << "BEGINING PREPROCESSING" << endl;
        ucout << " Summary" << endl;
        ucout << "  nrows=  " << get_nrows() << endl;
        ucout << "  ncols=  " << get_ncols() << endl;
        ucout << "  nnzero= " << get_nnzero() << endl;
        ucout << endl)
DEBUGPR(10,
  rowmajor.convert( colmajor );
  classify_constraints(ucout);
  )
row_map.resize(colmajor.get_nrows());
{for (size_type i=0; i<row_map.size(); i++) row_map[i] = i;}
col_map.resize(colmajor.get_ncols());
{for (size_type i=0; i<col_map.size(); i++) col_map[i] = i;}


//#define DEBUG_PREPROCESS TRUE

#if DEBUG_PREPROCESS
  {for (size_type i=0; i<UpperBound.size(); i++)
    ucout << i << "\t" << LowerBound[i] << " " << UpperBound[i] << endl;}
  {for (size_type i=0; i<rhsx.size(); i++)
    ucout << i << "\t" << senx[i] << " " << rhsx[i] << endl;}
  ucout << "-----------------------------------------------------------" << endl;
  ucout << "Original:" << endl;
  ucout << "Column major:" << endl;
  ucout << colmajor << endl;
  ucout << "-------------" << endl;
  ucout << "Row major:" << endl;
  ucout << rowmajor << endl;
  ucout << "Original problem ends above." << endl;
  ucout << "-----------------------------------------------------------" << endl;
#endif

DEBUGPR(2, ucout << " Preprocessing Columns" << endl;)
preprocess_cols();
delete_rows();
rowmajor.convert( colmajor );

// The bounds have been modified; #constraints
// may also have been reduced;

#if DEBUG_PREPROCESS
  ucout << "-----------------------------------------------------------" << endl;
  ucout << "After process cols:" << endl;
  ucout << "Column major:" << endl;
  ucout << colmajor << endl;
  ucout << "-------------" << endl;
  ucout << "Row major:" << endl;
  ucout << rowmajor << endl;
  ucout << "Problem after process cols ends above." << endl;
  ucout << "-----------------------------------------------------------" << endl;
#endif
  
DEBUGPR(2, ucout << " Preprocessing Rows" << endl;)
preprocess_rows();
colmajor.convert( rowmajor );
delete_rows();

#if DEBUG_PREPROCESS
  ucout << "-----------------------------------------------------------" << endl;
  ucout << "After process rows:" << endl;
  ucout << "Column major:" << endl;
  ucout << colmajor << endl;
  ucout << "-------------" << endl;
  ucout << "Row major:" << endl;
  ucout << rowmajor << endl;
  ucout << "Problem after process rows ends above." << endl;
  ucout << "-----------------------------------------------------------" << endl;
#endif

DEBUGPR(2, ucout << " Preprocessing Columns" << endl;)
preprocess_cols();
delete_rows();
*/
/* WEH - it isn't clear how this is being used, so I'm commenting it out

    // Put the code to fill in GUBS, VLB, and VUB Here 
    // (GUBS currently not done)

UpperBound.resize(get_nvars());
LowerBound.resize(get_nvars());
UpperBound = ERR;
LowerBound = ERR;

for (int i=0; i<get_nrows(); i++) {
  int rbeg           = rowmajor.get_matbeg()[i];
  const int* rind    = rowmajor.get_matind().data() + rbeg;
  const double* rval = rowmajor.get_matval().data() + rbeg;
  int xi, yi;
  double xv,yv;
	
  switch (ctype[i]) {

	case ROW_TYPE_VARUB:
	    if (vtype[rind[0]] == binary) {
	       xi = rind[0];
	       xv = rval[0];
	       yi = rind[1];
	       yv = rval[1];
	       }
	    else {
	       yi = rind[0];
	       yv = rval[0];
	       xi = rind[1];
	       xv = rval[1];
	       }
	    vub[yi].set_var ( xi );
	    vub[yi].set_val ( -xv/yv );	  
	    break;

	case ROW_TYPE_VAREQ:
	    if (vtype[rind[0]] == binary) {
	       xi = rind[0];
	       xv = rval[0];
	       yi = rind[1];
	       yv = rval[1];
	       }
	    else {
	       yi = rind[0];
	       yv = rval[0];
	       xi = rind[1];
	       xv = rval[1];
	       }
	    vlb[yi].set_var( xi );
	    vub[yi].set_var( xi );
	    vlb[yi].set_val( -xv/yv );	   
	    vub[yi].set_val( -xv/yv );	   	    
	    break;

	case ROW_TYPE_VARLB:
	    if (vtype[rind[0]] == binary) {
	       xi = rind[0];
	       xv = rval[0];
	       yi = rind[1];
	       yv = rval[1];
	       }
	    else {
	       yi = rind[0];
	       yv = rval[0];
	       xi = rind[1];
	       xv = rval[1];
	       }
	    vlb[yi].set_var( xi );
	    vlb[yi].set_val (-xv/yv );
	    break;

	default:
	    break;
        }
  }

rowmajor.convert( colmajor );
 
#if DEBUG_PREPROCESS
  ucout << "-----------------------------------------------------------" << endl;
  ucout << "Final:" << endl;
  ucout << "Column major:" << endl;
  ucout << colmajor << endl;
  ucout << "-------------" << endl;
  ucout << "Row major:" << endl;
  ucout << rowmajor << endl;
  ucout << "Final problem after all processing ends above." << endl;
  ucout << "-----------------------------------------------------------" << endl;
#endif

DEBUGPR(1,
	ucout << "FINISHED PREPROCESSING" << endl;
        ucout << " Summary" << endl;
        ucout << "  nrows=  " << get_nrows() << endl;
        ucout << "  ncols=  " << get_ncols() << endl;
        ucout << "  nnzero= " << get_nnzero() << endl;
        ucout << endl)

DEBUGPR(10,
  classify_constraints(ucout);
  )

//
// Finish up by reinitializing the LP class with the new/reduced sparse matrix
//
BasicArray<CharString> variableNames(get_ncols());
{for (int i=0; i<get_ncols(); i++)
  variableNames[i] = lp()->getVariableName(i);}
BasicArray<CharString> constraintNames(get_nrows());
{for (int j=0; j<get_nrows(); j++)
  constraintNames[j] = lp()->getRowName(j);}

string name(lp()->name());

abort();
*/
/** TODO
lp()->loadProblem(	colmajor,
		name, 
		(int) (lp()->getObjSense()),
		objx,
		LowerBound,
		UpperBound,
		variableNames,
		senx,
		rhsx,
		constraintNames);

return OK;

}
*/


/***** NOT CURRENTLY USED 
//
// flip a constraint so it is a <= constraint
//
//
static void Flip(int nzcnt, int *rind, double *rval, char& sense, double& rhs)
{
  register int j;

  rhs = -rhs;
  sense = (sense == 'L') ? 'G' : 'L';
  for (j = 0; j < nzcnt; j++)
    rval[j] = -rval[j];
}


//
// classify_one_constraint - classifies a linear constraint
//	INPUT
//	 nzcnt		- # nonzeros in the constraint
//	 rind		- array of indeces for the nonzeroes
//	 rval		- list of values for the nonzeroes
//	 sense		- the sense of the constraint
//	 rhs		- the value of the RHS
//	 vtype		- the variable types
//	RETURN
//	 		- the constraint group id
//
//
static constraintGroup classify_one_constraint(int nzcnt, 
		int* rind, double* rval, 
		char sense, double rhs, 
		const BasicArray<variableType>& vtype)
{
register int bpcnt,   // binary positive count
             bncnt,   // binary negative count
             ipcnt,   // integer positive count
             incnt,   // integer negative count
             cpcnt,   // continuous positive count
             cncnt,   // continuous negative count
             bcnt,    // binary count
             pcnt,    // positive count
             ncnt,    // negative count
             i, j;
constraintGroup rowclass = ROW_TYPE_UNCLASSIFIED;

#ifndef FALSE
#define FALSE 0
#endif

if (nzcnt == 0)
  return (ROW_TYPE_UNCLASSIFIED);

//
// Put >= constraint in <= form
//
unsigned flip = FALSE;
if (sense == 'G') {
   Flip(nzcnt, rind, rval, sense, rhs);
   flip = TRUE;
   }

//
// We replace the set sign function by the lines of code that will in rsign.
//  SetSign (nzcnt, rind, rval, sense, rhs);
//
    
//
// INITIAL COUNTER VALUES FOR THE CURRENT ROW
//
                 
    
//
// COUNT THE ENTRIES IN THE CURRENT ROW
//
                 
bpcnt = bncnt = ipcnt = incnt = cpcnt = cncnt = 0;
for (j = 0; j < nzcnt; j++) 
  {
  if (epsilon_negative(rval[j])) {		// Count negative variables
     switch (vtype[rind[j]]) {
	case binary:
	    bncnt++;
	    break;
	case generalInteger:
	    incnt++;
	    break;
	case continuous:
	    cncnt++;
	    break;
	default:
	    break;
	}
     }
  else {
     switch (vtype[rind[j]]) {		// Count positive variables
	case binary:
	    bpcnt++;
	    break;
	case generalInteger:
	    ipcnt++;
	    break;
	case continuous:
	    cpcnt++;
	    break;
	default:
	    break;
	}
     }
  }
    
bcnt = bncnt + bpcnt;
//icnt = incnt + ipcnt;			Not used right now
//ccnt = cncnt + cpcnt;			Not used right now
pcnt = cpcnt + ipcnt + bpcnt;
ncnt = cncnt + incnt + bncnt;


//
// CLASSIFY THE CONSTRAINT
//
     
//
//   All variables are binary
//
if ((rowclass == ROW_TYPE_UNCLASSIFIED) && (bcnt == nzcnt)) {

   //
   // Find out whether the inequality has nonzero coefficients
   // that are all equal to either -1 or 1.
   //
   // If not, then i >= 0 equals a variable that has a non-1 or 1 coeff.
   //
   for (i = -1, j = 0; j < nzcnt; j++) {
     if (! epsilon_equal(rval[j],1) && ! epsilon_equal(rval[j],-1)) {
	if (i < 0) i = j;
	else  break;
	}
     }

   if (j == nzcnt) {
      if (i < 0) {
	 if (epsilon_equal(rhs, -bncnt + 1)) {
	    if (sense == 'L')
	       rowclass = ROW_TYPE_BINSUM1UB;
	    else
	       rowclass = ROW_TYPE_BINSUM1EQ;
	    }
         else if (epsilon_equal(rhs, bpcnt - 1)) {
	    if (sense == 'L')
	       rowclass = ROW_TYPE_BINSUM1LB;
	    else
	       rowclass = ROW_TYPE_BINSUM1EQ;
            }
         else {
	    if (sense == 'L')
	       rowclass = ROW_TYPE_BINSUM1UBK;
	    else
	       rowclass = ROW_TYPE_BINSUM1EQK;
	    }
         }
      else {
         //
         // i is negative and RHS equals -bncnt
         //
         if (epsilon_negative(rval[i]) && epsilon_equal(rhs,-bncnt)) {
	    if (sense == 'L')
	       rowclass = ROW_TYPE_BINSUM1VARUB;
	    else
	       rowclass = ROW_TYPE_BINSUM1VAREQ;
	    }
         //
         // General binary variable constraint
         //
         else {
	    if (sense == 'L')
	       rowclass = ROW_TYPE_ALLBINUB;
	    else
	       rowclass = ROW_TYPE_ALLBINEQ;
	    }
         }
      }
   else {
      double sum;
      for (sum = 0.0, j = 0; j < nzcnt; j++) {
        if (epsilon_negative(rval[j]))
	   sum  += rval[j];
        }
      for (j = 0; j < nzcnt; j++) {
        if (epsilon_negative(rval[j])) {
	   if (epsilon_equal(rhs, sum - rval[j])) {
	      if (sense == 'L')
	         rowclass = ROW_TYPE_BINSUMVARUB;
	      else
	         rowclass = ROW_TYPE_BINSUMVAREQ;
	      break;
	      }
	   }
        }

      if (j == nzcnt) {
         if (sense == 'L')
	    rowclass = ROW_TYPE_ALLBINUB;
         else
	    rowclass = ROW_TYPE_ALLBINEQ;
         }
      }
   }
    
//
//   None of the variables is binary
//

if ((rowclass == ROW_TYPE_UNCLASSIFIED) && (bcnt == 0)) {
   if (sense == 'L')
      rowclass = ROW_TYPE_NOBINUB;
   else
      rowclass = ROW_TYPE_NOBINEQ;
   }

//
//   Binary as well as integer or continuous variables
//
     
if (rowclass == ROW_TYPE_UNCLASSIFIED) {
   if (epsilon_negative(rhs) || epsilon_positive(rhs) || (bcnt != 1)) {
      if (sense == 'L')
         rowclass = ROW_TYPE_MIXUB;
      else
         rowclass = ROW_TYPE_MIXEQ;
      }
   else {
      if (nzcnt == 2) {
         if (sense == 'L') {
            if ((ncnt == 1) && (bncnt == 1))
               rowclass = ROW_TYPE_VARUB;
            if ((pcnt == 1) && (bpcnt == 1))
               rowclass = ROW_TYPE_VARLB;
            }
         else
            rowclass = ROW_TYPE_VAREQ;
         }
      else {
         if ((ncnt == 1) && (bncnt == 1)) {
            if (sense == 'L')
               rowclass = ROW_TYPE_SUMVARUB;
            else
               rowclass = ROW_TYPE_SUMVAREQ;
            }
         }
      }
   }
    
if (rowclass == ROW_TYPE_UNCLASSIFIED) {
   if (sense == 'L')
      rowclass = ROW_TYPE_MIXUB;
   else
      rowclass = ROW_TYPE_MIXUB;
   }
    
//
// Flip the sense of the row back
//
if (flip)
   Flip(nzcnt, rind, rval, sense, rhs);

return (rowclass);
}
****/


//
// classify_constraints - classifies all constraints in a MILPProblem
//
//
void MILPProblem::classify_constraints(ostream& os)
{ 
ctype.resize(get_nrows());

int cnt_bs1u  = 0;
int cnt_bs1e  = 0;
int cnt_bs1l  = 0;
int cnt_bs1uk = 0;
int cnt_bs1ek = 0;
int cnt_bs1vu = 0;
int cnt_bs1ve = 0;
int cnt_bsvu  = 0;
int cnt_bsve  = 0;
int cnt_svu   = 0;
int cnt_sve   = 0;
int cnt_vu    = 0;
int cnt_ve    = 0;
int cnt_vl    = 0;
int cnt_mixu  = 0;
int cnt_mixe  = 0;
int cnt_nbu   = 0;
int cnt_nbe   = 0;
int cnt_abu   = 0;
int cnt_abe   = 0;
  
//
// Variables for classification counters MIPLIB
//
 
#ifdef DEBUG_CLASSIFY
int cnt_g;
int cnt_k;
int cnt_e;
int cnt_f;
int cnt_i;
int cnt_p;
int cnt_c;
int cnt_s;
int cnt_u;
int cnt_l;
#endif

  
    
/*
register int i;
for ( i = 0; i < get_nrows(); i++) {
  int* matind = rowmajor.get_matind().data() + rowmajor.get_matbeg()[i];
  Ereal<double>* matval = rowmajor.get_matval().data() + rowmajor.get_matbeg()[i];
  switch (ctype[i] = classify_one_constraint( rowmajor.get_matcnt()[i], 
			      matind,
			      matval,
			      senx[i], 
			      rhsx[i], 
			      vtype) ) {
	  
	case ROW_TYPE_BINSUM1UB:
          cnt_bs1u++;
	  ctype[i] = ROW_TYPE_BINSUM1UB;

#if 0
   //Adding a GUB HERE.  Assume GUBTABLE exists.  Will allocate memory
   //for GUBINFO
	  GUBINFO *newgub = NEW(GUBINFO);
	  newgub->nzcnt = rowcnt[rowind[i]];
	  newgub->ind = NEWV(int, newgub->nzcnt);
	  newgub->val = NEWV(int, newgub->nzcnt);
	  for(j = 0; j < newgub->nzcnt ; j++)
	    {
	      newgub->ind[j] = rowind[rowbeg[i]+j];
   //Store the GUBS always in <= FORM
	      newgub->val[j] = (senx[i] != 'G') ? 
		(int) rowval[rowbeg[i] + j] :
	       -(int) rowval[rowbeg[i] + j];
	    }
	  newgub->sense = (senx[i] != 'G') ? senx[i] : 'L';
	  newgub->rhs = (int) rhsx[i];
	  newgub->lpix = i;

	  gubtable.gub[(gubtable.cnt)++] = newgub;

#endif
          break;

        case ROW_TYPE_BINSUM1EQ:
          cnt_bs1e++;
	  ctype[i] = ROW_TYPE_BINSUM1EQ;
#if 0
	  AddGub(p, i);
   //Allocate Memory YourSelf.  Not worrying about GUBS now.
#endif
          break;

        case ROW_TYPE_BINSUM1LB:
          cnt_bs1l++;
	  ctype[i] = ROW_TYPE_BINSUM1LB;
          break;

        case ROW_TYPE_BINSUM1UBK:
          cnt_bs1uk++;
	  ctype[i] = ROW_TYPE_BINSUM1UBK;
          break;

        case ROW_TYPE_BINSUM1EQK:
          cnt_bs1ek++;
	  ctype[i] = ROW_TYPE_BINSUM1EQK;
          break;

        case ROW_TYPE_BINSUM1VARUB:
          cnt_bs1vu++;
	  ctype[i] = ROW_TYPE_BINSUM1VARUB;
          break;
	  
        case ROW_TYPE_BINSUM1VAREQ:
          cnt_bs1ve++;
	  ctype[i] = ROW_TYPE_BINSUM1VAREQ;
          break;

        case ROW_TYPE_BINSUMVARUB:
          cnt_bsvu++;
	  ctype[i] = ROW_TYPE_BINSUMVARUB;
          break;

        case ROW_TYPE_BINSUMVAREQ:
          cnt_bsve++;
	  ctype[i] = ROW_TYPE_BINSUMVAREQ;
          break;

        case ROW_TYPE_SUMVARUB:
          cnt_svu++;
	  ctype[i] = ROW_TYPE_SUMVARUB;
          break;

        case ROW_TYPE_SUMVAREQ:
          cnt_sve++;
	  ctype[i] = ROW_TYPE_SUMVAREQ;
          break;

        case ROW_TYPE_VARUB:
          cnt_vu++;
	  ctype[i] = ROW_TYPE_VARUB;
          break;

        case ROW_TYPE_VAREQ:
          cnt_ve++;
	  ctype[i] = ROW_TYPE_VAREQ;
          break;

        case ROW_TYPE_VARLB:
          cnt_vl++;
	  ctype[i] = ROW_TYPE_VARLB;
          break;

        case ROW_TYPE_MIXUB:
          cnt_mixu++;
	  ctype[i] = ROW_TYPE_MIXUB;
          break;

        case ROW_TYPE_MIXEQ:
          cnt_mixe++;
	  ctype[i] = ROW_TYPE_MIXEQ;
          break;

        case ROW_TYPE_NOBINUB:
          cnt_nbu++;
	  ctype[i] = ROW_TYPE_NOBINUB;
          break;

        case ROW_TYPE_NOBINEQ:
          cnt_nbe++;
	  ctype[i] = ROW_TYPE_NOBINEQ;
          break;

        case ROW_TYPE_ALLBINUB:
          cnt_abu++;
	  ctype[i] = ROW_TYPE_ALLBINUB;
          break;

        case ROW_TYPE_ALLBINEQ:
          cnt_abe++;
	  ctype[i] = ROW_TYPE_ALLBINEQ;
          break;

	default:
	  break;
        }
    }
*/


#ifdef DEBUG_CLASSIFY
  cnt_g = cnt_mixu + cnt_mixe + cnt_nbu + cnt_nbe + cnt_svu + cnt_sve;
  cnt_k = cnt_abu;
  cnt_e = cnt_abe;
  cnt_f = cnt_bsvu + cnt_bsve + cnt_bs1vu + cnt_bs1ve;
  cnt_i = cnt_bs1uk + cnt_bs1ek;
  cnt_p = cnt_bs1u;
  cnt_c = cnt_bs1l;
  cnt_s = cnt_bs1e;
  cnt_u = cnt_vu + cnt_ve;
  cnt_l = cnt_vl;
#endif

// 
// Here you can do whatever logging you wish.  Just print for now...
//


#define PRINT_ROW_CLASS 1
 
#if PRINT_ROW_CLASS
  os << "Number of constraints: " << get_nrows() << endl;
   
   if (cnt_mixu  > 0) os << "    Number of constraints of type G (MIXUB):        " <<  cnt_mixu << endl;
   if (cnt_mixe  > 0) os << "    Number of constraints of type G (MIXEQ):        " <<  cnt_mixe << endl;
   if (cnt_nbu   > 0) os << "    Number of constraints of type G (NOBINUB):      " <<  cnt_nbu << endl;
   if (cnt_nbe   > 0) os << "    Number of constraints of type G (NOBINEQ):      " <<  cnt_nbe << endl;
   if (cnt_svu   > 0) os << "    Number of constraints of type G (SUMVARUB):     " <<  cnt_svu << endl;
   if (cnt_sve   > 0) os << "    Number of constraints of type G (SUMVAREQ):     " <<  cnt_sve << endl;
   if (cnt_abu   > 0) os << "    Number of constraints of type K (ALLBINUB):     " <<  cnt_abu << endl;
   if (cnt_abe   > 0) os << "    Number of constraints of type E (ALLBINEQ):     " <<  cnt_abe << endl;
   if (cnt_vu    > 0) os << "    Number of constraints of type U (VARUB):        " <<  cnt_vu << endl;
   if (cnt_ve    > 0) os << "    Number of constraints of type U (VAREQ):        " <<  cnt_ve << endl;
   if (cnt_vl    > 0) os << "    Number of constraints of type L (VARLB):        " <<  cnt_vl << endl;
   if (cnt_bsvu  > 0) os << "    Number of constraints of type F (BINSUMVARUB):  " <<  cnt_bsvu << endl;
   if (cnt_bsve  > 0) os << "    Number of constraints of type F (BINSUMVAREQ):  " <<  cnt_bsve << endl;
   if (cnt_bs1vu > 0) os << "    Number of constraints of type F (BINSUM1VARUB): " <<  cnt_bs1vu << endl;
   if (cnt_bs1ve > 0) os << "    Number of constraints of type F (BINSUM1VAREQ): " <<  cnt_bs1ve << endl;
   if (cnt_bs1u  > 0) os << "    Number of constraints of type P (BINSUM1UB):    " <<  cnt_bs1u << endl;
   if (cnt_bs1e  > 0) os << "    Number of constraints of type S (BINSUM1EQ):    " <<  cnt_bs1e << endl;
   if (cnt_bs1l  > 0) os << "    Number of constraints of type C (BINSUM1LB):    " <<  cnt_bs1l << endl;
   if (cnt_bs1uk > 0) os << "    Number of constraints of type I (BINSUM1UBK):   " <<  cnt_bs1uk << endl;
   if (cnt_bs1ek > 0) os << "    Number of constraints of type I (BINSUM1EQK):   " <<  cnt_bs1ek << endl;
#endif

#define DEBUG_CLASSIFY 0
  
#if DEBUG_CLASSIFY
   if (cnt_g > 0) os << "    Number of constraints of type G: " << cnt_g << endl;
   if (cnt_k > 0) os << "    Number of constraints of type K: " << cnt_k << endl;
   if (cnt_e > 0) os << "    Number of constraints of type E: " << cnt_e << endl;
   if (cnt_f > 0) os << "    Number of constraints of type F: " << cnt_f << endl;
   if (cnt_i > 0) os << "    Number of constraints of type I: " << cnt_i << endl;
   if (cnt_p > 0) os << "    Number of constraints of type P: " << cnt_p << endl;
   if (cnt_c > 0) os << "    Number of constraints of type C: " << cnt_c << endl;
   if (cnt_s > 0) os << "    Number of constraints of type S: " << cnt_s << endl;
   if (cnt_u > 0) os << "    Number of constraints of type U: " << cnt_u << endl;
   if (cnt_l > 0) os << "    Number of constraints of type L: " << cnt_l << endl;
  os << "-----------------------------------------------------------" << endl;
#endif

return;
}



/**** NOT CURRENTLY USED
//
// process_one_column
//	INPUT
//	 cobj - coeff for this variable in the objective function
//	 ccnt - # nonzero coeffs in the column
//	 cind - index of nonzero coeffs
//	 cval - value of nonzero coeffs
//	 senx - sense of the constraints in the column
//	RETURN
//	      - flag indicating whether or not the column can be fixed
//		at an upper or lower bound
//
// WEH - I changed the sense of the two main if statements here.  It seems that
// Jeff was assuming that he had maximization problems.
//
static CColStat process_one_col(double cobj, int ccnt, const int* cind,
			  const double* cval, CharString& senx)
{
//
// cobj is nonpositive
//
if (cobj < EPSILON) {
   int i;
   for (i=0; i<ccnt; i++) {
     if ( (cval[ i ] > EPSILON  && senx[ cind[i] ] == 'G') ||
	  (cval[ i ] < -EPSILON && senx[ cind[i] ] == 'L') ) {
	/// Do nothing
	;
        }
     else {
	break;
        }
     }
      
   if (i == ccnt) return COL_FIX_AT_UB;
   }

//
// cobj is nonnegative
//
if (cobj > -EPSILON) {
   int i;
   for (i=0; i<ccnt; i++ ) {
     if ( (cval[ i ] > EPSILON  && senx[ cind[i] ] == 'L') ||
	  (cval[ i ] < -EPSILON && senx[ cind[i] ] == 'G') ) {
	/// Do nothing
	;
        }
     else {
	break;
        }
     }
      
   if(i == ccnt) return COL_FIX_AT_LB;
   }
  
return COL_OK;
}
****/



/*
//
// preprocess_cols - updates the LowerBound and UpperBound arrays to 
//			fix columns
//
//
void MILPProblem::preprocess_cols()
{
row_redundant.resize(get_nrows());
row_redundant.reset();

for (int j=0; j<get_ncols(); j++) {
  CColStat colstat = COL_OK;
  if (UpperBound[j] != LowerBound[j])
     process_one_col( 
		objx[j], 
		colmajor.get_matcnt()[j],
		colmajor.get_matind().data() + colmajor.get_matbeg()[ j ],
		colmajor.get_matval().data() + colmajor.get_matbeg()[ j ],
		senx );

  switch( colstat ) {

	case COL_FIX_AT_LB:
	    DEBUGPR(10, ucout << "  Preprocess: fixing variable " << j
		  << " at lower bound " << LowerBound[ j ] << endl;)
	    UpperBound[j] = LowerBound[j];
	    if (UpperBound[ j ] < -INFINITY) {
	       DEBUGPR(10, ucout << "  Preprocess: Fixing variable " << j
		      << " at -INFINITY [optimality]!!!" << endl;)
	       for (int i=0; i<colmajor.get_matcnt()[ j ]; i++ ) {
		 int row = colmajor.get_matind()[ colmajor.get_matbeg()[j]+i ];
		 DEBUGPR(10, ucout << 
			"  Preprocess: Deleting redundant constraint "
			 << row << endl;)
		 row_redundant.set(row);
		 }
	       }
	    break;

	case COL_FIX_AT_UB:
	    DEBUGPR(10, ucout << "  Preprocess: fixing variable " << j
		  << " at upper bound " << UpperBound[j] << endl;)
	    LowerBound[j] = UpperBound[j];
	    if (UpperBound[ j ] > INFINITY) {
	       DEBUGPR(10, ucout << "  Preprocess: Fixing variable " << j
		      << " at INFINITY [optimality]!!!" << endl;)
	       for (int i=0; i<colmajor.get_matcnt()[ j ]; i++ ) {
		 int row = colmajor.get_matind()[ colmajor.get_matbeg()[j]+i ];
		 DEBUGPR(10, ucout << 
			"  Preprocess: Deleting redundant constraint "
			<< row << endl;)
		 row_redundant.set(row);
		 }
	       }

	case COL_OK:
	    break;

	default:
	    cerr << "  Preprocess: Unknown col stat " << (int)colstat << endl;
	    break;
	}
  }
}
*/

void MILPProblem::delete_rows()
{
size_type nrows = row_redundant.size();
for (int i=row_redundant.size()-1; i>=0; i--) {
  if (row_redundant(i)) {
     colmajor.delete_row(i);
     for (size_type j=i; j<nrows-1; j++) {
       senx[j] = senx[j+1];
       rhsx[j] = rhsx[j+1];
       }
     int ndx=row_map.size()-1;
     while (ndx > i) {
       if (row_map[ndx] != -1) {
          row_map[ndx]--;
          }
       ndx--;
       }
     row_map[i] = -1;
     }
  }
senx.resize(colmajor.get_nrows());
rhsx.resize(colmajor.get_nrows());
}




#define INFEASIBLEROW ROW_INFEASIBLE
#define REDUNDANTROW  ROW_REDUNDANT
#define MODIFIEDROW   ROW_MODIFIED

typedef int BOOLEAN;

//static double rowslack;

#define ISINT(x) INTEGRAL(x)



/**** NOT CURRENTLY USED

static void AddImplication(int  , // index
			   char , // sense
			   double ) // val
{
// Do Nothing...
}




#define SetBound( i, s, v ) SETBOUND( LowerBound, UpperBound, (i), (s), (v) )
static void SETBOUND(DoubleVector& LowerBound, DoubleVector& UpperBound,
		      int index, char sense, double val )
{
if (sense == 'L')
   LowerBound[index] = val;
else if (sense == 'U')
   UpperBound[index] = val;
else
   cerr << "SetBound(): Unknown sense \"" << sense << "\"" << endl;
}


static void Flip( int n, const int rind[], double rval[],
		  char& sense, double& rhs )
{
rhs *= -1;
for( int i = 0; i < n; i++ ) rval[i] *= -1;
sense = (sense == 'L') ? 'G' : 'L';
}
***/

/* BDP - This is only used by the old preprocessor
CRowStat MILPProblem::process_one_constraint( int& rcnt, int* rind, 
			double* rval,
			char rsen, double& rrhs, 
			DoubleVector& LowerBound, DoubleVector& UpperBound,
			BOOLEAN root, BOOLEAN probe,
			int* ix, double* val,
			BasicArray<variableType>& vtype)
{
  CRowStat rowstat = ROW_OK;

  int j;
  double diff, deltamin, deltamax;
  int nzcnt;
  double rhs, minlhs, maxlhs, lbvalue, ubvalue, frac;
  char sense;
  BOOLEAN flip = FALSE, changed = FALSE;
  
  //
  // Check for empty row
  //
  if( rcnt <= 0 ) return ROW_OK;
  
  //
  // PUT CONSTRAINT IN THE FORM: sum {j in N} aj xj <= a0 
  //
  if( rsen == 'G' )
    {
      Flip (rcnt, rind, rval, rsen, rrhs);
      flip = TRUE;
    }
  
  //
  // CREATE A LOCAL COPY OF THE CONSTRAINT WITH ALL FIXED VARIABLES
  // ELIMINATED AND VARIABLES WITH ZERO COEFFICIENTS REMOVED
  //
  sense = rsen;
  rhs = rrhs;
  for (nzcnt = 0, j = 0; j < rcnt; j++) {
    if (rval[j] < -EPSILON || rval[j] > EPSILON) {
      if (LowerBound[rind[j]] > UpperBound[rind[j]] - EPSILON) {
	rhs -= rval[j] * LowerBound[rind[j]];
      }
      else {
	ix[nzcnt]   = rind[j];
	val[nzcnt++] = rval[j];
      }
    }
  }
  

  //
  // SPECIAL CASE
  //
  if (nzcnt == 0) {
    if (rhs < -EPSILON || (rhs > EPSILON && sense == 'E')) {
      rowstat = INFEASIBLEROW;
      DEBUGPR(10, ucout << Errmsg("  INFEASIBLE: nzcnt==0  (%f , %c)\n", rhs, sense));
    }
    else {
      rowslack = rhs;
      rowstat = REDUNDANTROW;
      DEBUGPR(10, ucout << Errmsg("  REDUNDANTROW: nzcnt==0  (%f , %c)\n", rhs, sense));
    }
    goto EXIT;
  }
  
#ifdef PHASE1
  //
  // PHASE I: ROOT ONLY
  //
  //    - CHECK INFEASIBILITY
  //    - CHECK REDUNDNACY
  //
  DEBUGPR(10, ucout << "PHASE I    Root=" << root << endl);
  
  //
  // Compute the maximum and minimum of the left hand side
  //
  
  minlhs = maxlhs = (double) 0;
  for (j = 0; j < nzcnt; j++) {
    if (val[j] > EPSILON) {
      minlhs += val[j] * LowerBound[ix[j]];
      maxlhs += val[j] * UpperBound[ix[j]];
    }
    else {
      minlhs += val[j] * UpperBound[ix[j]];
      maxlhs += val[j] * LowerBound[ix[j]];
    }
  }
  
  if (root) {
    
    //
    // Check infeasibility and redundancy
    //
    
    if (minlhs > rhs + EPSILON) {
      
      DEBUGPR(2, ucout << Errmsg("  INFEASIBLE: MINLHS > RHS (%f > %f)\n", minlhs, rhs));
      
      rowstat = INFEASIBLEROW;
      goto EXIT;
    }
    
    if (sense == 'E' && maxlhs < rhs - EPSILON) {
      
      DEBUGPR(2, ucout << Errmsg("  INFEASIBLE: SENSE 'E' && MAXLHS < RHS (%f < %f)\n", maxlhs, rhs));
      
      rowstat = INFEASIBLEROW;
      goto EXIT;
    }
    
    if (sense == 'L' && maxlhs < rhs + EPSILON) {
      
      DEBUGPR(2, ucout << Errmsg("  REDUNDANT: SENSE 'L' && MAXLHS < RHS (%f < %f)\n", maxlhs, rhs));
      
      rowslack = rhs - maxlhs;
      rowstat = REDUNDANTROW;
      goto EXIT;
    }
    
    if (sense == 'E') {
      if (nzcnt == 1) {
	lbvalue = ubvalue = rhs/val[0];
	if (ubvalue < LowerBound[ix[0]] - EPSILON || lbvalue > UpperBound[ix[0]] + EPSILON) {
	  
	  DEBUGPR(2, ucout << Errmsg("  INFEASIBLE: equality #nz=1 %f (%f,%f)\n", lbvalue, LowerBound[ix[0]], UpperBound[ix[0]]));
	  
	  rowstat = INFEASIBLEROW;
	  goto EXIT;
	}
	else {
	  if (vtype[ix[0]] != continuous) {
	    frac = lbvalue - floor (lbvalue);
	    if (frac > EPSILON && frac < 1 - EPSILON) {
	      
	      DEBUGPR(2, ucout << Errmsg ("INFEASIBLE: equality #nz=1 non-continuous %f\n", frac));
	      
	      rowstat = INFEASIBLEROW;
	      goto EXIT;
	    }
	  }
	  
	  DEBUGPR(10, ucout << Errmsg("  FIXING VARIABLE %d AT %f\n", ix[0], lbvalue));
	  
	  if (lbvalue > LowerBound[ix[0]] + EPSILON) {
	    if (probe) {
	      AddImplication (ix[0], 'L', lbvalue);
	    }
	    SetBound(ix[0], 'L', lbvalue);
	  }
	  if (ubvalue < UpperBound[ix[0]] - EPSILON) {
	    if (probe) {
	      AddImplication (ix[0], 'U', lbvalue);
	    }
	    SetBound(ix[0], 'U', lbvalue);
	  }
	  changed = TRUE;
	}
      }
      else {
	if (maxlhs < rhs + EPSILON) {
	  for (j = 0; j < nzcnt; j++) {
	    if (val[j] > EPSILON) {
	      
	      DEBUGPR(10, ucout << Errmsg("  FIXING VARIABLE %d AT UPPER BOUND %f\n", ix[j], UpperBound[ix[j]]));
	      
	      if (probe) {
		AddImplication (ix[j], 'L', UpperBound[ix[j]]);
	      }
	      SetBound(ix[j], 'L', UpperBound[ix[j]]);
	    }
	    else {
	      
	      DEBUGPR(10, ucout << Errmsg("  FIXING VARIABLE %d AT LOWER BOUND %f\n", ix[j], LowerBound[ix[j]]));
	      
	      if (probe) {
		AddImplication (ix[j], 'U', LowerBound[ix[j]]);
	      }
	      SetBound(ix[j], 'U', LowerBound[ix[j]]);
	    }
	    changed = TRUE;
	  }
	}
      }
    }
  }
#endif
  

  //
  // PHASE II: ROOT ONLY; NOT WHEN PROBING
  //
  //     - COEFFICIENT REDUCTION
  //
  
#ifdef PHASE2
  if (root && !probe) {
    DEBUGPR(2, ucout << "PHASE II" << endl);
    
    //
    // STANDARD REDUCTION
    //
    
    if (maxlhs > rhs + EPSILON) {
      for (deltamin = deltamax = (double) 0, j = 0; j < nzcnt; j++) {
	if (vtype[ix[j]] == binary) {
	  if (val[j] > EPSILON) {
	    if ((diff = rhs - maxlhs + val[j]) > EPSILON) {
	      
	      DEBUGPR(10, ucout << Errmsg("  IMPROVING COEFFICIENT OF BINARY VARIABLE %d (%f) --> (%f) TYPE I\n", ix[j], val[j], val[j] - diff));
	      
	      val[j] -= diff;
	      deltamax += diff;
	    }
	  }
	  else {
	    if ((diff = rhs - maxlhs - val[j]) > EPSILON) {
	      
	      DEBUGPR(10, ucout << Errmsg("  IMPROVING COEFFICIENT OF BINARY VARIABLE %d (%f) --> (%f) TYPE II\n", ix[j], val[j], val[j] + diff));
	      
	      val[j] += diff;
	      deltamin += diff;
	    }
	  }
	}
      }
      rhs -= deltamax;
      maxlhs -= deltamax;
      minlhs += deltamin;
    }
    
    //
    // EUCLIDEAN REDUCTION
    //
    
    //
    // Euclidean reduction can only be used for rows that do not contain
    // continuous variables
    //
    
#ifdef EUCLID_REDUCTION
    for (j = 0; j < nzcnt; j++) {
      if (vtype[ix[j]] == continuous) {
	break;
      }
    }
    
    if (j == nzcnt) {
      
      //
      // Euclidean reduction
      //
      // Do the first element separately. This saves some comparisons and
      // assignments
      //
      
      double absa = fabs (val[0]);
      int kexp;
      for (kexp = 0; kexp < MAXK; kexp++, absa *= 10) {
	if (ISINT(absa)) {
	  break;
	}
      }
      
      if (kexp == MAXK) {
	
	DEBUGPR(10, ucout << Errmsg("  EUCLIDEAN REDUCTION: Numerical problems (%e,%e)\n", val[0], absa));
	
	goto PHASE_III;
      }
      
      int tkexp = (int) pow ((double) 10, (double) kexp);
#if 0
#if defined(SUN) || defined(HP) || defined(SG) || defined(DEC) || defined(PC486)
      int n2 = nint (absa);
#endif
#ifdef IBM
      n2 = (int) nearest (absa);
#endif
#else
      int n2 = nint (absa);
#endif
      
      //
      // Do the remain elements
      //
      
      for (j = 1; j < nzcnt; j++) {
	absa = fabs (val[j]);
	for (k = 0; k < MAXK; k++, absa *= 10) {
	  if (ISINT(absa)) {
	    break;
	  }
	}
	
	if (k == MAXK) {
	  
	  DEBUGPR(10, ucout << Errmsg("  EUCLIDEAN REDUCTION: Numerical problems (%e,%e)\n", val[j], absa));
	  
	  goto PHASE_III;
	}
	
	if ((k -= kexp) > 0) {
	  int tk = (int) pow ((double) 10, (double) k);
	  kexp += k;
	  tkexp *= tk;
	  n2 *= tk;
	}
	
#if 0
#if defined(SUN) || defined(HP) || defined(SG) || defined(DEC) || defined(PC486)
        int n1;
	if ((n1 = nint (absa)) < n2)
#endif          
#ifdef IBM
	  if ((n1 = (int) nearest (absa)) < n2)
#endif
#else
        int n1;
	if ((n1 = nint (absa)) < n2)
#endif
	    {
	      int tmp = n1;
	      n1 = n2;
	      n2 = tmp;
	    }
	
	for (;;) {
	  int irmd = n1 - (n1/n2)*n2;
	  if (irmd == 0) {
	    break;
	  }
	  n1 = n2;
	  n2 = irmd;
	}
      }
      int gdc = n2;
      
      //
      // Modify the constraint
      //
      
      if (gdc > 1) {
	for (j = 0; j < nzcnt; j++) {
	  val[j] = (val[j] * tkexp) / gdc;
	}
	rhs = floor (((rhs * tkexp) / gdc) + EPSILON);
	
	minlhs = (minlhs * tkexp) / gdc;
	maxlhs = (maxlhs * tkexp) / gdc;
      }
    }
#endif // EUCLID_REDUCTION
  }
#endif
  


#ifdef PHASE3
#ifdef EUCLID_REDUCTION
 PHASE_III:			// This target only needed for Euclid Reducts
#endif
  
  //
  // PHASE III:
  //
  //    - FEASIBILITY FIXING
  //    - UPDATE BOUNDS
  //
  DEBUGPR(2, ucout << "PHASE III" << endl);
  
  //
  // Feasibility fixing for binary variables
  //
  
  for (j = 0; j < nzcnt; j++) {
    if (vtype[ix[j]] == binary) {
      if (val[j] > EPSILON) {
	if (minlhs + val[j] > rhs + EPSILON) {
	  
	  DEBUGPR(10, ucout << Errmsg("  FIXING BINARY VARIABLE %d AT LOWER BOUND\n", ix[j]));
	  
	  if (probe) {
	    AddImplication (ix[j], 'U', (double) 0);
	  }
	  SetBound(ix[j], 'U', (double) 0);
	  changed = TRUE;
	}
      }
      else {
	if (minlhs - val[j] > rhs + EPSILON) {
	  
	  DEBUGPR(10, ucout << Errmsg("  FIXING BINARY VARIABLE %d AT UPPER BOUND\n", ix[j]));
	  
	  if (probe) {
	    AddImplication (ix[j], 'L', (double) 1);
	  }
	  SetBound(ix[j], 'L', (double) 1);
	  changed = TRUE;
	}
      }
    }
  }
  
  //
  // Compute new lower and upper bounds
  //
  
  if (nzcnt == 1) {
    if (val[0] > EPSILON) {
      ubvalue = rhs / val[0];
      if (ubvalue < UpperBound[ix[0]] - EPSILON) {
	
	DEBUGPR(10, ucout << Errmsg("  MODIFYING UPPER BOUND CONTINUOUS-1 %d (%e)\n", ix[0], ubvalue));
	
	if (probe && ubvalue < LowerBound[ix[0]] + EPSILON) {
	  AddImplication (ix[0], 'U', ubvalue);
	  changed = TRUE;
	}
	SetBound(ix[0], 'U', ubvalue);
	if (!probe) {
	  changed = TRUE;
	}
      }
    }
    else {
      lbvalue = rhs / val[0];
      if (lbvalue > LowerBound[ix[0]] + EPSILON) {
	
	DEBUGPR(10, ucout << Errmsg("  MODIFYING LOWER BOUND CONTINUOUS-1 %d (%e)\n", ix[0], lbvalue));
	
	if (probe && lbvalue > UpperBound[ix[0]] - EPSILON) {
	  AddImplication (ix[0], 'L', lbvalue);
	  changed = TRUE;
	}
	SetBound(ix[0], 'L', lbvalue);
	if (!probe) {
	  changed = TRUE;
	}
      }
    }
  }
  else {
    if (minlhs > -SMALLINF) {
      for (j = 0; j < nzcnt; j++) {
	if (LowerBound[ix[j]] < UpperBound[ix[j]] - EPSILON) {
	  if (val[j] > EPSILON) {
	    ubvalue = ((rhs - minlhs)/val[j]) + LowerBound[ix[j]];
	    if (vtype[ix[j]] == continuous) {
	      if (ubvalue < UpperBound[ix[j]] - EPSILON) {
                
		DEBUGPR(10, ucout << Errmsg("  MODIFYING UPPER BOUND CONTINUOUS-2 %d (%f)\n", ix[j], ubvalue));
		
		if (probe && ubvalue < LowerBound[ix[j]] + EPSILON) {
		  AddImplication (ix[j], 'U', ubvalue);
		  changed = TRUE;
		}
		SetBound(ix[j], 'U', ubvalue);
		if (!probe) {
		  changed = TRUE;
		}
	      }
	    }
	    else {
	      if (floor (ubvalue + EPSILON) < UpperBound[ix[j]] - EPSILON) {
		
		DEBUGPR(10, ucout << Errmsg("  MODIFYING UPPER BOUND NON-CONTINUOUS-1 %d (%f,%f)\n", ix[j], ubvalue, floor (ubvalue + EPSILON)));
		
		if (probe && floor (ubvalue + EPSILON) < LowerBound[ix[j]] + EPSILON) {
		  AddImplication (ix[j], 'U', floor (ubvalue + EPSILON));
		  changed = TRUE;
		}
		SetBound(ix[j], 'U', floor (ubvalue + EPSILON));
		if (!probe) {
		  changed = TRUE;
		}
	      }
	    }
	  }
	  else {
	    lbvalue = ((rhs - minlhs) / val[j]) + UpperBound[ix[j]];
	    if (vtype[ix[j]] == continuous) {
	      if (lbvalue > LowerBound[ix[j]] + EPSILON) {
		
		DEBUGPR(10, ucout << Errmsg("  MODIFYING LOWER BOUND CONTINUOUS-2 %d (%e)\n", ix[j], lbvalue));
		
		if (probe && lbvalue > UpperBound[ix[j]] - EPSILON) {
		  AddImplication (ix[j], 'L', lbvalue);
		  changed = TRUE;
		}
		SetBound(ix[j], 'L', lbvalue);
		if (!probe) {
		  changed = TRUE;
		}
	      }
	    }
	    else {
	      if (ceil (lbvalue - EPSILON) > LowerBound[ix[j]] + EPSILON) {
                
		DEBUGPR(10, ucout << Errmsg("  MODIFYING LOWER BOUND NON-CONTINUOUS-1 %d (%g,%g)\n", ix[j], lbvalue, ceil (lbvalue - EPSILON)));
		
		if (probe && ceil (lbvalue - EPSILON) > UpperBound[ix[j]] - EPSILON) {
		  AddImplication (ix[j], 'L', ceil (lbvalue - EPSILON));
		  changed = TRUE;
		}
		SetBound(ix[j], 'L', ceil (lbvalue - EPSILON));
		if (!probe) {
		  changed = TRUE;
		}
	      }
	    }
	  }
	}
      }
    }
    
    if (sense == 'E') {
      if (maxlhs < ((1.0 - EPSILON) * INF)) {
	for (j = 0; j < nzcnt; j++) {
	  if (LowerBound[ix[j]] < UpperBound[ix[j]] - EPSILON) {
	    if (val[j] > EPSILON) {
	      lbvalue = (rhs - maxlhs + val[j] * UpperBound[ix[j]]) / val[j];
	      if (vtype[ix[j]] == continuous) {
		if (lbvalue > LowerBound[ix[j]] + EPSILON) {
		  
		  DEBUGPR(10, ucout << Errmsg("  MODIFYING LOWER BOUND CONTINUOUS-3 %d (%f)\n", ix[j], lbvalue));
		  
		  if (probe && lbvalue > UpperBound[ix[j]] - EPSILON) {
		    AddImplication (ix[j], 'L', lbvalue);
		    changed = TRUE;
		  }
		  SetBound(ix[j], 'L', lbvalue);
		  if (!probe) {
		    changed = TRUE;
		  }
		}
	      }
	      else {
		if (ceil (lbvalue - EPSILON) > LowerBound[ix[j]] + EPSILON) {
		  
		  DEBUGPR(10, ucout << Errmsg("  MODIFYING LOWER BOUND NON-CONTINUOUS-2 %d (%f)\n", ix[j], ceil (lbvalue - EPSILON)));
		  
		  if (probe && ceil (lbvalue - EPSILON) > UpperBound[ix[j]] - EPSILON) {
		    AddImplication (ix[j], 'L', ceil (lbvalue - EPSILON));
		    changed = TRUE;
		  }
		  SetBound(ix[j], 'L', ceil (lbvalue - EPSILON));
		  if (!probe) {
		    changed = TRUE;
		  }
		}
	      }
	    }
	    else {
	      ubvalue = (rhs - maxlhs + val[j] * LowerBound[ix[j]]) / val[j];
	      if (vtype[ix[j]] == continuous) {
		if (ubvalue < UpperBound[ix[j]] - EPSILON) {
		  
		  DEBUGPR(10, ucout << Errmsg("  MODIFYING UPPER BOUND CONTINUOUS-3 %d (%f from %f)\n", ix[j], ubvalue, UpperBound[ix[j]]));
		  
		  if (probe && ubvalue < LowerBound[ix[j]] + EPSILON) {
		    AddImplication (ix[j], 'U', ubvalue);
		    changed = TRUE;
		  }
		  SetBound(ix[j], 'U', ubvalue);
		  if (!probe) {
		    changed = TRUE;
		  }
		}
	      }
	      else {
		if (floor (ubvalue + EPSILON) < UpperBound[ix[j]] - EPSILON) {
		  
		  DEBUGPR(10, ucout << Errmsg("  MODIFYING UPPER BOUND NON-CONTINUOUS-2 %d (%f from %f)\n", ix[j], floor (ubvalue + EPSILON), UpperBound[ix[j]]));
		  
		  if (probe && floor (ubvalue + EPSILON) < LowerBound[ix[j]] + EPSILON) {
		    AddImplication (ix[j], 'U', floor (ubvalue + EPSILON));
		    changed = TRUE;
		  }
		  SetBound(ix[j], 'U', floor (ubvalue + EPSILON));
		  if (!probe) {
		    changed = TRUE;
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
#endif

  
  //
  // COPY THE LOCAL CONSTRAINT BACK INTO THE ORIGINAL CONSTRAINTS DELETING
  // VARIABLES WITH ZERO COEFFICIENTS.
  //
  
  if (root && !probe) {
    for (rcnt = 0, j = 0; j < nzcnt; j++) {
      if (val[j] < -EPSILON || val[j] > EPSILON) {
	rind[rcnt]     = ix[j];
	rval[rcnt++] = val[j];
      }
    }
    rsen = sense;
    rrhs = rhs;
    
    //
    // DEFENSIVE PROGRAMMING
    //
    
    if (rcnt == 0) {
      rowslack = (double) rrhs;
      rowstat = REDUNDANTROW;
    }
  }
  
  EXIT:
  
  //
  // PUT BACK THE CONSTRAINT IN ITS ORIGINAL FORM
  //
  if (flip)
    Flip (rcnt, rind, rval, rsen, rrhs);
  
  //
  // Return the row status
  //
  if (rowstat == ROW_OK && changed == TRUE)
    rowstat = MODIFIEDROW;

  return rowstat;
} */

/****************************************************************************
*  Compresses the matrix by deleting empty rows. The #rows may be changed.
*  Implies that, in addition to matxxx, rhsx and senx could get modified.
****************************************************************************/
/*
void CRowMajorMatrix::del_empty_rows( void )
{
  int rows_sofar, nzcnt, row;

  for( rows_sofar = 0, nzcnt = 0, row = 0;
       row < get_nrows(); row++ )
    {
      if( get_matcnt()[ row ] > 0 )
	{
	  for( int col = get_matbeg()[ row ];
	       col < get_matbeg()[ row ] + get_matcnt()[ row ];
	       col++ )
            {
#if 0 //XXX
	      if( matval[ col ] > -EPSILON && matval[ col ] < EPSILON )
	        {
		  cherr << "Preprocess: Zero coefficient detected:"
			<< row << ", " << col << endl;
                }
#endif
	      
	      matind[ nzcnt ] = get_matind()[ col ];
	      matval[ nzcnt ] = get_matval()[ col ];
	      nzcnt++;
            }

	  matbeg[ rows_sofar ] = nzcnt - get_matcnt()[ row ];
	  matcnt[ rows_sofar ] = get_matcnt()[ row ];
	  rhsx[ rows_sofar ] = get_rhsx()[ row ];
	  senx[ rows_sofar ] = get_senx()[ row ];
	  rows_sofar++;
        }
    }

  matbeg[ rows_sofar ] = nzcnt;
  nrows = rows_sofar ;
  matvalsz = nzcnt;
}
*/


/*
void MILPProblem::preprocess_rows()
{
int changed;
IntVector ix(get_ncols());
DoubleVector val(get_ncols());

row_redundant.resize(get_nrows());
row_redundant.reset();
  
do {
   changed = FALSE;
      
   for (int i=0; i<get_nrows(); i++ ) {
     if (!row_redundant(i) && rowmajor.get_matcnt()[i] > 0) {
        int rcnt = rowmajor.get_matcnt()[i];
        int* matind =rowmajor.get_matind().data() + rowmajor.get_matbeg()[ i ];
        double* matval =rowmajor.get_matval().data() + rowmajor.get_matbeg()[i];
	CRowStat rowstat = process_one_constraint(
		rcnt,
		matind,
		matval,
		senx[i],
		rhsx[ i ],
		LowerBound,
		UpperBound,
		TRUE,
		FALSE,
		ix.data(),
		val.data(),
		vtype);

        if (rcnt == 0) {
           if (rowstat != ROW_INFEASIBLE)
              rowstat = ROW_EMPTY;
           }
        else {
           rowmajor.resize(i,rcnt);
           }

	switch( rowstat ) {

		case ROW_INFEASIBLE:
		    DEBUGPR(2, ucout << "  Row " << i << " is infeasible" << endl)
		    break;

		case ROW_REDUNDANT:
		    DEBUGPR(10, ucout << "  Row " << i << " is redundant" << endl)
		    row_redundant.set(i);
		    break;

		case ROW_EMPTY:
		    DEBUGPR(10, ucout << "  Row " << i << " is empty" << endl)
		    row_redundant.set(i);
		    break;

		case ROW_MODIFIED:
		    DEBUGPR(10, ucout << "  Row " << i << " is modified" << endl)
		    changed = TRUE;
		    break;

		case ROW_OK:
		    break;

		default:
		    cerr << "  Preprocess: invalid row stat "
			  << (int)rowstat << endl;
		    break;
		}
	}
     }
   } while(changed);
  
//
// Remove all simple upper or lower bound constraints
//
for (int i=0; i<get_nrows(); i++) {
  if (rowmajor.get_matcnt()[ i ] == 1)
     row_redundant.set(i);
  }
}
*/



void MILPProblem::generate_mapped_milp(CharString& fname, CharString& mapfile,
						const bool write_flag)
{
if (!lp)
   EXCEPTION_MNGR(runtime_error, "MILPProblem::setup_mapped_milp -- no LP is currently defined!");
MILPSymbInfo info;

BasicArray<CharString> vnames, cnames;
lp()->getVariableNames(vnames);
lp()->getRowNames(cnames);

//
// Assume that the LP name is *.<suffix>, which refers to the filename that it
// was read in from.
//
CharString name = lp()->name();
char* str = strrchr(name.data(), (int)'.');
if (str != NULL)
  {  
   name.resize(str - name.data());
  }

info.preprocess_ILP(name, cnames, vnames, mapfile, fname, write_flag);
}



void MILPProblem::set_labels(const CharString& rowlabels, 
						const CharString& collabels)
{
if (!lp)
   EXCEPTION_MNGR(runtime_error, "MILPProblem::set_labels -- LP object has not been setup!");

BasicArray<CharString> rlabel(lp()->getNumRows());
ifstream rfstr(rowlabels.data());
if (!rfstr)
   EXCEPTION_MNGR(runtime_error, "MILPProblem::set_labels -- missing file \"" << rowlabels << "\"");
{for (size_type i=0; i<rlabel.size(); i++) {
  rfstr >> rlabel[i];
  if (!rfstr)
     EXCEPTION_MNGR(runtime_error,"MILPProblem::set_labels -- error reading in the " << i << "-th row label");
  }}
lp()->setRowNames(rlabel);

BasicArray<CharString> clabel(lp()->getNumCols());
ifstream cfstr(collabels.data());
if (!cfstr)
   EXCEPTION_MNGR(runtime_error,"MILPProblem::set_labels -- missing file \"" << collabels << "\"");
{for (size_type i=0; i<clabel.size(); i++) {
  cfstr >> clabel[i];
  if (!cfstr)
     EXCEPTION_MNGR(runtime_error,"MILPProblem::set_labels -- error reading in the " << i << "-th col label");
  }}
lp()->setVariableNames(clabel);
}


// This code is by JE.  It was easiest to put it here.  It builds the
// adjacency structures for use by the ramp-up heuristic.

void MILPProblem::buildAdjSets(adjSetObj& adjSet)
{
  // There's an entry in adjSet for every possible branching choice
  // That means one for each integer variable and one for each SOS

  adjSet.resize(numIntegerVars() + numSets());

  // Make a temporary row-major version of the constraint matrix

  RMSparseMatrix< Ereal<double> > rmj;

  rmj.convert(colmajor);

  // First, loop over all integer variables.
  // We want to put in any variable that is integer an appears in the
  // some common row with this variable.

  for (int jint=0; jint < numIntegerVars(); jint++)
    {
      // Get the corresponding column of constraint matrix

      int j = integerVarNdx[jint];

      // Scan this column and look at rows it intersects

      int colSize   = colmajor.get_matcnt()[j];
      int* rowIndex = colmajor.get_matind().data() + colmajor.get_matbeg()[j];

      for (int e=0; e<colSize; e++)
	{
	  int i = rowIndex[e];

	  // Now look across the row and find all its columns (in j2)

	  int rowSize   = rmj.get_matcnt()[i];
	  int* colIndex = rmj.get_matind().data() + rmj.get_matbeg()[i];

	  for (int re=0; re<rowSize; re++)
	    {
	      int j2 = colIndex[re];

	      // If this is the original column, ignore it

	      if (j2 != j)
		{
		  // If this variables is integer, find its index
		  // among the integer variables

		  int jint2 = LPtoIntegerVar[j2];

		  // If it's integer, make sure it's in the adjacent set

		  if (jint2 >= 0)
		    adjSet[jint].insert(jint2);
		}
	    }
	}

      // Next, staying with the same column, we also want to insert
      // all the sets that include this column.  Fortunately, this 
      // information is readily available in setReverseNdx. The value
      // inserted is setNumber + numInts.

      if (numSets() > 0)
	{
	  for (size_type se=0; se < setReverseNdx[jint].size(); se++)
	    {
	      int bndx = setReverseNdx[jint][se] + numIntegerVars();
	      adjSet[jint].insert(bndx);
	    }
	}

      // We've now found all integer variables and sets adjoining this column
      
    }
  
  // Next, we want to build the set of all things adjacent to a set.
  // This consists of all the variables it contains, and all the *other*
  // sets those variables are in.

  // Loop over all sets.

  for (int snum=0; snum < numSets(); snum++)
    {
      // To get the actual index, add numInts

      int bndx = snum + numIntegerVars();

      // Loop over all variables in set number snum

      for (size_type e=0; e < specialSets[snum].size(); e++)
	{
	  int jint = specialSets[snum][e];

	  // The variable goes into the adjacency set

	  adjSet[bndx].insert(jint);

	  // Now find all sets the variable is in (in snum2)

	  for (size_type se=0; se < setReverseNdx[jint].size(); se++)
	    {
	      int snum2 = setReverseNdx[jint][se];

	      // If it's the original set, ignore it

	      if (snum2 != snum)
		{
		  // To get the branch index for snum2, add numInts

		  int bndx2 = snum2 + numIntegerVars();
		  
		  // Make sure this index is in the adjacency set

		  adjSet[bndx].insert(bndx2);
		}
	    }
	}
    }

  DEBUGPR(100,ucout << "Adjacent sets:\n";
	  for(size_type s=0; s<adjSet.size(); s++)
             {
               ucout << "Adjacent to " << s << ':';
	       for(size_type t=0; t<<adjSet.size(); t++)
		 if (adjSet[s].contains(t)) ucout << ' ' << t;
	       ucout << endl;
	     }
	  );

}

	      
	  
  
		  


} // namespace pico
