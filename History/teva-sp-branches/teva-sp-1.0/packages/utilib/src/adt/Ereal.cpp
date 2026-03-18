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

#include <utilib/Ereal.h>
#include <utilib/_math.h>


namespace utilib {

#if !defined( COUGAR) && !defined(RS6K) && !defined(TFLOPS_SERVICE) && !defined(REDSTORM)
template<>
double Ereal<double>::positive_infinity_val = MAXDOUBLE;


template<>
double Ereal<double>::negative_infinity_val = -MAXDOUBLE;


template<>
float Ereal<float>::positive_infinity_val = MAXFLOAT;


template<>
float Ereal<float>::negative_infinity_val = -MAXFLOAT;
#endif

} // namespace utilib
