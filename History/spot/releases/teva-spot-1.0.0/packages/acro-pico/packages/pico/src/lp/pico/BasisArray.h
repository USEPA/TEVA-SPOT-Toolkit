/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file BasisArray.h
 *
 * Defines the pico::BasisArray enumerated type.
 */

#ifndef pico_BasisArray_h
#define pico_BasisArray_h

#include <acro_config.h>
#include <utilib/EnumBitArray.h>
#include <pico/lpEnum.h>

namespace pico {

///
/// This object implements an array of two-bit values that correspond
/// to the enumerated types in basisState.  The main elements of this array
/// have the same look and feel as a SimpleArray object.  However, this
/// object uses a special, compact representation of the data elements, which
/// forces it to be a separate object.  This object does not impose a
/// restriction on the maximum length of the array.
///
/// When writing out a BasisArray, five states are represented:
///      L       atLower
///      B       basic
///      U       atUpper
///      N       nonbasic
///      x       error (internal problem with BasisArray
///
typedef utilib::EnumBitArray<1,basisState> BasisArray;

} // namespace pico


#endif
