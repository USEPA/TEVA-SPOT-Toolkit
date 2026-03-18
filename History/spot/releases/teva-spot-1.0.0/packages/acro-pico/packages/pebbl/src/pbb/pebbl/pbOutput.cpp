/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// pbOutput.cpp
//
// Parallel branching class code for PEBBL -- code dealing with output to
// stdout, log files, and so forth.
//
// Jonathan Eckstein
//


#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <utilib/CharString.h>
#include <utilib/mpiUtil.h>
#include <utilib/nicePrint.h>
#include <pebbl/parBranching.h>

using namespace std;
using namespace utilib;

namespace pebbl {


void parallelBranching::printSolValue(std::ostream& stream)
{
  if (uMPI::iDoIO)
    {
      int taggingWasActive = CommonIO::tagging_active();
      if (taggingWasActive)
	CommonIO::end_tagging();
      serialPrintSolValue(stream);
      if (taggingWasActive)
	CommonIO::begin_tagging();
    }
}


void parallelBranching::printSolution(const char* header,
				      const char* footer,
				      ostream& outStream)
{
  DEBUGPR(120,ucout << "parallelBranching::printSolution(): "
	  << "rank = " << uMPI::rank 
	  << ", incumbentValue=" << incumbentValue 
	  << ", incumbentSource = " << incumbentSource << "\n");

  // MPI_ANY_SOURCE means there is no (integer) feasible solution.
  // For now the serial code will have to recognize this case

  // Do we have to move the solution in order to print it?

  if (printSolutionSynch &&
      (incumbentSource != uMPI::ioProc) &&
      (incumbentSource != MPI_ANY_SOURCE))
    {
      // Yes, we have to move the solution
      // Either send or receive it

      if (uMPI::rank == incumbentSource)
	{
	  PackBuffer solBuf;
	  packSolution(solBuf);
	  uMPI::send((void *) solBuf.buf(),
		     solBuf.size(),
		     MPI_PACKED,
		     uMPI::ioProc,
		     printSolutionTag);
	}
      else if (uMPI::rank == uMPI::ioProc)
	{
	  MPI_Status status;
	  UnPackBuffer solBuf(solutionBufferSize());
	  uMPI::recv((void *) solBuf.buf(),
		     solBuf.size(),
		     MPI_PACKED,
		     incumbentSource,
		     printSolutionTag,
		     &status);
	  solBuf.reset(&status);
	  unpackSolution(solBuf);
	}
    }
  
  // Now print the solution 

  if (uMPI::rank == (printSolutionSynch ? uMPI::ioProc : incumbentSource))
    {
      int taggingWasActive = CommonIO::tagging_active();
      if (taggingWasActive)
	CommonIO::end_tagging();
      serialPrintSolution(header,footer,outStream);
      if (taggingWasActive)
	CommonIO::begin_tagging();
    }

  DEBUGPR(1,ucout << '\n');

}


void parallelBranching::solutionToFile()
{
  // If we already put the solution out, return on all processors.

  int flag = FALSE;
  if (iAmFirstHub() &&
      (earlyOutputMinutes > 0) && 
      (lastSolValOutput == incumbentValue))
    flag = TRUE;
  uMPI::broadcast((void*) &flag,1,MPI_INT,firstHub());
  if (flag)
    return;

  // Otherwise, set up a stream and print to it.

  ostream* outStreamP = 0;
  if ((printSolutionSynch && uMPI::iDoIO) ||
      (!printSolutionSynch && uMPI::rank == incumbentSource))
    outStreamP = openSolutionFile();
  printSolution("","",*outStreamP);
  if (outStreamP)
    closeSolutionFile(outStreamP);
}


void parallelBranching::statusLine(loadObject& l, const char* tag) 
{
  LOG_EVENT(3,start,statusPrintLogState);
  branching::statusLine(l,tag);
  ucout << Flush;
  LOG_EVENT(3,end,statusPrintLogState);
}


//  Validation log stuff

ostream* parallelBranching::valLogFile()
{
  if (validateLog)
    {
      char name[32];
      sprintf(name,"val%05d.log",uMPI::rank);
      return new ofstream(name,restarted ? ios::app : ios::out);
    }
  else
    return 0;
}


void parallelBranchSub::valLogPackChildPrint()
{
  *vout << "packchild " << uMPI::rank << ' ' << bGlobal()->probCounter
    << ' ' << bound << ' ';
  valLogWriteID();
  valLogPackChildExtra();
  *vout << endl;
}


void parallelBranchSub::valLogPackPrint()
{
  *vout << "pack ";
  valLogWriteID();
  valLogPackExtra();
  *vout << endl;
}


void parallelBranchSub::valLogUnpackPrint()
{
  *vout << "unpack ";
  valLogWriteID(' ');
  *vout << childrenLeft;
  valLogUnpackExtra();
  *vout << endl;
}


//  Printout of processor configuration.

void parallelBranching::printConfiguration(ostream& stream)
{
  if (!uMPI::iDoIO)
    return;
  CommonIO::end_tagging();
  stream << "\nPEBBL Configuration:\n";
  hyphens(stream,19) << '\n';
  int pWidth = digitsNeededFor(uMPI::size);
  stream.width(pWidth);
  stream << numHubs() << " cluster" << plural(numHubs());
  int mod  = uMPI::size % cluster.typicalSize;
  if (mod)
    stream << ": " << numHubs() - 1;
  stream << " of size " << cluster.typicalSize;
  if (mod)
    stream << ", 1 of size " << mod;
  stream << '\n';
  stream.width(pWidth);
  stream << uMPI::size << " processor" << plural(uMPI::size) << '\n';
  int pureHubs    = uMPI::size - totalWorkers();
  int workerHubs  = numHubs() - pureHubs;
  int pureWorkers = totalWorkers() - workerHubs;
  configLine(stream,pWidth,pureWorkers,"pure worker");
  configLine(stream,pWidth,pureHubs,   "pure hub");
  configLine(stream,pWidth,workerHubs, "worker-hub");
  stream << "\nTarget timeslice: " << timeSlice << " second";
  if (timeSlice != 1)
    stream << 's';
  stream << ".\n\n";
  CommonIO::begin_tagging();
}


void parallelBranching::configLine(ostream& stream,
				   int      pWidth,
				   int      number,
				   const char*    kind)
{
  if (number == 0)
    return;
  int padding = 13 - strlen(kind);
  stream.width(pWidth);
  stream << number << ' ' << kind;
  stream.width(1);
  stream << plural(number);
  stream.width(padding);
  stream << "(";
  printPercent(stream,number,uMPI::size) << ")\n";
}


void parallelBranching::printSPStatistics(ostream& stream)
{
  CommonIO::end_tagging();
  int combinedTable[numStates];
  uMPI::reduce(subCount,combinedTable,numStates,MPI_INT,MPI_SUM,uMPI::ioProc);
  spTotal = combinedTable[boundable];
  int totalUnderHubControl = uMPI::sum(hubHandledCount);
  int totalDelivered       = uMPI::sum(spDeliverCount);
  int totalReleased        = uMPI::sum(spReleaseCount);
  int totalRebalanced      = uMPI::sum(rebalanceSPCount);
  int totalLoadBalanced    = 0;
  if (numHubs() > 1)
    totalLoadBalanced = uMPI::sum(loadBalSPCount);

  if (uMPI::iDoIO)
    {
      const char* hubControlString   = "Tokens at Hubs";
      const char* scatterString      = "Scattered to Hubs";
      const char* rebalanceString    = "Rebalanced to Local Hub";
      const char* nlScatterString    = "Scattered to Nonlocal Hubs";
      if (numHubs() == 1)
	{
	  hubControlString   = "Tokens at Hub";
	  scatterString      = "Scattered to Hub";
	  rebalanceString    = "Rebalanced to Hub";
	  nlScatterString    = "";
	}

      const char* rampBoundString = "Bounded during ramp up";
      const char* rampPoolString = "In pool at end of ramp up";

      if (restarted)
	{
	  rampBoundString = "";
	  rampPoolString  = "Loaded from checkpoint";
	}

      int fieldWidth = strlen(rebalanceString);
      fieldWidth = max(fieldWidth,(int) strlen(rampBoundString));
      fieldWidth = max(fieldWidth,(int) strlen(rampPoolString));
      fieldWidth = max(fieldWidth,(int) strlen(nlScatterString));

      int numWidth = digitsNeededFor(max(spTotal,rampUpPool));

      printSPTable(stream,combinedTable,fieldWidth);
      if (!restarted)
	printSPLine(stream,
		    rampUpBounds,
		    rampBoundString,
		    spTotal,
		    fieldWidth,
		    numWidth);
      printSPLine(stream,
		  rampUpPool,
		  rampPoolString,
		  spTotal,
		  fieldWidth,
		  numWidth);
      printSPLine(stream,
		  totalUnderHubControl,
		  hubControlString,
		  spTotal,
		  fieldWidth,
		  numWidth);
      printSPLine(stream,
		  totalReleased - totalRebalanced,
		  scatterString,
		  spTotal,
		  fieldWidth,
		  numWidth);
      printSPLine(stream,
		  totalRebalanced,
		  rebalanceString,
		  spTotal,
		  fieldWidth,
		  numWidth);
      printSPLine(stream,
		  globalLoad.messages.hubDispatch.sent,
		  "Dispatched from a Hub",
		  spTotal,
		  fieldWidth,
		  numWidth);
      printSPLine(stream,
		  totalDelivered,
		  "Moved between Workers",
		  spTotal,
		  fieldWidth,
		  numWidth);

      if (numHubs() > 1)
	{
	  printSPLine(stream,
		      globalLoad.messages.nonLocalScatter.sent,
		      "Scattered to Nonlocal Hubs",
		      spTotal,
		      fieldWidth,
		      numWidth);
	  printSPLine(stream,
		      totalLoadBalanced,
		      "Balanced between Hubs",
		      spTotal,
		      fieldWidth,
		      numWidth);
	}

      stream << '\n';
      CommonIO::begin_tagging();
    }
}



void parallelBranching::printTimings(ostream& stream)
{
  CommonIO::end_tagging();

  int oldPrecision = stream.precision(1);
  stream.setf(ios::fixed,ios::floatfield);

  timingPrintNameWidth = strlen("Thread/Function");
  if (uMPI::size > 1) 
    {
      int tmp = strlen("Problem Broadcast");
      timingPrintNameWidth = max(timingPrintNameWidth,tmp);
    }
  {
    int tmp = strlen(preprocessPhaseName());
    timingPrintNameWidth = max(timingPrintNameWidth, tmp);
  }

  string rampUpString = "Ramp-up";
  if (restarted)
    rampUpString = "Loading checkpoint";
  timingPrintNameWidth = max(timingPrintNameWidth,(int) rampUpString.size());

  messagesReceivedThisProcessor = preprocessMessages + rampUpMessages;
  if (uMPI::rank > 0)
    messagesReceivedThisProcessor += 2;

  parBranchingThreadObj* thread;
  double overhead = setupTime;
  ListItem<parBranchingThreadObj*> *l1;

  for(l1=threadsList.head(); l1; l1=threadsList.next(l1) )
    {
      thread = l1->data();
      overhead += thread->overheadTime();
      double hubTime = thread->hubTime;
      hub->adjustTime(hubTime);
      thread->adjustTime(-hubTime);
      if (uMPI::sum(thread->active) > 0)
	{
	  int l = strlen(thread->name);
	  if (l > timingPrintNameWidth)
	    timingPrintNameWidth = l;
	}
      messagesReceivedThisProcessor += thread->messageCount();      
    }

  double idleProcTime;
  double procThreadTime;
  sched.timing(searchTime,idleProcTime,procThreadTime);
  double schedProcTime = searchTime - procThreadTime - idleProcTime;
  if (schedProcTime < 0)
    schedProcTime = 0;
  searchTime += broadcastTime + preprocessTime + setupTime + rampUpTime;

  totalCPU = uMPI::sum(searchTime);
  maxCPU   = uMPI::max(searchTime);

  totalMessages = uMPI::sum(messagesReceivedThisProcessor);

  timingPrintTimeWidth    = max(7,digitsNeededFor(maxCPU) + 2);
  timingPrintPNWidth      = max(6,digitsNeededFor(uMPI::size));
  timingPrintMessageWidth = max(8,digitsNeededFor(totalMessages));

  if (uMPI::iDoIO)
    {
      stream << "Average search time ";
      stream.width(timingPrintTimeWidth);
      stream << totalCPU/uMPI::size << " seconds.\n";
      stream << "Maximum search time ";
      stream.width(timingPrintTimeWidth);
      stream << maxCPU << " seconds.\n";

      stream << '\n';

      if (numHubs() > 1)
	{
	  int r = loadBalancer->numberOfRounds();
	  int s = loadBalancer->numberOfSurveyRestarts();
	  stream << r << " load balancing round" << plural(r) << ", "
	    << s << " survey restart" << plural(s) << ".\n\n";
	}

      timingPrintText(stream,' ',' ',
		      "","","Average",
		      "% of","% where","",
		      "Messages","% of");
      timingPrintText(stream,' ',' ',
		      "Thread/Function","Number","Seconds",
		      "Total","Active","COV",
		      "Received","Msgs");
      timingPrintText(stream);
    }

  if (combineTimingsMax)
    totalCPU = uMPI::size*maxCPU;

  timingPrintData(stream,
		  "Problem Broadcast",
		  uMPI::size > 1,
		  broadcastTime,
		  2*(uMPI::rank > 0));

  timingPrintData(stream,
		  preprocessPhaseName(),
		  true,
		  preprocessTime,
		  preprocessMessages);
  
  timingPrintData(stream,
		  rampUpString.c_str(),
		  true,
		  rampUpTime,
		  rampUpMessages);
  
 for (l1=threadsList.head(); l1;  l1=threadsList.next(l1))
   {
      thread = l1->data();
      timingPrintData(stream,
		    thread->name,
		    thread->active,
		    thread->usefulTime(),
		    thread->messageCount());
       }

  timingPrintData(stream,
		  "Scheduler",
		  true,
		  schedProcTime,
		  0);

  timingPrintData(stream,
		  "Other Overhead",
		  overhead > 0,
		  overhead,
		  0);

  timingPrintData(stream,
		  "Idle",
		  idleProcTime > 0,
		  idleProcTime,
		  0);
  
  if (uMPI::iDoIO)
    timingPrintText(stream,'-',' ');

  timingPrintData(stream,
		  "Total",
		  true,
		  searchTime,
		  messagesReceivedThisProcessor);

  if (uMPI::iDoIO) 
    {
      if (checkpointsEnabled)
	{
	  int cpsWritten = checkpointNumber - restartCPNum;
	  stream << endl << cpsWritten 
		 << " checkpoint" << plural(cpsWritten)
		 << " written, consuming " << checkpointTotalTime 
		 << " seconds.\n";
	}
      stream.setf(ios::fixed,ios::floatfield);
      stream.precision(2);
      stream << "\nMessages per subproblem: " 
	     << ((double) totalMessages)/((double) spTotal) 
	     << "\n\n";
    }

  stream.precision(oldPrecision);
  stream.unsetf(ios::floatfield);

  CommonIO::begin_tagging();
}


void parallelBranching::timingPrintText(ostream& stream,
					char  percentTotalFill,
					char  restFill,
					const char* text1,
					const char* text2,
					const char* text3,
					const char* text4,
					const char* text5,
					const char* text6,
					const char* text7,
					const char* text8)
{
  stream.fill(restFill);

  stream.setf(ios::left,ios::adjustfield);
  stream.width(timingPrintNameWidth);
  stream << text1 << ' ';
  stream.setf(ios::right,ios::adjustfield);

  stream.width(timingPrintPNWidth);
  stream << text2 << ' ';

  stream.width(timingPrintTimeWidth);
  stream << text3 << ' ';
  
  stream.fill(percentTotalFill);

  stream.width(6);
  stream << text4 << ' ';

  stream.fill(restFill);

  stream.width(7);
  stream << text5 << ' ';

  stream.width(5);
  stream << text6 << ' ';

  stream.fill(percentTotalFill);

  stream.width(timingPrintMessageWidth);
  stream << text7 << ' ';

  stream.width(6);
  stream << text8 << '\n';

  stream.fill(' ');
}


void parallelBranching::timingPrintData(ostream& stream,
					const char* name,
					int present,
					double time,
					double messageCount)
{
  int count = uMPI::sum(present);
  double timeSum = uMPI::sum(time);
  double timeSumSq = uMPI::sum(time*time);
  double baseLine;
  if (combineTimingsMax)
    baseLine = count*maxCPU;
  else
    baseLine = uMPI::sum(present*searchTime);

  double messageSum = uMPI::sum(messageCount);

  if (!uMPI::iDoIO || (count == 0))
    return;

  double mean     = timeSum/count;
  double variance = max((double) 0,timeSumSq/count - mean*mean);
  double stdev    = sqrt(variance);
  double cov;
  if (mean > 0)
    cov = stdev/mean;
  else
    cov = 0;

  stream.setf(ios::left,ios::adjustfield);
  stream.width(timingPrintNameWidth);
  stream << name << ' ';
  stream.setf(ios::right,ios::adjustfield);

  stream.width(timingPrintPNWidth);
  stream << count << ' ';

  stream.width(timingPrintTimeWidth);
  stream.setf(ios::fixed,ios::floatfield);
  stream.precision(1);
  stream << mean << " ";
  
  printPercent(stream,timeSum,totalCPU) << "  ";
  
  printPercent(stream,timeSum,baseLine) << ' ';

  stream.width(5);
  stream.setf(ios::fixed,ios::floatfield);
  stream.precision(2);
  stream << cov << ' ';

  stream.width(timingPrintMessageWidth);
  stream.setf(ios::fixed,ios::floatfield);
  stream.precision(0);
  stream << messageSum << ' ';

  printPercent(stream,messageSum,totalMessages) << '\n';
}


void parallelBranching::postRampUpAbort(double aggBound)
{
  // Figure out time and print some statistics

  double maxRampTime = uMPI::max(rampUpTime);
  if (uMPI::rank == uMPI::ioProc)
    {
      CommonIO::end_tagging();
      ucout << "Aborting after ramp-up phase:\n\n";
      ucout.precision(2);
      ucout << "Ramp-up time:             " << maxRampTime  << " seconds\n";
      ucout << "Subproblems bounded:      " << rampUpBounds << endl
	    << "Subproblems left in pool: " << rampUpPool   << "\n\n";
      ucout.precision(statusLinePrecision);
      ucout.width(12);
      if (abs(incumbentValue) > 1e10)
	ucout.setf(ios::scientific,ios::floatfield);
      else 
	ucout.setf(ios::fixed,ios::floatfield);
      ucout << "Incumbent value: " << incumbentValue << endl;
      if (abs(aggBound) > 1e10)
	ucout.setf(ios::scientific,ios::floatfield);
      else 
	ucout.setf(ios::fixed,ios::floatfield);
      ucout << "Aggregate bound: " << aggBound       << endl;
      ucout.width(6);
      ucout.precision(3);
      ucout << "Gap:             " << 100.0*relGap(aggBound) << " %\n\n";
      CommonIO::begin_tagging();
    }

  ucout.flush();
  cleanAbort();
  
}


void spToken::debugPrint(ostream& s) const
{
  coreSPInfo::debugPrint(s);
  s << '-';
  switch (whichChild)
    {
    case self:
      s << "self";
      break;
    case anyChild:
      s << "any-child";
      break;
    default:
      s << "child-" << whichChild;
    }
};


// void parallelBranching::writeCachedOutput()
// {
//   ucout << "Writing cached debug output.\n";
//   *outputCache << '\0';
//   if (outputCache->fail())
//     {
//       ucout << "Debug output has been truncated.\n";
//       outputCacheBuf[outputCacheSize - 1] = '\0';
//     }
//   char name[32];
//   sprintf(name,"debug%05d.txt",uMPI::rank);
//   ofstream dump(name,ios::out);
//   dump << outputCacheBuf;
// }


} // namespace pebbl

#endif
