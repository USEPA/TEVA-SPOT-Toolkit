/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file utilib_config_explicit.h
 *
 * Management for the use of 'explicit' in C++ code.
 */

#ifndef config_explicit_h
#define config_explicit_h
#include <utilib/utilib_config.h>
#ifndef UTILIB_HAVE_EXPLICIT
#define explicit
#endif
#endif
