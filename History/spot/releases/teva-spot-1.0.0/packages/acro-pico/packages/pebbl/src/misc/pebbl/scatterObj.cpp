/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// scatterObj.cpp
//
// Logic for making random decisions as to whether to move work to other
// processors/clusters.
//
// Jonathan Eckstein
//

#include <acro_config.h>
#include <pebbl/scatterObj.h>

namespace pebbl {


void scatterObj::reset(double targetRatio_,
		       double minProb_,
		       double targetProb_,
		       double maxProb_)
{
  targetRatio=targetRatio_;
  minProb=minProb_;
  targetProb=targetProb_;

  if (targetRatio > 0)
    factor1 = (targetProb - minProb)/targetRatio;
  else
    factor1 = 1;
  if (targetRatio < 1)
    factor2 = (maxProb_ - targetProb)/(1 - targetRatio);
  else
    factor2 = 1;
}


double scatterObj::probability(double ratio)
{
  if (ratio > 1)
    ratio = 1;
  else if (ratio < 0)
    ratio = 0;
  if (ratio < targetRatio)
    return minProb + factor1*ratio;
  else   // (ratio >= targetRatio)
    return targetProb + factor2*(ratio - targetRatio);
}

} // namespace pebbl
