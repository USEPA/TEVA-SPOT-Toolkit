/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file Triang_trunc_50.h
 *
 * \depricated
 */
//
// Triang_trunc_50.h - generates a triangular truncated distribution
// with probability 0.5 of mutating above or below the point
//
// Triang_trunc.h adapted by B. D. Schimel from Uniform.h
// Uniform.h adapted from GNU code by Dirk Grunwald.
//

#if 0

#ifndef utilib_Triang_trunc_50_h
#define utilib_Triang_trunc_50_h

#include <utilib/utilib_config.h>
#include <utilib/Triangular.h>

namespace utilib {


class Triang_trunc_50: public Triangular
{
public:

  ///
  Triang_trunc_50() : Triangular(0,0.0,1.0) {}

  ///
  template <class RNGT>
  Triang_trunc_50(RNGT *gen, double low=0.0, double high=1.0)
	:Triangular(gen,low,high) { }

  ///
  void set_base(double deltab, double lwr, double upr, double x);

protected:

  ///
  double sig(double deltab, double dist);

};


inline void Triang_trunc_50::set_base(double deltab, double lwr, double upr, 
								double x)
{
  double dist,temp;

  //cutoff factors
  dist = x-lwr;
  sigma_lwr = sig(deltab,dist);
  dist = upr-x;
  sigma_upr = sig(deltab,dist);

  //probability of stepping toward the lower bound
  prob_lwr=0.5;

}


inline double Triang_trunc_50::sig(double deltab, double dist)
{
  double sigma;

  //cutoff factor sigma
  if(dist < 0.0)
    sigma=0.0;
  else if(dist < deltab )
    sigma=dist/deltab;
  else
    sigma=1.0;


  return(sigma);

}

} // namespace utilib

#endif

#endif
