/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file parMilpParams.h
 *
 * Defines the pico::parMilpParams class.
 */

#ifndef pico_parMilpParams_h
#define pico_parMilpParams_h

#include <acro_config.h>
#include <utilib/ParameterSet.h>

namespace pico {

/// Defines parameters, etc, used for general mixed-integer linear programming
class parMilpParams : virtual public utilib::ParameterSet
{
public:

  ///
  parMilpParams();

  ///
  int pCostTreeRadix;

  ///
  int pCostShareCutoff;

  ///
  int pCostMinBcastSize;

  ///
  double rampUpTableInitFrac;

  ///
  double numPCInitSmoothFactor;

  ///
  int treeSizeCrossFac;

  ///
  int numPCCrossFac;

  ///
  bool ofile;
};

} // namespace pico

#endif
