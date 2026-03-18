/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file Triang_bal.h
 *
 * \depricated
 */
//
// Triang_bal.h - generates a triangular balanced distribution
//
// Triang_bal.h adapted by B. D. Schimel from Uniform.h
// Uniform.h adapted from GNU code by Dirk Grunwald.
//

#if 0

#ifndef utilib_Triang_bal_h
#define utilib_Triang_bal_h


#include <utilib/utilib_config.h>
#include <utilib/Triangular.h>

namespace utilib {


class Triang_bal: public Triangular
{
public:

  ///
  Triang_bal() : Triangular() {}

  ///
  template <class RNGT>
  Triang_bal(RNGT* gen, double low=0.0, double high=1.0)
	: Triangular(gen,low,high)
	{ }

  void set_base(double deltab, double lwr, double upr, double x);

protected:

  ///
  double expected_step(double deltab, double dist, double& sigma);

};


inline void Triang_bal::set_base(double deltab, double lwr, double upr, double x)
{
double dist;
double e_lwr, e_upr;

//expected step sizes
dist = x-lwr;
e_lwr = -expected_step(deltab,dist,lwr);
dist = upr-x;
e_upr = expected_step(deltab,dist,upr);

//probability of stepping toward the lower bound
if (e_upr-e_lwr > 0.0)
   prob_lwr=e_upr/(e_upr-e_lwr);
else
   prob_lwr = 0.5;
}


inline double Triang_bal::expected_step(double deltab, double dist, 
							double& sigma)
{
double e_step;

//cutoff factor
if (dist < 0.0)
   sigma=0.0;
else if(dist < deltab )
   sigma=dist/deltab;
else
   sigma=1.0;

//expected relative step sizes
if(sigma == 1.0)
   e_step=deltab/3;
else if(sigma == 0.0)
   e_step = 0.0;
else
   e_step=(3*deltab*sigma-2*deltab*sigma*sigma)/(6-3*sigma);

return(e_step);
}

} // namespace utilib

#endif

#endif
