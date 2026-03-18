/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file IntMatrix.h
 *
 * Defines the \c IntMatrix typedef
 */

#ifndef utilib_IntMatrix_h
#define utilib_IntMatrix_h

#include <utilib/Num2DArray.h>

namespace utilib {

/**
 * \typedef IntMatrix
 *
 * Type for Num2DArray<int> classes.
 */
typedef Num2DArray<int> IntMatrix;

} // namespace utilib

#endif
