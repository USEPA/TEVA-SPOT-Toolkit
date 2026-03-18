/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file Exponential.h
 *
 * Defines the utilib::Exponential class
 */

#ifndef utilib_Exponential_h
#define utilib_Exponential_h

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
class Exponential: public SimpleRandomVariable<double>
{
public:

  /// Constructor
  Exponential() : b(1.0) {}

  /// Constructor templated on rng
  template <class RNGT>
  Exponential(RNGT* gen, double _meanval=1.0)
	: SimpleRandomVariable<double>(), uvar()
	{ generator(gen); uvar.generator(gen); b=_meanval;}

  /// Returns the mean of this distribution.
  double meanval()
	{return b;}

  /// Sets the mean of this distribution.
  void meanval(double _b)
	{b=_b;}

  ///
  double operator()();

protected:

  ///
  void reset_generator()
        {uvar.generator(&pGenerator);}

  /// The mean of this distribution.
  double b;

  /// Used to generate the r.v.
  Uniform uvar;

};


inline double Exponential::operator()()
{
if (!pGenerator)
   EXCEPTION_MNGR(runtime_error, "Exponential::operator() : Attempting to use a NULL RNG.")

double temp = - b * log( uvar() );
return(temp);
}

} // namespace utilib

#endif
