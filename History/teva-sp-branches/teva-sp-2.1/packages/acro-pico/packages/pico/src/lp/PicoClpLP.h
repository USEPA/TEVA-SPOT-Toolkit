/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file PicoClpLP.h
 *
 * Defines CLP stuff.
 */

#ifndef pico_PicoClpLP_h
#define pico_PicoClpLP_h

#include <acro_config.h>
#ifdef ACRO_USING_CLP

#include <OsiClpSolverInterface.hpp>
#include <pico/PicoLPCutMngmnt.h>
#include <pico/PicoLPSubclass.h>


namespace pico {

  // Changed ClpSimplex to CoinWarmStartBasis below (JE, 8/2005)
inline basisState translate_status(CoinWarmStartBasis::Status val)
{
  switch (val) {
	case CoinWarmStartBasis::basic:
		return pico::basic;

	case CoinWarmStartBasis::atUpperBound:
		return pico::atUpper;

	case CoinWarmStartBasis::atLowerBound:
		return pico::atLower;

	default:
		return pico::nonbasic;
  };
}

// basisIndex is between 0 and the current number of rows + number of columns.
// 0 to num_cols - 1 is column basis information; higher values for rows

inline CoinWarmStartBasis::Status translate_status(basisState val, 
						   bool addedRow)
{
  switch (val) {
  case pico::basic:
    return CoinWarmStartBasis::basic;

  case pico::atUpper:
    return CoinWarmStartBasis::atUpperBound;

  case pico::atLower:
    return CoinWarmStartBasis::atLowerBound;
		
    // If this is the auxilary variable for an added cut row, return
    // what a cut row variable is if it is not basic (and is tight).
    // For structural variables and core rows, nonbasic should be
    // interpreted according to what getBasis does.
  case pico::nonbasic:
    if (addedRow)
      return CoinWarmStartBasis::atLowerBound;
    else
      return CoinWarmStartBasis::isFree;

  default:
    return CoinWarmStartBasis::isFree;
  };
}

template <>
void PicoLPSubclass<OsiClpSolverInterface,PicoLPCutMngmnt>::initialize()
{
    messageHandler()->setLogLevel(0);
}

// CAP: CLP seems to have no status flags that will allow one to interpret 
// unboundedness for either the primal or the dual.  It has a status for 
// infeasible in each (in part because OSI requires that), but one cannot 
// necessarily infer unboundedness from infeasibility in the dual problem 
// (primal = dual of dual).  It's correct to simply say nothing is proven.  
// Be careful about depending upon reasonable functionality for this test.

template <>
bool PicoLPSubclass<OsiClpSolverInterface,PicoLPCutMngmnt>::isProvenDualUnbounded() const
{
return(false);
}

template <>
bool PicoLPSubclass<OsiClpSolverInterface,PicoLPCutMngmnt>::isProvenPrimalUnbounded() const
{
return(false);
}

template <>
void PicoLPSubclass<OsiClpSolverInterface,PicoLPCutMngmnt>::getBasis(BasisArray& basisBuffer) const
{
if (! (modelPtr_->statusExists()) )
  EXCEPTION_MNGR(std::runtime_error, "CLP basis not available\n");

int numCols = getNumCols();
int numRows = getNumRows();
int i=0;

// JE 8/2005 -- commented out this (reasonable-looking) implementation
// in favor of something that matches new trial implementation of setBasis
// for (; i < numCols; i++) {
//   ClpSimplex::Status val = modelPtr_->getColumnStatus(i);
//   basisBuffer.put(i, translate_status(val) );
//   }
// for (int j=0; i < (numCols+numRows); i++,j++) {
//   ClpSimplex::Status val = modelPtr_->getRowStatus(j);
//   basisBuffer.put(i, translate_status(val) );
//   }

// Attempt at a new implementation; the first line is a bit ugly! 
CoinWarmStartBasis* cwsbp = (CoinWarmStartBasis*) getWarmStart();
for (; i < numCols; i++) 
  basisBuffer.put(i,translate_status(cwsbp->getStructStatus(i)));
for (int j=0; i < (numCols+numRows); i++,j++) 
  basisBuffer.put(i,translate_status(cwsbp->getArtifStatus(j)));

}

template <>
basisState PicoLPSubclass<OsiClpSolverInterface,PicoLPCutMngmnt>::getBasis(int whichVariable) const
{
int numCols = getNumCols();

// JE 8/2005 Changed this, since I'm not sure it works
// if (whichVariable >= numCols) {
//    return translate_status( modelPtr_->getRowStatus(whichVariable-numCols) );
//    }
// else {
//    return translate_status( modelPtr_->getColumnStatus(whichVariable) );
//    }

// This might be very inefficient, I'm not sure...
CoinWarmStartBasis* cwsbp = (CoinWarmStartBasis*) getWarmStart();
if (whichVariable >= numCols) 
  return translate_status(cwsbp->getArtifStatus(whichVariable - numCols));
else 
  return translate_status(cwsbp->getStructStatus(whichVariable));
}


template <>
void PicoLPSubclass<OsiClpSolverInterface,PicoLPCutMngmnt>::setBasis(BasisArray& basisBuffer)
{
int numCols = getNumCols();
int numRows = getNumRows();
int i=0;

// Attempt at doing things in a way we know affects the basis -- JE 8/2005
 CoinWarmStartBasis cwsb;
 cwsb.setSize(numCols,numRows);
 for (i=0; i < numCols; i++)
   cwsb.setStructStatus(i,translate_status(basisBuffer[i],
					   (i >= numCols + num_core_rows)));
 for (int j=0; i < (numCols+numRows); i++,j++)
   cwsb.setArtifStatus(j,translate_status(basisBuffer[i],i));
 #ifdef ACRO_VALIDATING
 bool status = 
 #endif
                setWarmStart(&cwsb);
 DEBUGPR(1000,
   if (!status)
    ucout << "PicoCLP::setBasis -- warning: basis rejected." << std::endl;
 )

// This is the original code
// for (i=0; i < numCols; i++) 
//   {
//     modelPtr_->setColumnStatus(i, translate_status( basisBuffer[i], (i >= numCols + num_core_rows ) ));
//   }
// for (int j=0; i < (numCols+numRows); i++,j++) {
//   modelPtr_->setRowStatus(j, translate_status( basisBuffer[i], i ) );
//   }

}


template<>
priceRule PicoLPSubclass<OsiClpSolverInterface,PicoLPCutMngmnt>::getPricingRule() const
{
/// TODO
return defaultPricing;
}


template<>
void PicoLPSubclass<OsiClpSolverInterface,PicoLPCutMngmnt>::setPricingRule(priceRule rule)
{
/// TODO
}


template<>
PicoLPSubclass<OsiClpSolverInterface,PicoLPCutMngmnt>::PicoLPSubclass()
{
solverMethod = primalSimplex;
}


template<>
void PicoLPSubclass<OsiClpSolverInterface,PicoLPCutMngmnt>::write(char *filename, ilp_format format)
{
  // actually just use cplex mps and lp formats for now.
if (format == mps_format)
  {
  writeMps(filename);
  return;
  }
// default for the other formats.
 PicoLPInterface::write(filename, format);
}

template<>
void PicoLPSubclass<OsiClpSolverInterface,PicoLPCutMngmnt>::setMatrixCoeff(int row, int col, double newValue)
{
  // The true says to keep the new element even if it's zero.
    //modifyCoefficient(row,col,newValue, true);
}

typedef PicoLPSubclass<OsiClpSolverInterface,PicoLPCutMngmnt> PicoClpLP;

} // namespace pico

#endif

#endif
