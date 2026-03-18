/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file earlyOutputThread.h
 * \author Jonathan Eckstein
 *
 * Auxiliary thread used to orchetrate early output of incumbent solutions
 * from the parallel layer.
 */

#ifndef pebbl_earlyOutputThread_h
#define pebbl_earlyOutputThread_h

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <pebbl/parBranchThreads.h>

namespace pebbl {


class earlyOutputObj : public messageTriggeredPBThread
{
public:

  earlyOutputObj(parallelBranching* global_);

  RunStatus handleMessage(double* controlParam);

  void activateEarlyOutput();
  
private:

  int computeBufferSize(parallelBranching* global_);

  enum { outputRequestSignal, outputDeliverSignal, outputConfirmSignal };

  void writeEarlyOutput();

  void confirmEarlyOutput(double outputVal);

  void sendMessage(int dest);

  PackBuffer outBuf;

};

} // namespace pebbl

#endif

#endif
