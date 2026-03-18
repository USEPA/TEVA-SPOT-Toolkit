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
#include <pebbl/parKnapsack.h>

using namespace pebbl;
using namespace std;


bool runSerial(int argc,char** argv)
{
  binaryKnapsack instance;
  bool flag = instance.setup(argc,argv);
  if (flag)
    {
      instance.reset();
      instance.solve();
    }
  return flag;
}


int main(int argc, char* argv[])
{
  bool flag = true;

  try 
    {

      ///
      /// Reset the UTILIB global timing information
      ///
      InitializeTiming();

#ifdef ACRO_HAVE_MPI

      uMPI::init(&argc,&argv,MPI_COMM_WORLD);
      int nprocessors = uMPI::size;

      if (parallel_exec_test<parallelBranching>(argc,argv,nprocessors)) 
	{
	  CommonIO::begin();
	  CommonIO::setIOFlush(1);

	  parallelBinaryKnapsack instance;
	  flag = instance.setup(argc,argv);
	  if (flag)
	    {
	      instance.reset();
	      instance.printConfiguration();
	      instance.solve();
	    }

	  CommonIO::end();
	}
      else 
	flag = runSerial(argc,argv);

      uMPI::done();

#else

      flag = runSerial(argc,argv);

#endif

    }

  ///
  /// Use a standard block of catch routines, which catches all 
  /// STL exception types explicitly
  ///
  STD_CATCH(;)

  return !flag;
}
