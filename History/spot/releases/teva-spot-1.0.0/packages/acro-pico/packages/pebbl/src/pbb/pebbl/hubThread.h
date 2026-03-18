/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file hubThread.h
 * \author Jonathan Eckstein
 *
 * Hub thread for PEBBL, header file.  Most of the intelligence is in
 * the parBranching module, which can more easily access "global" data.
 */

#ifndef pebbl_hubThread_h
#define pebbl_hubThread_h

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <pebbl/parBranchThreads.h>

namespace pebbl {


class hubObj : public messageTriggeredPBThread
{
public:

  hubObj(parallelBranching* global_);

  RunStatus handleMessage(double* controlParam);

protected:

  void preExitAction();

private:
  
  int computeBufferSize(parallelBranching* global_);
  
};

} // namespace pebbl

#endif 

#endif
