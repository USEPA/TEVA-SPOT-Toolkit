/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file nicePrint.cpp
 *
 * Helper functions for printing
 *
 * \author Jonathan Eckstein
 */

#ifdef NON_ANSI
#include <iostream.h>
#else
#include <iostream>
#endif

#include <utilib/_math.h>
#include <utilib/nicePrint.h>


namespace utilib {

using std::max;

int digitsNeededFor(double value)
{
  if (fabs(value) > 0.0)
    return max((int) floor(log10(fabs(value))) + 1,1);
  else
    return 1;
}


ostream& hyphens(ostream& stream,int n)
{
  stream.width(n);
  stream.fill('-');
  stream << "";
  stream.fill(' ');
  return stream;
}


const char* plural(int count,const char* suffix)
{
  if (count == 1)
    return "";
  else
    return suffix;
}


ostream& printPercent(ostream& stream,
		      double numerator, 
		      double denominator)
{
  stream.setf(ios::fixed,ios::floatfield);
  int oldPrecision = stream.precision(1);
  stream.width(5);
  if (denominator != 0)
    stream << 100*numerator/denominator;
  else if (numerator == 0)
    stream << 0.0;
  else
    stream << "N/A";
  stream.unsetf(ios::floatfield);
  stream.precision(oldPrecision);
  stream << '%';
  return stream;
}

} // namespace utilib
