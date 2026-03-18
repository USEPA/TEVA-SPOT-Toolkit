/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// cutFinder.cpp
//

// value of 0 is reserved for scanCutPool


#include <acro_config.h>
#include <pico/cutFinder.h>
#include <pico/BCMip.h>

using namespace std;

namespace pico {


// This is simple and we'd like to have it in the definition, but can't
// because of header-file cycles (BCMipNode not fully defined till now).

  // Default readiness.  Finder is ready as long as it has not been run
  // on this problem since the last resolve.

double cutFinder::readiness(BCMipNode *current)
{
if (current->LPResolved(finderType))
  return(1.0);
else return(0.0);
}

} // namespace pico
