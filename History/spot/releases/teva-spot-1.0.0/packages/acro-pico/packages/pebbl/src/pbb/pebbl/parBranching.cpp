/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// parBranching.cpp
//
// Parallel branching class code for PEBBL.  This module contains global
// class routines called in a synchronous fashion on all processors.
//
// Jonathan Eckstein
//

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <utilib/mpiUtil.h>
#include <pebbl/parBranching.h>

using namespace std;

namespace pebbl {


int spToken::packedSize = -1;   // Negative forces token size computation 

CHUNK_ALLOCATOR_DEF(spToken,100);


void parallelBranching::reset(bool /*VBflag*/)
{
  // Set up cluster tracking

  cluster.reset(uMPI::rank, uMPI::size, clusterSize, numClusters, 
		hubsDontWorkSize);

  // Initialize outgoing buffer objects
  // Hold two tokens and two acks
  // (but can expand).  Don't scavenge
  // until three are in use.

  workerOutQ.reset(3,hubMessageSize(2,2));  

  // Initialize stuff to do with scattering

  releaseDecision.reset(targetWorkerKeepFac/totalWorkers(),   
			minScatterProb,
			targetScatterProb,  // Initialize scattering decision
			maxScatterProb);    // information.

  otherHubRelease.reset(numWorkers()/totalWorkers(),
			minNonLocalScatterProb,
			targetNonLocalScatterProb,
			maxNonLocalScatterProb);

  // Initialize buffer objects

  deliverSPBuffers.reset(maxSPPacking,deliverSPTag,3);
  auxDeliverSPQ.reset(3,3*sizeof(int));

  // Don't scavenge Subproblem delivery buffers until
  // three are in use.  Starting size is set later.

  dispatchSPBuffers.reset(maxDispatchPacking,
			  forwardSPTag,
			  3*clusterSize);
  hubAuxBufferQ.reset(3*clusterSize,
		      workerAuxObj::computeBufferSize(this));

  // Initialize worker pool -- this must be done on all processors
  // now, because of the ramp-up phase.

  if ( depthFirst )
    workerPool =   // stack
      new doublyLinkedPool<parallelBranchSub,parLoadObject>(true);
  else if ( breadthFirst )
    workerPool =   // queue
      new doublyLinkedPool<parallelBranchSub,parLoadObject>(false);
  else             // heap
    workerPool = new heapPool<parallelBranchSub,parLoadObject,
      DynamicSPCompare<parallelBranchSub> >();

  workerPool->setGlobal(this);

  if (iAmWorker())
    {
      forceWorkerToRun = false;

      serverPool.setGlobal(this); 

      if (!iAmHub())
	{
	  spAckAddress.resize(maxSPAcks);
	  spAckBound.resize(maxSPAcks);
	  spAckCount = 0;
	  boundKnownToHub = sense*MAXDOUBLE;
	}

      releaseProbCount = 0;
      releaseTestCount = 0;
      workerOutBuffer  = workerOutQ.getFree();
#ifdef ACRO_VALIDATING
      hubMessageSeqNum = 0;
#endif
    }

  if (iAmHub())
    {
      if ( depthFirst )
      	hubPool = 
	  new doublyLinkedPool<spToken,parLoadObject>(true);   //stack
      else if ( breadthFirst )
      	hubPool = 
	  new doublyLinkedPool<spToken,parLoadObject>(false);  //queue
      else
	hubPool = 
	  new heapPool<spToken,parLoadObject,DynamicSPCompare<spToken> >();

      hubPool->setGlobal(this); 
    
      wantLoadInformCount  = 0;
      sumWorkerReportCount = 0;
      hubDispatchCount     = 0;

      hubLastPrint = 0;

      wantLoadInform.resize(numWorkers());
      workerToInform.resize(numWorkers());
      workerLoadReport.resize(numWorkers());
      workerLoadEstimate.resize(numWorkers());
      workerReportCount.resize(numWorkers());
      workerRebalanceCount.resize(numWorkers());
      workerTransitPool.resize(numWorkers());
      workerHeapObj.resize(numWorkers());

      for (int w=0; w < numWorkers(); w++)
	{
	  workerReportCount[w] = 0;
	  wantLoadInform[w] = false;
	  workerTransitPool[w].setGlobal(this);

	  workerHeapObj[w].global = this; 
	  workerHeapObj[w].w = w;
	  heapOfWorkers.add(workerHeapObj[w]);
	}
      termCheckInProgress  = false;
      wantAnotherTermCheck = false;

    }

  // Quality balancing is not compatible with depth- or breadth-first search.

  if (qualityBalance && (depthFirst || breadthFirst))
    {
      qualityBalance = false;
      if (uMPI::iDoIO)
	{
	  CommonIO::end_tagging();
	  ucout << "\nQuality balancing inhibited because "
	    "search is not best-first.\n";
	  CommonIO::begin_tagging();
	}
    }

  // Prepare for checkpointing if need be

  checkpointsEnabled = (checkpointMinutes > 0);
  checkpointing      = false;
  checkpointNumber   = 0;
  restartCPNum       = 0;

  // Clear abort flag

  aborting    = false;
  abortReason = NULL;

  // The code below hasn't been used for a long time; comment out -- JE

//   // If we are going to be dumping debug output to an internal buffer,
//   // create it.

//   if (outputCacheNeeded())
//     {
// #if !defined(ACRO_HAVE_SSTREAM)
//       outputCacheBuf = new char[outputCacheSize];
//       outputCache    = new ostrstream(outputCacheBuf,outputCacheSize,ios::out);
// #else
//       outputCache    = new ostringstream(outputCacheBuf,ios::out);
// #endif
//     }

}


// Constructor for parallel branching class.
// Sets threads to null.

parallelBranching::parallelBranching() :
  
  // Ramp-up variables
  rampUpFlag(false),
  rampUpMessages(0),
  rampUpPool(0),
  rampUpBounds(0),
  rampUpTime(0),
  crossoverFlag(0),
  // No current subproblem
  currentParSP(NULL),
  heapOfWorkers("Worker Tracking")
{

  searchInProgress = false;
  
  incumbentSource = MPI_ANY_SOURCE;

  workerPool        = 0;
  worker            = 0;
  workerAux         = 0;
  incumbentCaster   = 0;
  incumbentSearcher = 0;
  hub               = 0;
  loadBalancer      = 0;
  spServer          = 0;
  spReceiver        = 0;
  earlyOutputter    = 0;
  workerOutBuffer   = 0;
  hubPool           = 0;

  rememberPackSize = -1;

  needHubPruning = 0;

  hubHandledCount = 0;
  spReleaseCount  = 0;
  spDeliverCount  = 0;
  loadBalSPCount  = 0;

  rebalanceSPCount = 0;
  rebalanceCount   = 0;
  myHubsRebalCount = 0;

  messagesReceivedThisProcessor = 0;
  totalMessages                 = 0;
  preprocessMessages            = 0;

}


// Helper to set debug levels

void parallelBranching::setDebugLevel(int level)
{
  setDebug(level);
  workerOutQ.setDebug(level);
  dispatchSPBuffers.setDebug(level);
  dispatchSPBuffers.bufferQ.setDebug(level);
  deliverSPBuffers.setDebug(level);
  deliverSPBuffers.bufferQ.setDebug(level);
  hubAuxBufferQ.setDebug(level);
}


void parallelBranching::setDebugLevelWithThreads(int level)
{
  setDebugLevel(level);
  if (worker)
    worker->setDebug(level);
  if (level > workerDebug)
    workerDebug = level;
  if (workerAux)
    workerAux->setDebug(level);
  if (incumbentCaster)
    incumbentCaster->setDebug(level);
  if (incumbentSearcher)
    incumbentSearcher->setDebug(level);
  if (hub)
    hub->setDebug(level);
  if (loadBalancer && (level > loadBalDebug))
    loadBalancer->setDebug(level);
  if (spServer)
    spServer->setDebug(level);
  if (spReceiver)
    spReceiver->setDebug(level);
  if (earlyOutputter)
    spReceiver->setDebug(level);
}


// Destructor for class.  Kill any threads still alive.  Zap auxiliary pools.

parallelBranching::~parallelBranching()
{
  DEBUGPR(200,ucout << "Destruction of parallelBranching object.\n");

  if (workerPool)
     delete workerPool;
  if (worker)
     delete worker;
  if (workerAux)
     delete workerAux;
  if (incumbentCaster)
     delete incumbentCaster; 
  if (incumbentSearcher)
     delete incumbentSearcher;
  if (hub)
     delete hub;
  if (loadBalancer)
     delete loadBalancer;
  if (spServer)
     delete spServer;
  if (spReceiver)
     delete spReceiver;
  if (earlyOutputter) 
    delete earlyOutputter;

  if (iAmWorker() && workerOutBuffer)
      delete workerOutBuffer;

  if (iAmHub() && hubPool)
    {
      DEBUGPR(200,ucout << "Hub parBranching destruction.\n");

      delete hubPool;

      hubAuxBufferQ.completeAll(workerTag); // Make sure all workers
                                            // get the message to shut down.
    }

//   if (outputCacheNeeded())
//     writeCachedOutput();

  DEBUGPR(200,ucout << "parallelBranching destruction complete.\n");
}


// Does a parallel search.
  
double parallelBranching::parallelSearchFramework(parSPHandler* handler_)
{
  DEBUGPR(300,ucout << "parallelSearchFramework invoked\n");

#ifdef EVENT_LOGGING_PRESENT
  if ((logEvent::eventLog() > 0) && 
      (preprocLogState == logEvent::undefinedState))
    {
      preprocLogState     = logEvent::defineState("Preproc","yellow");
      workerLogState      = logEvent::defineState("Worker","green");
      boundLogState       = logEvent::defineState("Bounding","DarkGreen");
      foundIncLogState    = logEvent::defineState("New Incumb","magenta");
      pruneLogState       = logEvent::defineState("Pruning","orange");
      hubPruneLogState    = logEvent::defineState("Hub Pruning","brown");
      hubLogState         = logEvent::defineState("Hub","red");
      statusPrintLogState = logEvent::defineState("Status Print","white");
      rebalLogState       = logEvent::defineState("Rebalance","LightBlue");
      pCreateLogState     = logEvent::defineState("SP Make","PaleGreen");
      pBoundedLogState    = logEvent::defineState("SP Bounded","black");
      pDeadLogState       = logEvent::defineState("SP Dead","purple");
      pSplitLogState      = logEvent::defineState("SP Split","SpringGreen");
      rampUpLogState      = logEvent::defineState("Ramp Up","gold");
      checkpointLogState  = logEvent::defineState("Checkpoint","chocolate");
    }
#endif

  baseWallTime = WallClockSeconds();

  // Must initialize scheduler before opening log file because creating
  // thread objects will define some events specific to threads.

  DEBUGPR(300,ucout << "Setting up scheduler\n");
  double startSchedInitTime = CPUSeconds();
  initializeScheduler();

  // JE, possibly temporary.  If a "pure" hub, debug
  // everything at hubDebug level.

  if (iAmHub() && !iAmWorker() && (hubDebug > debug))
    setDebugLevelWithThreads(hubDebug);

  IF_LOGGING_EVENTS(1,logEvent::open());

  // Validation log file used to be opened here because preprocess
  // did some logging.  With the new ramp-up, we don't need that any more.
  // Now it should be done after we can tell if we restarted or not.

  outputInProgress = false;
  nextOutputTime   = baseWallTime + earlyOutputMinutes*60;
  lastSolValOutput = sense*MAXDOUBLE;

  checkpointTriggerTime = baseWallTime + checkpointMinutes*60;
  checkpointTotalTime   = 0;

  DEBUGPR(300,ucout << "Preprocessing\n");
  double startPreprocessTime = CPUSeconds();
  LOG_EVENT(1,start,preprocLogState);
  preprocess();
  LOG_EVENT(1,end,preprocLogState);
  
  double startSetupTime = CPUSeconds();
  preprocessTime = startSetupTime - startPreprocessTime;

  // Try moving all load object initializations here

  clusterLoad.init(this);
  globalLoad.init(this);
  lastWorkerReport.init(this);
  serverPool.load().init(this);
  workerPool->load().init(this,true,true);  // Pick up SP & message counts
  if (iAmHub())
    {
      // For hub pool, pick up message counts if not a worker
      hubPool->load().init(this,false,!iAmWorker()); 
      for (int w=0; w < numWorkers(); w++)
	{
	  workerLoadReport[w].init(this);
	  workerLoadEstimate[w].init(this);
	  workerTransitPool[w].load().init(this);
	}
    }

  // Set up a handler for subproblems
  
  if (handler_)
    handler = handler_;
  else if (lazyBounding)
    handler = new parLazyHandler;
  else if (eagerBounding)
    handler = new parEagerHandler;
  else
    handler = new parHybridHandler;

  handler->setPGlobal(this);

  workerLastPrint     = 0;
  workerLastPrintTime = WallClockSeconds();

  baseTime = CPUSeconds();

  setupTime = baseTime - startSetupTime + 
              startPreprocessTime - startSchedInitTime;

  hubLastPrintTime    = WallClockSeconds();
  workerLastPrintTime = hubLastPrintTime;
  loadBalancer->surveyObject.lastPrintTime = hubLastPrintTime;

  DEBUGPR(150,ucout << "Starting search at baseTime=" << baseTime << endl);

  searchInProgress = true;

  if (reconfigure)
    restart = true;

  // Deal with checkpoints

  restarted         = false;
  writingCheckpoint = false;
  readingCheckpoint = false;

  if (restart)
    {
      readingCheckpoint = true;

      if (uMPI::rank == uMPI::ioProc)
	ucout << "Trying to restart from checkpoint\n\n";
      ucout.flush();

      restarted = restartFromCheckpoint();

      if (uMPI::rank == uMPI::ioProc)
	{
	  if (restarted)
	    ucout << "Checkpoint loaded successfully.\n";
	  else
	    ucout << "Warning: unable to read checkpoint files.  "
		  << "Reverting to normal ramp-up.\n\n";
	  ucout.flush();
	}

      readingCheckpoint = false;
    }

  // Now we know if we managed to restart from the checkpoint.
  // That will tell us to open the validation log files in either
  // out or append mode.

  vout = valLogFile();
  valLogInit();

  // If we didn't load from a checkpoint, do ramp up.

  if (!restarted)
    rampUpSearch();

  // If there are any problems left in the pool, do the real search.

  if (rampUpPool > 0)
    {
      prepareCPAbort();
      sched.execute();
    }

  // Clean up

  spToken::memClear();
  if (!handler_)
    delete handler;

  searchInProgress = false;

  IF_LOGGING_EVENTS(1,logEvent::close(););

  if (checkpointNumber > 0)
    deleteCheckpointFile(checkpointNumber,uMPI::rank);

  if (validateLog)
    {
      if (uMPI::rank == 0)
	valLogFathomPrint();
      delete vout;
    }

  uMPI::barrier();   // Safety check to make sure that all processors think
                     // they're done before trying to call the destructor
                     // on any processor.

  return incumbentValue;
};


// To create the scheduler object we need.

void parallelBranching::initializeScheduler()
{
  sched.setDebug(schedulerDebug);
  highPriorityGroup = sched.add(new ThreadQueue(round_robin));
  baseGroup         = sched.add(new ThreadQueue(time_weighted_priority));
  placeTasks();

  ListItem<parBranchingThreadObj*> *l1;
  for(l1 = threadsList.head(); l1;  l1=threadsList.next(l1))
      if (l1->data()->active) l1->data()->setDebugLevel(debug);
  
}


//  Places tasks into the scheduler object.
//  NOTE: we create all threads, but don't place them all in the scheduler.
//  If processors created different subsets of the possible threads,
//  there could be tag ID mismatches.

void parallelBranching::placeTasks()
{
  worker = new workerThreadObj(this);
  placeTask(worker,iAmWorker(),baseGroup);

  incumbentThreadExists = hasParallelIncumbentHeuristic() &&
                          useIncumbentThread;
  incumbentSearcher = createIncumbentSearchThread();
  placeTask(incumbentSearcher,
	    iAmWorker() && incumbentThreadExists,
	    baseGroup);

  hub = new hubObj(this);
  placeTask(hub,iAmHub(),highPriorityGroup);

  incumbentCaster = new incumbCastObj(this);
  placeTask(incumbentCaster,true,highPriorityGroup);

  spReceiver = new spReceiverObj(this);
  placeTask(spReceiver,iAmWorker(),highPriorityGroup);

  // Now that we know the spReceiver buffer size, set up the buffer size 
  // tracking for other processors (it will be the same on all workers)

  if (iAmWorker())
    {
      knownBufferSize.resize(uMPI::size);
      for (int i=0; i<uMPI::rank; i++)
	knownBufferSize[i] = spReceiver->sizeOfBuffer();
      DEBUGPR(100,ucout <<"Initialized knownBufferSize to " 
	      << spReceiveBuf << '/' << knownBufferSize[0] << endl);
    }

  // Back to creating threads...

  spServer = new spServerObj(this);
  placeTask(spServer,iAmWorker(),highPriorityGroup);

  loadBalancer = new loadBalObj(this);
  placeTask(loadBalancer,iAmHub() && (uMPI::size > 1),highPriorityGroup);
  
  workerAux = new workerAuxObj(this);
  placeTask(workerAux,iAmWorker() && !iAmHub(),highPriorityGroup);

  if (earlyOutputMinutes > 0)
    {
      earlyOutputter = new earlyOutputObj(this);
      placeTask(earlyOutputter,true,highPriorityGroup);
    }

}


void parallelBranching::placeTask(parBranchingThreadObj* thread,
				  int cond,
				  int group)
{
  DEBUGPR(200,ucout << "parallelBranching::placeTask" << endl);
  DEBUGPR(300,thread->dump(ucout));
  thread->active = cond;
  if (cond)
    {
      DEBUGPRP(100,ucout << "Placing task " << thread->name);
      thread->startup();
      DEBUGPRP(100,ucout << " (started)");
      sched.insert(thread,group,0,thread->name);
      DEBUGPR(100,ucout << " (inserted)" << endl);
    }
  threadsList.add(thread); 
}


parallelBranchSub* parallelBranching::makeParRoot()
{
  DEBUGPR(75,ucout << "parallelBranching::makeRoot invoked.\n");
  parallelBranchSub* root = blankParallelSub();
  root->setRoot();
  root->id.creatingProcessor = 0;
  DEBUGPR(10,ucout << "Created root problem " << root << '\n');
  if (valLogOutput())
    root->valLogCreatePrint(0);
  return root;
}


// Standard setup pattern for parallel branching

bool parallelBranching::setup(int& argc, char**& argv)
{
  IF_LOGGING_EVENTS(1,logEvent::init(););

  int flag = false;

  // The I/O processor reads the problem as in serial.
  // The others just initialize timers and set up parameters.

  if (uMPI::iDoIO)
    flag = (int)branching::setup(argc,argv);
  else
    {
      startTime = CPUSeconds();
      startWall = WallClockSeconds();
      // Don't check for errors here; the I/O processor will do that.
      processParameters(argc,argv,min_num_required_args);
      set_parameters(plist,false);
    }

  // The debugSeqDigits parameter needs extra processing 

  CommonIO::numDigits = debugSeqDigits; 

  // Broadcast the read-in success flag to everbody

  uMPI::broadcast(&flag,1,MPI_INT,uMPI::ioProc);

  // If things worked, broadcast the problem to everybody

  if (flag)
    broadcastProblem();

  return (bool)flag;
}


// To broadcast a problem to all processors

void parallelBranching::broadcastProblem()
{
  if (uMPI::size == 1)
    {
      broadcastTime = 0;
      return;
    }

  double startBroadcastTime = CPUSeconds();

  IF_LOGGING_EVENTS(1,logEvent::init(););

  if(uMPI::iDoIO)                   // If we are the special IO processor
    {
      PackBuffer outBuf(8192);             // Pack everything into a buffer.
      packAll(outBuf);
      int probSize = outBuf.size();        // Figure out length.
      DEBUGPR(70,ucout << "Broadcast size is " << probSize << " bytes.\n");
      uMPI::broadcast(&probSize,          // Broadcast length.
		      1,
		      MPI_INT,
		      uMPI::ioProc);
      uMPI::broadcast((void*) outBuf.buf(), // Now broadcast buffer itself.
		      probSize,
		      MPI_PACKED,
		      uMPI::ioProc);
    }

  else   // On the other processors, we receive the same information...

    {
      int probSize;                          // Get length of buffer
      uMPI::broadcast(&probSize,             // we're going to get.
		      1,
		      MPI_INT,
		      uMPI::ioProc);
      DEBUGPR(70,ucout << "Received broadcast size is " << probSize << 
	      " bytes.\n");
      UnPackBuffer inBuf(probSize);          // Create a big enough
      inBuf.reset(probSize);                 // temporary buffer.
      uMPI::broadcast((void *) inBuf.buf(),  // Get the data...
		      probSize,
		      MPI_PACKED,
		      uMPI::ioProc);
      DEBUGPR(100,ucout << "Broadcast received.\n");
      unpackAll(inBuf);                      // ... and unpack it.
      DEBUGPR(100,ucout << "Unpack seems successful.\n");
    }

  broadcastTime = CPUSeconds() - startBroadcastTime;
  DEBUGPR(70,ucout << "Problem broadcast done.\n");

  // Compute and remember size of packed subproblems.
  rememberPackSize = spAllPackSize();
  DEBUGPR(50,ucout << "Packed subproblem size is " 
	  << rememberPackSize << ".\n");
  deliverSPBuffers.bufferQ.setStartingBufferSize(rememberPackSize);
}


void parallelBranching::rampUpIncumbentSync()
{
  DEBUGPR(100,ucout << "Entered rampUpIncumbentSync(): value=" 
	  << incumbentValue << ", source=" << incumbentSource << endl);
  MPI_Op reduceOp = MPI_MIN;    // For minimization
  if (sense == maximization)    // Change if maximization
    reduceOp = MPI_MAX;
  double bestIncumbent = sense*MAXDOUBLE;
  uMPI::reduceCast(&incumbentValue,&bestIncumbent,1,MPI_DOUBLE,reduceOp);
  DEBUGPR(100,ucout << "Got value " << bestIncumbent << endl);

  int sourceRank = uMPI::size;
  if (incumbentValue == bestIncumbent)
    sourceRank = incumbentSource;
  int lowestRank = 0;
  uMPI::reduceCast(&sourceRank,&lowestRank,1,MPI_INT,MPI_MIN);
  DEBUGPR(100,ucout << "Got source " << lowestRank << endl);

  if (bestIncumbent != incumbentValue)
    {
      needPruning = true;
      newIncumbentEffect(bestIncumbent);
      if ((uMPI::rank == 0) && trackIncumbent)
	ucout << "New incumbent found: value=" << bestIncumbent
	      << ", source=" << lowestRank
	      << ", time=" << CPUSeconds() - baseTime 
	      << (rampingUp() ? " (ramp-up)" : "") << endl;
    }
  
  incumbentValue  = bestIncumbent;
  incumbentSource = lowestRank;

  DEBUGPR(100,ucout << "Leaving rampUpIncumbentSync(): value=" 
	  << incumbentValue << ", source=" << incumbentSource << endl);

  if (uMPI::rank > 0)
    rampUpMessages += 4;
}


void parallelBranching::rampUpSearch()
{
  // Decide that we're getting started

  double startRampUpTime = CPUSeconds();
  LOG_EVENT(1,start,rampUpLogState);
  rampUpFlag = true;
  DEBUGPR(1,ucout << "Starting ramp-up phase\n");

  initialGuess();

  // Insert the root on all processors, and synchronously chomp
  // through subproblems until we've got nothing to do (lucky!)
  // or the continueRampUp() method tells use to stop.

  workerPool->insert(makeParRoot());

  while((spCount() > 0) && keepRampingUp())
    {
      processSubproblem();
      if (uMPI::rank == 0)
	statusPrint(workerLastPrint,
		    workerLastPrintTime,
		    workerPool->updatedLoad(),
		    "r");
    }

  DEBUGPR(1,ucout << "Synchronous ramp-up loop complete.\n");

  rampUpCleanUp();  // Derived codes can do any clean up they need to

  if (haveCurrentSP())
    unloadCurrentSPtoPool();

  rampUpIncumbentSync();

  double aggBoundAtCrossover = workerPool->updatedLoad().aggregateBound;
  
  // Now pick through the worker pools (all identical, WE HOPE)
  // and place all suproblems destined for this particular processor
  // in a temporary pool.  Also adjust all load tracking objects.
  // The temporary pool is a stack unless the breadthFirst() parameter
  // is set, in which case it's a queue.

  DEBUGPR(10,ucout << "Starting ramp-up crossover, bounded="
	  << subCount[bounded] << endl);
  if (uMPI::iDoIO)
    DEBUGPR(2,statusLine(workerPool->updatedLoad(),"X"));
  
  crossoverFlag = true;
  rampUpPool = 0;

  doublyLinkedPool<parallelBranchSub,loadObject> tempPool(!breadthFirst);
  tempPool.load().init(this);

  globalLoad.reset();
  clusterLoad.reset();
  if (iAmHub())
    for (int w=0; w<numWorkers(); w++)
      workerLoadEstimate[w].reset();

  while(workerPool->size() > 0)
    {
      // Get a subproblem from the pool and count it in the global load

      parallelBranchSub* sp = workerPool->remove();
      globalLoad += *sp;
      DEBUGPR(100,ucout << "Ramp-up crossover: " << sp 
	      << ", global load " << globalLoad << endl);

      // Figure out which processor will be the worker for this SP.
      // workerRank is the rank in the set of workers, and 
      // procRank is this worker's rank in the set of processors.

      int workerRank = rampUpPool++ % totalWorkers();
      int procRank   = cluster.globalFollower(workerRank);
      DEBUGPR(100,ucout << "For processor " << procRank << endl);
      
      // Figure out if the processor belongs to this cluster, and if so
      // count the subproblem in the clusterLoad and perhaps the hub
      // information

      if (whichCluster(procRank) == whichCluster(uMPI::rank))
	{
	  clusterLoad += *sp;
	  DEBUGPR(100,ucout << "My cluster, load " << clusterLoad << endl);
	  if (iAmHub())
	    {
	      int workerClusterRank = whichWorker(procRank);
	      DEBUGPR(100,ucout << "Worker rank in cluster = " 
		      << workerClusterRank << endl);
	      workerLoadReport[workerClusterRank]   += *sp;
	      workerLoadEstimate[workerClusterRank] += *sp;
	    }
	}

      // If this is the worker processor owning the subproblem,
      // save it in the temporary pool; otherwise, discard.

      if (procRank == uMPI::rank)
	{
	  DEBUGPR(100,ucout << "Keeping...\n");
	  tempPool.insert(sp);
	}
      else
	{
	  DEBUGPR(100,ucout << "Eliminating.\n");
	  sp->recycle();
	}
    }

  // Move any saved problems back into the worker pool.

  DEBUGPR(100,ucout << "Transferring problems to worker pool...\n");

  while(tempPool.size() > 0)
    workerPool->insert(tempPool.remove());

  DEBUGPR(100,ucout << "Done moving problems to worker pool...\n");

  crossoverFlag = false;      // Crossover is complete

  // Adjust subproblem counters to avoid multiple counting, and
  // start serial numbers from 0 on processors other than [0].
  // Also, save subproblem counters

  rampUpBounds = subCount[bounded];

  if (uMPI::rank > 0)
    {
      probCounter = 0;
      for(int i=0; i<numStates; i++)
	subCount[i] = 0;
    }
  DEBUGPR(10,ucout << "bounded=" << subCount[bounded] << endl);

  // Set initial loads correctly, and possibly dump them.

  clusterLoad.update();
  globalLoad.update();
  DEBUGPR(20,clusterLoad.dump(ucout,"clusterLoad"));
  DEBUGPR(20,globalLoad.dump(ucout,"globalLoad"));
  if (iAmHub())
    for (int w=0; w<numWorkers(); w++)
      workerLoadEstimate[w].update();
  globalLoad.boundedSPs = rampUpBounds;
  clusterLoad.boundedSPs = rampUpBounds;
 
  if (iAmFirstHub())
    DEBUGPR(1,statusLine(globalLoad,"x"));

  // Set printing times.  These are only really meaningful on processor 0, 
  // and we might get some output immediately on other processors if
  // hubsPrintStatus or workersPrintStatus are set.

  hubLastPrintTime = workerLastPrintTime;
  hubLastPrint     = workerLastPrint;
  
  loadBalancer->surveyObject.lastPrintTime = workerLastPrintTime;
  loadBalancer->surveyObject.lastPrint     = workerLastPrint;

  // Declare ramp-up to be over and tidy up.

  DEBUGPR(1,ucout << "Ramp-up phase complete, bounded=" << rampUpBounds
	  << " global-pool=" << rampUpPool 
	  << " local-pool=" << workerPool->size() << endl);

  rampUpFlag = false;
  LOG_EVENT(1,end,rampUpLogState);
  rampUpTime = CPUSeconds() - startRampUpTime;

  // Abort here if rampUpOnly option is set

  if (rampUpOnly)
    postRampUpAbort(aggBoundAtCrossover);

}


// JE -- this was looking identical to the code in branching::; try removing
// void parallelBranching::solve()
// {
//   double run_start = CPUSeconds();
//   search();
//   searchTime = CPUSeconds() - run_start;

//   //
//   // Final I/O
//   //

//   ucout << endl;
//   printAllStatistics();
//   ucout << endl;

//   solutionToFile();

//   if (abortReason)
//     ucout << "RUN ABORTED: " << abortReason << endl << endl;
// }




// Called when it's time to abort

void parallelBranching::setupAbort()
{
  // If we already know we're aborting, don't do anything
  if (aborting)
    return;

  aborting = true;

  if (iAmHub())
    {
      if (clusterNumber() == 0)
	ucout << "\nStarting to abort (" << abortReason << ")\n";
      alertWorkers(startAbortSignal);
    }
}


// This makes sure to delete all subproblems and tokens during an
// abort.  This way, all subproblems will be destructed before various
// other things (otherwise you can get seg faults on exit).

void parallelBranching::clearAllSPsForAbort()
{
  // Clear the hub pool.  If we are aborting, we should be in 
  // quiesence, and the worker transit pool should already be empty.

  if (iAmHub())
    hubPool->clear();

  // Clear the worker-related pools.

  if (iAmWorker())
    {
      if (haveCurrentSP())
	unloadCurrentSPtoPool();
      workerPool->clear();
      serverPool.clear();
    }
}


spToken::spToken(parallelBranchSub* sp,int childNum,int childCount) :
  coreSPInfo(*sp)
{
  if (!sp->canTokenize())
    EXCEPTION_MNGR(runtime_error, "Attempt to make a token from a subproblem "
		"for which canTokenize() == false");
  spProcessor         = uMPI::rank;
  whichChild          = childNum;
  memAddress          = sp;
  childrenRepresented = childCount;
  sp->tokenCount     += childCount;
  globalPtr           = sp->bGlobal();
  depth		  = -1;			// dummy initialization
};



parLoadObject parallelBranching::updatedPLoad()
{
  parLoadObject l = workerPool->updatedLoad();
  DEBUGPR(200,ucout << "upDatedPLoad: workerPool load is " << l << endl);
  if (haveCurrentSP())
    l.addLoad(currentParSP->bound,1);

  DEBUGPR(200,ucout << "upDatedPLoad: calculated " << l << endl);

  return l;
};



void spToken::packEmpty(PackBuffer& buf)
{
spToken dummy(NULL);
#ifdef ACRO_VALIDATING
  //
  // WEH - initialize data to avoid error warnings in purify
  //
  dummy.bound = -999.0;
  dummy.integralityMeasure = -999.0;
  dummy.state = dead;
  dummy.id.setEmpty();
  dummy.depth = -999;
  dummy.childrenRepresented = -999;
  dummy.spProcessor = -999;
  dummy.whichChild = -999;
  dummy.memAddress = NULL;
#endif
dummy.pack(buf);
}


// bool parallelBranching::outputCacheNeeded()
// {
//   if ((hubDebug > 0) && iAmHub())
//     return true;
//   if ((workerDebug > 0) && iAmWorker())
//     return true;
//   return false;
// }


double parallelBranching::qualityTolerance(double bound)
{
  double tolObj = qualityBalanceObjFac*fabs(bound);
  double tolGap = qualityBalanceGapFac*fabs(incumbentValue - bound);
  return min(tolObj,tolGap);
}


loadObject parallelBranching::load()
{
  parLoadObject l = workerPool->load();
  if (haveCurrentSP())
    l += *currentParSP;

  return l;
}


// Cancel all outstanding receive requests so we don't hang
// on exit.
void parallelBranching::cancelComm()
{
ListItem<parBranchingThreadObj*>* tptr;
for(tptr=threadsList.head(); tptr; tptr=threadsList.next(tptr) )
  tptr->data()->cancelComm();

workerOutQ.clear();
deliverSPBuffers.clear();
auxDeliverSPQ.clear();
dispatchSPBuffers.clear();
hubAuxBufferQ.clear();
}


// Call this routine if you want to terminate cleanly during the middle of 
// a parallel search.  Hopefully there will be no hangs or error 
// messages.

void parallelBranching::cleanAbort()
{
  // Clean up validation logs.

  if (validateLog)
    delete vout;

  // Make sure we don't get a memory leak message (the "false" does that)

  spToken::memClear(false);

  // Cancel all outstanding receive requests so we don't hang
  // on exit.

  cancelComm();

  // Synchronize, terminate MPI, and try for a clean exit.

  uMPI::barrier();
  uMPI::done();
  exit(0);
}



} // namespace pebbl

#endif

