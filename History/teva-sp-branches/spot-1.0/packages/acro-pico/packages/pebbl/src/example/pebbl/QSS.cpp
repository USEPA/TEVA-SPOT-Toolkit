/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file QSS.cpp
 * \author William Hart
 *
 * QSS driver.
 */

#include <utilib/seconds.h>
#include <pebbl/serialQSS.h>

using namespace pebbl;


int main(int argc, char* argv[])
{
try {
  ///
  /// Reset the UTILIB global timing information
  ///
  InitializeTiming();
  ///
  /// If we're using MPI, then initialize the MPI data structures
  ///
#if defined(ACRO_HAVE_MPI) && defined(PARALLEL_QSS)
  uMPI::init(&argc,&argv,MPI_COMM_WORLD);
  int nprocessors = uMPI::size;
  ///
  /// Do parallel optimization if MPI indicates that we're using more than
  /// one processor
  ///
  if (parallel_exec_test<parallelBranching>(argc,argv,nprocessors)) {
     ///
     /// Manage parallel I/O explicitly with the utilib::CommonIO tools
     ///
     CommonIO::begin();
     CommonIO::setIOFlush(1);
     ///
     /// Create the optimizer, initialize it with command line parameters
     /// and run it.
     ///
     parallelQSS optimizer;
     if (optimizer.setup(argc,argv)) {
        optimizer.reset();
        optimizer.solve();
        }
     ///
     /// Clean up parallel I/O
     ///
     CommonIO::end();
     }
  else {
#endif
     ///
     /// Do serial optimization
     ///
     serialQSS optimizer;
     if (optimizer.setup(argc,argv)) {
        optimizer.reset();
        optimizer.solve();
        }
#ifdef USING_MPI
     }
  uMPI::done();
#endif
  }
///
/// Use a standard block of catch routines, which catches all 
/// STL exception types explicitly
///
STD_CATCH(;)

return 0;
}



#if 0
using namespace utilib;
using namespace pico;

extern void exit_fn();

int main(int argc, char* argv[])
{
//
// Uncomment the following if you want to make exceptions generate aborts,
// which is useful for debugging
//
utilib::exception_mngr::set_exit_function(exit_fn);

try {

    int        debug=0;
    bool       debug_solver_params=false;
    bool       using_abort=false;

    utilib::ParameterSet params;
    params.create_parameter("debug",debug,"<int>","0",
          "TODO",utilib::ParameterLowerBound<int>(0));
    params.create_parameter("debug-solver-params",debug_solver_params,
                "<bool>","false",
                "If true, print solver parameters");
    params.create_parameter("using-abort",using_abort,
                "<bool>","false",
                "If true, then abort on an error");

    ParameterList plist;
    plist.register_parameters(params);
    plist.register_parameters<serialBB>();
    plist.process_parameters(argc,argv,1);
    params.set_parameters(plist,false);
    if (params.get_parameter<bool>("help")) {
       plist.write_registered_parameters(ucout);
       return -1;
       }

   if (using_abort)
      utilib::exception_mngr::set_mode(utilib::exception_mngr::Abort);
   //
   //  Initialization step
   //
   InitializeTiming();
   //
   // Construct the optimizer
   //
   ucout << "Solving problem: " << argv[1] << endl;

   serialBB opt;
   opt.set_parameters(plist,false);
   //
   // Initialize the optimizer with the filename
   //
   opt.initialize(argv[1]);
   opt.reset();
   if (debug_solver_params)
      opt.write_parameter_values(ucout);
   ucout << endl << "Running serialBB" << endl;
   //
   // Run the optimizer
   //
   opt.search();
   cout << endl << "Final Solution:  Value = "
	      << opt.incumbentValue << endl << endl;
   opt.solutionToFile();
   opt.printAllStatistics();
   }
catch (std::exception& err) {
   cerr << "Error running serialBB - " << err.what() << endl;
   }

return 0;
}
#endif
