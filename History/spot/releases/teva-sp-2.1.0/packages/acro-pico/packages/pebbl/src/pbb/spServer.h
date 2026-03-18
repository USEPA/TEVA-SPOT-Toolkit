/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file spServer.h
 * \author Jonathan Eckstein
 *
 * This thread receives tokens from other processors, and forwards the
 * corresponding subproblem data.  
 */

#ifndef pebbl_spServer_h
#define pebbl_spServer_h

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

namespace pebbl {


class spServerObj : public messageTriggeredPBThread
{
public:

  spServerObj(parallelBranching* global_);

  RunStatus handleMessage(double* controlParam);

  static int computeBufferSize();

protected: 

  void preExitAction();

};

} // namespace pebbl

#endif

#endif
