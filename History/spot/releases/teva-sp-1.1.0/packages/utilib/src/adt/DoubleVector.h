/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file DoubleVector.h
 *
 * Defines the DoubleVector typedef
 */

#ifndef utilib_DoubleVector_h
#define utilib_DoubleVector_h

#include <utilib/NumArray.h>

namespace utilib {

/**
 * \typedef DoubleVector
 *
 * Type for NumArray<double> classes.
 */
typedef NumArray<double> DoubleVector;

} // namespace utilib

#endif
