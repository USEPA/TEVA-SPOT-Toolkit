/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// mipBase.cpp
//
// A base class for PICO MIP.  This class contains only static members
// and enum declarations that we would like to share between the various
// main MIP classes.
//
// Cindy Phillips
//

#include <acro_config.h>
#include <pico/mipBase.h>

using namespace std;

namespace pico {

const char* mipBase::MIPTimingStringArray[mipBase::numMIPTimingStates] =
{
  "Preprocessing",
  "Pseudocost Init",
  "MIP heuristic runs"
  };
}
