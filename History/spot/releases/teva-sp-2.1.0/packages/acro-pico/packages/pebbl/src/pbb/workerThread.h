/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file workerThread.h
 * \author Jonathan Eckstein
 *
 * Thread object used by the worker in PEBBL
 */

#ifndef pebbl_workerThread_h
#define pebbl_workerThread_h

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <pebbl/parBranching.h>
#include <pebbl/parBranchThreads.h>

namespace pebbl {


class workerThreadObj : public parBranchSelfAdjThread
{
public:

  workerThreadObj(parallelBranching* global_);

  RunStatus run(double* controlParam);

  ThreadState state()
    {
      if (ready())
	return ThreadReady;
      else
	return ThreadBlocked;
    };

  bool emptyPool() 
    { 
      return (workerPool == 0) || (workerPool->size() == 0); 
    };

  bool ready();
  bool blocked() { return !ready(); };

private:  

  branchPool<parallelBranchSub,parLoadObject> *workerPool;

};

} // namespace pebbl

#endif

#endif
