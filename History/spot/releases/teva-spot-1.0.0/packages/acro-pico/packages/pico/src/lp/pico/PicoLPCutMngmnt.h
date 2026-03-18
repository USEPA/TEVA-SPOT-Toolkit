/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file PicoLPCutMngmnt.h
 *
 * Defins the pico::PicoLPCutMngmnt class.
 */

#ifndef pico_PicoLPCutMngmnt_h
#define pico_PicoLPCutMngmnt_h

#include <acro_config.h>
#include <pico/PicoLPInterface.h>

namespace pico {

///
/// A class that manages cuts in a CPLEX-like manner.  Used in the class
/// heirarchy for PicoCpxInterface.
///
/// IMPORTANT: This assumes that cuts are added at the end of the
/// matrix and that when cuts are deleted, the matrix is simply
/// compressed (rows remain in order).  We keep a running counter so
/// that each cut added to the LP at any time is given a new unique
/// number higher than any previously given.  Then we can determine a
/// cut's actual row number in the current LP by finding it's rank
/// among all active cuts (all active cuts are stored after all the
/// core ones).
/// TODO: add testers for LP solvers to make sure they manage rows according
/// to our assumptions!
/// Note: this same SimpleSplayTree manages row names and a similar splay tree
/// manages column names (look up names by row or column rank).  
/// See PicoLPInterface.h.
///
class PicoLPCutMngmnt : virtual public PicoLPInterface
{
public:

  ///
  void execAddRows(BasicArray<picoRowCut*>& cutlist, int num_rows);

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
  // This assumes cutlist is sized exactly (used e.g. for binding cuts that are
  // stored in each node and can be so sized efficiently)
  void replaceRows(BasicArray<picoRowCut*>& cutlist);

  ///
    // This sets the core variables from the basis.  All the slack variables for the
    // additional cuts are set to atLower except for the basic ones (from degenerate
    // binding or nonbinding cuts).  basic_list (1 or 2) is a list of the indices (in cutlist 1 or 2)
    // of the cuts with basic slack variables.  The first pair is for persistent cuts, the second
  // for binding (normal) cuts. Warning: rows must be loaded into the LP before calling this.

    virtual void setBasisWithCuts(BasicArray<picoRowCut*>& cutlist1,
				  BasicArray<int>& basic_list1,
				  BasicArray<picoRowCut*>& cutlist2,
				  BasicArray<int>& basic_list2,
			  	  BasisArray& basisBuffer);

  ///
  virtual void execDeleteRows(const int num, const int * rowIndices) = 0;

  /// Put the (integer) ranks for the cuts in cutlist into the rankList

  void getRanks(BasicArray<picoRowCut *>& cutlist, IntVector &rankList);

  ///
  virtual basisState getBasis(int whichVariable) const = 0;

  ///
  void getBasis(BasisArray& basisBuffer) const
	{PicoLPInterface::getBasis(basisBuffer);}

  /** Get basis info for the slack variable of a given cut # core_rows 
    * + cut rank (all column variables are first in the basis)
    */
  basisState getBasis(picoRowCut&  cut)
	{ return(getBasis(getNumCols() + rowInfo.rank(LPInfoKey(cut.lp_cut_index,0)))); }

  /// Get the value of the slack variable for this cut
  double getSlackVal(picoRowCut&  cut)
	{
	   int rowRank = rowInfo.rank(LPInfoKey(cut.lp_cut_index,0));
	   double rowActivity = getRowActivity()[rowRank];
	   constraintSense rowSense = getRowSense(rowRank);
	   double rhs = getRightHandSide(rowRank);
	   double slack = 0.0;
	   switch (rowSense) {
		case 'L':
			slack = rhs - rowActivity;
			break;
		case 'G':
			slack = rowActivity - rhs;
			break;
 //  ranged -- need to compare with bounds
	        case 'R': 
		        EXCEPTION_MNGR(std::runtime_error,"Time to implement slack variable computations for ranged rows\n");
			break;
	        case 'N':
	                EXCEPTION_MNGR(std::runtime_error,"Attempt to compute slack for an unrestricted row.\n");
                        break;
 	        case 'E':
		  #ifdef ACRO_VALIDATING
		       if (!isZero(rowActivity - rhs))
		         EXCEPTION_MNGR(std::runtime_error, "Nonzero slack for an equality constraint\n");
		  #endif
		       break;
	   }
	   return slack;
	}

  /// Get the current row number of a cut (this can change whenever other cuts
  /// are deleted)
  int getCurrRowNum(picoRowCut & cut)
    {return(rowInfo.rank(LPInfoKey(cut.lp_cut_index,0)));}

  ///
  virtual double getRowPrice(const int whichRow) const = 0;

  ///
  virtual const double * getRowPrice() const = 0;


  ///
  void getRowPrice(double* solBuffer, int howMany, int* colIndeces) const
	{PicoLPInterface::getRowPrice(solBuffer,howMany,colIndeces);}

  ///
  void getRowPrice(double* solBuffer, int first=0, int last=-1) const
	{PicoLPInterface::getRowPrice(solBuffer,first,last);}

  ///
  double getRowPrice(picoRowCut& cut)
	{return(getRowPrice(rowInfo.rank(LPInfoKey(cut.lp_cut_index,0))));}

};

} // namespace pico

#endif
