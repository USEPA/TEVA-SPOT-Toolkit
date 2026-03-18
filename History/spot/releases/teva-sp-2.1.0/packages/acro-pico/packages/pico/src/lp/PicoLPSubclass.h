/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file PicoLPSubclass.h
 *
 * Defines the pico::PicoLPSubclass class.
 */

#ifndef pico_PicoLPSubclass_H
#define pico_PicoLPSubclass_H

#include <acro_config.h>
#include <pico/PicoLPInterface.h>

namespace pico {


///
/// This class provides a template for creating a subclass of PicoLPInterface
/// using multiple inheritance to integrate with an existing LP class
/// in the OsiSolverInterface heirarchy.
///
/// Note:
///
/// 1. This interface is significantly complicated by the fact that 
///	I've done a lot of overloading of the method names in 
///	OsiSolverInterface.  If we had created new names for the
///	methods in PicoLPInterface, this wouldn't have been a very big
///	deal.
///
/// 2. When instantiated, this class defines _all_ abstract virtual methods
///	that are needed.  However, there are several abstract virtual methods 
///	that are defined by PicoLPInterface but which are not in 
///	OsiSolverInterface.  These methods must be defined to provide the
///	full functionality of the PicoLPInterface API.  These methods can
///	be 'instantiated' by the developer through template specialization,
///	which avoids further messy subclassing because of method overloading.
///	See PicoCpxLP.h.
///
template <class T, class V>
class PicoLPSubclass : virtual public OsiSolverInterface, public T,
					public V, virtual public PicoLPInterface
{
public:
      ///
      PicoLPSubclass() {}

      ///
      PicoLPSubclass(const PicoLPSubclass& lp)
		{ *this = lp; }
      ///
      virtual ~PicoLPSubclass() {}

      ///
      void initialize()
	{
	  V::initialize();
	}


      void reset()
	{
	PicoLPInterface::reset();
	// Unfortunately, at least for CLP, resetting the Osi interface throws away the problem
	//	T::reset();
	}

      //  This was not compatible with the ParameterSet parameter mechanism
      //  OsiSolverInterface* clone(bool copyData = true) const
      // 		{return( new PicoLPSubclass<T,V>( *this ) );}

      //  Added this so Mikhail can extract a "clean" OSI object.
      //  Worked on some systems but not others! 
      //  OsiSolverInterface* OsiOnlyClone(bool copyData = true) const
      //     {return T::clone(copyData);}

      /// Makes a copy of an LP object in "clean" OSI format
      /// Possibly this is not complete, but it seems to work for
      /// One application tried so far.

      OsiSolverInterface* OsiOnlyCopy() const
	{
	  OsiSolverInterface* copy = new T;
	  // Dunno whether to use T:: or PicoLPInterface:: below,
	  // or whether it even matters.
	  copy->loadProblem(*(T::getMatrixByRow()),
			    T::getColLower(),
			    T::getColUpper(),
			    T::getObjCoefficients(),
			    T::getRowLower(),
			    T::getRowUpper());
	  // Possibly we need to do more stuff here... guess I'll find out...
	  return copy;
	};

      ///
      double getColLower(int whichVariable) const
                {return T::getColLower()[whichVariable];}

      ///
      void getColLower(int whichVariable, Ereal<double>& val) const
		{PicoLPInterface::getColLower(whichVariable,val);}

      ///
      const double* getColLower() const
                {return T::getColLower();}

      ///
      void getColLower(double* lowerBounds, int* indexList, int len) const
		{PicoLPInterface::getColLower(lowerBounds,indexList,len);}

      ///
      void getColLower(double* lowerBounds, int first, int last) const
		{PicoLPInterface::getColLower(lowerBounds,first,last);}

      ///
      void getColLower(BasicArray<Ereal<double> >& array) const
		{PicoLPInterface::getColLower(array);}

      ///
      const double* getColUpper() const
                {return T::getColUpper();}

      ///
      double getColUpper(int whichVariable) const
                {return T::getColUpper()[whichVariable];}

      ///
      void getColUpper(int whichVariable, Ereal<double>& val) const
		{PicoLPInterface::getColUpper(whichVariable,val);}

      ///
      void getColUpper(double* upperBounds, int* indexList, int len) const
		{PicoLPInterface::getColUpper(upperBounds,indexList,len);}

      ///
      void getColUpper(double* upperBounds, int first, int last) const
		{PicoLPInterface::getColUpper(upperBounds,first,last);}

      ///
      void getColUpper(BasicArray<Ereal<double> >& array) const
		{PicoLPInterface::getColUpper(array);}

      ///
      virtual double getRowLower(int whichVariable) const
                {return T::getRowLower()[whichVariable];}

      ///
      const double* getRowLower() const
                {return T::getRowLower();}

      ///
      void getRowLower(double* lowerBounds, int* indexList, int len) const
		{PicoLPInterface::getRowLower(lowerBounds,indexList,len);}

      ///
      void getRowLower(double* lowerBounds, int first, int last) const
		{PicoLPInterface::getRowLower(lowerBounds,first,last);}

      ///
      void getRowLower(BasicArray<Ereal<double> >& array) const
		{PicoLPInterface::getRowLower(array);}

      ///
      const double* getRowUpper() const
                {return T::getRowUpper();}

      ///
      double getRowUpper(int whichVariable) const
                {return T::getRowUpper()[whichVariable];}

      ///
      void getRowUpper(double* upperBounds, int* indexList, int len) const
		{PicoLPInterface::getRowUpper(upperBounds,indexList,len);}

      ///
      void getRowUpper(double* upperBounds, int first, int last) const
		{PicoLPInterface::getRowUpper(upperBounds,first,last);}

      ///
      void getRowUpper(BasicArray<Ereal<double> >& array) const
		{PicoLPInterface::getRowUpper(array);}
      ///
      const double* getRightHandSide() const
                {return T::getRightHandSide();}

      ///
      double getRightHandSide(int whichConstraint) const
                {return T::getRightHandSide()[whichConstraint];}

      ///
      void getRightHandSide(double* rhs, int begin, int end) const
		{PicoLPInterface::getRightHandSide(rhs,begin,end);}

      ///
      void getRightHandSide(BasicArray<Ereal<double> >& array) const
		{PicoLPInterface::getRightHandSide(array);}

      ///
      const double* getObjCoefficients() const
                {return T::getObjCoefficients();}

      ///
      double getObjCoefficient(int whichVariable) const
                {return T::getObjCoefficients()[whichVariable];}

      ///
      void getObjCoefficients(double* coef, int first, int last) const
                {PicoLPInterface::getObjCoefficients(coef,first,last);}

      ///
      void getObjCoefficients(BasicArray<Ereal<double> >& array) const
                {PicoLPInterface::getObjCoefficients(array);}

      ///
      double getColSolution(const int whichCol) const
                {return T::getColSolution()[whichCol];}

      ///
      const double * getColSolution() const
                {return T::getColSolution();}

      ///
      void getColSolution(double* solBuffer, int howMany, int* colIndeces) const
                {PicoLPInterface::getColSolution(solBuffer,howMany,colIndeces);}

      ///
      void getColSolution(double* solBuffer, int first=0, int last=-1) const
		{PicoLPInterface::getColSolution(solBuffer,first,last);}

      ///
      double getRowPrice(const int whichRow) const
                {return T::getRowPrice()[whichRow];}

      ///
      const double * getRowPrice() const
                {return T::getRowPrice();}

      ///
      void getRowPrice(double* solBuffer, int howMany, int* colIndeces) const
                {PicoLPInterface::getRowPrice(solBuffer,howMany,colIndeces);}

      ///
      void getRowPrice(double* solBuffer, int first=0, int last=-1) const
                {PicoLPInterface::getRowPrice(solBuffer,first,last);}

      ///
      double getRowPrice(picoRowCut& cut)
		{return V::getRowPrice(cut);}

      ///
      double getReducedCost(const int whichCol) const
                {return T::getReducedCost()[whichCol];}

      ///
      const double * getReducedCost() const
                {return T::getReducedCost();}

      ///
      void getReducedCost(double* solBuffer, int howMany,
                                int* colIndeces) const
                {PicoLPInterface::getReducedCost(solBuffer,howMany,colIndeces);}

      ///
      void getReducedCost(double* solBuffer, int first=0, int last=-1) const
                {PicoLPInterface::getReducedCost(solBuffer,first,last);}

      ///
      void getBasis(BasisArray& array) const
		{PicoLPInterface::getBasis(array);}

      ///
      basisState getBasis(picoRowCut& cut)
		{return V::getBasis(cut);}
      ///
      void setColLower(int index, double value)
                {T::setColLower(index,value); myState=unsolved;}

      ///
      void setColUpper(int index, double value)
                {T::setColUpper(index,value); myState=unsolved;}

      /// 
      void setColLower(double* lowerBounds, int* indexList, int len)
		{PicoLPInterface::setColLower(lowerBounds,indexList,len);}

      /// 
      void setColUpper(double* upperBounds, int* indexList, int len)
		{PicoLPInterface::setColUpper(upperBounds,indexList,len);}

      /// 
      void setColLower(double* lowerBounds, int first, int last)
		{PicoLPInterface::setColLower(lowerBounds,first,last);}

      /// 
      void setColUpper(double* upperBounds, int first, int last)
		{PicoLPInterface::setColUpper(upperBounds,first,last);}

      ///
      void setRowLower( int elementIndex, double elementValue )
		{T::setRowLower(elementIndex,elementValue); myState=unsolved;}

      ///
      void setRowUpper( int elementIndex, double elementValue )
		{T::setRowUpper(elementIndex,elementValue); myState=unsolved;}

      ///
      void setRowBounds(int index, double lower, double upper)
		{PicoLPInterface::setRowBounds(index,lower,upper);}

      ///
      void addCol(const CoinPackedVectorBase& vec,
                          const double collb, const double colub,
                          const double obj)
                {PicoLPInterface::addCol(vec,collb,colub,obj);}

      ///
      void addCol(const CharString& colName, int colLength, int* rowNumbers,
                          double* matrixElements,
                          const double collb, const double colub,
                          const double obj)
		{PicoLPInterface::addCol(colName,colLength,rowNumbers,
			matrixElements,collb,colub,obj);}

      ///
      void addCols(const int numcols,
                           const CoinPackedVectorBase * const * cols,
                           const double* collb, const double* colub,
                           const double* obj)
                {PicoLPInterface::addCols(numcols,cols,collb,colub,obj);}

      ///
      void addCols(const BasicArray<CharString>& name,
                        const BasicArray<CoinPackedVectorBase*>& cols,
                        const BasicArray<double>& collb,
                        const BasicArray<double>& colub,
                        const BasicArray<double>& obj)
		{PicoLPInterface::addCols(name,cols,collb,colub,obj);}

      ///
      void addCols(const BasicArray<CoinPackedVectorBase*>& cols,
                        const BasicArray<double>& collb,
                        const BasicArray<double>& colub,
                        const BasicArray<double>& obj)
		{PicoLPInterface::addCols(cols,collb,colub,obj);}

      ///
      void deleteCols(const int num, const int * colIndices)
                {PicoLPInterface::deleteCols(num, colIndices);}

      ////
      void deleteCols(const int first, const int last)
		{PicoLPInterface::deleteCols(first,last);}

      ///
      void addRow(const CoinPackedVectorBase& vec,
                          const double rowlb, const double rowub)
                {PicoLPInterface::addRow(vec, rowlb, rowub);}

      ///
      void addRow(const CharString& name, const CoinPackedVectorBase& vec,
                          const double rowlb, const double rowub)
                {PicoLPInterface::addRow(name, vec, rowlb, rowub);}

      ///
      void addRow(const CoinPackedVectorBase& vec,
                          const char rowsen, const double rowrhs,
                          const double rowrng)
                {PicoLPInterface::addRow(vec, rowsen, rowrhs, rowrng);}

      ///
      void addRow(const CharString& name, const CoinPackedVectorBase& vec,
                          const char rowsen, const double rowrhs,
                          const double rowrng)
                {PicoLPInterface::addRow(vec, rowsen, rowrhs, rowrng);}

      ///
      void addRows(const int numrows,
                           const CoinPackedVectorBase * const * rows,
                           const double* rowlb, const double* rowub)
                {PicoLPInterface::addRows(numrows, rows, rowlb, rowub);}

      ///
      void addRows(const int numrows,
                           const CoinPackedVectorBase * const * rows,
                           const char* rowsen, const double* rowrhs,
                           const double* rowrng)
                {PicoLPInterface::addRows(numrows, rows, rowsen, rowrhs,
                                                rowrng);}

      ///
      void addRows(const BasicArray<CharString>& names, const int numrows,
                           const CoinPackedVectorBase * const * rows,
                           const double* rowlb, const double* rowub)
                {PicoLPInterface::addRows(names, numrows, rows, rowlb, rowub);}

      ///
      void addRows(const BasicArray<CharString>& names, const int numrows,
                           const CoinPackedVectorBase * const * rows,
                           const char* rowsen, const double* rowrhs,
                           const double* rowrng)
                {PicoLPInterface::addRows(names, numrows, rows, rowsen, rowrhs,
                                                rowrng);}

      ///
      void addRows(const BasicArray<CoinPackedVectorBase*>& rows,
                        const BasicArray<double>& rowlb,
                        const BasicArray<double>& rowub)
		{PicoLPInterface::addRows(rows,rowlb,rowub);}

      ///
      void addRows(const BasicArray<CharString>& names,
                        const BasicArray<CoinPackedVectorBase*>& rows,
                        const BasicArray<double>& rowlb,
                        const BasicArray<double>& rowub)
		{PicoLPInterface::addRows(names,rows,rowlb,rowub);}

      ///
      void addRows(const BasicArray<CoinPackedVectorBase*>& rows,
                        const BasicArray<char>& rowsen,
                        const BasicArray<double>& rowrhs,
                        const BasicArray<double>& rowrng)
		{PicoLPInterface::addRows(rows,rowsen,rowrhs,rowrng);}

      ///
      void addRows(const BasicArray<CharString>& names,
                        const BasicArray<CoinPackedVectorBase*>& rows,
                        const BasicArray<char>& rowsen,
                        const BasicArray<double>& rowrhs,
                        const BasicArray<double>& rowrng)
		{PicoLPInterface::addRows(names,rows,rowsen,rowrhs,rowrng);}

      ///
      void addRows(BasicArray<picoRowCut*>& cutlist, int numRows)
		{PicoLPInterface::addRows(cutlist,numRows);}

      ///
      void deleteRows(const int num, const int * rowIndices)
                {PicoLPInterface::deleteRows(num,rowIndices);}

      ///
      void deleteRows(const int first, const int last)
                {PicoLPInterface::deleteRows(first,last);}

      ///
      void deleteRows(BasicArray<picoRowCut*>& cutlist) 
		{PicoLPInterface::deleteRows(cutlist);} 

     ///
     int readMps(const char* filename, const char *extension = "mps")
		{return PicoLPInterface::readMps(filename, extension);}

     ///
     void setObjSense(pebbl::optimizeSense minOrMax)
                {PicoLPInterface::setObjSense(minOrMax);}
     ///
     virtual void setMatrixCoeff(int row, int col, double newValue)
                {PicoLPInterface::setMatrixCoeff(row, col, newValue);}


     ///
     constraintSense getRowSense(int whichConstraint) const
		{
		const char* tmp = T::getRowSense();
		PicoLPInterface::constraintSense sense = notRestricted;
		switch (tmp[whichConstraint]) {
		  case 'L':     sense = lessOrEqual; break;
		  case 'G':     sense = greaterOrEqual; break;
		  case 'E':     sense = equal; break;
		  case 'R':     sense = ranged; break;
		  case 'N':     sense = notRestricted; break;
		  default:
		        EXCEPTION_MNGR(std::runtime_error,"PicoLPSubclass::getRowSense - bad sense type=" << tmp[whichConstraint]);
		  };
		return sense;
		}

     ///
     void getRowSense(char* constr, int first, int last) const
		{PicoLPInterface::getRowSense(constr,first,last);}



     /** The following methods must be redefined with template specialization. */
 
        ///
  bool isTimeLimitReached() const
    {
      // For this method, it's OK to return false by default, since PICO is not enforcing
      // any time limits right now.
    return false;
    }

        ///
  bool isProvenPrimalUnbounded() const
    {    EXCEPTION_MNGR(std::runtime_error, "isProvenPrimalUnbounded() is not implemented in the PicoLPInterface for this LP Solver\n");
    return false;
    }

        ///
  bool isProvenDualUnbounded() const
    {    EXCEPTION_MNGR(std::runtime_error, "isProvenDualUnbounded() is not implemented in the PicoLPInterface for this LP Solver\n");
    return false;
    }

        ///
  basisState getBasis(int whichVariable) const {return nonbasic;}

        ///
  void setBasis(BasisArray& ) {}

	///
  priceRule getPricingRule() const {return defaultPricing;}

	///
  void setPricingRule(priceRule ) {}

	///
  double getTimeLimit() const {return 0.0;}

	///
  void setTimeLimit(double )
	{EXCEPTION_MNGR(std::runtime_error,"Unable to set time limit.");}

	///
  clockType getClockType() const {return cpuTime;}

	///
  void setClockType(clockType )
	{EXCEPTION_MNGR(std::runtime_error,"Unable to set clock type.");}

	///
  const CoinPackedMatrix * getMatrixByRow() const
		{return T::getMatrixByRow();}

	///
  const CoinPackedMatrix * getMatrixByCol() const
		{return T::getMatrixByCol();}

	///
  void getMatrixByRow(RMSparseMatrix<Ereal<double> >& mat) const
		{PicoLPInterface::getMatrixByRow(mat);}

	///
  void getMatrixByCol(CMSparseMatrix<Ereal<double> >& mat) const
		{PicoLPInterface::getMatrixByCol(mat);}

	///
  void loadProblem(CMSparseMatrix<Ereal<double> >& matrix,
                        BasicArray<Ereal<double> >& collb,
                        BasicArray<Ereal<double> >& colub,
                        BasicArray<Ereal<double> >& obj,
                        BasicArray<Ereal<double> >& rowlb,
                        BasicArray<Ereal<double> >& rowub,
                        BasicArray<CharString>& rNames,
                        BasicArray<CharString>& cNames)
		{PicoLPInterface::loadProblem(matrix,collb,colub,obj,rowlb,
				rowub,rNames,cNames);}

	///
  void loadProblem(CMSparseMatrix<Ereal<double> >& matrix,
                        BasicArray<Ereal<double> >& collb,
                        BasicArray<Ereal<double> >& colub,
                        BasicArray<Ereal<double> >& obj,
                        BasicArray<Ereal<double> >& rowlb,
                        BasicArray<Ereal<double> >& rowub)
		{PicoLPInterface::loadProblem(matrix,collb,colub,obj,rowlb,
				rowub);}

	///
  void loadProblem(const CoinPackedMatrix& matrix,
                             const double* collb, const double* colub,
                             const double* obj,
                             const double* rowlb, const double* rowub)
                {PicoLPInterface::loadProblem(matrix,collb,colub,obj,rowlb,rowub);}

	/** */
  void loadProblem(const CoinPackedMatrix& matrix,
                             const double* collb, const double* colub,
                             const double* obj,
                             const double* rowlb, const double* rowub,
                             CharString* rNames, CharString* cNames)
		{PicoLPInterface::loadProblem(matrix,collb,colub,obj,
				rowlb,rowub);}

  /** */
  void loadProblem(const CoinPackedMatrix& matrix,
                             const double* collb, const double* colub,
                             const double* obj,
                             const char* rowsen, const double* rowrhs,
                             const double* rowrng)
		{PicoLPInterface::loadProblem(matrix,collb,colub,obj,rowsen,rowrhs,rowrng);}

  /** */
  void loadProblem(const CoinPackedMatrix& matrix,
                             const double* collb, const double* colub,
                             const double* obj,
                             const char* rowsen, const double* rowrhs,
                             const double* rowrng,
                             CharString* rNames, CharString* cNames)
		{PicoLPInterface::loadProblem(matrix,collb,colub,obj,
				rowsen,rowrhs,rowrng);}

  ///
  void loadProblem(const int numcols, const int numrows,
			     const int* start, const int* index,
			     const double* value,
                             const double* collb, const double* colub,
                             const double* obj,
                             const double* rowlb, const double* rowub)
		{PicoLPInterface::loadProblem(numcols, numrows, start, index, 
				value, collb,colub,obj,rowlb,rowub);}

  ///
  void loadProblem(const int numcols, const int numrows,
			     const int* start, const int* index,
			     const double* value,
                             const double* collb, const double* colub,
                             const double* obj,
                             const char* rowsen, const double* rowrhs,
                             const double* rowrng)
		{PicoLPInterface::loadProblem(numcols, numrows, start, index, 
				value, collb,colub,obj,rowsen,rowrhs,rowrng);}

  ///
  void loadProblem(const CharString& filename,
                                        BasicArray<int>& listOfIntegersP,
                                        ilp_format format=unknown_format)
		{PicoLPInterface::loadProblem(filename,listOfIntegersP,
						format);}

  ///
  void loadProblem(const CharString& filename1,
		   const CharString& filename2,
                                        BasicArray<int>& listOfIntegersP,
                                        ilp_format format=unknown_format)
		{PicoLPInterface::loadProblem(filename1,filename2,
					listOfIntegersP, format);}

	///
  void loadProblem(UnPackBuffer& inBuffer)
		{PicoLPInterface::loadProblem(inBuffer);}


    // The next 5 variable-status query methods return correct (MIP)
    // answers when we are not solving (e.g. when we are cutting) and
    // otherwise return the OsiSolverInterface status.  We could just
    // return true/false as appropriate to represent a pure LP, but we
    // may eventually want to allow some IP solves (see next comment)

    ///
    bool isContinuous(int colIndex) const
      {
	if (pureLP) return(true);
	if (solving) return T::isContinuous(colIndex);
	if (vtype[colIndex] == continuous)
	  return(true);
	return(false);
      }

    ///
    bool isBinary(int colIndex) const
      {
	if (pureLP) return(false);
#if defined(DARWIN)
	if (solving) return OsiSolverInterface::isBinary(colIndex);
#else
	if (solving) return T::isBinary(colIndex);
#endif
	if (vtype[colIndex] == binary)
	  return(true);
	return(false);
      }

    ///
    bool isInteger(int colIndex) const
      {
	if (pureLP) return(false);
#if defined(DARWIN)
	if (solving) return OsiSolverInterface::isInteger(colIndex);
#else
	if (solving) return T::isInteger(colIndex);
#endif
	if (vtype[colIndex] == binary || vtype[colIndex] == generalInteger)
	  return(true);
	return(false);
      }

    ///
    bool isIntegerNonBinary(int colIndex) const
      {
	if (pureLP) return(false);
#if defined(DARWIN)
	if (solving) return OsiSolverInterface::isIntegerNonBinary(colIndex);
#else
	if (solving) return T::isIntegerNonBinary(colIndex);
#endif
	if (vtype[colIndex] == generalInteger)
	  return(true);
	return(false);
      }

    ///
    bool isFreeBinary(int colIndex) const
      {
	if (pureLP) return(false);
#if defined(DARWIN)
	if (solving) return OsiSolverInterface::isFreeBinary(colIndex);
#else
	if (solving) return T::isFreeBinary(colIndex);
#endif
	double varVal = getColSolution(colIndex);
	if (vtype[colIndex] == binary && varVal > 0 + integerTolerance && varVal < 1 - integerTolerance)
	  return(true);
	return(false);
      }

    // For the next four, we may eventually want to allow this for solvers that
    // have IP capabilities (like cplex), eg. for running iterative IP-based heuristics
    // using the AMPL-PICO interface.
    ///
    void setContinuous(int index)
    {ucout << "Warning: LP only.  Ignoring attempt to change variable status to continuous\n";}

    ///
    void setInteger(int index)
    {ucout << "Warning: LP only.  Ignoring attempt to change variable status to integer\n";}

    ///
    void setContinuous(const int* indices, int len)
    {ucout << "Warning: LP only.  Ignoring attempt to change variable status to continuous\n";}

    /// 
    void setInteger(const int* indices, int len)
    {ucout << "Warning: LP only.  Ignoring attempt to change variable status to integer\n";}


protected:

    ///
    void getRanks(BasicArray<picoRowCut *>& cutlist, IntVector &rankList)
      {V::getRanks(cutlist, rankList);}

    ///
    void execDeleteRows(const int num, const int * rowIndices)
		{T::deleteRows(num,rowIndices); myState=unsolved;}

    ///
    void execDeleteCols(const int num, const int * colIndices)
		{T::deleteCols(num,colIndices); myState=unsolved;}

    ///
    void execAddCol(const CoinPackedVectorBase& vec,
                          const double collb, const double colub,
                          const double obj)
		{T::addCol(vec,collb,colub,obj); myState=unsolved;}

    ///
    void execAddCols(const int numcols,
                           const CoinPackedVectorBase * const * cols,
                           const double* collb, const double* colub,
                           const double* obj)
		{OsiSolverInterface::addCols(numcols,cols,collb,colub,obj); myState=unsolved;}

    ///
    void execAddRow(const CoinPackedVectorBase& vec,
                          const double rowlb, const double rowub)
		{T::addRow(vec,rowlb,rowub); myState=unsolved;}

    ///
    void execAddRow(const CoinPackedVectorBase& vec,
                          const char rowsen, const double rowrhs,
                          const double rowrng)
		{T::addRow(vec,rowsen,rowrhs,rowrng); myState=unsolved;}

    ///
    void execAddRows(const int numrows,
                           const CoinPackedVectorBase * const * rows, 
                           const double* rowlb, const double* rowub)
		{OsiSolverInterface::addRows(numrows, rows, rowlb, rowub); myState=unsolved;}

    ///
    void execAddRows(const int numrows,
                           const CoinPackedVectorBase * const * rows,
                           const char* rowsen, const double* rowrhs,
                           const double* rowrng)
		{
		OsiSolverInterface::addRows(numrows,rows,rowsen,rowrhs,rowrng); 
		myState=unsolved;
		}

    ///
    void execAddRows(BasicArray<picoRowCut*>& cutlist, int num_rows)
		{V::execAddRows(cutlist, num_rows);}

    ///
    void execLoadProblem(const CoinPackedMatrix& matrix,
                             const double* collb, const double* colub,
                             const double* obj,
                             const double* rowlb, const double* rowub)
		{
		T::loadProblem(matrix,collb,colub,obj,rowlb,rowub); 
		myState=unsolved;
		}

    ///
    void execLoadProblem(const CoinPackedMatrix& matrix,
                             const double* collb, const double* colub,
                             const double* obj,
                             const char* rowsen, const double* rowrhs,
                             const double* rowrng)
		{
		T::loadProblem(matrix,collb,colub,obj,rowsen,rowrhs,rowrng); 
		myState=unsolved;
		}

    ///
    void execLoadProblem(const int numcols, const int numrows,
			     const int* start, const int* index,
			     const double* value,
                             const double* collb, const double* colub,
                             const double* obj,
                             const double* rowlb, const double* rowub)
		{
		T::loadProblem(numcols, numrows, start, index, value,
				collb,colub,obj,rowlb,rowub); 
		myState=unsolved;
		}

    ///
    void execLoadProblem(const int numcols, const int numrows,
			     const int* start, const int* index,
			     const double* value,
                             const double* collb, const double* colub,
                             const double* obj,
                             const char* rowsen, const double* rowrhs,
                             const double* rowrng)
		{
		T::loadProblem(numcols, numrows, start, index, value,
				collb,colub,obj,rowsen,rowrhs,rowrng); 
		myState=unsolved;
		}

  ///
  virtual void write(char *filename, ilp_format format=unknown_format)
    {PicoLPInterface::write(filename, format);};

  ///
  virtual void write(PackBuffer& outBuffer) const
    {PicoLPInterface::write(outBuffer);};

  ///
  virtual void printBrokenInfo()
    {PicoLPInterface::printBrokenInfo();};

private:

    ///
    void setObjSense(double tmp)
                {T::setObjSense(tmp);}

    ///
    const char * getRowSense() const
		{return T::getRowSense();}

    /** Hide this method, since PicoLPInterface is a _linear programming_ interf
ace! */
    void branchAndBound() {}

    //MSN: comment these out as well
    /*
    ///
    bool isContinuous(int colIndex) const {return false;}

    ///
    bool isBinary(int colIndex) const {return false;}

    ///
    bool isInteger(int colIndex) const {return false;}

    ///
    bool isIntegerNonBinary(int colIndex) const {return false;}

    ///
    bool isFreeBinary(int colIndex) const {return false;}

    ///
    void setContinuous(int index) {}

    ///
    void setInteger(int index) {}

    ///
    void setContinuous(const int* indices, int len) {}

    ///
    void setInteger(const int* indices, int len) {}
    */
    ///
    OsiVectorInt getFractionalIndices(const double etol=1.e-05) const
                {OsiVectorInt tmp; return tmp;}

};

} // namespace pico

#endif
