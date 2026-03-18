/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file RealVector.h
 *
 * Depending on the value of REALType, define real vectors as floats or doubles.
 * This is not a robust mechanism and I've only used the default 'double' 
 * option.
 */

#ifndef utilib_RealVector_h
#define utilib_RealVector_h

#include <utilib/real.h>
#if REALType == DoubleType
#include <utilib/DoubleVector.h>
#else
#include <utilib/FloatVector.h>
#endif

namespace utilib {

/**
 * \typedef RealVector
 *
 * The vector class that contains elements of type REAL.
 */
#if REALType == DoubleType
typedef DoubleVector RealVector ;
#else
typedef FloatVector RealVector ;
#endif

} // namespace utilib

#endif
