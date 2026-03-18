/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// parKnapsack.cpp
//
// Code for a parallel knapsack class for PEBBL.
//


#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <utilib/std_headers.h>
#include <pebbl/parKnapsack.h>

using namespace std;

namespace pebbl {

// Pack a description of the problem.

void parallelBinaryKnapsack::pack(PackBuffer& outBuffer)
{
  outBuffer << numItems << capacity;
  for(int i=0; i<numItems; i++)
    outBuffer << item[i].weight << item[i].value << item[i].name;
}


// unpack same.

void parallelBinaryKnapsack::unpack(UnPackBuffer& inBuffer)
{
  sumOfAllValues = 0;
  inBuffer >> numItems >> capacity;
  item.resize(numItems);
  for(int i=0; i<numItems; i++)
    {
      inBuffer >> item[i].weight >> item[i].value >> item[i].name;
      sumOfAllValues += item[i].value;
    }
}


// Pack and unpack subproblems.

void parBinKnapSub::pack(PackBuffer& outBuffer)
{
  DEBUGPRXP(150,global(),"numIn=" << numIn << ": ");
  outBuffer << numIn;
  for(int i=0; i<numIn; i++)
    {
      outBuffer << inList[i];
      DEBUGPRXP(150,global(),inList[i] << ' ');
    }
  DEBUGPRX(150,global(),'\n');
  outBuffer << numOut;
  DEBUGPRXP(150,global(),"numOut=" << numOut << ": ");
  for(int j=0; j<numOut; j++)
    {
      outBuffer << outList[j];
      DEBUGPRXP(150,global(),outList[j] << ' ');
    }
  DEBUGPRX(150,global(),'\n');
  if ((state == bounded) || (state == separated))
    outBuffer << splitItem;
}


int parallelBinaryKnapsack::spPackSize()
{
  return (3 + numItems)*sizeof(int);
}


void parBinKnapSub::unpack(UnPackBuffer& inBuffer)
{
  inBuffer >> numIn;
  DEBUGPRXP(150,global(),"numIn=" << numIn << ':');
  inList.resize(numIn);
  for(int i=0; i<numIn; i++)
    {
      int item;
      inBuffer >> item;
      inList[i] = item;
      capBase -= itemWeight(item);
      DEBUGPRXP(150,global(), ' ' << inList[i]);
    }      
  DEBUGPRX(150,global(),".\n");
  DEBUGPRX(150,global(),"capBase = " << capBase << ".\n");
  inBuffer >> numOut;
  DEBUGPRXP(150,global(),"numOut=" << numOut << ": ");
  outList.resize(numOut);
  for(int j=0; j<numOut; j++)
    {
      inBuffer >> outList[j];
      DEBUGPRXP(250,global(),outList[j] << ' ');
    }
    DEBUGPRX(250,global(),".\n");
  if ((state == bounded) || (state == separated))
    inBuffer >> splitItem;
}


// The remaining stuff implements all the incumbent searching.


void parBinKnapSub::quickIncumbentHeuristic()
{
  if (state != bounded)
    return;
  DEBUGPRX(20,global(),"Quick incumbent check.\n");
  solution().completeGreedy();
  if (solution().obj > incumbentValue())
    updateIncumbent();
}


void parallelBinaryKnapsack::parallelIncumbentHeuristic(double* controlParam)
{
  int iterations = 0;
  
  do
    {
      DEBUGPR(25,ucout << "Incumbent heuristic: queue size = " 
	      << incQueueSize << ".\n");
      if (incQueueSize == 0)
	break;
      incumbQueueItem* s = removeTopOfIncQueue();
      s->mutate();
      if (s->obj > incumbentValue)
	{
	  s->updateIncumbent();
	  insertInIncQueue(s);  // If we get a new incumbent, keep it around.
	}
      else if (incQueueSize < incumbQueueTargetSize)
	insertInIncQueue(s);
      else
	{
	  delete s;
	  DEBUGPR(25,ucout << "Dropped from queue.\n");
	}
    }
  while (++iterations < *controlParam);
  *controlParam = iterations;
}


void parBinKnapSub::feedToIncumbentThread()
{
  if (state != bounded)
    return;
  global()->feedToIncumbentThread(solution());
}


void parallelBinaryKnapsack::feedToIncumbentThread(binKnapSolution& soln)
{
  if (incQueueSize < incumbQueueMaxSize)
    {
      insertInIncQueue(new incumbQueueItem(&soln));
      DEBUGPR(25,ucout << "Placed in incumbent queue (size=" <<
	      incQueueSize << ").\n");
    }
  else
    DEBUGPR(25,ucout << "Incumbent queue is full.\n");
}


void parallelBinaryKnapsack::insertInIncQueue(incumbQueueItem* s)
{
  if (lastInIncQueue != 0)
    lastInIncQueue->next = s;
  lastInIncQueue = s;
  if (firstInIncQueue == 0)
    firstInIncQueue = s;
  incQueueSize++;
}


incumbQueueItem* parallelBinaryKnapsack::removeTopOfIncQueue()
{
  if (--incQueueSize < 0)
    EXCEPTION_MNGR(runtime_error, "Tried to choose knapsack solution from empty queue");
  incumbQueueItem* s = firstInIncQueue;
  firstInIncQueue = s->next;
  return s;
}

} // namespace pebbl

#endif

