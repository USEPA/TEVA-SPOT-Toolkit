/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// parMIPTest.cpp
//
// Test of parallel MILP
//

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <utilib/seconds.h>
#include <pico/milp.h>
#include <pico/parMilp.h>

using using namespace std;

namespace pico;

#if defined(COUGAR)
extern "C" void heap_summary();
extern "C" void print_links();
#endif


int main(int argc, char** argv)
{
try {

  InitializeTiming();

  uMPI::init(&argc,&argv,MPI_COMM_WORLD);

  CommonIO::begin();
  CommonIO::setIOFlush(1);

  parMILP milp;
  MILPProblem problem;
  if (milp.setup(argc,argv,problem)) {
     milp.reset();
     milp.solve();
     }

  CommonIO::end();
  uMPI::done();

}
STD_CATCH(;)

return 0;
}


#else

int main()
{
  return -1;
}

#endif



#if 0

/// WEH - this is the old parMIP solver interface

#ifndef ANSI_HDRS
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#else
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <csignal>
#endif
#ifdef ACRO_HAVE_VALUES_H
#include <values.h>
#endif
#ifdef NON_ANSI
#include <iostream.h>
#include <fstream.h>
#else
#include <iostream>
#include <fstream>
#endif

#include <utilib/_math.h>
#include <utilib/seconds.h>
#include <utilib/mpiUtil.h>
#include <utilib/memdebug.h>
#include <utilib/ParameterList.h>
#include <pico/fundamentals.h>
// #include >lp.h>
#include <pico/milp.h>
#include <pico/gRandom.h>
#include <pico/parMilp.h>

#ifdef ANSI_NAMESPACES
using namespace std;
#endif
using namespace pico;
using namespace utilib;

#if defined(SOLARIS) | defined(LINUX)
extern "C" void p4_error(char*, int);
#endif

#if defined(COUGAR)
extern "C" void heap_summary();
extern "C" void print_links();
#endif

extern void exit_fn();


int program_running = 0;


int main(int argc, char** argv) 
{
  program_running = 1;
  try {

    //  signal(SIGINT,pico_abort_handler);
    //  signal(SIGKILL,pico_abort_handler);
    //  signal(SIGTERM,pico_abort_handler);
    //  signal(SIGSEGV,pico_abort_handler);

    InitializeTiming();
    uMPI::init(&argc,&argv,MPI_COMM_WORLD);

    CommonIO::begin();
    CommonIO::setIOFlush(1);
    int repeat=1;
    int  debug=0;
    bool debug_solver_params=false;
    int debugSeqDigits=0;
    bool ofile=false;
    bool stallForDebug=false;
    bool use_abort=false;

    utilib::ParameterSet params;
    params.create_parameter("repeat",repeat,"<int>","1",
          "TODO",utilib::ParameterLowerBound<int>(1));
    params.create_parameter("debug",debug,"<int>","0",
          "TODO",utilib::ParameterLowerBound<int>(0));
    params.create_parameter("debug-solver-params",debug_solver_params,
                "<bool>","false",
                "If true, print solver parameters");
    params.create_parameter("debugSeqDigits",debugSeqDigits,"<int>","0",
          "TODO",utilib::ParameterBounds<int>(0,10));
    params.create_parameter("ofile",ofile,
                "<bool>","false",
                "If true, create the output file parMIP-out");
    params.create_parameter("stallForDebug",stallForDebug,
                "<bool>","false",
                "If true, then stall the code for debugging");
    params.create_parameter("use_abort",use_abort,
                "<bool>","false",
                "If true, then abort when an error occurs");

    ParameterList plist;
    plist.register_parameters(params);
    plist.register_parameters<PicoLPSolver>();
    plist.register_parameters<MILPProblem>();
    plist.register_parameters<BBHConfig>();
    plist.register_parameters<parMILP>();
    plist.process_parameters(argc,argv,1);
    params.set_parameters(plist,false);

    if ((argc > 2) || params.get_parameter<bool>("help")) 
      {
	if (uMPI::iDoIO)
	  {
	    cout << "Usage: " << argv[0] 
		 << " {--parameter=value ...} MPS-file\n";
	    cout << "Usage: " << argv[0] 
		 << " {--parameter=value ...} LP-file\n";
	    cout << "Usage: " << argv[0] 
		 << " {--parameter=value ...} AMPL-model-file AMPL-data-file\n";
	    plist.write_registered_parameters(cout);
	  }
	CommonIO::end();
	uMPI::done();
	return -1;
      }

    if (use_abort) {
       utilib::exception_mngr::set_mode(utilib::exception_mngr::Abort);
       utilib::exception_mngr::set_exit_function(&exit_fn);
       }
    if (debug >= 100) ucout << "Past parameter initialization" << endl;
    ucout << Flush;

    //To allow debugger to attach to a second process
    if (stallForDebug)
      {
        ucout << uMPI::rank << " " << getpid() << endl << Flush;
	if (uMPI::rank == 0) {
	   int staller;
	   cin >> staller;
	   }
	uMPI::barrier();
      } 

    if (ofile) CommonIO::set_ofile("parMIP-out");
    CommonIO::numDigits = debugSeqDigits;

    MILPProblem problem;
    problem.set_parameters(plist,false);
    problem.lp.set_parameters(plist,false);
    problem.lp.init();
    problem.lp()->set_parameters(plist,false);
    if (uMPI::iDoIO && debug_solver_params) {
       ucout << "---- PicoLPSolver Parameters ----" << endl;
       problem.lp.write_parameter_values(ucout);
       ucout << endl << Flush;
       ucout << "---- PicoLPInterface Parameters ----" << endl;
       problem.lp()->write_parameter_values(ucout);
       ucout << endl << Flush;
       ucout << "---- MILPProblem Parameters ----" << endl;
       problem.write_parameter_values(ucout);
       ucout << endl << Flush;
       }

    for(int i=0; i < repeat; i++)
      {
	gRandomReSeed();
	parMILP * instance = new parMILP;  // pool type taken from global params
	instance->setMILPProblem(problem);
        instance->set_parameters(plist,false);
	instance->heurCfg.set_parameters(plist,false);
	instance->setDebugLevel(debug);
        instance->reset();
	ucout << Flush;
        if (uMPI::iDoIO && debug_solver_params) {
           ucout << "---- parMILP Parameters ----" << endl;
           instance->write_parameter_values(ucout);
           ucout << endl << Flush;
           }

	instance->readAndBroadcast(&argc,&argv);
	instance->printConfiguration(ucout);
	instance->search();

	ucout << Flush;
	CommonIO::end_tagging();
	if (uMPI::iDoIO)
	  {
	    if (instance->problemIsFeasible())
	      ucout << endl << "Final Solution:  Value = " 
		    << instance->incumbentValue << endl << endl;
	    else ucout << "\nProblem integer infeasible\n\n";
	  }
	instance->parallelSolutionToFile();
	instance->printAllStatistics(ucout);

	CommonIO::end();

	delete instance;
      }

    uMPI::done();
    program_running = 0;

    //  MEMDEBUG_DUMP(ucout);
    
  }


#if defined(COUGAR)
  MIP_CATCH( CommonIO::end(); heap_summary(); MPI_Abort(MPI_COMM_WORLD,1); )
#else
  MIP_CATCH( CommonIO::end(); MPI_Abort(MPI_COMM_WORLD,1); )
#endif

}

#endif
