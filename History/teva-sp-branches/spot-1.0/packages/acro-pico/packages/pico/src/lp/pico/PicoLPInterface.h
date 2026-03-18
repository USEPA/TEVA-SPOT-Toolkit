/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file PicoLPInterface.h
 *
 * Defines the pico::PicoLPInterface class.
 */

// TODOs
// 1. The BasicArray and CharString classes might be replaced with vector
// 	and string classes ... when we become more familiar with those
//	classes.
//
// 2. Add bound checks for row/col arguments?
//
// 3. For B&C applications, will we want 'unnamed' rows and columns?
//
// Note: an important distinction between PicoLPInterfaces and the Osi
// intefaces has to do with the notion of 'Solves'.  OSI solves are broken
// into 
//	initialSolve()	- presumably at the root of a B&B tree, using 
//				primal simplex methods
//	resolve()	- presumably at the subproblems in a B&B tree, using
//				dual simplex methods.
// The PicoLPInterface allows the user to determine the solver method
// dynamically, which is done using the 'solve()' method.
//
// 4. How many of the interface methods (especially the set/add methods)
//	should we _force_ to use Ereal<double> arguments?  Using this would
// 	significantly clarify the semantics of this API, but users might
//	find it inconvenient at times...
//

#ifndef pico_PicoLPInterface_h
#define pico_PicoLPInterface_h

#include <acro_config.h>
#include <acro_config.h>
#include <utilib/CommonIO.h>
#include <utilib/ParameterSet.h>
#include <utilib/SparseMatrix.h>
#include <utilib/Ereal.h>
#include <utilib/SimpleSplayTree.h>
#include <utilib/GenericKey.h>
#include <utilib/IntVector.h>
#include <utilib/LinkedList.h>
#include <pebbl/fundamentals.h>
#include <pebbl/pebblBase.h>
#include <pico/BasisArray.h>
#include <pico/picoRowCut.h>
#include <pico/ilp_read.h>
#include <pico/lpEnum.h>
#include <pico/mipEnum.h>
#include <OsiSolverInterface.hpp>

namespace pico {

using utilib::CommonIO;
using utilib::BasicArray;
using utilib::IntVector;
using utilib::CharString;
using utilib::GenericKey;
using utilib::SimpleSplayTree;
using utilib::SimpleSplayTreeItem;
using utilib::RMSparseMatrix;

///
/// PICO's LP Interface Abstract Base Class
///
/// Abstract Base Class for describing an interface to a linear program (LP).
/// This class derives from the OSISolverInterface base class, and it adds
/// functionality to the OSI solvers that is used in PICO.
///
///  The OsiSolverInterface is really the combination of MILP
///  and LP solver interfaces.  We actually have to manipulate some
///  of the MILP-only methods (like isBinary) so CGL (cut generation library,
///  which PICO forces compatibility with) with work correctly.
///  The actual override of those methods is in the PicoLPSubclass class.
///
class PicoLPInterface : virtual public OsiSolverInterface, 
  virtual public CommonIO,
  virtual public utilib::ParameterSet,
  public pebbl::pebblBase
{

public:

	/**
	 * Specialization of Generic Keys that deletes the pointer that
	 * is passed into this class.
 	 */
  class LPInfoKey : public GenericKey<int,CharString*>
  {
  public:

	///
     LPInfoKey(int key, const CharString* data)
		{
		Key=key;
		if (data)
		   Data=new CharString(*data);
		else 
		   Data=0;
		}

	///
     LPInfoKey() {Key=0; Data=0;}
	///
     LPInfoKey(const LPInfoKey& info)
		{
		Key=info.Key;
		if (info.Data)
		   Data = new CharString(*info.Data);
		else
		   Data=0;
		}

	///
     virtual ~LPInfoKey()
		{if (Data) delete Data;}
	///
     void write(std::ostream& os) const
                {
		if (Data) 
		   os << *Data;
		else
		   os << "NDX" << Key; 
		}
        ///
     void read(std::istream& )
                {}

  };

  friend int compare(const LPInfoKey& key1, const LPInfoKey& key2);


public:

  enum constraintSense { lessOrEqual='L', greaterOrEqual='G', equal='E', 
			ranged='R', notRestricted='N'};
  enum solutionMethod { primalSimplex, dualSimplex, barrier, unknownMethod };

  //
  // TODO: clarify the semantics of this class w.r.t. the OSI's states:
  //	abandoned
  //	provenOptimal
  //	provenPrimalInfeasible
  //	provenDualInfeasible
  //	primalObjectiveLimitReached
  //	dualObjectiveLimitReached
  //	iterationLimitReached
  //
  enum problemState
    {
      unsolved=0,    // Solution unknown or problem changed since last solve.
      suspended=1,   // The iteration limit was reached.
      solved=2,      // Solution known.
      infeasible=3,  // Problem proved to be primal infeasible.
      unbounded=4,   // Problem proved to be primal unbounded.
      pastCutoff=5,  // Problem proved to violate cutoff constraint.
               // This will allow us to fathom (can't be better than cutoff)
      broken=6       // Something went badly wrong.
    };


  // For debugging.  This should print what this object thinks are the rows loaded
  // into the solver.  Core rows will print row names.  Cuts will have their unique
  // splay-tree numbers.  We may eventually want to get more sophisticated (e.g. printing
  // only cuts) , but for first testing we need to make sure this set of rows exactly
  // matches with the milp world's thoughts.

void printRowInfo()
  {
    ucout << rowInfo;
  }

 int numRowsManaged()
   {return ((int) rowInfo.size());}

  // We might prefer that this be protected and make MILPProblem a friend.

  void setVariableTypes(BasicArray<variableType> &setFrom)
    {
      if (setFrom.size() != (size_type)getNumCols())
	EXCEPTION_MNGR(std::runtime_error,"PicoLPInterface::setVariableTypes: variable type data of size" << setFrom.size() << "should be " << getNumCols() << "\n");
      vtype.resize(getNumCols());
      vtype << setFrom;
      // It's possible that all the variable types are "continuous", but this will not normally be the case.  From now on,
      // we'll use the vtpe array as if this were a MILP.
      pureLP = false;
    }

  void setSolving() { solving = true;  };
  void setCutting() { solving = false; };

protected:

	///
  void resetInfo()
		{
		rowNameCtr=colNameCtr=0;
		rowInfo.clear();
		colInfo.clear();
		num_core_rows=0;
		}

	///
  int rowNameCtr;
	///
  int colNameCtr;
	/// The name of the LP object
  CharString myName;
	///
  SimpleSplayTree<LPInfoKey> rowInfo;
	///
  SimpleSplayTree<LPInfoKey> colInfo;

	/// 
  solutionMethod solverMethod;
	///
  bool have_basis;
	/// The current state of the LP;
  problemState myState;
	///
  int num_core_rows;
  ///
  int LPdebug;
  ///
  int lpWarnings;

  // Our tolerance for constraint feasibility when we explicitly test a vector
  // for feasibility.  We currently do not coordinate this with tolerances within
  // LP solvers.

  double constraintFeasTolerance;

  /// A flag to indicate this is a pure LP.  PICO can be used just as an interface to an LP solver
  /// via OSI.  In this case, the various MILP-related data structures will not be initialized.

  bool pureLP;

  /// A flag that's true when we're solving and false when we're cutting.  In the former case,
  /// the problem is a linear program and in the latter it's a mixed-integer program.

  bool solving;

  /// Duplicate the vtype array from MILPProblem for more efficient query service during cut generation
  BasicArray<variableType>   vtype;

  /// Constant offset for the objective
  double objConst;

public:

	///
  int status;

	/// Set the class name
  virtual void setName(const char* newName)
		{
		if (newName == NULL)
		   myName = "";
		else if (myName != newName)
		   myName = newName;
		}
	/// Get the class name
  virtual CharString& name()
		{return myName;}


  ///@name Solve methods 
  //@{
    ///
    virtual void initialize()
    		{}
    /// If you know the primal is bounded (e.g. at a non-root node of a branch-and-bound tree),
    /// say primalBounded is true.  This allows easy interpretation when the Osi solver interface
    /// only have proven dual information.
    virtual void solve(bool knowPrimalBounded = false);

    /// Indicate the algorithm used for solving the LP.
    virtual solutionMethod getLPMethod()
		{return solverMethod;}
    
    /// Set the algorithm used for solving the LP.
    virtual void setLPMethod(solutionMethod method)
		{solverMethod = method;}

    /** Get the pricing rule.  These apply to both primal and dual
	simplex.  Many LP packages have separate parameters for 
	primal and dual pricing, but this parameter applies to both primal
	and dual. */
    virtual priceRule getPricingRule() const = 0;

    /** Set the pricing rule. */
    virtual void setPricingRule(priceRule newRule) = 0;

    /** Get the iteration limit. The default is 0, no limit. */
    virtual int getIterationLimit()
		{
		int tmp;
		getIntParam(OsiMaxNumIteration,tmp);
		return tmp;
		}

    /** Set the iteration limit. A limit <= 0 is interpreted as no limit. */
    virtual void setIterationLimit(const int newLimit)
		{
		setIntParam(OsiMaxNumIteration,newLimit);
		setIntParam(OsiMaxNumIterationHotStart,newLimit);
		}

    /** Get the runtime limit (in seconds). */
    virtual double getTimeLimit() const = 0;

    /** Set the runtime limit (in seconds). */
    virtual void setTimeLimit(double newLimit) = 0;

    /** Get the clock type */
    virtual clockType getClockType() const = 0;

    /** Set the clock type */
    virtual void setClockType(clockType newType) = 0;

    /// Get the primal cutoff value.  
    virtual double getPrimalCutoff()
		{
		double tmp;
		getDblParam(OsiPrimalObjectiveLimit,tmp);
		return tmp;
		}

    /// Set the primal cutoff value.
    virtual void setPrimalCutoff(const double newCutoff)
		{
		setDblParam(OsiPrimalObjectiveLimit,newCutoff);
		}

    /// Get the dual cutoff value.  
    virtual double getDualCutoff()
		{
		double tmp;
		getDblParam(OsiDualObjectiveLimit,tmp);
		return tmp;
		}

    /// Set the dual cutoff value.
    virtual void setDualCutoff(const double newCutoff)
		{
		setDblParam(OsiDualObjectiveLimit,newCutoff);
		}

    /** */
    virtual void setBasis(BasisArray& basisBuffer) = 0;

          ///
    // This sets the core variables from the basis.  All the slack variables for the
    // additional cuts are set to nonbasic except for the basic ones (from degenerate
    // binding or nonbinding cuts).  basic_list is a list of the indices (in cutlist)
    // of the cuts with basic slack variables.
    virtual void setBasisWithCuts(BasicArray<picoRowCut*>& cutlist1, 
				  BasicArray<int>& basic_list1,
				  BasicArray<picoRowCut*>& cutlist2, 
				  BasicArray<int>& basic_list2,
				  BasisArray& basisBuffer) = 0;




  //@}


  //---------------------------------------------------------------------------
  ///@name Methods returning info on how the solution process terminated
  //@{
    ///
    virtual problemState state() {return myState;}
  //@}

  //---------------------------------------------------------------------------
    /// Is primal unboundedness proven?
    virtual bool isProvenPrimalUnbounded() const = 0;

    /// Is dual unboundedness proven?
    virtual bool isProvenDualUnbounded() const = 0;

    /// Has the time limit been reached?
    virtual bool isTimeLimitReached() const = 0;

  //---------------------------------------------------------------------------
  /**@name Problem information methods 
     
     These methods call the solver's query routines to return
     information about the problem referred to by the current object.
     Querying a problem that has no data associated with it result in
     zeros for the number of rows and columns, and NULL pointers from
     the methods that return vectors.
     
     Const pointers returned from any data-query method are valid as
     long as the data is unchanged and the solver is not called.
  */
  //@{
    /**@name Methods related to querying the input data */
    //@{
      ///
      CharString& getVariableName(int whichVariable)
		{
		SimpleSplayTreeItem<LPInfoKey>* item =
			colInfo.find_rank(whichVariable);
		if (item) {
		   if (! item->key().data()) {
		      item->key().data() = new CharString;
		      *(item->key().data()) = "var";
		      *(item->key().data()) += item->key().key();
		      }
		   return *(item->key().data());
		   }
		EXCEPTION_MNGR(std::runtime_error,"PicoLPInterface::getVariableName -- Bad name index " << whichVariable);
		return *(item->key().data());		// dummy stmt
		}

      ///
      void getVariableNames(BasicArray<CharString>& names)
		{
		size_type size=colInfo.size();
		names.resize(size);
		for (size_type i=0; i<size; i++) {
		  SimpleSplayTreeItem<LPInfoKey>* item =
			colInfo.find_rank(i);
		  if (item) {
		     if (item->key().data())
		        names[i] = *(item->key().data());
		     else {
		        CharString tmp = "var";
		        tmp += item->key().key();
		        names[i] = tmp;
		        }
		     }
		  else
		     EXCEPTION_MNGR(std::runtime_error,"PicoLPInterface::getVariableNames -- Bad index " << i);
		  }
		}

      ///
      CharString getRowName(int whichConstraint)
		{
		SimpleSplayTreeItem<LPInfoKey>* item =
			rowInfo.find_rank(whichConstraint);
		if (item) {
		   if (! item->key().data()) {
		      item->key().data() = new CharString;
		      *(item->key().data()) = "constraint";
		      *(item->key().data()) += item->key().key();
		      }
		   return *(item->key().data());
		   }
		EXCEPTION_MNGR(std::runtime_error, "PicoLPInterface::getRowName -- Bad name index " << whichConstraint);
		return *(item->key().data());		// dummy stmt
		}

      /// 
      void getRowNames(BasicArray<CharString>& names)
		{
		size_type size=rowInfo.size();
		names.resize(size);
		for (size_type i=0; i<size; i++) {
		  SimpleSplayTreeItem<LPInfoKey>* item =
			rowInfo.find_rank(i);
		  if (item) {
		     if (item->key().data())
		        names[i] = *(item->key().data());
		     else {
		        CharString tmp = "constraint";
		        tmp += item->key().key();
		        names[i] = tmp;
		        }
		     }
		  else
		     EXCEPTION_MNGR(std::runtime_error,"PicoLPInterface::getRowNames -- Bad index " << i);
		  }
		}

      ///
      virtual double getColLower(int whichVariable) const = 0;

      ///
      virtual void getColLower(int whichVariable, Ereal<double>& val) const
		{
		double tmp = getColLower(whichVariable);
		if (tmp == -getInfinity())
		   val = Ereal<double>::negative_infinity;
		else
		   val = tmp;
		}

      ///
      virtual const double* getColLower() const = 0;

      ///
      virtual void getColLower(double* lowerBounds, int* indexList, int len) const
		{
		for (int i=0; i<len; i++)
                  lowerBounds[i] = getColLower(indexList[i]);
		}
		

      ///
      virtual void getColLower(double* lowerBounds, int first=0, int last=-1) const
		{
		if (last==-1) last = getNumCols()-1;
		for (int i=first; i<=last; i++)
                  *(lowerBounds++) = getColLower(i);
		}

      ///
      virtual void getColLower(BasicArray<Ereal<double> >& array) const
		{
		int last=getNumCols();
		for (int i=0; i<last; i++)
                  array[i] = as_ereal(getColLower(i));
		}

      ///
      virtual const double* getColUpper() const = 0;

      ///
      virtual double getColUpper(int whichVariable) const = 0;

      ///
      virtual void getColUpper(int whichVariable, Ereal<double>& val) const
		{
		double tmp = getColUpper(whichVariable);
		if (tmp == -getInfinity())
		   val = Ereal<double>::negative_infinity;
		else
		   val = tmp;
		}

      ///
      virtual void getColUpper(double* upperBounds, int* indexList, int len) const
		{
		for (int i=0; i<len; i++)
                  upperBounds[i] = getColUpper(indexList[i]);
		}

      ///
      virtual void getColUpper(double* upperBounds, int first=0, int last=-1) const
		{
		if (last==-1) last=getNumCols()-1;
		for (int i=first; i<=last; i++)
                  *(upperBounds++) = getColUpper(i);
		}

      ///
      virtual void getColUpper(BasicArray<Ereal<double> >& array) const
		{
		int last=getNumCols();
		for (int i=0; i<last; i++)
                  array[i] = as_ereal(getColUpper(i));
		}

      ///
      virtual double getRowLower(int whichVariable) const = 0;

      ///
      virtual const double* getRowLower() const = 0;

      ///
      virtual void getRowLower(double* lowerBounds, int* indexList, int len) const
		{
		for (int i=0; i<len; i++)
                  lowerBounds[i] = getRowLower(indexList[i]);
		}
		

      ///
      virtual void getRowLower(double* lowerBounds, int first=0, int last=-1) const
		{
		if (last==-1) last = getNumRows()-1;
		for (int i=first; i<=last; i++)
                  *(lowerBounds++) = getRowLower(i);
		}

      ///
      virtual void getRowLower(BasicArray<Ereal<double> >& array) const
		{
		int last=getNumRows();
		for (int i=0; i<last; i++)
                  array[i] = as_ereal(getRowLower(i));
		}

      ///
      virtual const double* getRowUpper() const = 0;

      ///
      virtual double getRowUpper(int whichVariable) const = 0;

      ///
      virtual void getRowUpper(double* upperBounds, int* indexList, int len) const
		{
		for (int i=0; i<len; i++)
                  upperBounds[i] = getRowUpper(indexList[i]);
		}

      ///
      virtual void getRowUpper(double* upperBounds, int first=0, int last=-1) const
		{
		if (last==-1) last=getNumRows()-1;
		for (int i=first; i<=last; i++)
                  *(upperBounds++) = getRowUpper(i);
		}

      ///
      virtual void getRowUpper(BasicArray<Ereal<double> >& array) const
		{
		int last=getNumRows();
		for (int i=0; i<last; i++)
                  array[i] = as_ereal(getRowUpper(i));
		}

      ///
      virtual constraintSense getRowSense(int whichConstraint) const = 0;

      // TODO: This seems very inefficient.  Also is relying on coersion from constraintSense.
      ///
      virtual void getRowSense(char* constr, int first=0, int last=-1) const;
  
      ///
      virtual const double* getRightHandSide() const = 0;

      ///
      virtual double getRightHandSide(int whichConstraint) const = 0;

      ///
      virtual void getRightHandSide(double* rhs, int first=0, int last=-1) const
		{
		if (last==-1) last=getNumRows()-1;
		for (int i=first; i<=last; i++)
                  *(rhs++) = getRightHandSide(i);
		}

      ///
      virtual void getRightHandSide(BasicArray<Ereal<double> >& array) const
		{
		int last=getNumRows();
		for (int i=0; i<last; i++)
		  array[i] = as_ereal(getRightHandSide(i));
		}

      ///
      virtual const double* getObjCoefficients() const = 0;

      ///
      virtual double getObjCoefficient(int whichVariable) const = 0;

      ///
      virtual void getObjCoefficients(double* coef, int first=0, int last=-1) const
		{
		if (last==-1) last=getNumCols()-1;
		for (int i=first; i<=last; i++)
                  *(coef++) = getObjCoefficient(i);
		}

      ///
      virtual void getObjCoefficients(BasicArray<Ereal<double> >& array) const
		{
		int last=getNumCols();
		for (int i=0; i<last; i++)
                  array[i] = as_ereal(getObjCoefficient(i));
		}

      ///
      virtual const CoinPackedMatrix * getMatrixByRow() const = 0;

      ///
      virtual const CoinPackedMatrix * getMatrixByCol() const = 0;
 
      ///
      virtual void getMatrixByRow(RMSparseMatrix<Ereal<double> >& mat) const;

      ///
      virtual void getMatrixByCol(CMSparseMatrix<Ereal<double> >& mat) const;

      /// Returns true if vecToCheck is feasible for this LP problem (satisfies Ax=b,
      /// or Ax <= b, etc) within tolerance.  Returns false if infeasible.
      virtual bool checkFeasibility(DoubleVector & vecToCheck);

      /// Get the LP information all at once
      virtual void getInfo(BasicArray<Ereal<double> >& objCoefs, 
			CMSparseMatrix<Ereal<double> >& mat,
			CharString& sense, 
			BasicArray<Ereal<double> >& collb, 
			BasicArray<Ereal<double> >& colub,
			BasicArray<Ereal<double> >& rowlb, 
			BasicArray<Ereal<double> >& rowub) const;
    //@}
    
    /**@name Methods related to querying the solution */
    //@{
      /// 
      virtual double getColSolution(const int whichCol) const = 0;

      /// 
      virtual const double * getColSolution() const  = 0;
  
      /// 
      virtual void getColSolution(double* solBuffer, int howMany, 
				int* colIndeces) const
		{
		for (int i=0; i<howMany; i++)
		  *(solBuffer++) = getColSolution(colIndeces[i]);
		}

      ///
      virtual void getColSolution(double* solBuffer, int first=0, int last=-1) const
		{
		if (last==-1) last = getNumCols()-1;
		for (int i=first; i<=last; i++)
	          *(solBuffer++) = getColSolution(i);
		}

      ///
      virtual double getRowPrice(const int whichRow) const = 0;

      ///
      virtual const double * getRowPrice() const = 0;
  
      ///
      virtual void getRowPrice(double* solBuffer, int howMany, 
				int* colIndeces) const
		{
		for (int i=0; i<howMany; i++)
		  *(solBuffer++) = getRowPrice(colIndeces[i]);
		}

      ///
      virtual void getRowPrice(double* solBuffer, int first=0, int last=-1) const
		{
		if (last == -1) last = getNumRows()-1;
		for (int i=first; i<=last; i++)
		  *(solBuffer++) = getRowPrice(i);
		}

      ///
      virtual double getRowPrice(picoRowCut& cut) = 0;

      ///
      virtual double getReducedCost(const int whichCol) const = 0;

      /// 
      virtual const double * getReducedCost() const = 0;

      /// 
      virtual void getReducedCost(double* solBuffer, int howMany, 
				int* colIndeces) const
		{
		for (int i=0; i<howMany; i++)
		  *(solBuffer++) = getReducedCost(colIndeces[i]);
		}

      ///
      virtual void getReducedCost(double* solBuffer, int first=0, int last=-1) const
		{
		if (last==-1) last=getNumCols()-1;
		for (int i=first; i<=last; i++)
		  *(solBuffer++) = getReducedCost(i);
		}

      ///
      virtual bool basisAvailable() const
		{return have_basis;}

      ///
      virtual basisState getBasis(int whichVariable) const = 0;

      /** There are getNumCols()+getNumRows() basis indicators.
	  The first getNumCols() of them give basis states for the 
	  variable.  The remainder give basis states for the row slack
	  variables.  The states are given by an enum that follows the
	  typical conventions. */
      // TODO: make this more efficient in derived classes.  This will
      // allocate a new array call cplex to fill it, and deallocate it
      // (#rows + # cols) times
      // Note: if this method (or basis information in general) changes, check
      // the driver code for saving the root solution
      virtual void getBasis(BasisArray& basisBuffer) const
		{
		basisBuffer.resize(getNumCols()+getNumRows());
		for (size_type i=0; i<basisBuffer.size(); i++)
		  basisBuffer.put(i,getBasis(i));
		}

      ///
      virtual basisState getBasis(picoRowCut& cut) = 0;

      ///
      virtual double getSlackVal(picoRowCut& cut) = 0;

      ///
      double getObjValue() const
		{
		return OsiSolverInterface::getObjValue() + objConst;
		}

      ///
      double evalVector(BasicArray<double>& testvec);
    //@}
  //@}

  //---------------------------------------------------------------------------

  /**@name Problem modifying methods */
  //@{
    //-------------------------------------------------------------------------
    /**@name Changing names of variables and constraints */
    //@{
      ///
      void setVariableName(int whichVariable)
		{
		CharString name = "col";
		name += whichVariable;
		setVariableName(whichVariable,name);
		}

      ///
      void setVariableName(int whichVariable, const CharString& newName)
		{
		SimpleSplayTreeItem<LPInfoKey>* item =
			colInfo.find_rank(whichVariable);
		if (item) {
		   if (!(item->key().data()))
		      item->key().data() = new CharString;
		   *(item->key().data()) = newName;
		   }
                else
		   EXCEPTION_MNGR(std::runtime_error,"PicoLPInterface::setVariableName -- Bad name index " << whichVariable);
		}

      ///
      void setVariableNames(const BasicArray<CharString>& names);

      ///
      void setRowName(int whichConstraint)
		{
		CharString name = "col";
		name += whichConstraint;
		setRowName(whichConstraint,name);
		}

      /// 
      void setRowName(int whichConstraint, const CharString& newName)
		{
		SimpleSplayTreeItem<LPInfoKey>* item =
			rowInfo.find_rank(whichConstraint);
		if (item) {
		   if (!(item->key().data()))
		      item->key().data() = new CharString;
		   *(item->key().data()) = newName;
		   }
		else
		   EXCEPTION_MNGR(std::runtime_error, "PicoLPInterface::setRowName -- Bad name index " << whichConstraint);
		}

      ///
      void setRowNames(const BasicArray<CharString>& names);

    //@}

    /**@name Changing bounds on variables and constraints */
    //@{
      ///
      virtual void setColLower(int index, double value) = 0;

      /// 
      virtual void setColUpper(int index, double value) = 0;

      ///
      virtual void setColLower(double* lowerBounds, int* indexList, int len)
		{
		for (int i=0; i<len; i++)
		  setColLower(indexList[i], *(lowerBounds++));
		}

      ///
      virtual void setColUpper(double* upperBounds, int* indexList, int len)
		{
		for (int i=0; i<len; i++)
		  setColUpper(indexList[i], *(upperBounds++));
		}

      ///
      virtual void setColLower(double* lowerBounds, int first=0, int last=-1)
		{
		if (last==-1) last=getNumCols()-1;
		for (int i=first; i<=last; i++)
		  setColLower(i,*(lowerBounds++));
		}

      ///
      virtual void setColUpper(double* upperBounds, int first=0, int last=-1)
		{
		if (last==-1) last=getNumCols()-1;
		for (int i=first; i<=last; i++)
		  setColUpper(i,*(upperBounds++));
		}

      /// A revised 'default' method, which makes sure that the range is always nonnegative
      virtual void setRowBounds(int element_ndx, double lower, double upper)
		{
		if (getRowUpper(element_ndx) < upper) {
		   setRowUpper(element_ndx, upper);
		   setRowLower(element_ndx, lower);
		   }
		else {
		   setRowLower(element_ndx, lower);
		   setRowUpper(element_ndx, upper);
		   }
		}

      /// Change a single coefficient in the matrix
      /// Be careful. Not well supported across multiple solvers.
      virtual void setMatrixCoeff(int row, int col, double newValue)
	{
	EXCEPTION_MNGR(std::runtime_error, "Setting matrix coefficients isn't implemented for this LP interface");
	}

      /// Does this function really make sense in all caes????  
      virtual void setRowSense(int whichConstraint, constraintSense type);
    //@}
    
    //-------------------------------------------------------------------------
	/// Set the optimize sense
    virtual void setObjSense(pebbl::optimizeSense minOrMax)
		{setObjSense((double)minOrMax); myState=unsolved;}
    
    //-------------------------------------------------------------------------
    /**@name Methods to expand a problem.<br>
       Note that if a column is added then by default it will correspond to a
       continuous variable. */
    //@{
      /** */
      void addCol(const CharString& name, const CoinPackedVectorBase& vec,
			  const double collb, const double colub,   
			  const double obj)
		{
		size_type size = colInfo.size();
		//if ((int) size != getNumCols())
		  //Warning(errmsg("colInfo.size()=%d getNumCols()=%d",
				 //size,getNumCols()));
		execAddCol(vec, collb, colub, obj);
		addColInfo(size, &name);
		}

      /** */
      void addCol(const CoinPackedVectorBase& vec,
			  const double collb, const double colub,   
			  const double obj)
		{
		size_type size = colInfo.size();
		//if ((int) size != getNumCols())
		  //Warning(errmsg("colInfo.size()=%d getNumCols()=%d",
				 //size,getNumCols()));
		execAddCol(vec, collb, colub, obj);
		addColInfo(size);
		}

      /** */
      void addCols(const BasicArray<CharString>& name, const int numcols,
			   const CoinPackedVectorBase * const * cols,
			   const double* collb, const double* colub,   
			   const double* obj)
		{
		size_type size = colInfo.size();
		//if ((int) size != getNumCols())
		  //Warning(errmsg("colInfo.size()=%d getNumCols()=%d",
				 //size,getNumCols()));
		execAddCols(numcols, cols, collb, colub, obj);
		addColInfo(size,numcols,&name);
		}

      /** */
      void addCols(const int numcols,
			   const CoinPackedVectorBase * const * cols,
			   const double* collb, const double* colub,   
			   const double* obj)
		{
		size_type size = colInfo.size();
		//if ((int) size != getNumCols())
		  //Warning(errmsg("colInfo.size()=%d getNumCols()=%d",
				 //size,getNumCols()));
		execAddCols(numcols, cols, collb, colub, obj);
		addColInfo(size,numcols);
		}

      /** */
      void addCols(const BasicArray<CharString>& name, 
			const BasicArray<CoinPackedVectorBase*>& cols,
			const BasicArray<double>& collb, 
			const BasicArray<double>& colub,   
			const BasicArray<double>& obj)
		{
		size_type size = colInfo.size();
		size_type numcols=cols.size();
		//if ((int) size != getNumCols())
		  //Warning(errmsg("colInfo.size()=%d getNumCols()=%d",
				 //size,getNumCols()));
		for (size_type i=0; i<numcols; i++)
		  execAddCol(*(cols[i]), collb[i], colub[i], obj[i]);
		addColInfo(size,cols.size(),&name);
		}

      /** */
      void addCols(const BasicArray<CoinPackedVectorBase*>& cols,
			const BasicArray<double>& collb, 
			const BasicArray<double>& colub,   
			const BasicArray<double>& obj)
		{
		size_type size = colInfo.size();
		//if ((int) size != getNumCols())
		  //Warning(errmsg("colInfo.size()=%d getNumCols()=%d",
				 //size,getNumCols()));
		size_type numcols=cols.size();
		for (size_type i=0; i<numcols; i++)
		  execAddCol(*(cols[i]), collb[i], colub[i], obj[i]);
		addColInfo(size,cols.size());
		}

      /** */
      virtual void deleteCol(const int whichColumn)
		{ deleteCols(1,&whichColumn); }

      ///
      void deleteCols(const int num, const int * colIndices)
		{
		size_type size = colInfo.size();
		execDeleteCols(num, colIndices);
		delColInfo(size,num,colIndices);
		}

      /** */
      void deleteCols(const int first, const int last)
		{ for (int i=last; i>=first; i--) deleteCol(i); }
    
      /** */
      void addRow(const CoinPackedVectorBase& vec,
    			  const double rowlb, const double rowub)
		{
		size_type size = rowInfo.size();
		execAddRow(vec, rowlb, rowub);
		addRowInfo(size);
		}

      /** */
      virtual void addRow(const CharString& name, 
			const CoinPackedVectorBase& vec,
    			const double rowlb, const double rowub)
		{
		size_type size = rowInfo.size();
		execAddRow(vec, rowlb, rowub);
		addRowInfo(size,&name);
		}

      /** */
      void addRow(const CoinPackedVectorBase& vec,
    			  const char rowsen, const double rowrhs,   
    			  const double rowrng)
		{
		size_type size = rowInfo.size();
		execAddRow(vec, rowsen, rowrhs, rowrng);
		addRowInfo(size);
		}

      /** */
      virtual void addRow(const CharString& name, 
			const CoinPackedVectorBase& vec,
    			const char rowsen, const double rowrhs,   
    			const double rowrng)
		{
		size_type size = rowInfo.size();
		execAddRow(vec, rowsen, rowrhs, rowrng);
		addRowInfo(size,&name);
		}

      /** */
      void addRows(const int numrows,
			const CoinPackedVectorBase * const * rows,
			const double* rowlb, 
			const double* rowub)
		{
		size_type size = rowInfo.size();
		execAddRows(numrows, rows, rowlb, rowub);
		addRowInfo(size,numrows);
		}

      /** */
      virtual void addRows(const BasicArray<CharString>& names, 
			const int numrows,
			const CoinPackedVectorBase * const * rows,
			const double* rowlb, 
			const double* rowub)
		{
		size_type size = rowInfo.size();
		execAddRows(numrows, rows, rowlb, rowub);
		addRowInfo(size,numrows,&names);
		}

      /** */
      void addRows(const int numrows,
			const CoinPackedVectorBase * const * rows,
    			const char* rowsen, const double* rowrhs,   
    			const double* rowrng)
		{
		size_type size = rowInfo.size();
		execAddRows(numrows, rows, rowsen, rowrhs, rowrng);
		addRowInfo(size,numrows);
		}

      /** */
      virtual void addRows(const BasicArray<CharString>& names, 
			const int numrows,
			const CoinPackedVectorBase * const * rows,
    			const char* rowsen, const double* rowrhs,   
    			const double* rowrng)
		{
		size_type size = rowInfo.size();
		execAddRows(numrows, rows, rowsen, rowrhs, rowrng);
		addRowInfo(size,numrows,&names);
		}


      /** */
      virtual void addRows(const BasicArray<CoinPackedVectorBase*>& rows,
			const BasicArray<double>& rowlb, 
			const BasicArray<double>& rowub)
		{
		size_type size = rowInfo.size();
		for (size_type i=0; i<rows.size(); i++)
		  execAddRow(*(rows[i]), rowlb[i], rowub[i]);
		addRowInfo(size,rows.size());
		}

      /** */
      virtual void addRows(const BasicArray<CharString>& names, 
			const BasicArray<CoinPackedVectorBase*>& rows,
			const BasicArray<double>& rowlb, 
			const BasicArray<double>& rowub)
		{
		size_type size = rowInfo.size();
		size_type numrows=rows.size();
		for (size_type i=0; i<numrows; i++)
		  execAddRow(*(rows[i]), rowlb[i], rowub[i]);
		addRowInfo(size,numrows,&names);
		}

      /** */
      virtual void addRows(const BasicArray<CoinPackedVectorBase*>& rows,
    			const BasicArray<char>& rowsen,
			const BasicArray<double>& rowrhs,
    			const BasicArray<double>& rowrng)
		{
		size_type size = rowInfo.size();
		size_type numrows=rows.size();
		for (size_type i=0; i<numrows; i++)
		  execAddRow(*(rows[i]), rowsen[i], rowrhs[i], rowrng[i]);
		addRowInfo(size,numrows);
		}

      /** */
      virtual void addRows(const BasicArray<CharString>& names, 
			const BasicArray<CoinPackedVectorBase*>& rows,
    			const BasicArray<char>& rowsen,
			const BasicArray<double>& rowrhs,
    			const BasicArray<double>& rowrng)
		{
		size_type size = rowInfo.size();
		size_type numrows=rows.size();
		for (size_type i=0; i<numrows; i++)
		  execAddRow(*(rows[i]), rowsen[i], rowrhs[i], rowrng[i]);
		addRowInfo(size,numrows,&names);
		}

      ///
      virtual void addRows(BasicArray<picoRowCut*>& cutlist, int num_rows)
		{
		execAddRows(cutlist, num_rows);
		}

      ///

      virtual void replaceRows(BasicArray<picoRowCut*>& cutlist) = 0;

      ///
      virtual void deleteRow(const int whichRow)
		{ deleteRows(1,&whichRow); }

      ///
      void deleteRows(const int num, const int * rowIndices)
		{
		size_type size = rowInfo.size();
		execDeleteRows(num, rowIndices);
		delRowInfo(size,num,rowIndices);
		}

      ///
      virtual void deleteRows(const int first, const int last)
		{ for (int i=last; i>=first; i--) deleteRow(i); }

      ///
      virtual void deleteRows(BasicArray<picoRowCut*>& cutlist)
	{
	size_type num_to_delete=cutlist.size();
	IntVector rankList(num_to_delete);
	getRanks(cutlist,rankList);
	deleteRows(num_to_delete, rankList.data());
	}

    //@}
  //@}

  //---------------------------------------------------------------------------

  /**@name Methods to input a problem */
  //@{

    ///
    virtual void reset()
		{
		have_basis=false;
		status=0;
		myState=unsolved;
		}

    ///
    virtual void testLP();

    ///
    void updateProblem( BasicArray<Ereal<double> >& collb, 
			BasicArray<Ereal<double> >& colub,   
			BasicArray<Ereal<double> >& rowlb, 
			BasicArray<Ereal<double> >& rowub);

    ///
    virtual void loadProblem(CMSparseMatrix<Ereal<double> >& matrix,
			BasicArray<Ereal<double> >& collb, 
			BasicArray<Ereal<double> >& colub,   
			BasicArray<Ereal<double> >& obj,
			BasicArray<Ereal<double> >& rowlb, 
			BasicArray<Ereal<double> >& rowub,
			BasicArray<CharString>& rNames, 
			BasicArray<CharString>& cNames);

    ///
    virtual void loadProblem(CMSparseMatrix<Ereal<double> >& matrix,
			BasicArray<Ereal<double> >& collb, 
			BasicArray<Ereal<double> >& colub,   
			BasicArray<Ereal<double> >& obj,
			BasicArray<Ereal<double> >& rowlb, 
			BasicArray<Ereal<double> >& rowub);

    /** */
    virtual void loadProblem(const CoinPackedMatrix& matrix,
			     const double* collb, const double* colub,   
			     const double* obj,
			     const double* rowlb, const double* rowub)
		{PicoLPInterface::loadProblem(matrix,collb,colub,obj,rowlb,rowub,0,0);}

    /** */
    virtual void loadProblem(const CoinPackedMatrix& matrix,
			     const double* collb, const double* colub,   
			     const double* obj,
			     const double* rowlb, const double* rowub,
			     CharString* rNames, CharString* cNames);
			    
    /** */
    virtual void loadProblem(const CoinPackedMatrix& matrix,
			     const double* collb, const double* colub,
			     const double* obj,
			     const char* rowsen, const double* rowrhs,   
			     const double* rowrng)
		{PicoLPInterface::loadProblem(matrix,collb,colub,obj,rowsen,rowrhs,rowrng,0,0);}

    /** */
    virtual void loadProblem(const CoinPackedMatrix& matrix,
			     const double* collb, const double* colub,
			     const double* obj,
			     const char* rowsen, const double* rowrhs,   
			     const double* rowrng,
			     CharString* rNames, CharString* cNames);

    ///
    void loadProblem(const int numcols, const int numrows,
                             const int* start, const int* index,
                             const double* value,
                             const double* collb, const double* colub,
                             const double* obj,
                             const double* rowlb, const double* rowub);

    ///
    void loadProblem(const int numcols, const int numrows,
                             const int* start, const int* index,
                             const double* value,
                             const double* collb, const double* colub,
                             const double* obj,
                             const char* rowsen, const double* rowrhs,
                             const double* rowrng);

    /** Read the LP from a file */
    virtual void loadProblem(const CharString& filename, 
					BasicArray<int>& listOfIntegersP,
					ilp_format format=unknown_format);

    /** Read the LP from a file */
    virtual void loadProblem(const CharString& filename1, 
			     const CharString& filename2,
					BasicArray<int>& listOfIntegersP,
					ilp_format format=unknown_format);

    /** Read the LP from a PackBuffer object. */
    virtual void loadProblem(UnPackBuffer& inBuffer);

    /** Read an mps file from the given filename */
    virtual int readMps(const char* filename,
			 const char *extension = "mps");

    /** Write the LP into the given ostream with specified format. */
    void write(std::ostream& ostr, ilp_format format=unknown_format,
			const char* extension="");

    /** Like the write above, but specifically for producing a file.  Caution:
	haven't checked this for parallel.  Write the lp in the given format
        into the file called filename */

    virtual void write(char *filename, ilp_format format=unknown_format);

    /** Write the LP into a PackBuffer object. */
    virtual void write(PackBuffer& outBuffer) const;

/** Print out LP-specific error information for broken instances */
    virtual void printBrokenInfo() {};

  //@}

  //---------------------------------------------------------------------------

  ///@name Constructors and destructors
  //@{
    /// 
    PicoLPInterface()
	: OsiSolverInterface(),
	  solverMethod(unknownMethod),
	  LPdebug(0),
	  lpWarnings(2),
      solving(false),
      pureLP(true),
      constraintFeasTolerance(1e-7)
	{
	create_parameter("LPdebug",LPdebug,
		"<int>","0",
		"Controls the debugging level used within the LP",
		utilib::ParameterNonnegative<int>());
	create_parameter("lpWarnings",lpWarnings,
		"<int>","2",
		"Controls the warnings level used within the LP",
		utilib::ParameterNonnegative<int>());
	create_parameter("constraintFeasTolerance", constraintFeasTolerance,
			 "<double>", "1e-07", "tolerance for constraint (in)feasibility when we test the feasibility of a vector", utilib::ParameterNonnegative<double>());
	resetInfo();
	reset();
	}

    
    /// Copy constructor 
    PicoLPInterface(const PicoLPInterface &);
  
    /// Assignment operator 
    PicoLPInterface& operator=(const PicoLPInterface& rhs);

    // This was supposed to clone the OSI part of an interface
    // ... but only worked on some systems!
    // virtual OsiSolverInterface* OsiOnlyClone(bool copyData = true) const = 0;

    /// Copy the OSI part of an interface (may not be complete?)
    virtual OsiSolverInterface* OsiOnlyCopy() const = 0;
  
    /// 
    virtual ~PicoLPInterface () {}
  //@}

   /// Some debugging stuff added by JE to track down horrible splay tree bug

   void dumpColInfo(const char* tag) 
     { 
       ucout << tag << "-- dumpColInfo: colInfo.size()=" 
	     << colInfo.size() << ", getNumCols()=" << getNumCols() << std::endl;
       ucout << colInfo;
       ucout << std::endl; 
     };

   void checkColInfo(const char* tag)
     {
       if ((int) colInfo.size() != getNumCols())
	 {
	   ucout << "CheckColInfo failed!  ";
	   dumpColInfo(tag);
	   EXCEPTION_MNGR(std::runtime_error, "checkColInfo - colInfo.size() != getNumCols()");
	 }
     };

  //---------------------------------------------------------------------------


protected:

    ///
    void gutsOfCopy(const PicoLPInterface& lp);

    ///
    virtual void getRanks(BasicArray<picoRowCut *>& cutlist, IntVector &rankList) = 0;

    ///
    virtual void execDeleteRows(const int num, const int * rowIndices) = 0;

    ///
    virtual void execDeleteCols(const int num, const int * colIndices) = 0;

    ///
    virtual void execAddCol(const CoinPackedVectorBase& vec,
			  const double collb, const double colub,   
			  const double obj) = 0;

    ///
    virtual void execAddCols(const int numcols,
			   const CoinPackedVectorBase * const * cols,
			   const double* collb, const double* colub,   
			   const double* obj) = 0;

    ///
    virtual void execAddRow(const CoinPackedVectorBase& vec,
    			  const double rowlb, const double rowub) = 0;

    ///
    virtual void execAddRow(const CoinPackedVectorBase& vec,
    			  const char rowsen, const double rowrhs,   
    			  const double rowrng) = 0;

    ///
    virtual void execAddRows(const int numrows,
			   const CoinPackedVectorBase * const * rows, 
			   const double* rowlb, const double* rowub) = 0;
    ///
    virtual void execAddRows(const int numrows,
			   const CoinPackedVectorBase * const * rows,
    			   const char* rowsen, const double* rowrhs,   
    			   const double* rowrng) = 0;

    ///
    virtual void execAddRows(BasicArray<picoRowCut*>& cutlist, int num_rows) = 0;

    ///
    virtual void execLoadProblem(const CoinPackedMatrix& matrix,
			     const double* collb, const double* colub,   
			     const double* obj,
			     const double* rowlb, const double* rowub) = 0;
			    
    ///
    virtual void execLoadProblem(const CoinPackedMatrix& matrix,
			     const double* collb, const double* colub,
			     const double* obj,
			     const char* rowsen, const double* rowrhs,   
			     const double* rowrng) = 0;

    ///
    virtual void execLoadProblem(const int numcols, const int numrows,
                             const int* start, const int* index,
                             const double* value,
                             const double* collb, const double* colub,
                             const double* obj,
                             const double* rowlb, const double* rowub) = 0;

    ///
    virtual void execLoadProblem(const int numcols, const int numrows,
                             const int* start, const int* index,
                             const double* value,
                             const double* collb, const double* colub,
                             const double* obj,
                             const char* rowsen, const double* rowrhs,
                             const double* rowrng) = 0;

    /// Convert a double to an Ereal, using the LP's notion of infinity
    Ereal<double> as_ereal(double x) const
		{
		double inf = getInfinity();
		if (x == inf)
   		   return Ereal<double>::positive_infinity;
		if (x == -inf)
   		   return Ereal<double>::negative_infinity;
		return Ereal<double>(x);
		}

    /// Convert an Ereal to a double, using the LP's notion of infinity
    double as_double(const Ereal<double>& x) const
		{
		double inf = getInfinity();
		if (x == Ereal<double>::positive_infinity)
   		   return inf;
		if (x == Ereal<double>::negative_infinity)
  		   return -inf;
		return x;
		}

   ///
   void addColInfo(size_type size, const CharString* name=0)
		{
		if ((int)size >= colInfo.size()) {
                   if (name) {
		      LPInfoKey key(colNameCtr++, name);
		      colInfo.add(key);
		      }
                   else {
		      LPInfoKey key(colNameCtr++, 0);
		      colInfo.add(key);
		      }
                   }
		else if (name) {
		   SimpleSplayTreeItem<LPInfoKey>* item =
			colInfo.find_rank(size);
		   if (item) {
		      if (item->key().data())
		         *(item->key().data()) = *name;
		      else
			 item->key().data() = new CharString(*name);
		      }
		   else
		      EXCEPTION_MNGR(std::runtime_error, "PicoLPInterface::AddColinfo -- bad key " << size);
                   }
		}

   ///
   void addColInfo(size_type size, const int num, 
					const BasicArray<CharString>* names=0)
		{
		int j=0;
		for (size_type i=size; i<(size+num); i++, j++) {
		  if (names)
		     addColInfo(i,&((*names)[j]));
		  else
		     addColInfo(i);
		  }
		}


   ///
   void delColInfo(const size_type size, const int num, const int* indeces)
		{
		if (size > (size_type)colInfo.size()) return;
		for (int i=num-1; i>=0; i--) {
		  SimpleSplayTreeItem<LPInfoKey>* item =
					colInfo.find_rank(indeces[i]);
		  if (item) {
		     bool dummy;
		     colInfo.remove(item,dummy);
		     }
		  else
		     EXCEPTION_MNGR(std::runtime_error, "PicoLPInterface::delColinfo -- Bad name index " << indeces[i]);
		  }
		}

   ///
   void addRowInfo(size_type size, const CharString* name=0)
		{
		if (size >= (size_type)rowInfo.size()) {
                   if (name) {
		      LPInfoKey key(rowNameCtr++, name);
		      rowInfo.add(key);
		      }
                   else {
		      LPInfoKey key(rowNameCtr++, 0);
		      rowInfo.add(key);
		      }
                   }
		else if (name) {
		   SimpleSplayTreeItem<LPInfoKey>* item =
			rowInfo.find_rank(size);
		   if (item) {
		      if (item->key().data())
		         *(item->key().data()) = *name;
		      else
			 item->key().data() = new CharString(*name);
		      }
		   else
		      EXCEPTION_MNGR(std::runtime_error, "PicoLPInterface::AddRowinfo -- bad key " << size);
		   }
		}

   ///
   void addRowInfo(size_type size, const int num, 
					const BasicArray<CharString>* names=0)
		{
		int j=0;
		for (size_type i=size; i<(size+num); i++, j++) {
		  if (names)
		     addRowInfo(i,&((*names)[j]));
		  else
		     addRowInfo(i);
		  }
		}


   ///
   void delRowInfo(const size_type size, const int num, const int* indeces)
		{
		if ((int)size > rowInfo.size()) return;
	 	utilib::LinkedList<SimpleSplayTreeItem<LPInfoKey>*> bag;
		for (int i=num-1; i>=0; i--) {
		  SimpleSplayTreeItem<LPInfoKey>* item =
					rowInfo.find_rank(indeces[i]);
		  if (item) {
		     bag.push_back(item);
		     }
		  else
		     EXCEPTION_MNGR(std::runtime_error, "PicoLPInterface::delRowinfo -- Bad index rank " << indeces[i]);
		  }
		utilib::LinkedList<SimpleSplayTreeItem<LPInfoKey>*>::iterator curr = bag.begin();
		utilib::LinkedList<SimpleSplayTreeItem<LPInfoKey>*>::iterator end = bag.end();
		while (curr != end) {
		  bool dummy;
		  rowInfo.remove(*curr,dummy);
		  curr++;
		  }
		}

private:

    /// 
   //MSN: Hmmm, this may have to be commented out too
    virtual void setObjSense(double tmp) = 0;

    ///
    virtual const char * getRowSense() const = 0;

    ///
    void branchAndBound() {}

    ///
    OsiVectorInt getFractionalIndices(const double etol=1.e-05) const 
		{OsiVectorInt tmp; return tmp;}

};


///
inline int compare(const PicoLPInterface::LPInfoKey& key1, const PicoLPInterface::LPInfoKey& key2)
{ return key1.compare(key2); }

} // namespace pico

inline std::ostream& operator<< (std::ostream& ostr, 
				const pico::PicoLPInterface::LPInfoKey print_key)
{
print_key.write(ostr);
return ostr;
}

///
inline std::ostream& operator<< (std::ostream& ostr, 
				const pico::PicoLPInterface::problemState state)
{
switch (state) {
	case pico::PicoLPInterface::unsolved:		ostr << "unsolved";
						break;
	case pico::PicoLPInterface::suspended:	ostr << "suspended";
						break;
	case pico::PicoLPInterface::solved:		ostr << "solved";
						break;
	case pico::PicoLPInterface::infeasible:	ostr << "infeasible";
						break;
	case pico::PicoLPInterface::unbounded:	ostr << "unbounded";
						break;
	case pico::PicoLPInterface::pastCutoff:	ostr << "pastCutoff";
						break;
	case pico::PicoLPInterface::broken:		ostr << "broken";
						break;
	};
return ostr;
}


///
inline std::ostream& operator<< (std::ostream& ostr, 
			const pico::PicoLPInterface::constraintSense data)
{ostr << (char)data; return ostr;}

///
std::istream& operator>> (std::istream& istr, 
			pico::PicoLPInterface::constraintSense& data);

///
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& buf, 
			const pico::PicoLPInterface& lp)
{lp.write(buf); return buf;}

///
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buf, 
			pico::PicoLPInterface& lp)
{lp.loadProblem(buf); return buf;}

#endif
