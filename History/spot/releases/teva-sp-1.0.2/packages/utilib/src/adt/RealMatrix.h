/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file RealMatrix.h
 *
 * Depending on the value of REALType, define real matrices as floats or 
 * doubles.  This is not a robust mechanism and I've only used the default 
 * 'double' option.
 */


#ifndef utilib_RealMatrix_h
#define utilib_RealMatrix_h

#include <utilib/real.h>
#if REALType == DoubleType
#include <utilib/DoubleMatrix.h>
#else
#include <utilib/FloatMatrix.h>
#endif

namespace utilib {

/**
 * \typedef RealMatrix
 *
 * The matrix class that contains elements of type REAL.
 */
#if REALType == DoubleType
typedef DoubleMatrix RealMatrix ;
#else
typedef FloatMatrix RealMatrix ;
#endif

} // namespace utilib

#endif
