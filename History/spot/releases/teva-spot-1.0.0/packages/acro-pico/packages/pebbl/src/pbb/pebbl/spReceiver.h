/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file spReceiver.h
 * \author Jonathan Eckstein
 *
 * This thread receives subproblems from other processors.  
 */

#ifndef pebbl_spReceiver_h
#define pebbl_spReceiver_h

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

namespace pebbl {


class spReceiverObj : public messageTriggeredPBThread
{
public:

  spReceiverObj(parallelBranching* global_);

  RunStatus handleMessage(double* controlParam);

  int computeBufferSize(parallelBranching* global_);
};

} // namespace pebbl

#endif

#endif
