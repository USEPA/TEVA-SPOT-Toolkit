/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// pbHub.cpp
//
// Parallel branching class code for PEBBL -- code associated with hub.
//
// Jonathan Eckstein
//


#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <utilib/mpiUtil.h>
#include <utilib/seconds.h>
#include <pebbl/parBranching.h>
#include <pebbl/packPointer.h>

using namespace std;

namespace pebbl {


// Special debug output macro, just for the hub.

#define HUBDEBUG(level,action)  if (hubDebug >= level) { action; };



// Figure out if a worker should get more subproblems.

bool parallelBranching::needsWorkFromHub(int w)
{
  HUBDEBUG(200,ucout << "Considering worthiness of worker " << w << ".\n");
  if (workerCount(w) < workerSPThreshHub)
    return true;
  HUBDEBUG(200,ucout << "Above workerSPThreshHub.\n");
  if (qualityBalance && needsBetterWork(w))
    return true;
  HUBDEBUG(200,ucout << "Does not need better work.\n");
  return false;
};


// Does a worker have a high enough quality of work?

bool parallelBranching::needsBetterWork(int w)
{
  double bh  = hubPool->select()->bound; 
  double bw  = workerBound(w);
  double tol = qualityTolerance(bh);
  HUBDEBUG(200,ucout << "needsBetterWork: bh=" << bh << " bw=" << bw 
	   << " tol=" << tol << '\n');
  return (bw - bh)*sense > tol;
}


//  Methods that manipulate the order in which the hub feeds 
//  subproblems to workers, and the corresponding hub data structures.

int parallelBranching::moreDeservingWorker(int w1, int w2)
{
//   ucout << "moreDeservingWorker: workerSPThreshHub=" << workerSPThreshHub
// 	<< " w1=" << w1 << " w2=" << w2 
// 	<< " workerCount(w1)=" << workerCount(w1)
// 	<< " workerCount(w2)=" << workerCount(w2)
// 	<< " workerBound(w1)=" << workerBound(w1)
// 	<< " workerBound(w2)=" << workerBound(w2) << endl;

  if (qualityBalance)
    {
      int threshold = workerSPThreshHub;
      int s1 = min(workerCount(w1),threshold);
      int s2 = min(workerCount(w2),threshold);
      if (s1 == s2)
	{
	  if ( (workerBound(w1) - workerBound(w2))*sense > 0) 
	    return -1; 
	  else 
	    return 1;
	}
      else
	{
	  if ( s1 < s2 ) 
	    return -1; 
	  else 
	    return 1;
	}
    }

  // Quantity balancing only...

  if ( workerCount(w1) < workerCount(w2) ) 
    return -1; 
  else 
    return 1;
}


//  Figures out sizes of buffers to send and receive messages to hubs.

int parallelBranching::hubMessageSize(int tokens, int acks)
{
  int bSize = tokens*(sizeof(int) + spToken::packSize());
  bSize += parLoadObject::packSize(this);
  bSize += sizeof(int);
  bSize += acks*(sizeof(void*)+sizeof(double));
  bSize += 2*sizeof(int);
#ifdef ACRO_VALIDATING
  bSize += sizeof(int);   // If validating, put in sequence #'s
#endif
  return bSize;
}


//  Recalculate the load on a worker and adjust its position in the
//  auxiliary heap accordingly.

void parallelBranching::repositionWorker(int w)
{
  HUBDEBUG(150,ucout << "Repositioning worker " << w << "=["
	   << workerProc(w) << "], incumbent = " << incumbentValue 
	   << ", time=" << MPI_Wtime() - baseTime << '\n');
  if ((w == 0) && iAmWorker())
    workerLoadEstimate[w] = updatedPLoad();
  else
    workerLoadEstimate[w] = workerLoadReport[w];
  HUBDEBUG(110,ucout << "Worker " << w << "=[" << workerProc(w) 
	   << "] has reported load " << workerLoadEstimate[w] << endl);
  HUBDEBUG(150,workerLoadEstimate[w].dump(ucout,"copied reported"));
  // Avoid bogus incumbent mismatches by making sure the transit
  // load object has the same value and source as what the worker
  // reported.  If the hub has something different, that will show
  // up in the cluster load calculations when hubPool's load is added in.
  parLoadObject& transitLoad  = workerTransitPool[w].updatedLoad();
  transitLoad.incumbentValue  = workerLoadEstimate[w].incumbentValue;
  transitLoad.incumbentSource = workerLoadEstimate[w].incumbentSource;
  workerLoadEstimate[w] += transitLoad;
  HUBDEBUG(100,ucout << "Worker " << w << "=[" << workerProc(w) 
	   << "] has load " << workerLoadEstimate[w] 
	   << " and bound " << workerBound(w) << ".\n");
  HUBDEBUG(150,transitLoad.dump(ucout,"added for transit"));
  HUBDEBUG(150,workerLoadEstimate[w].dump(ucout,"after addition"));

  heapOfWorkers.refloatElement(workerHeapObj[w].location);
  HUBDEBUG(100,ucout << "Heap position now " << workerHeapObj[w].location
	   << endl);  
}


//  Update hub pools (for use if a better incumbent comes along).

void parallelBranching::hubPrune()
{
  HUBDEBUG(3,ucout << "Hub pruning invoked.\n");
  LOG_EVENT(2,start,hubPruneLogState);
  hubPool->prune(); 
  hubPool->load().update(); 
  HUBDEBUG(3,ucout << "Hub pool pruning complete.\n");
  HUBDEBUG(100,ucout << "Hub load is " << hubPool->load() << ".\n");
  for(int w=0; w<numWorkers(); w++)
    {
      HUBDEBUG(110,ucout << "Worker " << w <<" transit load was " 
	       << workerTransitPool[w].load() << ".\n");
      workerTransitPool[w].prune();
      workerTransitPool[w].load().update();
      HUBDEBUG(100,ucout << "Worker " << w <<" transit load is " 
	       << workerTransitPool[w].load() << ".\n");
      repositionWorker(w);
    }
  needHubPruning = false;
  if (iAmWorker())
    pruneIfNeeded();
  LOG_EVENT(2,end,hubPruneLogState);
  HUBDEBUG(3,ucout << "Hub pruning complete.\n");

}


//  The hub executes this to deal with a message from a worker.  Format:
//  [1,token]*,0,{1,load}{0},k,(k times)[spAckAddress,spAckBound],
//  (adjustment to messages.hubDispatch.received).
//  The last is non-zero when a subproblem from another cluster is 
//  scheduled for processing at this cluster, but was fathomed in the meantime.

void parallelBranching::handleHubMessage(UnPackBuffer& inBuf,int src)
{
  HUBDEBUG(100,ucout << "Hub message received from " << src << 
	   ", time=" << MPI_Wtime() - baseTime << '\n');
  int w = whichWorker(src);
  int* receiveCounterP = &messages.nonLocalScatter.received;
  int myWorker = (w >= 0) && (w < numWorkers());
  if (myWorker)
    {
      receiveCounterP = &messages.localScatter.received;
      HUBDEBUG(100,ucout << "My worker index " << w << ".\n");
      if(++sumWorkerReportCount >= globalLoadInformRate*numRemoteWorkers())
	{
	  HUBDEBUG(100,ucout << "sumWorkerReportCount=" 
		   << sumWorkerReportCount << '\n');
	  setToInformAll();
	  sumWorkerReportCount = 0;
	}
      else if (++workerReportCount[w] >= localLoadInformRate)
	setToInform(w);
      HUBDEBUG(100,ucout << "workerReportCount[" << w << "]="
	       << workerReportCount[w] << ", sumWorkerReportCount="
	       << sumWorkerReportCount << ".\n");
    }
  int flag;
  int tokensReceived = 0;
  while(inBuf >> flag, flag) 
    {
      (*receiveCounterP)++;
      tokensReceived++;
      MEMDEBUG_START_NEW("spToken")
      spToken* t = new spToken(this,inBuf);
      MEMDEBUG_END_NEW("spToken")
      if (t->canFathom())
	{
	  HUBDEBUG(100,ucout << "Ignoring " << t << " (fathomed).\n");
	  delete t;
	}
      else
	{
	  hubPool->insert(t);
	  HUBDEBUG(100,ucout << "Received token " << t << ".\n");
	}
    }
  hubHandledCount += tokensReceived;

  int howMany;
  inBuf >> howMany;
  if (howMany)
    {
      if (!myWorker)
	EXCEPTION_MNGR(runtime_error, "Worker " << src << 
		       " reported load to invalid hub "
		     << uMPI::rank);
      inBuf >> workerLoadReport[w];
      HUBDEBUG(100,ucout << "Load report is " 
	       << workerLoadReport[w] << ", with incumbent value "
	       << workerLoadReport[w].incumbentValue << '\n');
      HUBDEBUG(105,workerLoadReport[w].dump(ucout,"from worker"));
    }
  inBuf >> howMany;
  if ((howMany > 0) & !myWorker)
    EXCEPTION_MNGR(runtime_error, "Worker " << src << 
		   " acknowledged " << howMany << 
		    " subproblem(s) to invalid hub " << uMPI::rank);
  for(int i=0; i<howMany; i++)
    {
      spToken* t = (spToken*) unpackPointer(inBuf);
      double tBound;
      inBuf >> tBound;
      HUBDEBUG(100,ucout << "Acknowledgement with bound " << tBound << ".\n");
      if (!canFathom(tBound))
	{
	  HUBDEBUG(100,ucout << "Zapping " << t << " from transit pool.\n");
	  workerTransitPool[w].kill(t);
	}
    }
  inBuf >> howMany;
  if (howMany != 0)
    {
      if (myWorker)
	EXCEPTION_MNGR(runtime_error, 
		       "Worker in cluster sent dispatch count adjustment");
      messages.hubDispatch.received += howMany;
      HUBDEBUG(100,ucout << "hubDispatch.received adjusted by "
	       << howMany << " to " << messages.hubDispatch.received 
	       << ".\n");
    }
  bool rbFlag;
  inBuf >> rbFlag;     // See if this message contained "rebalanced" tokens.
  if (rbFlag)
    {
      if (!myWorker)
	EXCEPTION_MNGR(runtime_error, 
		       "Rebalance signal from worker [" << myWorker << 
			"], a member of another cluster.");
      workerRebalanceCount[w] += rbFlag;
      setToInform(w);
      HUBDEBUG(100,ucout << "Rebalance flag is " << rbFlag
	       << ", workerRebalanceCount[" << w << "]="
	       << workerRebalanceCount[w] << ".\n");
    }

#ifdef ACRO_VALIDATING
  int seqNo = 0;
  inBuf >> seqNo;
  DEBUGPR(100,ucout << "Got sequence " << src << '#'
	  << seqNo << endl);
#endif

  if (myWorker)
    repositionWorker(w);

  // No longer try to activate the hub here.  It will be done by the 
  // preExitAction of the hub thread.
}


//  Basic hub procedure.

void parallelBranching::activateHub()
{
#ifdef ACRO_VALIDATING
  double activateTime = MPI_Wtime();
  int    baseMessages = messages.hubDispatch.sent;
  HUBDEBUG(100,ucout << "Hub activation at time " 
	   << activateTime - baseTime << ".\n");
#else
  HUBDEBUG(100,ucout << "Hub activation\n");
  LOG_EVENT(1,start,hubLogState);
#endif

  if (!suspending())
    hubDistributeWork();

  hubCalculateLoads();

  // We have to do all sorts of special termination, abort, and
  // checkpointing stuff when there is only one hub.  For multiple hubs,
  // the load balance thread takes care of it.

  if (numHubs() == 1)
    {
      if (shouldAbort(clusterLoad.boundedSPs))
	  setupAbort();
      else if ((clusterLoad.count() > 0) && checkpointDue())
	{
	  setupCheckpoint();
	  if (uMPI::size == 1)
	    writeCheckpoint();
	}
    }

  if (needClusterCheck())
    {
      HUBDEBUG(10,ucout << "Quiesence suspected.\n");
      if (uMPI::size == 1)
	clusterTerminate();
      else
	{
	  if (termCheckInProgress)
	    {
	      HUBDEBUG(10,ucout << "termCheckInProgress set\n");
	      wantAnotherTermCheck = true;
	    }
	  else
	    {
	      termCheckInProgress  = true;
	      wantAnotherTermCheck = false;
	      loadBalancer->setUpClusterCheck(clusterLoad.messages);
	    }
	}
    }
  else if (!suspending())
    tellWorkersLoads();

  if ((earlyOutputMinutes > 0) && parallelNeedEarlyOutput())
    {
      outputInProgress = true;
      earlyOutputter->activateEarlyOutput();
    }

  LOG_EVENT(1,end,hubLogState);
#ifdef ACRO_VALIDATING
  HUBDEBUG(100,ucout << "Hub activation completed, dispatch count " <<
	   (messages.hubDispatch.sent - baseMessages) << " taking "
	   << MPI_Wtime() - activateTime << " seconds.\n");
#else
  HUBDEBUG(100,ucout << "Hub activation completed\n");
#endif
}


//  Invoke the basic hub procedure from another thread, keeping
//  track of the time spent.  This now checks whether the hub is
//  about to run anyway.  If so, it doesn't do anything.  It's better
//  for the hub to incorporate the information in any queued messages
//  before it makes any more decisions.

void parallelBranching::activateHub(parBranchingThreadObj* callingThread)
{
  sched.check_waiting_threads();
  if (!(hub->ready()))
    {
      double startHubTime = CPUSeconds();
      activateHub();
      callingThread->hubTime += CPUSeconds() - startHubTime;
    }
  else
    {
      HUBDEBUG(200,ucout << "Not activating hub due to pending messages.\n");
    }
}

    
//  Distributes work from hub to workers, or at least tries to.

void parallelBranching::hubDistributeWork()
{
  int w;
  HUBDEBUG(100,ucout << "Hub work distribution: " << hubPool->size() 
	   << " pool elements.\n");
  if (needHubPruning)
    hubPrune();
  HUBDEBUG(100,ucout << "Hub load is " << hubPool->load() << ".\n");
  if (iAmWorker())
    repositionWorker(0);
  while((hubPool->size() > 0) && needsWorkFromHub(w = mostDeservingWorker()))
    {
      int wProc  = workerProc(w);
      spToken* t = tokenToSend(hubPool);
      if (t == 0)                          // Safety: if no problem to send
	break;                             // bail out.
      int pProc = t->spProcessor;
      workerTransitPool[w].insert(t);
      messages.hubDispatch.sent++;
      HUBDEBUG(100,ucout << "Sending work (" << messages.hubDispatch.sent
		<< ") to w=" << w << "={" << wProc << "}: ");
      HUBDEBUG(100,ucout << "token " << t << " at " << (void*) t << '\n');
      if (++hubDispatchCount >= dispatchLoadInformRate*numWorkers())
	setToInformAll();
      if (pProc == uMPI::rank)
	deliverSP(t->id,
		  t->whichChild,
		  t->bound,
		  t->memAddress,
		  wProc,
		  t);
      else
	{
	  PackBuffer* outBuf = dispatchSPBuffers.startSegmentBuffer(pProc);
	  *outBuf << t->bound;
	  *outBuf << t->id;
	  *outBuf << t->whichChild;
	  packPointer(*outBuf,t->memAddress);
	  *outBuf << wProc;
	  packPointer(*outBuf,t);
	  dispatchSPBuffers.segmentDone(pProc);
	}
      repositionWorker(w);
    }
  HUBDEBUG(100,ucout << "Flushing dispatch buffers.\n");
  dispatchSPBuffers.flush();
  if (iAmWorker())
    {
      HUBDEBUG(100,ucout << "Flushing delivery buffers.\n");
      deliverSPBuffers.flush();
    }
  HUBDEBUG(100,ucout << "Buffer flushing complete.\n");
}

      
//  To decide which problem a hub should dispatch from its token pool.
//  If for some strange reason the pool has fathomable problems, throw
//  them away.  If this exhausts the pool, return null.

spToken* parallelBranching::tokenToSend(branchPool<spToken,parLoadObject> *thePool) 
{
  HUBDEBUG(120,ucout << "Selecting subproblem to send from hub.\n");
  spToken* t;
  do
    {
      t = thePool->select();
      if (t->canFathom())
	{
	  HUBDEBUG(100,ucout << "Killing " << t << '\n');
	  thePool->kill(t);
	  t = 0;
	}
    }
  while((t == 0) && (thePool->size() > 0));
  if (t == 0)
    return 0;
  HUBDEBUG(100,ucout << "Considering sending " << t << ", representing " 
	   << t->childrenRepresented << " problems.\n");
  if ((t->whichChild != anyChild) || (t->childrenRepresented == 1))
    {
      HUBDEBUG(100,ucout << "Selected.\n");
      thePool->remove(t);
      return t;
    }
  MEMDEBUG_START_NEW("spToken")
  spToken* tCopy = new spToken(*t);
  MEMDEBUG_END_NEW("spToken")
  tCopy->childrenRepresented = 1;
  tCopy->poolPtr = 0;
  (t->childrenRepresented--);
  HUBDEBUG(100,ucout << "Created copy token " << tCopy << ", representing " 
	   << tCopy->childrenRepresented << " problems.\n");
  return tCopy;
}


//  Calculate loads known to hub (along with message transit counts).
//  Result is left in the clusterLoad and clusterTransit members.

void parallelBranching::hubCalculateLoads()
{
  HUBDEBUG(100,ucout << "hubCalculateLoads()\n");

  clusterLoad = hubPool->updatedLoad(); 
  HUBDEBUG(150,clusterLoad.dump(ucout,"clusterLoad = hubPool->updatedLoad()"));

#ifdef MEMUTIL_PRESENT
  clusterLoad.setMemory();
#endif
  if (iAmWorker())
    workerLoadEstimate[0] = updatedPLoad();
  for (int w=0; w<numWorkers(); w++)
    clusterLoad += workerLoadEstimate[w];
  HUBDEBUG(150,clusterLoad.dump(ucout,"clusterLoad after "
	   "adding workerEstimates"));
  if (numHubs() == 1)
    {
      clusterLoad.senseBusy();
      globalLoad = clusterLoad;
    }
  else 
    clusterLoad.senseClusterBusy();
  HUBDEBUG(100,ucout << "Cluster load is " << clusterLoad << ".\n");
  HUBDEBUG(160,clusterLoad.dump(ucout,"clusterLoad"));
  if ((numHubs() == 1) || hubsPrintStatus)
    statusPrint(hubLastPrint,hubLastPrintTime,clusterLoad,"h");
}


//  Inform worker processors of current load situation.  

void parallelBranching::tellWorkersLoads()
{
  if (numWorkers() == iAmWorker())
    return;
  if (wantLoadInformCount == 0)
    return;
  HUBDEBUG(100,ucout << "Informing workers of loads, cluster = "
	   << clusterLoad << ", global = " << globalLoad  << ".\n");
  hubDispatchCount = 0;
  HUBDEBUG(100,ucout << "Informing " << wantLoadInformCount << " workers:");
  for(int i=0; i<wantLoadInformCount; i++)
    {
      int w = workerToInform[i];
      PackBuffer* outBuffer = hubAuxBufferQ.getFree();
      *outBuffer << (int) loadInfoSignal;
      *outBuffer << clusterLoad;
      *outBuffer << globalLoad;
      *outBuffer << workerRebalanceCount[w];
      hubAuxBufferQ.send(outBuffer,workerProc(w),workerTag);
      HUBDEBUG(100,ucout << ' ' << workerProc(w) << '/' << w);
      workerReportCount[w] = 0;
      wantLoadInform[w]    = false;
    }
  wantLoadInformCount = 0;
  HUBDEBUG(100,ucout << endl);
}


//  Code to support load balancing.

//  This decides which clusters are available for load balancing, either
//  as donors or recievers.

void parallelBranching::decideLoadBalAvailability(loadBalPair& eligible)
{
  double idealLoad = globalLoad.loadMeasure()*myWorkerFrac();
  double cLoad     = clusterLoad.loadMeasure();
  DEBUGPRX(150,loadBalancer,"Global load is " << globalLoad 
	   << ", ideal load is " << idealLoad 
	   << ", cluster load is " << cLoad << ".\n");
  eligible.reset();
  if (cLoad >= idealLoad*loadBalDonorFac)
    {
      DEBUGPRX(150,loadBalancer,"Load high enough to be a donor, "
	       << hubPool->load().count() << " problems available.\n");
      if (hubPool->load().count() >= loadBalMinSourceCount)
	{
	  eligible.donors++;
	  DEBUGPRX(150,loadBalancer,"Eligible donor.\n");
	}
    }
  else if (cLoad <= idealLoad*loadBalReceiverFac)
    {
      eligible.receivers++;
      DEBUGPRX(150,loadBalancer,"Eligible receiver.\n");
    }
}


// This fills a buffer with problems to be load balanced.

void parallelBranching::fillLoadBalBuffer(PackBuffer& buffer,
					  loadObject  receiverLoad,
					  int         receiver)
{
  double ratio = workerFrac(whichCluster(receiver))/myWorkerFrac();
  DEBUGPRX(150,loadBalancer,"Filling load balance buffer for ["
	   << receiver << "], ratio = " << ratio << '\n');

  spToken* t = hubPool->firstToUnload(); 
  loadObject donorLoad = clusterLoad;
  int i = 0;
  buffer.reset();

  while ((t) && 
	 (i < maxLoadBalSize) && 
	 (hubPool->load().count() >= loadBalMinSourceCount))
    {
      receiverLoad += *t;
      donorLoad    -= *t;
      if (receiverLoad.loadMeasure() > ratio*donorLoad.loadMeasure())
	{
	  t = 0;
	  break;
	}
      DEBUGPRX(160,loadBalancer,"Packed " << t << '\n');
      buffer << 1;
      t->pack(buffer);
      i++;
      hubPool->kill(t); 
      t = hubPool->nextToUnload();
    }
  buffer << 0;
  DEBUGPRX(50,loadBalancer,"Load balance buffer filled with " << i 
	   << " subproblems.\n");
  loadBalSPCount += i;
}


// This unloads the same message on the other end.

void parallelBranching::unloadLoadBalBuffer(UnPackBuffer& buffer)
{
  int flag;
  int i = 0;
  buffer >> flag;
  while(flag)
    {
      MEMDEBUG_START_NEW("spToken")
      spToken* t = new spToken(this,buffer);
      MEMDEBUG_END_NEW("spToken")
      hubPool->insert(t);
      DEBUGPRX(160,loadBalancer,"Unpacked " << t<< '\n');
      buffer >> flag;
      i++;
    }
  DEBUGPRX(50,loadBalancer,"Load balance buffer yielded " << i 
	   << " subproblems.\n");
  if (i > 0)
    activateHub(loadBalancer);
}


void parallelBranching::alertWorkers(int code)
{
  DEBUGPR(100,ucout << "Called alertWorkers(" << code << ")\n");
  for(int w=iAmWorker(); w<numWorkers(); w++)
    {
      PackBuffer* outBuffer = hubAuxBufferQ.getFree();
      *outBuffer << code;
      hubAuxBufferQ.send(outBuffer,workerProc(w),workerTag);
    }
};


//  This method applies only in the case of single hub.  For multiple
//  hubs, the job is done by the load balancer instead.  Returns true
//  if it looks like we're ready to write a checkpoint or terminate,
//  and we need to double check message counts.

bool parallelBranching::needClusterCheck()
{
  if (numHubs() > 1)
    return false;
  if (suspending())
    return !(clusterLoad.cpBusy());
  return !(clusterLoad.busy());
}


void parallelBranching::clusterTerminate()
{
  alertWorkers(terminateSignal);
  Scheduler::termination_flag = true;
  if (aborting)
    {
      if (clusterNumber() == 0)
	printAbortStatistics(globalLoad);
      clearAllSPsForAbort();
    }
}


void parallelBranching::setToInform(int w)
{
  if (!wantLoadInform[w])
    {
      wantLoadInform[w] = true;
      workerToInform[wantLoadInformCount++] = w;
      HUBDEBUG(100,ucout << "Set to inform worker " << workerProc(w)
	       << '/' << w << " of loads.\n");
    }
}


void parallelBranching::setToInformAll()
{
  HUBDEBUG(100,ucout << "Informing all workers of loads.\n");
  for (int w = iAmWorker(); w < numWorkers(); w++)
    setToInform(w);
}

} // namespace pebbl

#endif
