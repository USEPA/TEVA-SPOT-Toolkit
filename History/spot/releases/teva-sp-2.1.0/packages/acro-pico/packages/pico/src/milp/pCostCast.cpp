/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// pCostCast.cpp
//
// This thread receives and distributes pseudocost information from/to other
// processors.  
//
// Jonathan Eckstein
//

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI               // If no MPI, this module is a stub.

#include <pebbl/followerTree.h>
#include <pico/parMilp.h>

using namespace std;
using namespace pebbl;

namespace pico {


// Construction.

pCostCastObj::pCostCastObj(parMILP* global_) :
broadcastPBThread(global_,
		  "Pseudocost Broadcast",
		  "pCost Bcast",
		  "khaki",
		   2,30,
		  bufferSize(global_),
		  MessageID(),              // Make new private message id.
		  global_->pCostTreeRadix),
                  milpGlobal(global_)
{ }


// Buffer size calculation.

int pCostCastObj::bufferSize(parMILP* global_)
{
  int nInts = global_->MIP->numIntegerVars();
  return 2*sizeof(int) + nInts*(sizeof(int) + 2*sizeof(double));
}


// Called by parMILP worker to start broadcasts.


void pCostCastObj::startBroadcast(int n,
				  IntVector&    indices,
				  DoubleVector& upValues,
				  DoubleVector& downValues)
{
  numEntries = n;
  indexPtr   = &indices;
  upValPtr   = &upValues;
  downValPtr = &downValues;

  initiateBroadcast();
}


// Called by relay code to do initial load buffer.  Uses arrays
// remembered startBroadcast, and sets the originator to be this
// worker.

void pCostCastObj::initialLoadBuffer(PackBuffer* buf)
{
  originator = milpGlobal->cluster.myGlobalFollowerRank;
  loadBuffer(buf,*indexPtr,*upValPtr,*downValPtr);
}


// Called by relay code when relaying information.  Uses working storage,
// already filled by "unloadBuffer" call.

void pCostCastObj::relayLoadBuffer(PackBuffer* buf)
{
  loadBuffer(buf,relayIndices,relayUps,relayDowns);
}


// Common code to load buffer, when either initiating or relaying.
// The data members "orginator" and "numEntries" are assumed to be
// set at this point.

void pCostCastObj::loadBuffer(PackBuffer*   buf,
			      IntVector&    indices,
			      DoubleVector& upValues,
			      DoubleVector& downValues)
{
  *buf << originator;    // This is orinator's rank within the set of workers.
  *buf << numEntries;
  int i;
  for(i=0; i<numEntries; i++)
    *buf << indices[i];
  for(i=0; i<numEntries; i++)
    {
      *buf << upValues[i];
      *buf << downValues[i];
    }
}


// Called by run method to unload an incoming buffer.  It always returns
// true to signal that there is nothing preventing the broadcast from
// continuing.

bool pCostCastObj::unloadBuffer()
{
  inBuf >> originator;
  inBuf >> numEntries;
  relayIndices.resize(numEntries);
  relayUps.resize(numEntries);
  relayDowns.resize(numEntries);
  int i;
  for(i=0; i<numEntries; i++)
    inBuf >> relayIndices[i];
  for(i=0; i<numEntries; i++)
    {
      inBuf >> relayUps[i];
      inBuf >> relayDowns[i];
    }
  milpGlobal->incorporatePCosts(numEntries,
				relayIndices,
				relayUps,
				relayDowns);
  return true;
}


// This constructs the tree object used by the relay method.  We override
// the default because we don't want to broadcast to pure hubs.

nAryTree* pCostCastObj::makeTreeObject()
{
  return new nAryFollowerTree(milpGlobal->cluster,originator,radix);
};

// The originator is identified by its rank in the worker processors
int pCostCastObj::myOriginatorID()
{
  return(milpGlobal->cluster.myGlobalFollowerRank);
}

} // namespace pico

#endif
