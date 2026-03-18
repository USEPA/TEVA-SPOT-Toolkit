/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

//
// meritFunction.cpp
//
// 
//
// 
//

#include <acro_config.h>
#include <utilib/std_headers.h>
#include <utilib/Uniform.h>
#include <utilib/RNG.h>
#include <pico/meritFunction.h>

using namespace std;

namespace pico {
  
  using utilib::RNG;

  MeritFunction::MeritFunction(const int size, const int* ind, 
			       const int nind, const double* l, double a0) : 
    index(ind), nIndex(nind), alpha(nind), maxIndex(size)
  {
    assert(nind >= 0);
    assert(size >= nind);
    assert(a0 > 0.0 && a0 < 1.0);
    
    for (int i = 0; i < nind; i++) 
      {
	int ii = ind[i];
	assert(ii >= 0 && ii < size);
	alpha[i] = l[ii] + a0;
      }
  }
  
  MeritFunction::MeritFunction(const int size, const int* ind, 
			       const int nind, const double* l, 
			       RNG* rnd) :
    index(ind), nIndex(nind), alpha(nind), maxIndex(size)
  {
    assert(nind >= 0);
    assert(size >= nind);
    assert(rnd != NULL);
    
    randomAlpha ( l, rnd );

   //  ucout<<"\n("<<uMPI::rank<<") ";
//     for (int i = 0; i < nIndex; i++) ucout<<alpha[i]<<" ";
//     ucout<<"\n";

  }
  
  void MeritFunction::randomAlpha( const double* l, RNG* rnd )
  {
    utilib::Uniform u(rnd);
    for (int i = 0; i < nIndex; i++) 
      {
	int ii = index[i];
	assert(ii >= 0 && ii < maxIndex);
	alpha[i] = l[ii] + 0.95 * u() + 0.025;
      }
  }


  void MeritFunction::canonicalAlpha(const double* l)
  {
    for (int i = 0; i < nIndex; i++) 
      {
	int ii = index[i];
	assert(ii >= 0 && ii < maxIndex);
	alpha[i] = l[ii] + 0.5;
      }
  }


  void CanonicalMeritFunction::randomAlpha( const double* l, RNG* rnd )
  {
    canonicalAlpha(l);
  }

  
  double MeritFunction::value(const double* x) const
  {
    double v = 0.0;
    for (int i = 0; i < nIndex; i++) 
      {
	v += value(i, x[index[i]]);
      }
    return v;
  }
  
  void MeritFunction::gradient(const double* x, double* grad) const
  {
    for (int i = 0; i < nIndex; i++) 
      {
	grad[i] = derivative(i, x[index[i]]);
      }
  }
  
  
} // namespace pico
