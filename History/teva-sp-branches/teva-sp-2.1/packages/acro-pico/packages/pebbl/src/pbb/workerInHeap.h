/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file workerInHeap.h
 * \author Jonathan Eckstein
 *
 * A little class used by the hub code in PEBBL.  Had to be moved to its
 * own header file because it's used in a template.
 */

#ifndef pebbl_workerInHeap_h
#define pebbl_workerInHeap_h

#include <acro_config.h>
#include <utilib/std_headers.h>

namespace pebbl {


class parallelBranching;


class workerInHeap
{
public:
  int w,location;
  parallelBranching* global;

  int compare(const workerInHeap& other) const;
  bool equivalent(workerInHeap& other) {return false;};
  void write(std::ostream& wy) const {};
  void read(std::istream& we) {};

  workerInHeap(){ w = 0; location = 0; global = NULL; };

};

} // namespace pebbl

std::ostream& operator<<(std::ostream&, const pebbl::workerInHeap& );

#endif
