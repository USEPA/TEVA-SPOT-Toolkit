/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file default_rng.h
 * 
 * Header for access to the default random number generator.
 *
 * \todo cleanup the documentation for the rest of this header.
 */

#ifndef utilib_default_rng_h
#define utilib_default_rng_h

#include <utilib/utilib_config.h>
#if defined(__cplusplus) || defined(DOXYGEN)
#include <utilib/PM_LCG.h>

namespace utilib {
/**
 * A global rng that can be used as a default rng for function arguments.
 */
extern PM_LCG default_rng;

} // namespace utilib
#endif

#ifdef __cplusplus
extern "C" {
#endif
/**
 * A global routine that uses the \c default_rng rng to call a uniform number
 * generator.
 */
extern double LEC_runif(void);
#ifdef __cplusplus
}
#endif

/**
 * A macro that can be used in C to access a random number generator for uniform
 * random variables.
 */
#ifdef OLDSTYLE_RANLIB
#define ranf()  LEC_runif()
#else
#define ranf()  (*global_runif)()
#ifdef __cplusplus
extern "C" {
#endif
/**
 * A pointer to a function that accesses a random number generator
 * for uniform random variables. 
 */
extern double (*global_runif)(void);
#ifdef __cplusplus
}
#endif

/**
 * \def set_global_runif
 *
 * A macro for setting the function that provides global access to uniform 
 * random variables.
 */
#define set_global_runif(func)  global_runif = func
#endif

#endif
