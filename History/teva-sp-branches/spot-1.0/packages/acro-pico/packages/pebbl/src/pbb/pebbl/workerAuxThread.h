/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file workerAuxThread.h
 * \author Jonathan Eckstein
 *
 * Auxiliary thread object used by the worker in PEBBL when there is
 * no hub present on the same processor.
 */

#ifndef pebbl_workerAuxThread_h
#define pebbl_workerAuxThread_h

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

namespace pebbl {


class workerAuxObj : public messageTriggeredPBThread
{
public:

  workerAuxObj(parallelBranching* global_);

  RunStatus handleMessage(double* controlParam);

  static int computeBufferSize(parallelBranching* global_);

private:

  int terminateCheckValue;

};

} // namespace pebbl

#endif

#endif
