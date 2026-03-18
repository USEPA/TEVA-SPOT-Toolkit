/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file parPebblParams.h
 * \author William Hart
 *
 * Defines the pebbl::parallelPebblParams class.
 */

#ifndef pebbl_parPebblParams_h
#define pebbl_parPebblParams_h

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <utilib/ParameterSet.h>

namespace pebbl {


class parallelPebblParams : virtual public utilib::ParameterSet
{
public:

  ///
  parallelPebblParams();

  // Controls how long we do the synchronous ramp-up phase.  This may have
  // no effect if the continueRampUp() method has been overridden.

  int rampUpPoolLimit;
  int rampUpPoolLimitFac;
  int minRampUpSubprobsCreated;

  // Control parameters for clustering.

  int clusterSize;
  int numClusters;
  int hubsDontWorkSize;

  // Default thread control, priorities, and timeslices.

  bool   useIncumbentThread;
  double incSearchMaxControl;

  double workerThreadBias;

  double incThreadMaxBias;
  double incThreadMinBias;
  double incThreadBiasFactor;
  double incThreadBiasPower;

  double timeSlice;

  double incThreadGapSlices;

  // Miscellaneous printout control parameters.

  bool workersPrintStatus;
  bool hubsPrintStatus;
  bool printSolutionSynch;
  bool combineTimingsMax;
  bool trackIncumbent;

  // Safety checking control.

  bool checkTokensMatch;

  // Hub operation and hub-worker interacation.

  int maxSPAcks;
  int maxTokensInHubMsg;
  int localScatterQuantum;
  int localLoadInformRate;
  int globalLoadInformRate;
  int workerLoadReportRate;
  int dispatchLoadInformRate;
  int workerSPThreshHub;
  int workerSPThreshWorker;
  int workerSPThreshKeep;
  int workerSPThreshDrop;
  static int maxTokenQueuing;
  int maxDispatchPacking;
  int maxSPPacking;
  int spReceiveBuf;
  double targetWorkerKeepFac;
  double minScatterProb;
  double targetScatterProb;
  double maxScatterProb;
  int initForceReleases;

  // Incumbent broadcast

  int incumbTreeRadix;

  // Worker-hub "rebalancing" 

  bool rebalancing;
  double workerMaxLoadFactor;
  double workerTargetLoadFactor;

  // Quality-of-work balancing.

  bool qualityBalance;
  double qualityBalanceObjFac;
  double qualityBalanceGapFac;

  // Multiple hubs and load balancing

  double minNonLocalScatterProb;
  double targetNonLocalScatterProb;
  double maxNonLocalScatterProb;

  int loadBalTreeRadix;
  int maxLoadBalSize;
  double loadBalDonorFac;
  double loadBalReceiverFac;
  int loadBalMinSourceCount;

  double loadBalCPUFrac;
  double loadBalCPUFracPure;

  // Checkpoint management

  double checkpointMinutes;
  double checkpointMinInterval;

  bool restart;
  bool reconfigure;

  std::string checkpointDir;

  int abortCheckpoint;
  int abortCheckpointCount;

  // For debugging and testing purposes

  bool rampUpOnly;

  int schedulerDebug;
  int hubDebug;
  int workerDebug;
  int loadBalDebug;
  int abortDebug;

  //  int outputCacheSize;
  double boundWasteTime;

  int cpDebugCount;

  int debugSeqDigits;

  bool stallForDebug;
  bool forceParallel;

};


} // namespace pebbl

#endif

#endif
