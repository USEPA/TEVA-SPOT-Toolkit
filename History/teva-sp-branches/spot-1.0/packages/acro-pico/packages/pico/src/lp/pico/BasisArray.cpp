/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// BasisArray.cpp
//

#include <acro_config.h>
#include <pico/BasisArray.h>
#include <pico/lpEnum.h>

using namespace utilib;

#if !defined(SOLARIS)
namespace utilib {

template<>

int        EnumBitArray<1,pico::basisState>::enum_count = 0;

template<>
char*      EnumBitArray<1,pico::basisState>::enum_labels = "";

template<>
pico::basisState *EnumBitArray<1,pico::basisState>::enum_vals = NULL;

} // namespace pico
#endif
