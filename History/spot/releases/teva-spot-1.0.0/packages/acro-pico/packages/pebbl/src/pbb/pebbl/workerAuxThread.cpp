/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// workerAuxThread.cpp
//
// Auxiliary thread object used by the worker in PEBBL, when there is
// no hub present on the same processor.  
//
// If this thread is activated, it is the only way to terminate the
// parallel search on this processor.
//
// Jonathan Eckstein
//

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <pebbl/parBranching.h>

using namespace std;

namespace pebbl {



// Constructor.

workerAuxObj::workerAuxObj(parallelBranching* global_) :
messageTriggeredPBThread(global_,
			 "Worker Auxiliary",
			 "Worker Aux",
			 "aquamarine",
			 3,210,
			 computeBufferSize(global_),
			 global_->workerTag,
			 global_->myHub())
{ }


//  Routine to size input buffer.

int workerAuxObj::computeBufferSize(parallelBranching* global_)
{
  return 2*(sizeof(int) + parLoadObject::packSize(global_));
}


//  Main message-handling method.

ThreadObj::RunStatus workerAuxObj::handleMessage(double* controlParam)
{
  int signal;
  inBuf >> signal;

  DEBUGPRP(150,ucout << "Worker received message containing code "
	             << signal << ": ");

  switch (signal)
    {
    case terminateSignal:

      DEBUGPR(150,ucout << "termination signal.\n");
      Scheduler::termination_flag = true;
      if (global->aborting)
	global->clearAllSPsForAbort();
      break;
      
    case loadInfoSignal:

      DEBUGPR(150,ucout << "load information.\n");
      global->getLoadInfoFromHub(inBuf);
      break;
      
    case terminateCheckSignal:

      terminateCheckValue = global->messages.nonLocalScatter.sent +
	global->messages.general.sent;
      DEBUGPR(150,ucout << "termination check query, " 
	      << terminateCheckValue << " messages.\n");
      uMPI::isend(&terminateCheckValue,
		  1,
		  MPI_INT,
		  global->myHub(),
		  global->termCheckTag);
      break;
      
    case startCheckpointSignal:

      DEBUGPR(150,ucout << "start checkpoint signal.\n");
      global->setupCheckpoint();
      global->workerCommunicateWithHub();
      break;
      
    case writeCheckpointSignal:

      DEBUGPR(150,ucout << "write checkpoint signal.\n");
      global->writeCheckpoint();
      break;
      
    case startAbortSignal:

      DEBUGPR(150,ucout << "start abort signal.\n");
      if (global->abortDebug > 0)
	{
	  ucout << "Abort signalled!\n";
	  global->setDebugLevelWithThreads(global->abortDebug);
	}
      global->setupAbort();
      global->workerCommunicateWithHub();
      break;
      
    default:

    EXCEPTION_MNGR(runtime_error, 
		   "Worker auxiliary thread received unknown signal " 
		   << signal);
    }
  
  return RunOK;

};

} // namespace pebbl

#endif
