/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// LCG.cpp
//
// Implement the methods for LCG
//


#ifdef _MSC_VER
#include <time.h>
#else
#if defined(SGI)
#include <ctime>
#endif
#include <sys/time.h>
#endif

//
// Play some games to make sure that we get access to the right
// random number generator as well as the gettimeofday routine
//
 
#ifdef ANSI_HDRS
#include <cstdlib>
#else
#include <stdlib.h>
#endif // ANSI_HDRS
extern "C" {
long nrand48(unsigned short *xsubi);
};
// WEH - what platforms need this???
//#include <rand48.h>
#include <utilib/_generic.h>
#include <utilib/LCG.h>
 
#ifdef SUN4
extern "C" {
int gettimeofday(struct timeval* tp, struct timezone* tzp);
};
#endif


namespace utilib {

//
// The globally available random number generate that is used as a default
//
// LCG default_rng;




//
//
// Methods for LCG
//
//


unsigned long LCG::asLong()
{
#ifdef _MSC_VER
unsigned long temp = (unsigned long) 0.0;
double foo=0.0,tmp = 1.0/foo;
#else
nrand48(State);
unsigned long temp = (unsigned long) nrand48(State);
#endif
return(temp);
}



void LCG::reset()
{ 
/* Use _ftime _timeb stuff here
struct timeval tv;
struct timezone tz;

if (Seed == 0) {
   gettimeofday(&tv,&tz);
   State[0] = (unsigned short) tv.tv_usec;
   }
else {
   State[0] = (unsigned short) Seed;
   }
*/

State[1] = 0;
State[2] = 0;
}


void LCG::reseed(unsigned seed)
{
Seed = seed;
State[0] = State[1] = State[2] = 0;
reset();
}


LCG::LCG(unsigned short seed)
{ reseed(seed); }


#if defined(COUGAR) || defined(SGI) || defined(OSF) || defined(RS6K) || defined(CPLANT) || defined(TFLOPS_SERVICE)
void LCG::write(ostream& os) const
#else
void LCG::write(std::ostream& os) const
#endif
{
os << Seed << "\n";
os << State[0] << " " << State[1] << " " << State[2] << "\n";
}


#if defined(COUGAR) || defined(SGI) || defined(OSF) || defined(RS6K) || defined(CPLANT) || defined(TFLOPS_SERVICE)
void LCG::read(istream& is)
#else
void LCG::read(std::istream& is)
#endif
{
is >> Seed;
is >> State[0] >> State[1] >> State[2];
}

} // namespace utilib

