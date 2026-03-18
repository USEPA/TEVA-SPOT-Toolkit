/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file Geometric.h
 *
 * Defines the utilib::Geometric class
 */

#ifndef utilib_Geometric_h
#define utilib_Geometric_h

#ifndef ANSI_HDRS
#include <math.h>
#else
#include <cmath>
#endif

#include <utilib/Uniform.h>

namespace utilib {


/**
 * A class that generates exponentially distributed random variables.
 */
class Geometric: public SimpleRandomVariable<int>
{
public:

  /// Constructor
  Geometric() : p(0.0) {}

  /// Constructor templated on rng
  template <class RNGT>
  Geometric(RNGT* gen, double _p=0.0)
	: SimpleRandomVariable<int>(), uvar()
	{ generator(gen); uvar.generator(gen); p=_p; }

  /// Returns the probability of each success
  double probability()
	{return p;}

  /// Sets the probability of each success
  void probability(double _p)
	{p=_p;}

  ///
  int operator()(double _p)
	{
	p = _p;
	return operator()();
	}

  ///
  int operator()();

protected:

  ///
  void reset_generator()
        {uvar.generator(&pGenerator);}

  /// The probability of success for each trial
  double p;

  /// Used to generate the r.v.
  Uniform uvar;

};


inline int Geometric::operator()()
{
if (!pGenerator)
   EXCEPTION_MNGR(runtime_error, "Geometric::operator() : Attempting to use a NULL RNG.")

if (p == 0.0)
   EXCEPTION_MNGR(runtime_error, "Geometric::operator() : Attempting to use parameter p=0.0.")

if (p == 1.0)
   EXCEPTION_MNGR(runtime_error, "Geometric::operator() : Attempting to use parameter p=1.0.")

int temp = (int) ceil(log( uvar() ) / log(1-p))-1;
return(temp);
}

} // namespace utilib

#endif
