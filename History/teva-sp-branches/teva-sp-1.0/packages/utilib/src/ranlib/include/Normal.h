/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file Normal.h
 *
 * Defines the utilib::Normal class
 */

#ifndef utilib_Normal_h
#define utilib_Normal_h

#include <utilib/RandomVariable.h>
#include <utilib/Random.h>

namespace utilib {


/** 
 * A class that generates normal (Gaussian) deviates with a given mean 
 *	and variance
 */
class Normal: public ExternalRandomVariable<double>
{
public:

  /// Constructor
  Normal() : mean(0.0), stddev(1.0) {}

  /// Templated constructor with rng
  template <class RNGT>
  Normal(RNGT* gen, double _mean=0.0, double _variance=1.0)
	: ExternalRandomVariable<double>()
	{
	generator(gen);
	this->mean = _mean;
	stddev = sqrt(_variance);
	}

  /// mean of this distribution.
  double mean;

  /// Standard deviation of this distribution.
  double stddev;

  ///
  double operator()(double _mean, double _variance)
                {
		double tmean = this->mean;
		double tstddev = stddev;
		this->mean=_mean;
		stddev=sqrt(_variance);
		double status = ExternalRandomVariable<double>::operator()();
		this->mean=tmean;
		stddev=tstddev;
		return status;
                }

  ///
  double operator()()
	{return ExternalRandomVariable<double>::operator()();}

protected:

  ///
  double eval_c_func()
	{ return gennor(this->mean,stddev); }

};

} // namespace utilib

#endif
