/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file PicoCpxLP.h
 *
 * Defines CPLEX interface stuff.
 */

#ifndef pico_PicoCpxLP_h
#define pico_PicoCpxLP_h

#include <acro_config.h>
#ifdef ACRO_HAVE_CPLEX

#include <OsiCpxSolverInterface.hpp>
#include <pico/PicoLPCutMngmnt.h>
#include <pico/PicoLPSubclass.h>

// Copied from the OsiCpxSolverInterface.cpp file.  This used to be in the corresponding
// header file, but moved to the .cpp file and became inaccessible.  Check to see if we
// can move it back to the header file.  In the meantime, see if this works.  If we cannot
// get this moved back to the header, be careful to watch for any changes in this method.

static inline void
checkCPXerror( int err, std::string cpxfuncname, std::string osimethod )
{
  if( err != 0 )
    {
      char s[100];
      sprintf( s, "%s returned error %d", cpxfuncname.c_str(), err );
      std::cout << "ERROR: " << s << " (" << osimethod << 
	" in OsiCpxSolverInterface)" << std::endl;
      throw CoinError( s, osimethod.c_str(), "OsiCpxSolverInterface" );
    }
}


namespace pico {


template <>
void PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::initialize()
{
  messageHandler()->setLogLevel(0);
}

//
// TODO: is this right???
// CAP response: probably not. Why are we using MIP stats when we are using the solver
// as an LP?
//
template<>
bool PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::isTimeLimitReached() const
{
  //int stat = CPXgetstat( const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(), getMutableLpPtr() );
int stat = CPXgetstat( const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(), const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL) );
#if (CPX_VERSION >= 800)
return stat == CPXMIP_TIME_LIM_INFEAS || stat == CPXMIP_TIME_LIM_FEAS;
#else
return stat == CPX_TIME_LIM_INFEAS || stat == CPX_TIME_LIM_FEAS;
#endif
}


template<>
bool PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::isProvenPrimalUnbounded() const
{
int stat = CPXgetstat( const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(), const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL) );
#if (CPX_VERSION >= 800)
return stat == CPX_STAT_UNBOUNDED;
#else
int method = CPXgetmethod(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(), const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL) );
// stat for version < 8 is relative to solver. If running dual simplex and stat is
// infeasible, we do not necessarily know the primal is unbounded (could be infeasible).
 return ((method == CPX_ALG_PRIMAL && stat == CPX_UNBOUNDED));
#endif
}


template<>
bool PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::isProvenDualUnbounded() const
{
  // Version 8.0 and up returns status wrt the primal only.  There is no primal status that
  // proves dual unboundedness.  CAP.
#if (CPX_VERSION < 800)
int stat = 
#endif
CPXgetstat( const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(), const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL) );
#if (CPX_VERSION >= 800)
return false;
#else
int method = CPXgetmethod(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(), const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL) );
// stat for version < 8 is relative to solver. If running primal simplex and stat is
// infeasible, we do not necessarily know the dual is unbounded (could be infeasible).
 return ((method == CPX_ALG_DUAL && stat == CPX_UNBOUNDED));
#endif
}


template<>
double PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::getColLower(int whichVariable) const
{
double ans;
CPXgetlb(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL),&ans,whichVariable,whichVariable);
return ans;
}


template<>
double PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::getColUpper(int whichVariable) const
{
double ans;
CPXgetub(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL),&ans,whichVariable,whichVariable);
return ans;
}


template<>
double PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::getRowLower(int whichVariable) const
{
char sense;
CPXgetsense(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL),&sense,whichVariable,whichVariable);
double rhs;
CPXgetrhs(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL),&rhs,whichVariable,whichVariable);
double range;
CPXgetrngval(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL),&range,whichVariable,whichVariable);
double lower, upper;
convertSenseToBound(sense, rhs, range, lower, upper);
return lower;
}


template<>
double PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::getRowUpper(int whichVariable) const
{
char sense;
CPXgetsense(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL),&sense,whichVariable,whichVariable);
double rhs;
CPXgetrhs(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL),&rhs,whichVariable,whichVariable);
double range;
CPXgetrngval(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL),&range,whichVariable,whichVariable);
double lower, upper;
convertSenseToBound(sense, rhs, range, lower, upper);
return upper;
}


template<>
double PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::getRightHandSide(int whichConstraint) const
{
double rhs;
CPXgetrhs(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL),&rhs,whichConstraint,whichConstraint);
return rhs;
}

template<>
PicoLPInterface::constraintSense PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::getRowSense(int whichConstraint) const
{
char csense;
CPXgetsense( const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL), &csense, whichConstraint, whichConstraint);

PicoLPInterface::constraintSense sense = notRestricted;
switch (csense) {
  case 'L':     sense = lessOrEqual; break;
  case 'G':     sense = greaterOrEqual; break;
  case 'E':     sense = equal; break;
  case 'R':     sense = ranged; break;
  case 'N':     sense = notRestricted; break;
  default:
        EXCEPTION_MNGR(std::runtime_error,"PicoLPSubclass::getRowSense - bad sense type=" << csense);
  };
return sense;
}


template<>
double PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::getObjCoefficient(int whichVariable) const
{
double ans;
int err = CPXgetobj(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL),&ans,whichVariable,whichVariable);
checkCPXerror( err, "CPXgetobj", "getObjCoefficient" );
return ans;
}


template<>
double PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::getColSolution(const int whichCol) const
{
double ans = 0.0;
int probType = CPXgetprobtype(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL));
#if (CPX_VERSION >= 800)
if ( probType == CPXPROB_MILP ) {
#else
if ( probType == CPXPROB_MIP ) {
#endif
   int err = CPXgetmipx( (const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr()), (const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL)), &ans, whichCol, whichCol);
   if ( err == CPXERR_NO_INT_SOLN )
      ans = 0.0;
   else
      checkCPXerror( err, "CPXgetmipx", "getColSolution" );
   }
else {
   int err = CPXgetx( const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL), &ans, whichCol, whichCol );
   if ( err == CPXERR_NO_SOLN )
      ans = 0.0;
   else
      checkCPXerror( err, "CPXgetx", "getColSolution" );
   }
return ans;
}


template<>
double PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::getRowPrice(const int whichRow) const
{
double ans;
int err = CPXgetpi(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL),&ans,whichRow,whichRow);
if (err == CPXERR_NO_SOLN)
   ans = 0.0;
else
   checkCPXerror( err, "CPXgetpi", "getRowPrice" );
return ans;
}


template<>
double PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::getReducedCost(const int whichCol) const
{
double ans;
int err = CPXgetdj(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL),&ans,whichCol,whichCol);
if (err == CPXERR_NO_SOLN)
   ans = 0.0;
else
   checkCPXerror( err, "CPXgetdj", "getReducedCost" );
return ans;
}


template <>
void PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::getBasis(BasisArray& basisBuffer) const
{
int numCols = getNumCols();
int basisSize = numCols + getNumRows();
utilib::IntVector tempBasis(basisSize);
int tmp = CPXgetbase(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL),(int*)(tempBasis.data()),(int*)(&tempBasis[numCols]));
if (tmp)
   EXCEPTION_MNGR(std::runtime_error,"CPLEX would not retreive basis");
for (int i = 0; i < basisSize; i++) {
  int val = tempBasis[i];
  if (val == 0) basisBuffer.put(i, atLower);
  else if (val == 3) basisBuffer.put(i, nonbasic);
  else basisBuffer.put(i, (basisState)val);
  }
}

template <>
basisState PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::getBasis(int whichVariable) const
{
int numCols = getNumCols();
utilib::IntVector tempBasis(numCols + getNumRows());
int tmp = CPXgetbase(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL),
		(int*)(tempBasis.data()),(int*)(&tempBasis[numCols]));
if (tmp)
  EXCEPTION_MNGR(std::runtime_error, "PicoLPSubclass<T,V>::getBasis -- "
		 "Couldn't retreive basis");
int val = tempBasis[whichVariable];
//
// Convert to Osi standard representation for the basis
//
if (val == 0) return atLower;
if (val == 3) return nonbasic;
return (basisState)val;
}


template <>
void PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::setBasis(BasisArray& basisBuffer)
{
static BasicArray<int> tempBasis;

int num_cols = getNumCols();
int basisSize = getNumRows() + num_cols;
tempBasis.resize(basisSize);
for (int i = 0; i < basisSize; i++)
  {
  tempBasis[i] = (int)basisBuffer(i);
  // translate differences between PICO values and cplex values
  if (tempBasis[i] == atLower)
    tempBasis[i] = 0;
  else if (tempBasis[i] == nonbasic)
    {
  // If this is the slack variable for an added cut and the value is nonbasic, set it
  // to atLower (this is the value a slack variable can be if it's not basic
      if (i >= num_cols + num_core_rows)
	tempBasis[i] = 0;
      else tempBasis[i] = 3;
    }
  }

status = CPXcopybase(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL),(int*)(tempBasis.data()),
					(int*)(&tempBasis[getNumCols() ]));
if (status && lpWarnings)
   ucout << "Warning: cplex would not load basis, status = " << status;
else
  {
  // We may want to rethink at some point.  I don't think the MIP uses this 
  // and I don't think we suppress basis copying if this is false.
  have_basis = true;
  // Might want to set state to unsolved to indicate a possible mismatch between
  // the solution and the basis, but for now, we won't do it here.
  //  myState= unsolved;
  }
}


template<>
priceRule PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::getPricingRule() const
{
/// TODO
return defaultPricing;
}


template<>
void PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::setPricingRule(priceRule rule)
{
/// TODO
}


template<>
double PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::getTimeLimit() const
{
double ans;
int tmp = CPXgetdblparam(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),CPX_PARAM_TILIM,&ans);
if (tmp != 0)
  {
    EXCEPTION_MNGR(std::runtime_error, "Unable to get time limit");
  }
return ans;
}


template<>
void PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::setTimeLimit(double time)
{
int tmp = CPXsetdblparam(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),CPX_PARAM_TILIM,time);
if (tmp)
  EXCEPTION_MNGR(std::runtime_error,"Unable to set time limit");
}


template<>
clockType PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::getClockType() const
{
int ans;
int tmp = CPXgetintparam(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),CPX_PARAM_CLOCKTYPE,&ans);
if (tmp != 0)
  {
    EXCEPTION_MNGR(std::runtime_error,"Unable to get clock type");
  }
if ((clockType) ans != cpuTime && (clockType)ans != wallTime)
  EXCEPTION_MNGR(std::runtime_error, "Unknown clock type " <<  ans)
return (clockType)ans;
}


template<>
void PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::setClockType(clockType cType)
{
if (cType != cpuTime && cType != wallTime)
  EXCEPTION_MNGR(std::runtime_error, "Improper clock type " << cType)
int tmp = CPXsetintparam(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(),CPX_PARAM_CLOCKTYPE, (int)cType);
if (tmp)
  EXCEPTION_MNGR(std::runtime_error, "Unable to set clock type")
}



template<>
PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::PicoLPSubclass()
{
solverMethod = primalSimplex;
// TODO
// Turn cplex preprocessing off till we handle presolve infeasibility
// errors more gracefully
CPXsetintparam(const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getEnvironmentPtr(), CPX_PARAM_PREIND, 0);
}

/* Comment this out till I can figure out why the cplex call isn't working
template<>
void PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::setColUpper(double* upperBounds, int* indexList, int len)
{
  char c = 'U';
  // Going to getting lp pointer with default values rather than
  // OsiCpxSolverInterface::FREECACHED_COLUMN, for debugging
  int err = CPXchgbds( getEnvironmentPtr(), const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL), len, indexList, &c, upperBounds);
  checkCPXerror( err, "CPXchgbds", "setColUpper" );
}

template<>
void PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::setColLower(double* lowerBounds, int* indexList, int len)
{
  char c = 'L';
  int err = CPXchgbds( getEnvironmentPtr(), const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL), len, indexList, &c, lowerBounds);
  checkCPXerror( err, "CPXchgbds", "setColLower" );
}
*/

template<>
void PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::write(char *filename, ilp_format format)
{
  // actually just use cplex mps and lp formats for now.
int writeStatus;
if (format == mps_format)
  {
    // This will write with original names, but since PICO currently
    // doesn't give the real variable names (etc) to the cplex solver,
    // it will be equivalent to the "REW" filetype = generic names
  writeStatus = CPXwriteprob(getEnvironmentPtr(), const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL), filename, "MPS");
  if (writeStatus)
    ucout << "Error writing lp to file\n";
  return;
  }
if (format == lp_format)
  {
  writeStatus = CPXwriteprob(getEnvironmentPtr(), const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL), filename, "LP");
  if (status)
    ucout << "Error writing lp to file\n";
  return;
  }
// default for the other formats.
 PicoLPInterface::write(filename, format);
}

template<>
void PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::printBrokenInfo()
{
    int stat = CPXgetstat( getEnvironmentPtr(), const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL) );
    ucout << "Broken LP has cplex status " << stat << "\n";
}

template<>
void PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>::setMatrixCoeff(int row, int col, double newValue)
{
    int stat = CPXchgcoef(getEnvironmentPtr(), const_cast<PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt>*>(this)->OsiCpxSolverInterface::getLpPtr(OsiCpxSolverInterface::KEEPCACHED_ALL), row, col, newValue);
    if (stat != 0) EXCEPTION_MNGR(std::runtime_error, "Cplex error setting matrix coefficient for (" << row << ", " << col << ")")
}

typedef PicoLPSubclass<OsiCpxSolverInterface,PicoLPCutMngmnt> PicoCpxLP;

} // namespace pico

#endif

#endif
