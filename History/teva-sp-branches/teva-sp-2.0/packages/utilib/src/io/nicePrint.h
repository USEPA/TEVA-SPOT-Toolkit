/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file nicePrint.h
 *
 * Defines helper functions for printing.
 *
 * \author Jonathan Eckstein
 */

#ifndef utilib_nicePrint_h
#define utilib_nicePrint_h

#include <utilib/utilib_config.h>
#ifdef UTILIB_HAVE_STD
#include <iostream>
#else
#include <iostream.h>
#endif

namespace utilib {

/**
 * TODO.
 */
int digitsNeededFor(double value);

/**
 * TODO.
 */
std::ostream& hyphens(std::ostream& stream,int n);

/**
 * TODO.
 */
const char* plural(int count,const char* suffix="s");

/**
 * TODO.
 */
std::ostream& printPercent(std::ostream& stream,
		      double numerator, 
		      double denominator = 1);

} // namespace utilib

#endif
