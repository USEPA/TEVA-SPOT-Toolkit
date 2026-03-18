/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file MILPProblem.h
 *
 * Defines the pico::MILPProblem class.
 */

#ifndef pico_MILPProblem_h
#define pico_MILPProblem_h

#include <acro_config.h>
#include <utilib/BasicArray.h>
#include <utilib/BitArray.h>
#include <utilib/SparseMatrix.h>
#include <utilib/DoubleVector.h>
#include <utilib/HashedSet.h>
#include <pebbl/pebblBase.h>
#include <pico/serialMilpParams.h>
#include <pico/PicoLPSolver.h>
#include <pico/mipEnum.h>

namespace pico {

using utilib::BitArray;
using utilib::DoubleVector;
using utilib::IntVector;
using utilib::CharString;

//
// Enumeration Types
//
enum CRowStat
{
    BAD_ROW_STAT,
    ROW_INFEASIBLE,
    ROW_REDUNDANT,
    ROW_MODIFIED,
    ROW_EMPTY,
    ROW_OK
};


// Thie appears in various places around the MIP and is defined here
// since it's the first file included.

enum branch_type {branch_up, branch_down, no_branch};


// Typedef of objects used to hold variable/set adjacency structure (JE)
// It's just an array splay trees of integers. 

 typedef BasicArray< utilib::HashedSet<int> > adjSetObj;
 


/// Problem class definition for a mixed integer linear programming problem.
class MILPProblem : public pebbl::pebblBase, public CommonIO, public serialMilpParams
{
public:

  /// Make an empty MIP, for parallel initialization
  MILPProblem();

  ///
  virtual ~MILPProblem();

  ///
  void initialize(BasicArray<int> & listOfIntegers, int _debug=0);

  ///
  void initialize(char* filename, int _debug=0);

  /// A "constructor" when starting up from a packed problem representation
  void initialize(utilib::UnPackBuffer& inBuffer);

  /// To rerun with a closely-related problem now specified in the same LP 
  /// object.  See documentation for restoreIntBounds().
  void reset();

  /// Pieces of initialization common to all forms of construction. lp must be 
  /// valid.
  void constructGuts();

  /// Restore (all) variable bounds saved when the MILPProblem was created.  
  /// This can be
  /// useful for resetting a problem instance.  reset() does *not* restore these
  /// bounds because we want to be able to chance the bounds externally and have
  /// those used during the reset.
  void restoreVarBounds();

  /// Preprocess the constraint matrix
  int preprocess();

  ///
  bool IShouldWrite()
    {return(reduceLog);};

  /// Read in row/col labels from two files
  void set_labels(const CharString& rowlabels_file, 
					const CharString& collabels_file);

  /// Generate a MILP application with explicit variable mapping
  void generate_mapped_milp(CharString& filename, CharString& mapfile,
						const bool write_flag=true);

  /// Generate a MILP application with explicit variable mapping
  void generate_mapped_milp(CharString& filename, const bool write_flag=true)
		{
		CharString mapfile = ""; 
		generate_mapped_milp(filename,mapfile,write_flag);
		}

  /// Points to LP representation
  PicoLPSolver lp;

	/// Array of length numAllVars() giving variable types
  BasicArray<variableType>   vtype;
	/// Array of length ### giving the row types
  BasicArray<constraintGroup> ctype;

  	///
  int numIntegerVars() { return (int)integerVarNdx.size(); }
  	///
  int numBinaryVars() { return NumBinaryVars; }
	///
  int numGeneralIntVars() { return numIntegerVars() - NumBinaryVars; };
	///
  int numAllVars() { return lp()->getNumCols(); }
  int numSets() {return NumSets;}

  ///
  void loadProblem(utilib::UnPackBuffer& unpack)
	{
	lp()->loadProblem(unpack);
	initialize(unpack);
	}

  ///
  void loadProblem(char* filename1, char* filename2=0)
	{
        if (filename2)
	   lp()->loadProblem(filename1,filename2,integerVarNdx);
        else
	   lp()->loadProblem(filename1,integerVarNdx);
	initialize(integerVarNdx,debug);
	}

  ///
  void print_stats(std::ostream& ostr) 
	{
	ostr << "MILPProblem Statistics" << std::endl;
	ostr << "Number of variables:                 " << numAllVars() << std:: endl;
	ostr << "Number of binary variables:          " << numBinaryVars() << std:: endl;
	ostr << "Number of integer variables:         " << numIntegerVars() << std:: endl;
	ostr << "Number of general integer variables: " << numGeneralIntVars() << std:: endl;
	ostr << "Number of special ordered sets:      " << numSets() << std:: endl;
	ostr << "Number of constraints:               " << ctype.size() << std:: endl;
	ostr << "Number of nonzeros:                  " << lp()->getNumElements() << std::endl;
	}

	/// Array of length numIntegerVars() giving indices of integer 
	/// variables in the LP variable array.  This is a BasicArray<int> to
  /// be compatible with what is passed to PicoLPInterface::loadProblem
  BasicArray<int> integerVarNdx;

  /// A copy of the original integer variable indices used for
  //resetting when the LP changes slightly (these can change during preprocessing fixing, etc)

  BasicArray<int> integerVarCopy;

  /// Keep around the original lower and upper bounds for the
  ///variables.  This is needed to rerun a slight variant of a
  ///problem since integer bounds can be changed by branching  and
  ///continuous variable bounds can be changed by preprocessing. Bounds
  ///for integer variables will of course be forced to be integer
  ///bounds eventually, but at the start, they are doubles just like
  ///the bounds on the rational variables.  In the spirit of really
  ///resetting, we'll keep the original bounds.

DoubleVector originalLowerBounds;
DoubleVector originalUpperBounds;

	/// Array of length numIntegerVars() giving
    	/// indices of generalInteger variables in the node-specific integer
    	/// bounds arrays (entries for binary variables = -1).
	/// This is wasteful, since in general most of the integer variables 
	/// will be binary, but this allows initialization of the array when 
	/// the MLIPProblem is initialized, and allows indexing by branchVariable.
  IntVector reverseBoundsNdx;	

  /// To use the AMPL-PICO interface, we now must be able to map an LP variable number
  /// to an integer variable number.  This array has length numAllVars().  Continuous
  /// or initially-fixed variables have entry -1.  This wastes space, but will be very
  /// fast and there's only one array per processor.

  IntVector LPtoIntegerVar;

  /// Store special sets. For now these are sets of binary variables constrained to
  /// sum to exactly 1.  For now, variable numbers are ints.  Store integer variable number.

  BasicArray<IntVector> specialSets;

  /// Also store mappings from variables in special sets to set IDs.  This will increase efficiency
  /// when determining sets that are candidates for branching in a particular node.  This has length
  /// numIntegerVars().

 BasicArray<IntVector> setReverseNdx;

	/// Read the initial bounds from the LP (used to initialize the root).  Returns false if
 /// we determine the problem is infeasible 
  bool getbounds(BitArray& fixed, IntVector& intLowerBounds, 
			IntVector& intUpperBounds, 
			BitArray& binary_vals,
		 IntVector& spSetLowerBounds,
		 IntVector& spSetUpperBounds);


	/// Compute all counts/types from the LP and list of integer variables.
  void computeTypes();

  /// Find all the special sets. For now, these are sets of binary variables that
  /// are constrained to sum to exactly 1 (select 1 of many).
  void computeSets();

  // for debugging
  void printSets();
	///
  void classify_constraints(std::ostream& ostr);

	///
  void packMIP(utilib::PackBuffer& outBuffer);

 	/// For debugging
  void printMIP();

	///
  int get_nvars( void ) const { return colmajor.get_ncols(); }
	///
  int get_ncols( void ) const { return colmajor.get_ncols(); }
	///
  int get_nrows( void ) const { return colmajor.get_nrows(); }
	///
  int get_nnzero( void ) const { return colmajor.get_nnzero(); }

	///
  IntVector row_map;
	///
  IntVector col_map;

  /// For building the set of adjacency relationships between variables.
  /// Two integer variables are adjacent if they appear in a common
  /// constraint.  This is extended to sets as they are also possible
  /// branches.

  void buildAdjSets(adjSetObj& adjSet);


protected:

  ///
  // true means log preprocessing information
  bool reduceLog;

  ///
  // true means log even more preprocessing information
  // comments elsewhere say reduceLog must be set true when debugReduceLog
  // is true, but that is not enforced; it fails silently.
  bool debugReduceLog;

  // True if the user would like PICO to allow infinite bounds on
  // integer variables.  Sometimes solvers can find solutions to such
  // problems, but usually there are reasonable finite bounds one can
  // use, so we consider infinite bounds an error in general.  CPLEX
  // has some assumed bounds on LP files that we feel are not
  // intuitive.  So we require users to specify bounds.

  bool allowInfiniteIntegerVarBounds;

/***** Inferential Reduction declarations ***/

  static std::ostream *rout;
	///
  int preprocess_columns_one();
	///
  int preprocess_rows();
	/// 
  int preprocess_columns_two();
    ///
  std::ostream* redLogFile();
    ///
  void redLogInit();
    ///
  //prints an infeasible message to the rout ostream.  The messages being
  //printed by it now appear to be mostly devoid of meaning.
  void redLogInfeasible(char,int line);

  bool compare(Ereal<double> number, Ereal<double> referent, int compareType, int decision);

/***** End of Inferential Reduction declarations **/

	/// Delete empty/redundant rows
  void delete_rows();
	///
  BitArray row_redundant;
	///
  //contstraint[i] = type of constraint for var i.
  //see getObjSense() in OsiSoplexSolverInterface.cpp for details and values
  //TO DO: document this better.
  CharString senx;

	///
  BasicArray<Ereal<double> > lhsx;          //row lower bound
	///
  BasicArray<Ereal<double> > rhsx;          //row upper bound
	///
  BasicArray<Ereal<double> > objx;          //objective function coefficients
	///
  BasicArray<Ereal<double> > LowerBound;    //column lower bound
	///
  BasicArray<Ereal<double> > UpperBound;    //column upper bound
	///
  CMSparseMatrix<Ereal<double> > colmajor;  //the constraint matrix

	///
  int NumBinaryVars;			// How many 0/1 variables?

  int NumSets;     // How many special sets (currently selection of single binary variable)?

	/* BDP - This is only used by the old preprocessor
  CRowStat process_one_constraint( int& rcnt, int* rind,
                        double* rval,
                        char rsen, double& rrhs,
                        DoubleVector& LowerBound, DoubleVector& UpperBound,
                        int root, int probe,
                        int* ix, double* val,
                        BasicArray<variableType>& vtype);
  */

// The following are used for the Inferential Reduction code.  We might revisit their scope
//
// BDP - Reducing the scope of these to ::preprocess appears to be easy
// They are not used by anything outside of the preprocessor code.
  BasicArray<Ereal<double> > LowerBoundY;   //lower row bound
  
  BasicArray<Ereal<double> > UpperBoundY;   //upper row bound
  
/* Dual prices are not currently being used for preprocessing
 *  BasicArray<Ereal<double> > LowerBoundP;   //lower dual price bound
 *  
 *  BasicArray<Ereal<double> > UpperBoundP;   //upper dual price bound
 */  
  BasicArray<Ereal<double> > LowerRangeY;   //lower myopic range bound
  
  BasicArray<Ereal<double> > UpperRangeY;   //upper myopic range bound
 
  //Degree[i] = the number of non-fixed vars with nonzero coefs in row i
  IntVector Degree; 
 
  //Marker[i] = 1 if row i is forcing
  IntVector Marker;

// the following arrays are used in Inferential REduction Summary logging
// BDP - These could be reduced in scope.  They are not used for
// anything outside of the preprocessor.
  IntVector freed;    //freed[i] = 1 if row i has been freed

  IntVector cfixed;   //cfixed[i] = 1 if column i has been fixed

  IntVector rreduced; //rreduced[i] = 1 if row i has been reduced

  IntVector creduced; //creduced[i] = 1 if column i has been reduced

  IntVector rforced;  //rforced[i] = 1 if row i has been forced

  IntVector cforced;  //cforced[i] = 1 if column i has been forced

  IntVector weakened; //weakened[i] = 1 if row i has been weakened

};

} // namespace pico

#endif
