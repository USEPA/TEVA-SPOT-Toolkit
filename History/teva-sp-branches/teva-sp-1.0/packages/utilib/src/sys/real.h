/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file real.h
 *
 * Define the type of floating point representation that is commonly used.
 *
 * \author William E. Hart
 */

#ifndef utilib_real_h
#define utilib_real_h

#ifdef REAL
#undef REAL
#endif

/**
 * \def REALType
 *
 * Flag that is used to conditionally compile codes that work with REAL
 * data types based upon the underlying data type.
 */
#define REALType DoubleType
/**
 * \typedef REAL
 *
 * Defines the datatype commonly used for floating point representations.
 * In principle, this type could be set to double, float, etc.  However,
 * in practice, this has only been used with the type 'double', and there
 * are known mathematical routines for which these types are not 
 * interchangable.
 */
typedef double REAL;

#endif
