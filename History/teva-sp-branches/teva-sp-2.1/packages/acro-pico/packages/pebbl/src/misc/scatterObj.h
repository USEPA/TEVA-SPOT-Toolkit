/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file scatterObj.h
 * \author Jonathan Eckstein
 *
 * Logic for making random decisions as to whether to move work to other
 * processors/clusters.
 */

#ifndef pebbl_scatterObj_h
#define pebbl_scatterObj_h

#include <acro_config.h>

namespace pebbl {


class scatterObj
{
public:

  double probability(double ratio);

  scatterObj() {}

  void reset(double targetRatio_,
	     double minProb_,
	     double targetProb_,
	     double maxProb_);

private:
  
  double targetRatio;
  double minProb;
  double targetProb;
  double factor1,factor2;

};

} // namespace pebbl

#endif


