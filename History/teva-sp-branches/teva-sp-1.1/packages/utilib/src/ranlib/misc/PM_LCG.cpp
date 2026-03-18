/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// PM_LCG.cpp
//
// Implement the methods for LCG
//

#include <utilib/PM_LCG.h>
#include <utilib/seconds.h>
#include <utilib/_generic.h>
#include <utilib/_math.h>
 
extern "C" int PMrand(int*);

namespace utilib {

//
// The globally available random number generate that is used as a default
//
PM_LCG default_rng;



//
//
// Methods for LCG
//
//
unsigned long PM_LCG::asLong()
{
////
//// The first use of &state is not changed here
///
unsigned long temp = (unsigned long) PMrand(&state);
return(temp);
}


double PM_LCG::asDouble()
{
double ans = asLong()/ (double)(2147483647);
return ans;
}


void PM_LCG::reset()
{
if (jseed <= 0) // generate a random seed
  jseed = abs(std::max((int) CurrentTime(), 1));
state = jseed;
}


#if defined (COUGAR) || defined(SGI) || defined(OSF) || defined(CPLANT)
void PM_LCG::write(ostream& os) const
#else
void PM_LCG::write(std::ostream& os) const
#endif
{
os << jseed << " ";
os << state << "\n";
}


#if defined (COUGAR) || defined(SGI) || defined(OSF) || defined(CPLANT)
void PM_LCG::read(istream& is)
#else
void PM_LCG::read(std::istream& is)
#endif
{
is >> jseed >> state;
}

} // namespace utilib

