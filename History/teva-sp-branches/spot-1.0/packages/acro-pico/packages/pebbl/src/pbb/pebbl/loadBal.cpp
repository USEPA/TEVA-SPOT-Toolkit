/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// loadBal.cpp
//
// Principle code for PEBBL to have multiple hubs and load balance between
// them.
//
// Jonathan Eckstein
//

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <utilib/seconds.h>
#include <pebbl/parBranching.h>

using namespace std;

namespace pebbl {



// Code for coTree that surveys and broadcasts load information

loadBalSurvey::loadBalSurvey(coTreeReadyPBThread* thread_,
               treeTopology*        treeP_) :
  parBranchingCoTree(MPI_PACKED,
                     thread_,
                     treeP_),
   bufSize(parLoadObject::packSize(thread_->global) + 2*sizeof(int)),
   load(thread_->global),
   inBuf(bufSize),
   outBuf(bufSize),
   lastPrint(0),
   lastPrintTime(0)
{ }


void loadBalSurvey::initAction()
{
  load.resetAndUpdate();
  DEBUGPR(160,load.dump(ucout,"Initial survey load"));
}

void loadBalSurvey::upMessageAction()
{
  parLoadObject childLoad(global);
  inBuf.reset(thread->statusP());
  inBuf >> childLoad;
  DEBUGPR(160,childLoad.dump(ucout,"reported by child"));
  load += childLoad;
  DEBUGPR(160,load.dump(ucout,"Resulting survey load"));
}

void loadBalSurvey::upRelayAction()
{
  global->hubCalculateLoads();
  DEBUGPR(160,global->clusterLoad.dump(ucout,"clusterLoad"));
  load += global->clusterLoad;
  DEBUGPR(160,load.dump(ucout,"for subtree"));
  outBuf.reset();
  if (!(t->isRoot()))
    outBuf << load;
}

void loadBalSurvey::rootAction()
{
  load.senseBusy();
  DEBUGPR(160,load.dump(ucout,"calculated at root"));
  global->globalLoad = load;
  global->statusPrint(lastPrint,lastPrintTime,load,"g");
  if (global->shouldAbort(load.boundedSPs))
    global->setupAbort();
  else if ((load.count() > 0) && global->checkpointDue())
    global->setupCheckpoint();
  outBuf << load << global->aborting;
  if (global->checkpointsEnabled)
    outBuf << global->checkpointing;
}

void loadBalSurvey::downReceiveAction()
{
  inBuf.reset(thread->statusP());
  bool abDown = false;
  inBuf >> global->globalLoad >> abDown;
  DEBUGPR(160,global->globalLoad.dump(ucout,"globalLoad"));
  if (abDown && !(global->aborting))
    global->setupAbort();
  if (global->checkpointsEnabled)
    {
      bool cpDown = false;
      inBuf >> cpDown;
      DEBUGPR(160,ucout << "cpDown=" << cpDown << endl);
      if (cpDown && !(global->checkpointing))
	global->setupCheckpoint();
    }
  if (t->numChildren() > 0)
    {
      outBuf.reset();
      outBuf << global->globalLoad << global->aborting;
      if (global->checkpointsEnabled)
	outBuf << global->checkpointing;
    }
}


// Code for coTree that counts donors/receivers and numbers them.

// Constructor.

loadBalCount::loadBalCount(loadBalObj*   balThread_,
			   treeTopology* treeP_) :
  parBranchingCoTree(MPI_INT,
		     balThread_,
		     treeP_),
  childCount(treeP_->numChildren()),
  balThread(balThread_),
  upBuf(2),
  downBuf(4),
  childBuf(treeP_->numChildren(),4)
{ }


// coTree methods.

void loadBalCount::initAction()
{
  clusterCount.reset();
}

void loadBalCount::upMessageAction()
{
  int childNum = t->whichChild(status->MPI_SOURCE);
  loadBalPair receivedValues;
  receivedValues.donors    = upBuf[0];
  receivedValues.receivers = upBuf[1];
  DEBUGPR(150,ucout << "Child [" << status->MPI_SOURCE << "] reports "
	  << receivedValues << endl);
  childCount[childNum] = receivedValues;
  clusterCount += receivedValues;
}

void loadBalCount::upRelayAction()
{
  clusterCount += balThread->eligible;
  DEBUGPR(150,ucout << "Up Relay Subtree: " << clusterCount << endl);
  upBuf[0] = clusterCount.donors;
  upBuf[1] = clusterCount.receivers;
};

void loadBalCount::rootAction()
{
  balThread->total = clusterCount;
  subTreeCount     = clusterCount;
  DEBUGPR(150,ucout << "Root Total: " << balThread->total << endl);
  DEBUGPR(150,ucout << "Root Subtree: " << subTreeCount << endl);
}

void loadBalCount::downReceiveAction()
{
  balThread->total.donors    = downBuf[0];
  balThread->total.receivers = downBuf[1];
  subTreeCount.donors        = downBuf[2];
  subTreeCount.receivers     = downBuf[3];
  DEBUGPR(150,ucout << "Down Recv Total: " << balThread->total << endl);
  DEBUGPR(150,ucout << "Down Recv Subtree: " << subTreeCount.donors << endl);
}

void loadBalCount::downRelayAction()
{
  balThread->myID = subTreeCount;
  subTreeCount -= balThread->eligible;
}

void loadBalCount::downRelayLoopAction(int i)
{
  childBuf[i][0] = balThread->total.donors;
  childBuf[i][1] = balThread->total.receivers;
  childBuf[i][2] = subTreeCount.donors;
  childBuf[i][3] = subTreeCount.receivers;

  subTreeCount -= childCount[i];
}


//  Now the load balancing thread itself.

//  Construction.

loadBalObj::loadBalObj(parallelBranching* global_) :

coTreeReadyPBThread(global_,
		    (char*)((global_->numHubs() == 1) ?
		     "Termination Check"     :
		     "Load Balance/Termination"),
		    (char*)((global_->numHubs() == 1) ?
		     "Term Check"            :
		     "Load Bal"),
		    "blue",
		    2,150),

tree(global_->cluster, global_->loadBalTreeRadix),
synchObject(this,&tree),
surveyObject(this,&tree),
countObject(this,&tree),
termCheckObject(this,&tree),
receiverLoad(global_),
lbRandom(1),
bufferSize(computeBufferSize()),
inBuf(bufferSize)
{
  myState        = start;
  myCluster      = global->clusterNumber();
  roundNumber    = 0;
  surveyRestarts = 0;
  if (global->iAmWorker())
    maxCPUFrac = global_->loadBalCPUFrac;
  else
    maxCPUFrac = global_->loadBalCPUFracPure;
  outBufQ.reset(1,bufferSize);
}


//  Auxiliary method used in construction.  Figure out the largest 
//  Possible input buffer we'll need.

int loadBalObj::computeBufferSize()
{
  PackBuffer junkBuf(256);
  parLoadObject::packEmpty(junkBuf,global);
  junkBuf << 1;
  int val1 = junkBuf.size();
  junkBuf.reset();

  junkBuf << 1;
  spToken::packEmpty(junkBuf);
  junkBuf << 1;
  spToken::packEmpty(junkBuf);
  junkBuf << 0;
  int val2 = (int) (junkBuf.size()*(global->maxLoadBalSize/2 + 1));

  return std::max(val1,val2);
}


// Setting the debug level.

void loadBalObj::setDebugLevel(int level)
{
  if (global->loadBalDebug == -1)
    setDebug(level);
  else
    setDebug(global->loadBalDebug);
  synchObject.setDebug(debug);
  surveyObject.setDebug(debug);
  countObject.setDebug(debug);
  termCheckObject.setDebug(debug);
}


//  Decide if we need to block or proceed, when starting out a round
//  of load balancing.

bool loadBalObj::canStart()
{
  if (!global->clusterLoad.busy())
    {
      DEBUGPR(250,ucout << "Can start load balancing (idle).\n");
      return true;
    }

  if (global->suspending())
    {
      DEBUGPR(250,ucout << "Can start load balancing "
	      "(checkpoint or abort).\n");
      return true;
    }
  
  double aggCPU = CPUSeconds() - global->baseTime;
  if (run_time <= maxCPUFrac*aggCPU)
    {
      DEBUGPR(1000,ucout << "Can start load balancing.\n");
      return true;
    }
  DEBUGPR(1000,ucout << "Cannot start load balancing: aggCPU="
	  << aggCPU << ", run_time="<< run_time << ", ratio="
	  << run_time/aggCPU << "\n");
  return false;
}


//  State computation.

ThreadObj::ThreadState loadBalObj::state()
{
  if (state_flag == ThreadBlocked)
    if (canStart())
      state_flag = ThreadReady;
  DEBUGPR(1000,ucout << "Load balancer state query: " <<
	  (state_flag == ThreadReady ? "ready" :
	   (state_flag == ThreadBlocked ? "blocked" :
	    (state_flag == ThreadWaiting ? "waiting" : "?"))) << '\n');
  return state_flag;
};


//  Common computation of how many subproblems we've looked at.

int loadBalObj::subproblemsProcessed()
{
  return global->subCount[bounded] + global->subCount[dead];
};


//  Common code for setting up a receive and then putting the thread
//  in a wait state.

void loadBalObj::waitToReceive(MessageID& tag_)
{
  DEBUGPR(300,ucout << "Load balancer about to wait on tag " 
	  << (int) tag_ << ".\n");
  uMPI::irecv((void*) inBuf.buf(),
	      bufferSize,
	      MPI_PACKED,
	      MPI_ANY_SOURCE,
	      tag_,
	      &request);
  tag = tag_;
  state_flag = ThreadWaiting;
}


//  Now the big kahouna -- the state machine for the load balancer.

ThreadObj::RunStatus loadBalObj::runWithinLogging(double* controlParam)
{
  int canContinue;

#define jumpState(newState) { myState = newState; canContinue = TRUE; break; }

  DEBUGPR(150,ucout << "Load balancer running at time=" 
	  << MPI_Wtime() - global->baseTime 
	  << " checkpointing=" << global->checkpointing 
	  << " aborting=" << global->aborting << endl);

  do 

    {

      canContinue = false;

      switch(myState) 

	{

	case start:      // Start out here, but maybe block.

	  roundNumber++;

	  DEBUGPR(150,ucout << "Load balancer <start> state, round "
		  << roundNumber << ".\n");

	  if (global->numHubs() == 1)
	    jumpState(termCheckCluster);

	  myState = blockPoint;
	  if (!canStart())
	    {
	      state_flag = ThreadBlocked;
	      break;
	    }

	case blockPoint:   // Come here once it is time to start.

	  DEBUGPR(150,ucout << "Load balancer <blockPoint> state, round "
		  << roundNumber << ".\n");

	  myState = synchronizing;

	case synchronizing:

	  DEBUGPR(150,ucout << "Load balancer <synchronizing> state, round "
		  << roundNumber << ".\n");

	  if (synchObject.run())    // Do this repeatedly until synchObject
	    break;                  // is done.
	  
	  synchObject.reset();
	  myState = surveying;

	case surveying:             // Figure out loads.

	  DEBUGPR(150,ucout << "Load balancer <surveying> state, round "
		  << roundNumber << ".\n");

	  if (surveyObject.run())
	    break;
	  
	  surveyObject.reset();

 	  if (global->suspending())
 	    {
 	      if (global->globalLoad.cpBusy())
		{
		  jumpState(surveying);
		}
	      else
 		{
 		  setUpClusterCheck(global->globalLoad.messages);
 		  jumpState(termCheckCluster);
 		}		
 	    }

	  if (!global->globalLoad.busy())
	    {
	      setUpClusterCheck(global->globalLoad.messages);
	      jumpState(termCheckCluster);
	    }		

	  if (global->globalLoad.mismatch())
	    {
	      DEBUGPR(150,ucout << "Survey restart\n");
	      surveyRestarts++;
	      jumpState(start);
	    }

	  global->decideLoadBalAvailability(eligible);
	  DEBUGPR(50,ucout << "Eligible: " << eligible << '\n');

	  myState = counting;
	  
	case counting:

	  DEBUGPR(150,ucout << "Load balancer <counting> state, round "
		  << roundNumber << ".\n");

	  if (countObject.run())
	    break;

	  countObject.reset();

	  DEBUGPR(50,ucout << "LB Total: " << total << '\n');
	  DEBUGPR(50,ucout << "myID: " << myID << '\n');

	  lbPairs = std::min(total.donors,total.receivers);
	  DEBUGPR(50,ucout << lbPairs 
		  << " possible load balancing pairs.\n");
	  if (lbPairs == 0)
	    jumpState(start);

	  {
	    int offset     = lbRandom.asLong() % total.donors;
	    DEBUGPR(200,ucout << "offset=" << offset << '\n');
	    myID.donors    = (myID.donors + offset) % total.donors;
	    offset         = lbRandom.asLong() % total.receivers;
	    DEBUGPR(200,ucout << "offset=" << offset << '\n');
	    myID.receivers = (myID.receivers + offset) % total.receivers;
	    DEBUGPR(50,ucout << "myID: " << myID << '\n');
	  }

	  iAmDonor    = eligible.donors    && (myID.donors    < lbPairs);
	  iAmReceiver = eligible.receivers && (myID.receivers < lbPairs);
	  iAmRVPoint  = myCluster < lbPairs;

#ifdef ACRO_VALIDATING
	  if (iAmDonor)
	    DEBUGPR(50,ucout << "I am a donor.\n");
	  if (iAmReceiver)
	    DEBUGPR(50,ucout << "I am a receiver.\n");
	  if (iAmRVPoint)
	    DEBUGPR(150,ucout << "I am a rendezvous point.\n");
#endif

	  donorProc    = unknown;
	  receiverProc = unknown;
	  myReceiver   = unknown;

	  if (!iAmDonor)
	    jumpState(receiverInfo);

	  // Donor processors must now send their address to the rendezvous
	  // point.  If that is the same processor we're on, it's trivial.

	  if (myID.donors == myCluster)
	    {
	      donorProc = uMPI::rank;
	      DEBUGPR(150,ucout << "Donor is rendezvous point.\n");
	    }
	  else
	    {
	      DEBUGPR(150,ucout << "Sending null message to ["
		      << global->hubProc(myID.donors) << "].\n");
	      uMPI::isend((void*) &myID,      // This address does not matter
			  0,                  // Envelope-only message!
			  MPI_PACKED,
			  global->hubProc(myID.donors),
			  donorRVTag);
	    }
	  myState = receiverInfo;

	case receiverInfo:

	  DEBUGPR(150,ucout << "Load balancer <receiverInfo> state, round "
		  << roundNumber << ".\n");

	  if (!iAmReceiver)
	    jumpState(rendezvous);

	  // Receiver processors now send there addressed to the rendezvous
	  // point too.  Again, this might be trivial.

	  if (myID.receivers == myCluster)
	    {
	      receiverProc = uMPI::rank;
	      receiverLoad = global->clusterLoad;
	      DEBUGPR(150,ucout << "Receiver is rendezvous point.\n");
	    }
	  else
	    {
	      PackBuffer* outBufP = outBufQ.getFree();
	      *outBufP << global->clusterLoad;
	      outBufQ.send(outBufP,
			   global->hubProc(myID.receivers),
			   receiverRVTag);
	      DEBUGPR(150,ucout << "Send load " << 
		      global->clusterLoad << " to ["
		      << global->hubProc(myID.receivers) << "].\n");
	    }
	  myState = rendezvous;

	case rendezvous:

	  DEBUGPR(150,ucout << "Load balancer <rendezvous> state, round "
		  << roundNumber << ".\n");

	  if (!iAmRVPoint)
	    jumpState(rendezvousDone);

	  // At this point, we know we are a rendezvous point.
	  // First make sure the information from the donor is here.

	  if (donorProc != unknown)
	    jumpState(knowDonor);

	  waitToReceive(donorRVTag);
	  myState = donorWait;
	  break;

	case donorWait:      // Come back here when donor message arrives.

	  DEBUGPR(150,ucout << "Load balancer <donorWait> state, round "
		  << roundNumber << ".\n");

	  messagesReceived++;
	  donorProc = status.MPI_SOURCE;
	  DEBUGPR(150,ucout << "Donor is [" << donorProc <<"].\n");
	  myState = knowDonor;

	case knowDonor:         //  Now we do much the same for the receiver.

	  DEBUGPR(150,ucout << "Load balancer <knowDonor> state, round "
		  << roundNumber << ".\n");

	  if (receiverProc != unknown)
	    jumpState(knowReceiver);

	  waitToReceive(receiverRVTag);
	  myState = receiverWait;
	  break;

	case receiverWait:  // Come back here when receiver message arrives.

	  DEBUGPR(150,ucout << "Load balancer <receiverWait> state, round "
		  << roundNumber << ".\n");

	  messagesReceived++;
	  receiverProc = status.MPI_SOURCE;
	  inBuf.reset(&status);
	  inBuf >> receiverLoad;
	  DEBUGPR(150,ucout << "Receiver is [" << receiverProc 
		  << "], with load " << receiverLoad << ".\n");
	  myState = knowReceiver;
	                                 // Now we know donor and receiver.
	case knowReceiver:               // Get info to the donor, if it's   
                                         // a different processor.

	  DEBUGPR(150,ucout << "Load balancer <knowReceiver> state, round "
		  << roundNumber << ".\n");

	  if (donorProc == uMPI::rank)
	    {
	      myReceiver = receiverProc;
	      DEBUGPR(150,ucout << "Rendezvous point is also donor.\n");
	    }
	  else
	    {
	      PackBuffer* outBufP = outBufQ.getFree();
	      *outBufP << receiverLoad;
	      *outBufP << receiverProc;
	      DEBUGPR(150,ucout << "Forwarding information to [" 
		      << donorProc << "].\n");
	      outBufQ.send(outBufP,donorProc,returnRVTag);
	    }
	  myState = rendezvousDone;
	  
	case rendezvousDone:

	  DEBUGPR(150,ucout << "Load balancer <rendezvousDone> state, round "
		  << roundNumber << ".\n");

	  if (iAmDonor)
	    {
	      if (myReceiver != unknown)
		jumpState(donorKnowsReceiver);

	      waitToReceive(returnRVTag);
	      myState = returnWait;
	      break;
	    }
	  else if (iAmReceiver)
	    {
	      waitToReceive(loadBalTag);
	      myState = workWait;
	      break;
	    }
	  else
	    jumpState(start);

	case returnWait:                 // Come here when message from
	                                 // the rendezvous processor arrives.

	  DEBUGPR(150,ucout << "Load balancer <returnWait> state, round "
		  << roundNumber << ".\n");

	  messagesReceived++;
	  inBuf.reset(&status);
	  inBuf >> receiverLoad;
	  inBuf >> myReceiver;
	  DEBUGPR(150,ucout << "Receiver is [" << myReceiver 
		  << "], with load " << receiverLoad << ".\n");
	  myState = donorKnowsReceiver;
	  
	case donorKnowsReceiver:         // Now start the actual transfer.
	  
	  DEBUGPR(150,ucout << 
		  "Load balancer <donorKnowsReceiver> state, round "
		  << roundNumber << ".\n");
	  
	  {
	    PackBuffer* outBufP = outBufQ.getFree();
	    global->fillLoadBalBuffer(*outBufP,receiverLoad,myReceiver);
	    outBufQ.send(outBufP,myReceiver,loadBalTag);
	  }
	  
	  DEBUGPR(150,ucout << "Work sent to [" << myReceiver << "].\n");
	  jumpState(start);

	case workWait:                   // Come here when work arrives.

	  DEBUGPR(150,ucout << "Load balancer <workWait> state, round "
		  << roundNumber << ".\n");

	  messagesReceived++;
	  inBuf.reset(&status);
	  global->unloadLoadBalBuffer(inBuf);
	  jumpState(start);

	case termCheckCluster:

	  DEBUGPR(150,ucout << "Load balancer <termCheckCluster> state,"
		  << " round " << roundNumber << ".\n");

	  DEBUGPR(160,ucout << "workersReplied = " << workersReplied << endl);

	  if (workersReplied == global->numWorkers())
	    jumpState(termCheckTree);
	  tag = global->termCheckTag;
	  uMPI::irecv((void *) &workerCount,
		      1,
		      MPI_INT,
		      MPI_ANY_SOURCE,
		      tag,
		      &request);
	  state_flag = ThreadWaiting;
	  myState = termCheckClusterWait;
	  break;

	case termCheckClusterWait:

	  DEBUGPR(150,ucout << "Load balancer <termCheckClusterWait> state,"
		  << " round " << roundNumber << ".\n");

	  messagesReceived++;
	  termCheckObject.value += workerCount;
	  DEBUGPR(150,ucout << "workerCount = " << workerCount 
		  << ", sum = " << termCheckObject.value << '\n');
	  workersReplied++;
	  jumpState(termCheckCluster);

	case termCheckTree:

	  DEBUGPR(150,ucout << "Load balancer <termCheckTree> state,"
		  << " round " << roundNumber << ".\n");

	  if (termCheckObject.run())
	    break;

	  if(termCheckObject.value == termCheckTarget)
	    {
	      global->termCheckInProgress = false;
	      if (global->checkpointing)
		{
		  global->writeCheckpoint();
		  jumpState(start);
		}
	      DEBUGPR(150,ucout << "Termination confirmed.\n");
	      global->clusterTerminate();
	      jumpState(dead);
	    }
	      
	  DEBUGPR(150,ucout << "Termination disproved: target = "
		  << termCheckTarget << ", sum = " 
		  << termCheckObject.value << ".\n");

	  if (global->numHubs() == 1)
	    {
	      if (global->wantAnotherTermCheck)
		{
		  global->wantAnotherTermCheck = false;
		  setUpClusterCheck(global->clusterLoad.messages);
		}
	      else
		global->termCheckInProgress = false;
	      if (global->suspending())
		jumpState(termCheckCluster);
	    }

	  jumpState(surveying);

	case dead:

	  break;

	}

    }

  while(canContinue);

  DEBUGPR(200,ucout << "LB messages: "
	  << messagesReceived                 << " general + "
	  << synchObject.messagesReceived     << " synch + "
	  << surveyObject.messagesReceived    << " survey + "
	  << countObject.messagesReceived     << " count + "
	  << termCheckObject.messagesReceived << " term = "
	  << messageCount() << '\n');

  return RunOK;

}


// Called from load balance (multiple hubs) or activateHub (one hub)

void loadBalObj::setUpClusterCheck(pbMessageBlock& targetBlock)
{
  global->alertWorkers(terminateCheckSignal);
  workersReplied = global->iAmWorker();
  termCheckObject.reset();
  termCheckTarget = targetBlock.general.received + 
                    targetBlock.nonLocalScatter.received;
  termCheckObject.value = global->messages.general.sent +
                          global->messages.nonLocalScatter.sent;
  DEBUGPR(150,ucout << "Cluster check:  Target sum = " << termCheckTarget
	  << ", my value = " << termCheckObject.value  
	  << ", workersReplied = " << workersReplied << '\n');
}

} // namespace pebbl

#endif
