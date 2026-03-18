/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// PICO.cpp
//


#include <acro_config.h>
#include <utilib/seconds.h>
#include <pico/BCMip.h>
#include <pico/parBCMip.h>

using namespace pico;
using namespace std;

#if defined(COUGAR)
extern "C" void heap_summary();
extern "C" void print_links();
#endif


bool runSerial(int argc,char** argv)
{
  BCMip milp;
  BCMipProblem problem;
   
  if (!milp.setup(argc,argv,problem)) 
    return false;

  milp.reset();
  milp.solve();

  return true;
}


int main(int argc, char** argv)
{
  bool flag = false;

  try {

    InitializeTiming();

#ifdef ACRO_HAVE_MPI

    uMPI::init(&argc,&argv,MPI_COMM_WORLD);
    int nprocessors = uMPI::size;

    if (pebbl::parallel_exec_test<pebbl::parallelBranching>(argc,argv,nprocessors)) 
      {
	CommonIO::begin();
	CommonIO::setIOFlush(1);

	parBCMip milp;
	BCMipProblem problem;

	flag = milp.setup(argc,argv,problem);

	if (flag) 
	  {
	    milp.reset();
	    milp.solve();
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

  STD_CATCH(;)

  return (flag == false);
}
