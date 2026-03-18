/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file ILPArgList.h
 *
 * Defines the pico::ILPArgList typedef.
 */

#ifndef pico_ILPArgList_h
#define pico_ILPArgList_h

#include <acro_config.h>
#include <utilib/BasicArray.h>
#include <pico/ILPArgVal.h>

namespace pico {

typedef BasicArray<ILPArgVal> ILPArgList;

} // namespace pico

#endif
