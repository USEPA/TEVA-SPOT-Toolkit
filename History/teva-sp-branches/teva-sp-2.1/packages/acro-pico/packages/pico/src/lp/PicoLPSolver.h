/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file PicoLPSolver.h
 *
 * Defines the pico::PicoLPSolver class.
 *
 * Note: it would have been nice to simply call this LPSolver.  However this
 * conflicts with the LPSolver class in SOPLEX.  Perhaps when we have
 * namespaces...
 */

#ifndef pico_PicoLPSolver_h
#define pico_PicoLPSolver_h

#include <acro_config.h>
/// A C interface to solve an LP defined in row-major order
extern "C" int c_pico_solve_lp(char* solver,
                  int rows, int cols, int anz, int *ar, int *ac, double *a,
                  double *zobj, double *zrhs, double *zlb, double *zub,
                  char *sense, double *x, double *objv, int *inform,
                  double *lag);

#if defined(__cplusplus)

#include "acro_config.h"
#include <utilib/_generic.h>
#include <utilib/exception_mngr.h>
#include <utilib/BasicArray.h>
#include <utilib/RefCount.h>
#include <utilib/ParameterSet.h>
#include <pico/PicoLPInterface.h>

//
// Count the number of solvers being used
//
#if defined(USING_CPLEX)
#define CTR_CPLEX 1
#else
#define CTR_CPLEX 0
#endif
#if defined(ACRO_USING_SOPLEX)
#define CTR_SOPLEX 1
#else
#define CTR_SOPLEX 0
#endif
#if defined(ACRO_USING_CLP)
#define CTR_CLP 1
#else
#define CTR_CLP 0
#endif
#if defined(ACRO_USING_GLPK)
#define CTR_GLPK 1
#else
#define CTR_GLPK 0
#endif
#define PICOLPSOLVER_NUMSOLVERS 1 + CTR_CPLEX + CTR_SOPLEX + CTR_CLP + CTR_GLPK



namespace pico {

using utilib::RefCount;

#define __LPSolver_Interface PicoLPInterface


/*** This is a container class that managers the allocation/deallocation of
LP interface objects. */

class PicoLPSolver : public utilib::ParameterSet
{
public:

  ///
  enum SolverType {glpk, cplex, xpress, vol, osl, soplex, clp, none, defaultLP, unknownLP};

  /// Empty constructor only, to work with PICO's conventions for
  /// multiple-diamond hierarchy.
  PicoLPSolver()
		{
		ref = NULL;
		solver_type = PicoLPSolver::none;
		setup_parameters();
		}

  /// Constructor that uses a given LP pointer
  PicoLPSolver(__LPSolver_Interface* lp_, 
			utilib::EnumDataOwned own_=utilib::DataNotOwned)
		{
		ref = NULL;
		solver_type = PicoLPSolver::none;
		init(lp_,own_);
		setup_parameters();
		}

  /// Constructor initializes LP pointer
  PicoLPSolver(PicoLPSolver::SolverType type)
		{
		ref = NULL;
		solver_type = PicoLPSolver::none;
		init(type);
		setup_parameters();
		}

  /// Copy constructor
  PicoLPSolver(const PicoLPSolver& lp)  : ref(0)
		{
		*this = lp;
		}

  /// Destructor deletes the LP pointer
  ~PicoLPSolver()
		{if (ref && ref->decrement()) delete ref;}

  /// Initialize with a given LP solver interface
  void init(__LPSolver_Interface* lp_, 
			utilib::EnumDataOwned own_=utilib::DataNotOwned)
		{
		solver_type = PicoLPSolver::unknownLP; 
		ref = new RefCount<__LPSolver_Interface>(lp_,own_);
		}

  /// Initialize with a given LP type
  void init(SolverType type);

  /// Initialize by copying another LP object
  void init(const PicoLPSolver& lp)
		{*this &= lp;}

  /// Initialize with the value of the class parameter
  void init();

  ///
  PicoLPSolver& operator=(const PicoLPSolver& lp);

  ///
  PicoLPSolver& operator&=(const PicoLPSolver& lp)
        {
	if (ref != lp.ref) {
           if (ref && ref->decrement()) delete ref;
           ref = lp.ref;
           ref->increment();
	   solver_type = lp.solver_type;
	   }
        return *this;
        }
  ///
  __LPSolver_Interface* operator()() {return ref->data();}

  ///
  __LPSolver_Interface& operator*() {return *(ref->data());}

  /// A flag to detect if the LP pointer has been set.
  operator bool()
	{return ref;}

  /// Set the default LP type using a char string
  static SolverType lp_type(const char*);

  /// The default LP type
  static SolverType default_type;

  /// Get the type of solver

  SolverType getLPSolverType()
    {return(solver_type);};

  ///
  static const unsigned int num_solvers;

  ///
  static const char* solverlist[PICOLPSOLVER_NUMSOLVERS];

protected:

  ///
  __LPSolver_Interface* allocate_lp(PicoLPSolver::SolverType type,
				__LPSolver_Interface* old_lp=0);

  ///
  RefCount<__LPSolver_Interface>* ref;

  ///
  PicoLPSolver::SolverType solver_type;

  ///
  void setup_parameters();

  /// String used for the class parameter "lpType"
  std::string lpType_str;

  #ifdef USING_CPLEX
  ///
  bool cplex_flag;
  #endif

  #ifdef ACRO_USING_SOPLEX
  ///
  bool soplex_flag;
  #endif

  #ifdef ACRO_USING_CLP
  ///
  bool clp_flag;
  #endif

  #ifdef ACRO_USING_GLPK
  ///
  bool glpk_flag;
  #endif
};

} // namespace pico

#endif

#endif
