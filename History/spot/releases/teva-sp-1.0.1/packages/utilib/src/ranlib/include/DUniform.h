/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file DUniform.h
 *
 * Defines the utilib::DUniform class
 */

#ifndef utilib_DUniform_h
#define utilib_DUniform_h

#ifndef ANSI_HDRS
#include <math.h>
#else
#include <cmath>
#endif

#include <utilib/exception_mngr.h>
#include <utilib/RandomVariable.h>

namespace utilib {

#if !defined(COUGAR)
using std::runtime_error;
#endif

/**
 * A discrete uniform distribution.
 */
class DUniform: public SimpleRandomVariable<int>
{
public:

  /// Constructor
  DUniform() : pLow(0), pHigh(1), delta(1) {}

  /// Constructor templated on rng
  template <class RNGT>
  DUniform(RNGT* gen, int low=0, int high=1)
	: SimpleRandomVariable<int>()
	{
	generator(gen);
    	pLow = (low < high) ? low : high;
    	pHigh = (low < high) ? high : low;
    	delta = pHigh - pLow;
	}

  /// Returns the lower limit of the r.v.
  int low()
	{ return pLow; }

  /// Sets the lower limit of the r.v.
  int low(int x)
	{
  	int tmp = pLow;
  	pLow = x;
  	delta = pHigh - pLow;
  	return tmp;
	}

  /// Returns the upper limit of the r.v.
  int high()
	{ return pHigh; }

  /// Sets the upper limit of the r.v.
  int high(int x)
	{
  	int tmp = pHigh;
  	pHigh = x;
  	delta = pHigh - pLow;
  	return tmp;
	}

  ///
  int operator()()
	{
	if (!pGenerator)
   	   EXCEPTION_MNGR( runtime_error , "DUniform::operator() : Attempting to use a NULL RNG.")

	int temp = (int)floor(pLow + (pHigh-pLow+1) * pGenerator.asDouble());
	if (temp > pHigh)
   	   temp = pHigh;	// This handles the case when urand == 1.0
	return(temp);
	}

  /// Generates a random variable given a lower and upper limit for the r.v.
  int operator()(int low_, int high_)
	{
	low(low_);
	high(high_);
	return (*this)();
	}

protected:

  /// The lower limit of the r.v.
  int pLow;

  /// The upper limit of the r.v.
  int pHigh;

  /// The difference between \c pHigh and \c pLow.
  int delta;

};

} // namespace utilib

#endif
