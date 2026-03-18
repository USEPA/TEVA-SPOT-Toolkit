/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file incumbCast.h
 * \author Jonathan Eckstein
 *
 * This thread receives and distributes incumbent information from/to other
 * processors.  
 */

#ifndef pebbl_incumbCast_h
#define pebbl_incumbCast_h

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

namespace pebbl {

class parallelBranching;

class incumbCastObj : public broadcastPBThread
{
public:

  incumbCastObj(parallelBranching* );

  bool unloadBuffer();
  void initialLoadBuffer(PackBuffer* buf) { relayLoadBuffer(buf); };
  void relayLoadBuffer(PackBuffer* buf);

  void preExitAction();  // This thread has an exit action to activate the hub.
  
private:

  optimType minOrMax;
};

}  // namespace pebbl

#endif

#endif

