/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// serialLP.cpp
//

#include <acro_config.h>
#include <utilib/seconds.h>
#include <pico/milp.h>

using namespace std;
using namespace pico;

int main(int argc, char** argv)
{
try {

  InitializeTiming();

  MILP milp;
  milp.set_parameter("onlyRootLP",true);
  MILPProblem problem;

  if (milp.setup(argc,argv,problem)) {
     milp.reset();
     milp.solve();
     }

}
STD_CATCH(;)

return 0;
}


#if 0

/// WEH - this is the old serialLP solver driver


#ifndef ANSI_HDRS
#include <signal.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#else

#include <csignal>
#include <cmath>
#include <cstring>
#include <cstdlib>
#endif
#ifdef NON_ANSI
#include <iostream.h>
#include <fstream.h>
#else
#include <iostream>
#include <fstream>
#endif
#ifdef ACRO_HAVE_VALUES_H
#include <values.h>
#endif

#include <utilib/_math.h>
#include <utilib/seconds.h>
#include <utilib/paramTable.h>
#include <utilib/memdebug.h>
#include <utilib/ParameterList.h>
#include <pico/fundamentals.h>
#include <pico/PicoLPSolver.h>
#include <pico/milp.h>

#ifdef ANSI_NAMESPACES
using namespace std;
#endif
using namespace pico;
using namespace utilib;

#if defined(COUGAR)
extern "C" void heap_summary();
extern "C" void print_links();
#endif

extern void exit_fn();


int main(int argc, char** argv)
{

try {

signal(SIGFPE,pico_abort_handler);

InitializeTiming();
//CommonIO::begin();
//CommonIO::map_off();

int debug=0;
bool debug_solver_params=false;
bool preprocessIP=true;
bool use_abort=false;

ParameterSet params;
params.create_parameter("debug",debug,"<int>","0",
                "Debugging level",ParameterNonnegative<int>());
params.create_parameter("debug-solver-params",debug_solver_params,
		"<bool>","false",
                "If true, print solver parameters");
params.create_parameter("preprocessIP",preprocessIP,"<bool>","false",
                "If true, then preprocess the LP before solving it.");
params.create_parameter("use-abort",use_abort,"<bool>","false",
                "If true, then force an abort when an error occurs.");

ParameterList plist;
plist.register_parameters(params);
plist.register_parameters<PicoLPSolver>();
plist.register_parameters<MILPProblem>();
plist.register_parameters<PicoLPSolver>();
plist.process_parameters(argc,argv,1);
params.set_parameters(plist,false);
if (params.get_parameter<bool>("help")) {
   plist.write_registered_parameters(cout);
   return -1;
   }

if (use_abort) {
   utilib::exception_mngr::set_mode(utilib::exception_mngr::Abort);
   utilib::exception_mngr::set_exit_function(&exit_fn);
   }


MILPProblem problem;
problem.lp.set_parameters(plist,false);
problem.lp.init();
problem.lp()->set_parameters(plist,false);

  if (debug_solver_params) {
     cout << "---- lpSolver Parameters ----" << endl;
     problem.lp.write_parameter_values(cout);
     cout << endl;
     cout << "---- PicoLPInterface Parameters ----" << endl;
     problem.lp()->write_parameter_values(cout);
     cout << endl;
     }

BasicArray<int> listOfInts;

  // Assuming that the first argument is an MPS filename
  if (argc == 1)
     EXCEPTION_MNGR(runtime_error, "***No MPS file given as input");
  __LPSolver_Interface* localLpRef = problem.lp();
  /* Remove cplex_mps_format once WH's MPS reader is fixed. */
  if (argc == 2)
     localLpRef->loadProblem(argv[1], listOfInts);
  else
     localLpRef->loadProblem(argv[1], argv[2], listOfInts);

  if (debug) {
     localLpRef->write(ucout,verbose_format);
     ucout << Flush;
     }
  if (listOfInts.size() != 0)
     ucerr << "Integer variables in an LP... ignoring them." << endl;
  listOfInts.resize(0);

  if (preprocessIP) {
     problem.initialize(listOfInts, debug);
     problem.set_parameters(plist,false);
     problem.preprocess();
     }
 
  localLpRef->initialize();
  localLpRef->solve();
  PicoLPInterface::problemState solvedState = localLpRef->state();
  if (solvedState == PicoLPInterface::unbounded)
    {ucout << endl << "Problem is unbounded\n";}
  else if (solvedState == PicoLPInterface::infeasible)
    {ucout << endl << "Problem is infeasible\n";}
  
  else if (solvedState == PicoLPInterface::solved)
   {

  ucout << "\tLP value= " << localLpRef->getObjValue() << endl;
         ucout << endl << "Primal Solution (nonzeros)" << endl << endl;
	 DoubleVector fullSolution(localLpRef->getNumCols());
	 localLpRef->getColSolution(fullSolution.data());
	 for (int j = 0; j < localLpRef->getNumCols(); j++)
	   {
	   if (fullSolution[j] != 0)
	     ucout << "variable " << j << "(= " << localLpRef->getVariableName(j) << "): " << fullSolution[j] << "\n"; 
	   }

         ucout << endl << "Dual Solution (nonzeros)" << endl << endl;
	 DoubleVector dualSoln(localLpRef->getNumRows());
	 localLpRef->getRowPrice(dualSoln.data());
	 for (int j = 0; j < localLpRef->getNumRows(); j++)
	   {
	   if (dualSoln[j] != 0)
	     ucout << "row " << j << "(= " << localLpRef->getRowName(j) << "): " << dualSoln[j] << "\n"; 
	   }
   }
  else ucout << "Problem not solved, infeasible, or unbounded\n";

     localLpRef->reset();
  

//CommonIO::end();

if (plist.unused() > 0) {
   cout << "\nWARNING: unused parameters" << endl;
   plist.write_unused_parameters(cout);
   return -1;
   } 

}
#if defined(COUGAR)
  MIP_CATCH( heap_summary(); )
#else
  MIP_CATCH( ; )
#endif

}

#endif
