/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file meritFunction.h
 *
 * Defines the pico::MeritFunction class.
 */

#ifndef pico_meritFunction_h
#define pico_meritFunction_h

#include <acro_config.h>
#include <utilib/RNG.h>
#include <utilib/DoubleVector.h>

namespace pico {

  class MeritFunction 
    {
    protected:
      const int* index;
      const int nIndex;
      utilib::DoubleVector alpha;
      const int maxIndex;

    public:
      MeritFunction(const int size, const int* index, const int nind, 
		    const double* l, double a0);
      MeritFunction(const int size, const int* index, const int nind, 
		    const double* l, utilib::RNG* rnd);

      void virtual randomAlpha(const double* l, utilib::RNG* rnd);
      void canonicalAlpha(const double* l);

      virtual ~MeritFunction() { }

      const double* getAlpha() const { return alpha.data(); }

      double secondDerivative(int i, double x) const
	{
	  double ai = alpha[i]; 
	  double fra = (x < ai) ? ai - std::floor(ai) : std::ceil(ai) - ai;
	  return -2.0 / (fra * fra);
	}
      
      double derivative(int i, double x) const
	{
	  double ai = alpha[i]; 
	  double fra = (x < ai) ? ai - std::floor(ai) : std::ceil(ai) - ai;
	  return 2.0 * (ai - x) / (fra * fra);
	}

      double value(int i, double x) const
	{
	  double ai = alpha[i]; 
	  double v = (x < ai) ? ai - std::floor(ai) : std::ceil(ai) - ai;
	  v = (x - ai) / v;
	  return 1 - v * v;
	}
      
      double value(const double* x) const;
      void gradient(const double* x, double* grad) const;
    };

  //----------------------------------------------------------------------------

  class CanonicalMeritFunction: public MeritFunction
    {
    public:
      
      CanonicalMeritFunction(const int size, const int* index, 
			     const int nind, const double* l)
	:MeritFunction(size, index, nind, l, 0.5){}
      void virtual randomAlpha(const double* l, utilib::RNG* rnd);
    };

  
} // namespace pico

#endif



















