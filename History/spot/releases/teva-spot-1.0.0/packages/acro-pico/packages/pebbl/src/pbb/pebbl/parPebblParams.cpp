/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// parPebblParams.cpp
//


#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <utilib/std_headers.h>
#include <pebbl/parPebblParams.h>

using namespace std;

namespace pebbl {

using utilib::ParameterLowerBound;
using utilib::ParameterBounds;
using utilib::ParameterNonnegative;

int parallelPebblParams::maxTokenQueuing=128;

parallelPebblParams::parallelPebblParams()
{

/// GENERAL

  combineTimingsMax=false;
  create_categorized_parameter("combineTimingsMax",combineTimingsMax,
		"<bool>","false",
		"When combining times from multiple processor,\n\t"
		"take largest value",
		"Output");

/// RAMPUP

  rampUpPoolLimit=0;
  create_categorized_parameter("rampUpPoolLimit",rampUpPoolLimit,"<int>","0",
		"Pool size beyond which ramp-up phase may end\n\t"
		"(base ramp-up only)",
		"Ramp-up",
		ParameterNonnegative<int>());

  rampUpPoolLimitFac=1;
  create_categorized_parameter("rampUpPoolLimitFac",rampUpPoolLimitFac,"<double>","1",
		"Average number of subproblems per processor\n\t"
		"immediately after ramp-up. Can be > 1\n\t"
		"(base ramp-up only)",
		"Ramp-up",
		ParameterNonnegative<double>());

  minRampUpSubprobsCreated=0;
  create_categorized_parameter("minRampUpSubprobsCreated",minRampUpSubprobsCreated,"<int>","0",
		"Force this many subproblem creations before ramp up ends\n\t (only guaranteed in base ramp up)",
		"Ramp-up",
		ParameterNonnegative<int>());

/// PARALLEL SEARCH

  loadBalCPUFracPure=0.05;
  create_categorized_parameter("loadBalCPUFracPure",loadBalCPUFracPure,
		"<double>","0.05",
		"Target fraction of CPU time to deveote to\n\t"
		"load balancing between clusters, on 'pure' hubs\n\t"
		"(those that are not also workers)",
		"Parallel Search",
		ParameterBounds<double>(0.0,1.0));

  clusterSize=64;
  create_categorized_parameter("clusterSize",clusterSize,"<int>","64",
		"Maximum number of processors controlled by a single hub",
		"Parallel Search",
		ParameterLowerBound<int>(1));

  numClusters=1;
  create_categorized_parameter("numClusters",numClusters,"<int>","1",
		"Forces a minimum number of clusters",
		"Parallel Search",
		ParameterLowerBound<int>(1));

  hubsDontWorkSize=10;
  create_categorized_parameter("hubsDontWorkSize",hubsDontWorkSize,"<int>","10",
		"Size of cluster above which hubs\n\t"
		"do not also function as workers",
		"Parallel Search",
		ParameterLowerBound<int>(2));

  checkTokensMatch=true;
  create_categorized_parameter("checkTokensMatch",checkTokensMatch,
		"<bool>","true",
		"Check that tokens match the subproblems\n\t"
		"they point to",
		"Parallel Search");

  maxSPAcks=128;
  create_categorized_parameter("maxSPAcks",maxSPAcks,"<int>","128",
		"Maximum subproblem acknowledgements that a\n\t"
		"worker can send to a hub in one message",
		"Parallel Search",
		ParameterLowerBound<int>(1));

  maxTokensInHubMsg=256;
  create_categorized_parameter("maxTokensInHubMsg",maxTokensInHubMsg,
		"<int>","256",
		"Maximum tokens that can be released from a worker\n\t"
		"to a hub in one message",
		"Parallel Search",
		ParameterLowerBound<int>(1));

  localScatterQuantum=8;
  create_categorized_parameter("localScatterQuantum",localScatterQuantum,
		"<int>","8",
		"Workers forced to communicate with hub if they\n\t"
		"have at least this many subproblems to release",
		"Parallel Search",
		ParameterLowerBound<int>(1));

  localLoadInformRate=5;
  create_categorized_parameter("localLoadInformRate",localLoadInformRate,
		"<int>","5",
		"Number of messages from single worker to hub until\n\t"
		"hub informs worker of load situation",
		"Parallel Search",
		ParameterLowerBound<int>(1));

  globalLoadInformRate=5;
  create_categorized_parameter("globalLoadInformRate",globalLoadInformRate,
		"<int>","5",
		"Average number of messages per worker to hub until\n\t"
		"hub informs worker of load situation",
		"Parallel Search",
		ParameterLowerBound<int>(1));

  workerLoadReportRate=20;
  create_categorized_parameter("workerLoadReportRate",workerLoadReportRate,
		"<int>","20",
		"Workers forced to report status to hub after\n\t"
		"creating this many subproblems",
		"Parallel Search",
		ParameterLowerBound<int>(1));

  dispatchLoadInformRate=20;
  create_categorized_parameter("dispatchLoadInformRate",
		dispatchLoadInformRate,"<int>","20",
		"Average number of subproblems dispatched per worker\n\t"
		"before hub forced to inform workers of loads",
		"Parallel Search",
		ParameterLowerBound<int>(1));

  workerSPThreshHub=12;
  create_categorized_parameter("workerSPThreshHub",workerSPThreshHub,
		"<int>","12",
		"If worker has fewer than this number of subproblems,\n\t"
		"hub will send it work",
		"Parallel Search",
		ParameterNonnegative<int>());

  workerSPThreshWorker=10;
  create_categorized_parameter("workerSPThreshWorker",workerSPThreshWorker,
		"<int>","10",
		"Worker forced to report its status to hub if it has\n\t"
		"less than this many subproblems",
		"Parallel Search",
		ParameterNonnegative<int>());

  workerSPThreshKeep=1;
  create_categorized_parameter("workerSPThreshKeep",workerSPThreshKeep,
		"<int>","1",
		"Workers will not release any more subproblems if\n\t"
		"their local pool is smaller than this",
		"Parallel Search",
		ParameterNonnegative<int>());

  workerSPThreshDrop=4;
  create_categorized_parameter("workerSPThreshDrop",workerSPThreshDrop,
		"<int>","4",
		"Worker reports status if its local pools reduces\n\t"
		"by this many subproblems",
		"Parallel Search",
		ParameterNonnegative<int>());

  create_categorized_parameter("maxTokenQueuing",maxTokenQueuing,"<int>","128",
		"Largest number of tokens released to a hub in one message",
		"Parallel Search",
		ParameterLowerBound<int>(1));

  maxDispatchPacking=8;
  create_categorized_parameter("maxDispatchPacking",maxDispatchPacking,
		"<int>","8",
		"Maximum number of subproblems dispatched from a hub\n\t"
		"in one message",
		"Parallel Search",
		ParameterLowerBound<int>(1));

  maxSPPacking=32;
  create_categorized_parameter("maxSPPacking",maxSPPacking,"<int>","32",
		"Maximum number of subproblems packed into one message",
		"Parallel Search",
		ParameterLowerBound<int>(1));

  // This default it not really used because if this parameter is not 
  // explicitly set, the spPackSize() method and maxSPPacking are used instead.
  spReceiveBuf=4096;
  create_categorized_parameter("spReceiveBuf",spReceiveBuf,"<int>",
		"set from maxSPPacking/spPackSize()",
		"Buffer bytes initially allocated to receive\n\t"
		"subproblems from other processors",
		"Parallel Search",
		ParameterLowerBound<int>(32));

  targetWorkerKeepFac=0.7;
  create_categorized_parameter("targetWorkerKeepFac",targetWorkerKeepFac,
		"<double>","0.7",
		"Preferred fraction of subproblems that remain local\n\t"
		"at creating worker",
		"Parallel Search",
		ParameterBounds<double>(0.0,1.0));

  minScatterProb=0.05;
  create_categorized_parameter("minScatterProb",minScatterProb,
		"<double>","0.05",
		"Smallest possible fraction of subproblems released\n\t"
		"from workers",
		"Parallel Search",
		ParameterBounds<double>(0.0,1.0));

  targetScatterProb=0.05;
  create_categorized_parameter("targetScatterProb",targetScatterProb,
		"<double>","0.25",
		"Preferred fraction of subproblems released from workers",
		"Parallel Search",
		ParameterBounds<double>(0.0,1.0));

  maxScatterProb=0.9;
  create_categorized_parameter("maxScatterProb",maxScatterProb,"<double>","0.9",
		"Maximum fraction of subproblems released from workers",
		"Parallel Search",
		ParameterBounds<double>(0.0,1.0));

  initForceReleases=5;
  create_categorized_parameter("initForceReleases",initForceReleases,
		"<int>","5",
		"Each worker releases the first initForceReleases\n\t"
		"subproblems it creates",
		"Parallel Search",
		ParameterLowerBound<int>(0));

  rebalancing=true;
  create_categorized_parameter("rebalancing",rebalancing,"<bool>","true",
		"Enable rebalancing mechanism from workers to hubs",
		"Parallel Search");

  workerMaxLoadFactor=2.0;
  create_categorized_parameter("workerMaxLoadFactor",workerMaxLoadFactor,
		"<double>","2.0",
		"Rebalance subproblems away from worker if its load\n\t"
		"exceeds an even cluster balance by this factor",
		"Parallel Search",
		ParameterLowerBound<double>(0.0));

  workerTargetLoadFactor=0.5;
  create_categorized_parameter("workerTargetLoadFactor",workerTargetLoadFactor,
		"<double>","0.5",
		"Fraction of ideal load to try reach after rebalancing",
		"Parallel Search",
		ParameterLowerBound<double>(0.0));

  qualityBalance=true;
  create_categorized_parameter("qualityBalance",qualityBalance,"<bool>","true",
		"Makes hubs shift load between workers based on quality\n\t"
		"as well as nthe umber of subproblems",
		"Parallel Search");

  qualityBalanceObjFac=0.02;
  create_categorized_parameter("qualityBalanceObjFac",qualityBalanceObjFac,
		"<double>","0.02",
		"Quality balancing tolerance relative to objective",
		"Parallel Search",
		ParameterLowerBound<double>(0.0));

  qualityBalanceGapFac=0.05;
  create_categorized_parameter("qualityBalanceGapFac",qualityBalanceGapFac,
		"<double>","0.05",
		"Quality balancing tolerance relative to gap",
		"Parallel Search",
		ParameterBounds<double>(0.0,1.0));

  minNonLocalScatterProb=0.0;
  create_categorized_parameter("minNonLocalScatterProb",minNonLocalScatterProb,
		"<double>","0.0",
		"Smallest probability s subproblem will be scattered\n\t"
		"to some other cluster's hub",
		"Parallel Search",
		ParameterBounds<double>(0.0,1.0));

  targetNonLocalScatterProb=0.33;
  create_categorized_parameter("targetNonLocalScatterProb",
		targetNonLocalScatterProb,
		"<double>","0.33",
		"Preferred probability a subproblem will be scattered\n\t"
		"to some other cluster's hub",
		"Parallel Search",
		ParameterBounds<double>(0.0,1.0));

  maxNonLocalScatterProb=0.9;
  create_categorized_parameter("maxNonLocalScatterProb",maxNonLocalScatterProb,
		"<double>","0.9",
		"Largest probability a subproblem will be scattered\n\t"
		"to some other cluster's hub",
		"Parallel Search",
		ParameterBounds<double>(0.0,1.0));

  loadBalTreeRadix=2;
  create_categorized_parameter("loadBalTreeRadix",loadBalTreeRadix,"<int>","2",
		"Branching factor for load balancing tree",
		"Parallel Search",
		ParameterLowerBound<int>(1));
		
  maxLoadBalSize=1024;
  create_categorized_parameter("maxLoadBalSize",maxLoadBalSize,"<int>","1024",
		"Maximum subproblems in one load balancing message\n\t"
		"between clusters",
		"Parallel Search",
		ParameterLowerBound<int>(1));

  loadBalDonorFac=1.3;
  create_categorized_parameter("loadBalDonorFac",loadBalDonorFac,
		"<double>","1.3",
		"A cluster may give up work if it is above\n\t"
		"this factor times the average cluster load",
		"Parallel Search",
		ParameterLowerBound<double>(1.0));

  loadBalReceiverFac=0.7;
  create_categorized_parameter("loadBalReceiverFac",loadBalReceiverFac,
		"<double>","0.7",
		"Clusters ask for work if below this factor times\n\t"
		"the average cluster load",
		"Parallel Search",
		ParameterBounds<double>(0.0,1.0));

  loadBalMinSourceCount=32;
  create_categorized_parameter("loadBalMinSourceCount",loadBalMinSourceCount,
		"<int>","32",
		"Minimum subproblem count for a cluster to give up work",
		"Parallel Search",
		ParameterLowerBound<int>(1));
		
  loadBalCPUFrac=0.05;
  create_categorized_parameter("loadBalCPUFrac",loadBalCPUFrac,
		"<double>","0.05",
		"Target fraction of CPU time to deveote to\n\t"
		"load balancing between clusters, on hubs that are\n\t"
		"also workers",
		"Parallel Search",
		ParameterBounds<double>(0.0,1.0));

/// PARALLEL THREAD CONTROL

  useIncumbentThread=true;
  create_categorized_parameter("useIncumbentThread",useIncumbentThread,"<bool>","true",
		"Each worker dedicates a 'thread' to heuristic\n\t"
		"incumbent search, if a heuristic exists",
		"Parallel Thread Control");

  workerThreadBias=100.0;
  create_categorized_parameter("workerThreadBias",workerThreadBias,"<double>","100.0",
		"Scheduling priority for main worker thread",
		"Parallel Thread Control",
		ParameterNonnegative<double>());

  incThreadMaxBias=20.0;
  create_categorized_parameter("incThreadMaxBias",incThreadMaxBias,
		"<double>","20.0",
		"Maximum scheding  priority of incumbent search thread",
		"Parallel Thread Control",
		ParameterNonnegative<double>());

  incThreadMinBias=1.0;
  create_categorized_parameter("incThreadMinBias",incThreadMinBias,
		"<double>","1.0",
		"Minimum scheduling priority of incumbent search thread",
		"Parallel Thread Control",
		ParameterNonnegative<double>());

  incThreadBiasFactor=100.0;
  create_categorized_parameter("incThreadBiasFactor", incThreadBiasFactor,
		"<double>","100.0",
		"Incumbent thread priority =\n\t"
		"incThreadBiasFactor*relativeGap^incThreadBiasPower",
		"Parallel Thread Control",
		ParameterNonnegative<double>());

  incThreadBiasPower=1.0;
  create_categorized_parameter("incThreadBiasPower",incThreadBiasPower,
		"<double>","1.0",
		"Incumbent thread priority =\n\t"
		"incThreadBiasFactor*relativeGap^incThreadBiasPower",
		"Parallel Thread Control",
		ParameterNonnegative<double>());

// This is something of a kludge, but it will have to do for now.
// Set default time slice depending on the performance of the system we're on.
// Janus (Cougar) is 1 millisecond.
// Default is 10 milliseconds.  My basic rule of thumb
// is twenty times the time it takes the scheduler to switch threads.

#ifdef COUGAR
#define TIMESLICE 0.001
#define TIMESLICESTRING "0.001"
#else
#define TIMESLICE 0.01
#define TIMESLICESTRING "0.01"
#endif

  timeSlice = TIMESLICE;
  create_categorized_parameter("timeSlice",timeSlice,
		"<double>",TIMESLICESTRING,
		"Target timeslice for 'threads' in seconds",
		"Parallel Thread Control",
		ParameterLowerBound<double>(1e-7));

  incThreadGapSlices = 10;
  create_categorized_parameter("incThreadGapSlices",incThreadGapSlices,
		"<double>","10",
		"Minimum number of timeslices between heuristic runs",
		"Parallel Thread Control",
		ParameterNonnegative<double>());

/// ACRO_VALIDATING

  forceParallel=false;
  create_categorized_parameter("forceParallel",forceParallel,
		"<bool>","false",
		"Force the use of a parallel PEBBL solver, even if the number\n\t"
		"of processors is one",
		"Debugging");

  workersPrintStatus=false;
  create_categorized_parameter("workersPrintStatus",workersPrintStatus,
		"<bool>","false",
		"All worker processors print status lines\n\t"
		"(default is first hub only)",
		"Debugging");

  hubsPrintStatus=false;
  create_categorized_parameter("hubsPrintStatus",hubsPrintStatus,
		"<bool>","false",
		"All hubs print status lines\n\t"
		"(default is first hub only)",
		"Debugging");

  printSolutionSynch=true;
  create_categorized_parameter("printSolutionSynch",printSolutionSynch,
		"<bool>","true",
		"Only the designated IO processor (typically 0)\n\t"
		"is allowed to write the solution",
		"Output");

  trackIncumbent=false;
  create_categorized_parameter("trackIncumbent",trackIncumbent,
		"<bool>","false",
		"Print whenever there is a new incumbent",
		"Output");

  schedulerDebug=0;
  create_categorized_parameter("schedulerDebug",schedulerDebug,"<int>","0",
		"Special debug output level for 'thread' scheduler",
		"Debugging",
		ParameterNonnegative<int>());

  hubDebug=0;
  create_categorized_parameter("hubDebug",hubDebug,"<int>","0",
		"Special debug output level for hub logic",
		"Debugging",
		ParameterNonnegative<int>());

  workerDebug=0;
  create_categorized_parameter("workerDebug",workerDebug,"<int>","0",
		"Special debug output level for worker logic",
		"Debugging",
		ParameterNonnegative<int>());

  abortDebug=0;
  create_categorized_parameter("abortDebug",abortDebug,"<int>","0",
		"Special debug level for pure worker processors\n\t"
		"applied only when abort signal received",
		"Debugging",
		ParameterNonnegative<int>());

//   outputCacheSize=2*1024*1024;
//   create_categorized_parameter("outputCacheSize",outputCacheSize,
// 		"<int>","2 megabytes",
// 		"Size of optional buffer to store debug output",
// 		"Debugging",
// 		ParameterLowerBound<int>(1));

  loadBalDebug=-1;
  create_categorized_parameter("loadBalDebug",loadBalDebug,"<int>","-1",
		"Special debug output level for load balancing logic",
		"Debugging",
		ParameterLowerBound<int>(-1));

  boundWasteTime=0;
#ifdef ACRO_VALIDATING
  create_categorized_parameter("boundWasteTime",boundWasteTime,"<double>","0",
		"Seconds to waste when bounding a subproblem\n\t"
		"(to simulate a slower bounding routine)",
		"Debugging",
		ParameterNonnegative<double>());
#endif

  debugSeqDigits=0;
  create_categorized_parameter("debugSeqDigits",debugSeqDigits,
		"<int>","0",
  		"Number of sequence digits prepended to output lines\n\t"
		"(makes output suitable for the 'sort' utility)",
		"Debugging",
		utilib::ParameterBounds<int>(0,10));

  stallForDebug=false;
  create_categorized_parameter("stallForDebug",stallForDebug,
                "<bool>","false",
                "If true, then stall the code for debugging",
		"Debugging");

/// CHECKPOINTING

  checkpointMinutes=0;   // Means checkpoints disabled by default
  create_categorized_parameter("checkpointMinutes",checkpointMinutes,
		"<double>","0",
		"Desired minutes between starting to write checkpoints\n\t"
		"(0 disables checkpointing)",
		"Checkpointing",
		ParameterNonnegative<double>());

  checkpointMinInterval=1;
  create_categorized_parameter("checkpointMinInterval",checkpointMinInterval,
		"<double>","0",
		"Lower bound on computing time between checkpoints",
		"Checkpointing",
		ParameterNonnegative<double>());

  const char* envVarName = "PEBBL_CHECKPOINT_DIR";
  const char* envDir = getenv(envVarName);
  string explanation("Directory to place checkpoint files\n\tDefault value ");
  if (envDir != NULL)
    {
      checkpointDir = envDir;
      explanation += "was set from";
    }
  else
    {
      checkpointDir = "";
      explanation += "may be set via";
    }
  explanation += " environment variable ";
  explanation += envVarName;
  create_categorized_parameter("checkpointDir",checkpointDir,"<string>",
		checkpointDir.c_str(),explanation.c_str(),
		"Checkpointing"
		);

  abortCheckpoint = 0;
  create_categorized_parameter("abortCheckpoint",abortCheckpoint,"<int>","0",
		"Debug: abort at the checkpoint with this sequence "
		"number",
		"Checkpointing",
		ParameterNonnegative<int>());

  abortCheckpointCount = 0;
  create_categorized_parameter("abortCheckpointCount",abortCheckpointCount,
		"<int>","0",
		"Debug: abort after writing this many checkpoints",
		"Checkpointing",
		ParameterNonnegative<int>());

  cpDebugCount = 0;
  create_categorized_parameter("cpDebugCount",cpDebugCount,"<int>","0",
		"Debug: dump info for this many problems per checkpoint",
		"Checkpointing",
		ParameterNonnegative<int>());

  restart=false;
  create_categorized_parameter("restart",restart,"<bool>","false",
		"Restart from a previously saved checkpoint",
		"Checkpointing");

  reconfigure=false;
  create_categorized_parameter("reconfigure",reconfigure,"<bool>","false",
		"Read checkpoint serially:\n\t"
		"Allows changes in number of processors.\n\t"
		"Implies 'restart'.",
		"Checkpointing");
 
/// INCUMBENT

  incumbTreeRadix=32;
  create_categorized_parameter("incumbTreeRadix",incumbTreeRadix,"<int>","32",
		"Branching factor for trees used to broadcast\n\t"
		"incumbent values",
		"Incumbent",
		ParameterLowerBound<int>(1));

  incSearchMaxControl=50.0;
  create_categorized_parameter("incSearchMaxControl",incSearchMaxControl,
		"<double>","50.0",
		"Maximum control parameter (application dependent)\n\t"
		"passed to incumbent search thread",
		"Incumbent",
		ParameterLowerBound<double>(1.0));

/// TERMINATION

  rampUpOnly=false;
  create_categorized_parameter("rampUpOnly",rampUpOnly,"<bool>","false",
		"Force quit after ramp-up phase",
		"Termination");

}

} // namespace pebbl

#endif
