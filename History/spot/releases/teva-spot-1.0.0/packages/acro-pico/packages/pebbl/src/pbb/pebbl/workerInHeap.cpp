/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// workerInHeap.cpp
//
// A little class used by the hub code in PEBBL.  Had to be moved to its
// own header file because it's used in a template.
//
// Jonathan Eckstein
//


#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <pebbl/parBranching.h>

using namespace std;

namespace pebbl {


int workerInHeap::compare(const workerInHeap& other) const
{
  return  global->moreDeservingWorker(w,other.w);
}

} // namespace pebbl


ostream& operator<<(ostream& wy, pebbl::workerInHeap w)
{
  wy << "[w=" << w.w <<", global=" << w.global << " location=" << w.location <<"]"<<endl;
return wy;
}

#endif
