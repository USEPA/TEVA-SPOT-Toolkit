/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file RNG.h
 *
 * Defines the utilib::RNG class.
 */

#ifndef utilib_RNG_h
#define utilib_RNG_h

#ifdef NON_ANSI
#include <iostream.h>
#else
#include <iostream>
#endif
#ifndef ANSI_HDRS
#include <assert.h>
#include <math.h>
#include <limits.h>
#else
#include <cassert>
#include <cmath>
#include <climits>
#endif

#include <utilib/utilib_dll.h>

#ifdef ANSI_NAMESPACES
using namespace std;
#endif

namespace utilib {

#if !defined(ANSI_NAMESPACES)
using std::ostream;
using std::istream;
#endif

/**
 * Abstract class for random number generators.  Adapted from GNU class
 *	by Dirk Grunwald.
 */
class RNG
{
public:

  /// Return a long in 0 ... LONG_MAX.
  virtual unsigned long asLong() = 0;

  /** Reset the number generator to prepare it for use.
    * If the initial see was 'random', this resets the generator
    * to the value of that initial seed.
    */
  virtual void reset() = 0;

  /// Return float in [0,1[ 
  virtual float asFloat();

  /// Return double in [0,1[ 
  virtual double asDouble();

  /// Write the rng state to an output stream.
  virtual void write(ostream& output) const = 0;

  /// Read an rng state from an input stream.
  virtual void read(istream& input) = 0;
   
};


inline double RNG::asDouble()
{
double ans = static_cast<double>(asLong())/ static_cast<double>(LONG_MAX);
return ans;
}


inline float RNG::asFloat()
{
float  ans = (float) asDouble();
return ans;
}

} // namespace utilib

inline ostream& operator << (ostream& output, const utilib::RNG& rng)
{
rng.write(output);
return(output);
}

inline istream& operator >> (istream& input, utilib::RNG& rng)
{
rng.read(input);
return(input);
}

#endif
