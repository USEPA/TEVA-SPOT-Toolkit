/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file lpEnum.h
 *
 * Defines LP enumeration types.
 */

#ifndef pico_lpEnum_h
#define pico_lpEnum_h

#include <acro_config.h>
#include <utilib/enum_def.h>
#include <utilib/PackBuf.h>

namespace pico {

enum basisState { nonbasic=0, basic=1, atUpper=2, atLower=3 };

enum priceRule { defaultPricing, steepestEdgePricing };

// Used for LP time-limit stopping criteria
enum clockType {cpuTime, wallTime};
 
}  // namespace pico

ENUM_STREAMS( pico::basisState )

#endif
