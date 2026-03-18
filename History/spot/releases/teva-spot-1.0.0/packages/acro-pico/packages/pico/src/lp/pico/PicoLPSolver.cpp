/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// PicoLPSolver.cpp
//

#include <acro_config.h>
#include <utilib/BasicArray.h>
#include <pico/PicoLPSolver.h>



#ifdef ACRO_HAVE_CPLEX
#include <pico/PicoCpxLP.h>
#define __LPSolver_default cplex
#endif

#ifdef ACRO_USING_CLP
#define USE_MEMCPY
#include <pico/PicoClpLP.h>
#ifndef __LPSolver_default
#define __LPSolver_default clp
#endif
#endif

#ifdef ACRO_USING_GLPK
#include <pico/PicoGlpkLP.h>
#ifndef __LPSolver_default
#define __LPSolver_default glpk
#endif
#endif

#ifdef ACRO_USING_SOPLEX
#include <pico/PicoSoplexLP.h>
#ifndef __LPSolver_default
#define __LPSolver_default soplex
#endif
#endif

#ifndef __LPSolver_default
#define __LPSolver_default none
#endif

namespace pico {


const unsigned int PicoLPSolver::num_solvers = PICOLPSOLVER_NUMSOLVERS;

const char* PicoLPSolver::solverlist[] = {
  "default"
  #ifdef ACRO_HAVE_CPLEX
  , "cplex"
  #endif
  #ifdef ACRO_USING_SOPLEX
  , "soplex"
  #endif
  #ifdef ACRO_USING_CLP
  , "clp"
  #endif
  #ifdef ACRO_USING_GLPK
  , "glpk"
  #endif
  };


PicoLPSolver::SolverType PicoLPSolver::default_type = __LPSolver_default;


PicoLPSolver& PicoLPSolver::operator=(const PicoLPSolver& lp)
{
#ifdef ACRO_HAVE_CPLEX
cplex_flag  = lp.cplex_flag;
#endif
#ifdef ACRO_USING_CLP
clp_flag    = lp.clp_flag;
#endif
#ifdef ACRO_USING_GLPK
glpk_flag   = lp.glpk_flag;
#endif
#ifdef ACRO_USING_SOPLEX
soplex_flag = lp.soplex_flag;
#endif
lpType_str = lp.lpType_str;

solver_type = lp.solver_type;
ref = new RefCount<__LPSolver_Interface>(allocate_lp(solver_type,lp.ref->data()),utilib::AssumeOwnership);
return *this;
}



void PicoLPSolver::init()
{
#ifdef ACRO_HAVE_CPLEX
if (cplex_flag) {
   init( cplex );
   return;
   }
#endif

#ifdef ACRO_USING_CLP
if (clp_flag) {
   init( clp );
   return;
   }
#endif

#ifdef ACRO_USING_GLPK
if (glpk_flag) {
   init( glpk );
   return;
   }
#endif

#ifdef ACRO_USING_SOPLEX
if (soplex_flag) {
   init( soplex );
   return;
   }
#endif

if (lpType_str != "") {
   init( lp_type( lpType_str.c_str() ) );
   return;
   }

init( defaultLP );
}


void PicoLPSolver::init(PicoLPSolver::SolverType type)
{
if (type == solver_type)
   return;

if (ref && ref->decrement()) delete ref;

if ((type == defaultLP) && (default_type == none))
   return;

if (type == none)
   return;

if (type == defaultLP)
   type = default_type;

ref = new RefCount<__LPSolver_Interface>(allocate_lp(type),utilib::AssumeOwnership);
solver_type = type;
} 




__LPSolver_Interface* PicoLPSolver::allocate_lp(PicoLPSolver::SolverType type,
			__LPSolver_Interface* old_lp)
{
if (type == defaultLP)
   type = default_type;

__LPSolver_Interface* ans=0;

switch (type) {
	case cplex:
#ifdef ACRO_HAVE_CPLEX
		if (old_lp)
		   ans = new PicoCpxLP(*dynamic_cast<PicoCpxLP*>(old_lp));
		else
		   ans = new PicoCpxLP();
#endif
		if (ans == 0)
		  EXCEPTION_MNGR(std::runtime_error, "Unable to create a cplex LP solver.  Check configuration.\n"); 
		break;

	case soplex:
#ifdef ACRO_USING_SOPLEX
		if (old_lp)
		   ans = new PicoSoplexLP(*dynamic_cast<PicoSoplexLP*>(old_lp));
		else
		   ans = new PicoSoplexLP();
#endif
		if (ans == 0)
		  EXCEPTION_MNGR(std::runtime_error, "Unable to create a soplex LP solver.  Check configuration.\n"); 
		break;

	case glpk:
#ifdef ACRO_USING_GLPK
		if (old_lp)
		   ans = new PicoGlpkLP(*dynamic_cast<PicoGlpkLP*>(old_lp));
		else
		   ans = new PicoGlpkLP();
#endif
		if (ans == 0)
		  EXCEPTION_MNGR(std::runtime_error, "Unable to create a glpk LP solver.  Check configuration.\n"); 

		break;

	case clp:
#ifdef ACRO_USING_CLP
		if (old_lp)
		   ans = new PicoClpLP(*dynamic_cast<PicoClpLP*>(old_lp));
		else
		   ans = new PicoClpLP();
#endif
		if (ans == 0)
		  EXCEPTION_MNGR(std::runtime_error, "Unable to create a CLP LP solver.  Check configuration.\n"); 
		break;

	default:
                return NULL;
	};

// This allow us to set options in the solvers, etc.
 ans->initialize();

return ans;
}


// BUG? typename
PicoLPSolver::SolverType PicoLPSolver::lp_type(const char* str)
{
if (!str) 
   return none;
if (strcmp(str,"default") == 0)
   return __LPSolver_default;
if (strcmp(str,"soplex") == 0)
   return soplex;
if (strcmp(str,"clp") == 0)
   return clp;
if (strcmp(str,"cplex") == 0)
   return cplex;
if (strcmp(str,"glpk") == 0)
   return glpk;
if (strcmp(str,"none") == 0)
   return none;

throw "PicoLPSolver::set_default - unknown LP type";
#if !defined(COUGAR) && !defined(REDSTORM)
return none;
#endif
}


void PicoLPSolver::setup_parameters()
{
lpType_str = "default";
create_parameter("lpType",lpType_str,"<string>","default",
	"Defines the type of LP solver that is initialized.");

#ifdef ACRO_HAVE_CPLEX
cplex_flag=false;
create_parameter("cplex",cplex_flag,"<bool>","false","TODO");
#endif
#ifdef ACRO_USING_SOPLEX
soplex_flag=false;
create_parameter("soplex",soplex_flag,"<bool>","false","TODO");
#endif
#ifdef ACRO_USING_CLP
clp_flag=false;
create_parameter("clp",clp_flag,"<bool>","false","TODO");
#endif
#ifdef ACRO_USING_GLPK
glpk_flag=false;
create_parameter("glpk",glpk_flag,"<bool>","false","TODO");
#endif
}


} // namespace pico
