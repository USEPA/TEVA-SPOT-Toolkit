/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
//  gRandom.cpp
//
//  A global uniform random number -- coding convenience.
//
// Jonathan Eckstein
//


#include <acro_config.h>
#ifdef ACRO_HAVE_MPI
#include <utilib/mpiUtil.h>
#endif
#include <utilib/_math.h>
#include <utilib/_generic.h>
#include <utilib/Uniform.h>
#include <utilib/default_rng.h>
//#include <utilib/paramTable.h>
#include <pebbl/fundamentals.h>

using namespace utilib;

namespace pebbl {


PM_LCG  gRandomLCG(1);
Uniform gRandom(&gRandomLCG);


int randomSeed=1;


RNG* gRandomRNG() {return &gRandomLCG;}

void gRandomReSeed(int seed, bool processorVariation)
{
#ifdef ACRO_HAVE_MPI
  if (processorVariation && uMPI::running())
    {
      PM_LCG* rng = new PM_LCG(std::max(seed,1) + uMPI::rank);
      int numTwiddles = rng->asLong() % 10;
      for (int i=0; i<numTwiddles; i++)
        rng->asLong();
      seed = (int) rng->asLong();
    }
#endif
  gRandomLCG.reseed(std::max(seed,1));
}

void gRandomReSeed()
{ 
  gRandomReSeed(randomSeed,true/*processor variation*/); 
}

} // namespace pebbl
