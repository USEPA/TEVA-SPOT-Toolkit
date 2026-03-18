/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file Triang_trunc.h
 *
 * \depricated
 */
//
// Triang_trunc.h - generates a triangular truncated distribution
//
// Triang_trunc.h adapted by B. D. Schimel from Uniform.h
// Uniform.h adapted from GNU code by Dirk Grunwald.
//

#if 0

#ifndef utilib_Triang_trunc_h
#define utilib_Triang_trunc_h

#include <utilib/Triangular.h>

namespace utilib {


class Triang_trunc: public Triangular
{
public:

  ///
  Triang_trunc() : Triangular(0,0.0,1.0) {}

  ///
  template <class RNGT>
  Triang_trunc(RNGT *gen, double low=0.0, double high=1.0)
	:Triangular(gen,low,high) { }

  
  void set_base(double deltab, double lwr, double upr, double x);

protected:

  ///
  double sig(double deltab, double dist);

};


inline void Triang_trunc::set_base(double deltab, double lwr, double upr, double x)
{
  double dist,temp;

  //cutoff factors
  dist = x-lwr;
  sigma_lwr = sig(deltab,dist);
  dist = upr-x;
  sigma_upr = sig(deltab,dist);

  //probability of stepping toward the lower bound
  temp=sigma_lwr-0.5*sigma_lwr*sigma_lwr;
  prob_lwr=temp/(temp+sigma_upr-0.5*sigma_upr*sigma_upr);
}


inline double Triang_trunc::sig(double deltab, double dist)
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
