/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// serialMIPTest.cpp
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

/// WEH - this is the old serialMIP solver interface

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
#include <utilib/paramTable.h>
#include <utilib/memdebug.h>
#include <utilib/ParameterList.h>
#include <pico/fundamentals.h>
#include <pico/milp.h>


#ifdef ANSI_NAMESPACES
using namespace std;
#endif
using namespace utilib;
using namespace pico;


#if defined(COUGAR)
extern "C" void heap_summary();
extern "C" void print_links();
#endif

extern void exit_fn();
extern "C" void pico_abort_handler(int code);


int main(int argc, char** argv)
{

  try {

    int repeat=1;
    bool rootLP=false;
    bool preprocessOnly=false;
    bool preprocessIP=false;
    bool onlyRootLP=false;
    bool saveRootBoundingInfo=false;
    bool use_abort=false;

    int  debug=0;
    bool debug_solver_params=false;

    utilib::ParameterSet params;

    params.create_parameter("repeat",repeat,"<int>","1",
	  "TODO",utilib::ParameterLowerBound<int>(1));

    params.create_parameter("debug",debug,"<int>","0",
	  "TODO",utilib::ParameterLowerBound<int>(0));

    params.create_parameter("debug-solver-params",debug_solver_params,
                "<bool>","false",
                "If true, print solver parameters");

    params.create_parameter("rootLP",rootLP, "<bool>","false","TODO");

    params.create_parameter("preprocessOnly",preprocessOnly,
	  "<bool>","false","TODO");

    params.create_parameter("preprocessIP",preprocessIP,
	  "<bool>","false","TODO");

    params.create_parameter("use-abort",use_abort,
	  "<bool>","false","If true, then force an abort when an error occurs.");

    params.create_parameter("onlyRootLP",onlyRootLP, "<bool>","false","TODO");

    params.create_parameter("saveRootBoundingInfo",saveRootBoundingInfo,
        "<bool>","false",
        "Output root bounding information (e.g. values of integer\n\t"
	"variables) to the file root-bounding-info.bin.");

    ParameterList plist;
    plist.register_parameters(params);
    plist.register_parameters<PicoLPSolver>();
    plist.register_parameters<BBHConfig>();
    plist.register_parameters<MILPProblem>();
    plist.register_parameters<MILP>();
    plist.process_parameters(argc,argv,1);
    params.set_parameters(plist,false);
    if (params.get_parameter<bool>("help")) {
       plist.write_registered_parameters(ucout);
       return -1;
       }
    if (saveRootBoundingInfo && !onlyRootLP)
      EXCEPTION_MNGR(runtime_error, "saveRootBoundingInfo option can "
		     "only be used in conjunction with onlyRootLP");
    utilib::exception_mngr::set_exit_function(&exit_fn);

    if (use_abort) {
       utilib::exception_mngr::set_mode(utilib::exception_mngr::Abort);
       signal(SIGFPE,pico_abort_handler);
       }
    InitializeTiming();

    MILPProblem problem;
    problem.set_parameters(plist,false);
    problem.lp.set_parameters(plist,false);
    if (debug_solver_params) {
       ucout << "---- PicoLPSolver Parameters ----" << endl;
       problem.lp.write_parameter_values(ucout);
       ucout << endl << Flush;
       }
    problem.lp.init();
    problem.lp()->set_parameters(plist,false);
    if (debug_solver_params) {
       ucout << "---- PicoLPInterface Parameters ----" << endl;
       problem.lp()->write_parameter_values(ucout);
       ucout << endl << Flush;
       }

    BasicArray<int> listOfInts;

    //
    // for debugging, to catch transient bugs
    //
#ifdef ACRO_VALIDATING
    for (int i=0; i< repeat; i++)
      {
#endif
	// Assuming that the first argument is an MPS filename
	if (argc == 1)
	  EXCEPTION_MNGR(runtime_error, "No MPS file given as input");
	/* Remove cplex_mps_format once WH's MPS reader is fixed. */
        if (argc == 2)
	   problem.lp()->loadProblem(argv[1], listOfInts);
        else
	   problem.lp()->loadProblem(argv[1], argv[2], listOfInts);
#ifdef ACRO_VALIDATING
	if (debug > 2) {
	  problem.lp()->write(ucout,verbose_format);
	  ucout << Flush;
	}
	if (preprocessOnly)
	  {
	    problem.initialize(listOfInts, debug);
	    problem.set_parameters(plist,false);
	    problem.preprocess();
	  }
	else
#endif
	  if ((listOfInts.size() == 0) || (onlyRootLP == 1)) {
	    ucout << "No integer variables... solving the LP normally." 
		  << endl;

	    if (preprocessIP) {
	      problem.initialize(listOfInts, debug);
	      problem.set_parameters(plist,false);
	      problem.preprocess();
	    }
	    problem.lp()->solve();
	    PicoLPInterface::problemState solvedState = problem.lp()->state();
	    if (saveRootBoundingInfo)
	      {
	      ofstream savedSolution("root-bounding-info.bin");
	      if (savedSolution.bad())
		EXCEPTION_MNGR(runtime_error, 
				"Can't open file to save root solution");
	      // May want to adjust precision here!

	      savedSolution << (int)solvedState;
	      // Don't let numbers run together.  This isn't a packbuffer.
	      savedSolution << " ";

	      if (solvedState == PicoLPInterface::solved)
		{
		savedSolution << problem.lp()->getObjValue() << " ";
		// Any changes to MILPNode::getSolution() should propagate here
		DoubleVector solution(listOfInts.size());
		problem.lp()->getColSolution(solution.data(), 
				(int)solution.size(), listOfInts.data());
		savedSolution << solution << " ";
		// Any changes to the way basis information is 
		// stored/retrieved should propagate here.
		BasisArray basis(problem.lp()->getNumCols() + 
						problem.lp()->getNumRows());
		problem.lp()->getBasis(basis);
		savedSolution << basis;
		}

	      }
	    if (solvedState == PicoLPInterface::unbounded)
	      {ucout << endl << "Problem is unbounded\n";}
	    else if (solvedState == PicoLPInterface::infeasible)
	      {ucout << endl << "Problem is infeasible\n";}
	    
	    else if (solvedState == PicoLPInterface::solved)
	      {

	      ucout << "\tLP value= " << problem.lp()->getObjValue() << endl;
#ifdef ACRO_VALIDATING
	      if (debug > 0)
		{
		DoubleVector fullSolution(problem.lp()->getNumCols());
		problem.lp()->getColSolution(fullSolution.data());
		for (int j = 0; j < problem.lp()->getNumCols(); j++)
		  {
		    if (fullSolution[j] != 0)
		      ucout << "variable " << j << "(= " << 
					problem.lp()->getVariableName(j) << 
					"): " << fullSolution[j] << "\n"; 
		  }
		}
#endif
	      }
	    problem.lp()->reset();
	  }

	  else {
	    //
	    // Setup the MILP
	    //
	    problem.initialize(listOfInts, debug);
    	    problem.set_parameters(plist,false);
	    MILP *instance = new MILP();
	    // CAP: swapped these last two because, for a while, setMILPProblem depends upon
	    // the values of some paramters
	    instance->set_parameters(plist,false);
	    instance->setMILPProblem(problem);
	    instance->heurCfg.set_parameters(plist,false);
	    instance->setName(argv[1]);
	    if (debug_solver_params) {
	       ucout << "---- MILP Parameters ----" << endl;
	       instance->write_parameter_values(ucout);
	       ucout << endl << Flush;
	       }
	    instance->MILPInit();
	    instance->setDebug(debug);

	    // This seems like only a debugging feature.  
            // Not very efficient either.
#ifdef ACRO_VALIDATING
	    if (rootLP) {
	      if (preprocessIP)
		problem.preprocess();
	      problem.lp()->solve();
	      ucout << "Root LP value= " << problem.lp()->getObjValue() 
		    << endl;
	      problem.lp()->reset();
	    }
#endif

	    instance->search();

	    if (instance->problemIsFeasible())
	      ucout << endl << "Final Solution:  Value = " 
		   << instance->incumbentValue << endl << endl;
	    else ucout << "Integer Infeasible\n";
	    instance->solutionToFile();
	    instance->printAllStatistics();

	    delete instance;
	    
	  }
	
#ifdef ACRO_VALIDATING
      }
#endif

  //
  // Verify the all parameters have been processed
  //
  if (plist.unused() > 0) {
     ucout << "\nWARNING: unused parameters" << endl;
     plist.write_unused_parameters(ucout);
     ucout << Flush;
     return -1;
     }
  }

#if defined(COUGAR)
  MIP_CATCH( heap_summary(); )
#else
  MIP_CATCH( ; )
#endif

return 0;
}

#endif
