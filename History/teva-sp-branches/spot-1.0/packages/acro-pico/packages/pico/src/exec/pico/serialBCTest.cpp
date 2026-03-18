/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

#include <acro_config.h>
#include <utilib/seconds.h>
#include <pico/BCMip.h>

using namespace std;
using namespace pico;

#if defined(COUGAR)
extern "C" void heap_summary();
extern "C" void print_links();
#endif


int main(int argc, char** argv)
{
try {

  InitializeTiming();

  int nprocessors=1;

  BCMip milp;
  BCMipProblem problem;
   
  if (milp.setup(argc,argv,problem)) {
     milp.reset();
     milp.solve();
     }

}
STD_CATCH(;)

return 0;
}


#if 0

/// WEH - the old setup for BCMip

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
#ifdef HAVE_ACRO_VALUES_H
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
#include <utilib/ParameterList.h>
#include <utilib/memdebug.h>
#include <pico/fundamentals.h>
#include <pico/BCMip.h>

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

// FOR TESTING


namespace pico {
extern int dummy_CglKnapsackCover;
}

int main(int argc, char** argv)
{
try {

  int repeat=1;
  // Only used for problems that have no integer variables.
  // TODO: see if this doesn't make sense at all (b/c this is only IP
  // preprocessing.  If so, delete this option and never preprocess.
  bool preprocessLP=false;
// We used to have a rootLP parameter, which if true, would compute
// the bound on the root and display this LP relaxation value before
// doing the IP calculation.  But this is very inefficient, since the
// root LP is then solved twice.  For the moment, to get the LP
// relaxation value, I'm forcing the user to use this parameter, which
// will *only* solve the root LP and output its value.  To efficiently
// get the previous rootLP behavior, we'd need to move that parameter
// down to a class such as MILPNode that computes the root bound, or
// use the saveRootBoundingInfo/useSavedRootSolution functionality.
// Note that we need not offer this functionality in the branch-and-cut
// version; the serialMIP executable would be sufficient.  However, I
// hope this driver will become the standard (and presumably better supported) one.
 bool onlyRootLP=false;
 bool saveRootBoundingInfo=false;
#ifdef ACRO_VALIDATING
 bool use_abort=true;
#else
 bool use_abort=false;
#endif

 int  debug=0;
 bool debug_solver_params=false;

 utilib::ParameterSet params;
 params.create_parameter("repeat",repeat,"<int>","1",
	"Number of times to solve the input problem (for debugging)",utilib::ParameterLowerBound<int>(1));
 params.create_parameter("debug",debug,"<int>","0",
	"TODO",utilib::ParameterLowerBound<int>(0));
 params.create_parameter("debug-solver-params",debug_solver_params,
	"<bool>","false",
	"If true, print solver parameters");
 params.create_parameter("preprocessLP",preprocessLP,
	"<bool>","false","Use PICO's IP preprocessor on an LP iff true");
 params.create_parameter("onlyRootLP",onlyRootLP, "<bool>","false","Solve and output only the root LP");
 params.create_parameter("saveRootBoundingInfo",saveRootBoundingInfo,
        "<bool>","false",
        "Output root bounding information (e.g. values of integer\n\t"
	"variables) to the file root-bounding-info.bin.\n\t"
	"Only possible when onlyRootLP is on.\n\t"
        "Use with useSavedRootSolution to seed a later (e.g. large parallel) computation");
#ifdef ACRO_VALIDATING
 params.create_parameter("use-abort",use_abort,
			"<bool>",
			"true",
			"If true, then force an abort when an error occurs.");
#else
 params.create_parameter("use-abort",use_abort,
			"<bool>",
			"false",
			"If true, then force an abort when an error occurs.");

#endif

 ParameterList plist;
 plist.register_parameters(params);
 plist.register_parameters<PicoLPSolver>();
 plist.register_parameters<BCMipProblem>();
 plist.register_parameters<BCMip>();

 plist.process_parameters(argc,argv,1);
 params.set_parameters(plist,false);
 if (params.get_parameter<bool>("help")) {
   plist.write_registered_parameters(cout);
   return -1;
 }


 if (use_abort) {
   utilib::exception_mngr::set_mode(utilib::exception_mngr::Abort);
   utilib::exception_mngr::set_exit_function(&exit_fn);
   signal(SIGFPE,pico_abort_handler);
 }

 InitializeTiming();

 BCMipProblem problem;
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
     EXCEPTION_MNGR(runtime_error, "***No MPS file given as input");
  if (argc == 2)
    problem.lp()->loadProblem(argv[1], listOfInts);
  else
    problem.lp()->loadProblem(argv[1], argv[2], listOfInts);
#ifdef ACRO_VALIDATING
  if (debug > 2) {
    problem.lp()->write(ucout,verbose_format);
    ucout << Flush;
  }
#endif

  if ((listOfInts.size() == 0) || onlyRootLP) {
    ucout << "No integer variables... solving the LP normally." << endl;

    if (preprocessLP) {
      problem.initialize(listOfInts,debug);
      problem.set_parameters(plist,false);
      problem.preprocess();
    }
    problem.lp()->solve();
    PicoLPInterface::problemState solvedState = problem.lp()->state();
    if (solvedState == PicoLPInterface::unbounded)
      {ucout << endl << "Problem is unbounded\n";}
    else if (solvedState == PicoLPInterface::infeasible)
      {ucout << endl << "Problem is infeasible\n";}
  
    else if (solvedState == PicoLPInterface::solved)
      {
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
  }

  else {
     //
     // Setup the MILP
     //
     problem.initialize(listOfInts, debug);
     problem.set_parameters(plist,false);

     BCMip *instance = new BCMip;
     instance->setBCMipProblem(problem);
     instance->set_parameters(plist,false);
     instance->heurCfg.set_parameters(plist,false);
     if (debug_solver_params) {
       ucout << "---- BCMIP Parameters ----" << endl;
       instance->write_parameter_values(ucout);
       ucout << endl << Flush;
     }
     // Initializes the virtual base classes too by default
     instance->BCMipInit();
     instance->setDebug(debug);
     // Don't need to disable this right now.  Keep this (commented out)
     // as an example of the syntax.  TODO:  make sure disabling is done nicely.
          instance->disableBuiltInCutFinder(cutFinder::CGL_kc);
          instance->disableBuiltInCutFinder(cutFinder::CGL_oh);
          instance->disableBuiltInCutFinder(cutFinder::CGL_sr);
          instance->disableBuiltInCutFinder(cutFinder::CGL_LAP);
	  instance->disableBuiltInCutFinder(cutFinder::CGL_P);
     // This is the way to disable the cut pool for now.  FIX THIS!! (TODO)
     //     instance->disableBuiltInCutFinder(cutFinder::ANYCUT);
	  // instance->disableBuiltInCutFinder(cutFinder::CGL_gom);

     instance->search();

     // Handles integer infeasibility too
     instance->serialPrintSolution("\nFinal ","\n");
     instance->solutionToFile();
     instance->printAllStatistics();

     delete instance;  // Does this recycle all the other things allocated?
                    // it shouldn't delete the lp

     }
#ifdef ACRO_VALIDATING
   }
#endif

}
#if defined(COUGAR)
  MIP_CATCH( heap_summary(); )
#else
  MIP_CATCH( ; )
#endif

    /* OLD
catch (const char* str) {
  cerr << "ERROR! Caught serialBCTest exception string: " << str << endl;
}
catch (std::exception& str) {
  cerr << "ERROR! Caught serialBCTest exception object: " << str.what() << endl;
  } */

return 0;
}
#endif
