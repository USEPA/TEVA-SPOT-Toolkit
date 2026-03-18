/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file incumbSearchThread.h
 * \author Jonathan Eckstein
 *
 * Thread object to orchestrate asynchronous incumbent search
 */

#ifndef pebbl_incumbSearchThread_h
#define pebbl_incumbSearchThread_h

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <pebbl/parBranchThreads.h>

namespace pebbl {


class incumbSearchObj :
public parBranchSelfAdjThread
{
public:

  incumbSearchObj(parallelBranching* global_);

  RunStatus runWithinLogging(double* controlParam);

  ThreadState state();

  double nextRunTime;
};

} // namespace pebbl

#endif

#endif
