/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file IntVector.h
 *
 * Defines the \c IntVector typedef
 */

#ifndef utilib_IntVector_h
#define utilib_IntVector_h

#include <utilib/utilib_config.h>
#include <utilib/NumArray.h>

namespace utilib {

/**
 * \fn typedef NumArray<int> IntVector
 *
 * Type for NumArray<int> classes.
 */
typedef NumArray<int> IntVector;

} // namespace utilib

#endif
