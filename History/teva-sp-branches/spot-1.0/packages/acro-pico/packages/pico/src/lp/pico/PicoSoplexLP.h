/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file PicoSoplexLP.h
 *
 * Defines SOPLEX interface stuff.
 */

#ifndef pico_PicoSoplexLP
#define pico_PicoSoplexLP

#include <acro_config.h>
#ifdef ACRO_USING_SOPLEX

#include <pico/PicoLPCutMngmnt.h>
#include <pico/PicoLPSubclass.h>
#if defined OK
#undef OK
#endif
#include <OsiSpxSolverInterface.hpp>

namespace pico {

//
// TODO: is this right???
//

template <>
void PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::initialize()
{
  soplex::Param::setVerbose(0);
}

template<>
bool PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::isTimeLimitReached() const
{
double time;
time = spxsolver_.terminationTime();
return (time > 0) && (time <= spxsolver_.time());
}


template<>
bool PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::isProvenPrimalUnbounded() const
{ return (spxsolver_.status() == soplex::SoPlex::UNBOUNDED); }


template<>
bool PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::isProvenDualUnbounded() const
{ return (spxsolver_.status() == soplex::SoPlex::INFEASIBLE); }


template<>
double PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::getColLower(int whichVariable) const
{ return spxsolver_.lower(whichVariable); }
//{ return spxsolver_.lower(cmap[whichVariable]); }


template<>
double PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::getColUpper(int whichVariable) const
{ return spxsolver_.upper(whichVariable); }
//{ return spxsolver_.upper(cmap[whichVariable]); }


template<>
double PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::getRowLower(int whichVariable) const
{ return spxsolver_.lhs(whichVariable); }
//{ return spxsolver_.lhs(rmap[whichVariable]); }


template<>
double PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::getRowUpper(int whichVariable) const
{ return spxsolver_.rhs(whichVariable); }
//{ return spxsolver_.rhs(rmap[whichVariable]); }


template<>
double PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::getRightHandSide(int whichConstraint) const
{ return OsiSpxSolverInterface::getRightHandSide()[whichConstraint]; }


template<>
PicoLPInterface::constraintSense PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::getRowSense(int whichConstraint) const
{
char csense;
double tmp1,tmp2;
convertBoundToSense(spxsolver_.lhs(whichConstraint), spxsolver_.rhs(whichConstraint), csense, tmp1, tmp2);
//convertBoundToSense(spxsolver_.lhs(rmap[whichConstraint]), spxsolver_.rhs(rmap[whichConstraint]), csense, tmp1, tmp2);

PicoLPInterface::constraintSense sense = notRestricted;
switch (csense) {
  case 'L':     sense = lessOrEqual; break;
  case 'G':     sense = greaterOrEqual; break;
  case 'E':     sense = equal; break;
  case 'R':     sense = ranged; break;
  case 'N':     sense = notRestricted; break;
  default:
        EXCEPTION_MNGR(std::runtime_error, "PicoLPInterface::getRowSense - bad sense type=" << csense);
  };
return sense;
}


template<>
double PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::getObjCoefficient(int whichVariable) const
{ return spxsolver_.obj(whichVariable); }
//{ return spxsolver_.obj(cmap[whichVariable]); }


template<>
double PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::getColSolution(const int whichCol) const
{ return getColSolution()[whichCol]; }


template<>
double PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::getRowPrice(const int whichRow) const
{ return getRowPrice()[whichRow]; }


template<>
double PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::getReducedCost(const int whichCol) const
{ return getReducedCost()[whichCol]; }


template <>
basisState PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::getBasis(int whichVariable) const
{
soplex::SoPlex::VarStatus val;
if (whichVariable >= getNumCols()) {
   whichVariable -= getNumCols();
   val = spxsolver_.getBasisRowStatus( whichVariable );
   //val = spxsolver_.getBasisRowStatus( spxsolver_.number(rmap[whichVariable]) );
//   if (getRowSense(whichVariable) == ranged) {
     switch (val) {
       case soplex::SoPlex::ON_UPPER:
         return atUpper;

       case soplex::SoPlex::ON_LOWER:
       case soplex::SoPlex::FIXED:
         return atLower;

       case soplex::SoPlex::ZERO:
	 return nonbasic;

       case soplex::SoPlex::BASIC:
	 return basic;
      };
//   } else {
//      if (val == soplex::SoPlex::BASIC)
//         return basic;
//      else
//         return atLower;
//      }
   }
else {
   val = spxsolver_.getBasisColStatus( whichVariable );
   //val = spxsolver_.getBasisColStatus( spxsolver_.number(cmap[whichVariable]) );

   //
   // Convert to Osi standard representation for the basis
   //
   switch (val) {
     case soplex::SoPlex::ON_UPPER:
	return atUpper;

     case soplex::SoPlex::ON_LOWER:
     case soplex::SoPlex::FIXED:
        return atLower;

     case soplex::SoPlex::ZERO:
	return nonbasic;

     case soplex::SoPlex::BASIC:
	return basic;
     };
   }
return nonbasic;
}


template <>
void PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::getBasis(BasisArray& basisBuffer) const
{
for (size_type i=0; i<basisBuffer.size(); i++)
  basisBuffer.put(i,getBasis(i));
}


template<>
priceRule PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::getPricingRule() const
{
/// TODO
return defaultPricing;
}


template<>
void PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::setPricingRule(priceRule rule)
{
/// TODO
}


template<>
double PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::getTimeLimit() const
{
return spxsolver_.terminationTime();
}


template<>
void PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::setTimeLimit(double time)
{
spxsolver_.setTerminationTime(time);
}


template<>
clockType PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::getClockType() const
{
return wallTime;
}


template<>
void PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::setClockType(clockType cType)
{
EXCEPTION_MNGR(std::runtime_error, "Unable to set clock type");
}

template<>
void PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::setMatrixCoeff(int whichRow, int whichVariable, double newValue)
{ spxsolver_.changeElement(whichRow, whichVariable, newValue); }
//{ spxsolver_.changeElement(rmap[whichRow], cmap[whichVariable], newValue); }

template<>
PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>::PicoLPSubclass()
{
solverMethod = primalSimplex;
}


class PicoSoplexLP : public PicoLPSubclass<OsiSpxSolverInterface,PicoLPCutMngmnt>
{
public:

  void setBasis(BasisArray& basisBuffer)
	{
	row_status.resize(getNumRows());
	col_status.resize(getNumCols());

	int localNumCols = getNumCols();

	size_type bsize = basisBuffer.size();
	for (size_type i = 0; i < bsize; i++) {
  	  soplex::SoPlex::VarStatus val;

  	  switch (basisBuffer(i)) {
    	    case atLower:	val = soplex::SoPlex::ON_LOWER; break;
    	    case atUpper:	val = soplex::SoPlex::ON_UPPER; break;
    	    case basic:		val = soplex::SoPlex::BASIC;    break;
	    case nonbasic:
		// TODO: check that soplex interpretation of auxilary variables for rows is consistent
		// with Ax + Iy representation (classic slack where nonbasic row auxilary variables are
		// at lower.

	      if (i >= (size_type)(localNumCols + PicoLPInterface::num_core_rows))
		val = soplex::SoPlex::ON_LOWER;
	      else val = soplex::SoPlex::ZERO;
	      break;
    	    default:		val = soplex::SoPlex::ZERO;
    	    };

  	  if ((int)i >= localNumCols)
     	     row_status[i-localNumCols] = val;
     	     //row_status[spxsolver_.number(rmap[i-localNumCols])] = val;
  	  else
     	     col_status[i] = val;
     	     //col_status[spxsolver_.number(cmap[i])] = val;
  	  }

	spxsolver_.setBasis(row_status.data(), col_status.data());

	// We may want to rethink at some point.  I don't think the MIP uses this 
	// and I don't think we suppress basis copying if this is false.
	have_basis = true;

	// Might want to set state to unsolved to indicate a possible mismatch between
	// the solution and the basis, but for now, we won't do it here.
	//  myState= unsolved;

	}

protected:

  BasicArray<soplex::SoPlex::VarStatus> row_status;
  BasicArray<soplex::SoPlex::VarStatus> col_status;

};

} // namespace pico

#endif

#endif
