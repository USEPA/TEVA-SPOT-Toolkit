/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file pCostCast.h
 * \author Jonathan Eckstein
 *
 * Defines the pico::pCostCastObj class.
 */

#ifndef pico_pCostCast_h
#define pico_pCostCast_h

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

namespace pico {


class parMILP;    // Forward reference.


/// This thread receives and distributes pseudocost information from/to other
/// processors.  
class pCostCastObj : public pebbl::broadcastPBThread
{
public:

  pCostCastObj(parMILP* global_);

  // Hook used by parMILP to start broadcasts

  void startBroadcast(int n,
		      IntVector&    indices,
		      DoubleVector& upValues,
		      DoubleVector& downValues);

  // Common code for uffer loading.

  void loadBuffer(PackBuffer*   buf,
		  IntVector&    indices,
		  DoubleVector& upValues,
		  DoubleVector& downValues);

  // generic buffer load/unload calls.

  bool unloadBuffer();
  void initialLoadBuffer(PackBuffer* buf);
  void relayLoadBuffer(PackBuffer* buf);

  // Override standard tree object because we want to send only to workers.

  pebbl::nAryTree* makeTreeObject();

  int myOriginatorID();
  
private:

  int numEntries;

  parMILP* milpGlobal;

  int bufferSize(parMILP* global_);

  // Saves pointers to arrays passed to startBroadcast

  IntVector*    indexPtr;
  DoubleVector* upValPtr;
  DoubleVector* downValPtr;

  // Working storage

  IntVector    relayIndices;
  DoubleVector relayUps;
  DoubleVector relayDowns;

};

} // namespace pico

#endif

#endif
