/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// Ereal.cpp
//

#include <utilib/utilib_config.h>
#include <utilib/Ereal.h>
#include <utilib/_math.h>


#if !defined(AIX_CC)
namespace utilib {

#ifdef UTILIB_HAVE_LONG_DOUBLE    // long double is longer than double

template<>
long double Ereal<long double>::positive_infinity_val = MAXLONGDOUBLE;
template<>
long double Ereal<long double>::negative_infinity_val = -MAXLONGDOUBLE;

#else

template<>
double Ereal<long double>::positive_infinity_val = 1e307;
template<>
double Ereal<long double>::negative_infinity_val = -1e307;

#endif

template<>
double Ereal<double>::positive_infinity_val = 1e307;

template<>
double Ereal<double>::negative_infinity_val = -1e307;

} // namespace utilib

#endif
