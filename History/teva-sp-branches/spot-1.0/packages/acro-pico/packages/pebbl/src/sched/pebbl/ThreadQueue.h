/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file ThreadQueue
 *
 * Defines the pebbl::ThreadQueue class.
 */

#ifndef pebbl_ThreadQueue_h
#define pebbl_ThreadQueue_h

#include <acro_config.h>
#include <utilib/OrderedList.h>
#include <pebbl/ThreadObj.h>

namespace pebbl {

using utilib::CommonIO;
using utilib::OrderedList;

//
// ThreadQueue defines a mechanism for controling the how threads are
// scheduled within a given priority queue.
//
enum QueueType {round_robin, time_weighted_priority, bias_weighted_priority};


class ThreadQueue : public OrderedList<ThreadObj*,double>, public CommonIO
{
protected:

  QueueType queue_type;
  void reset(ThreadObj*);

public:

  double max_proc_priority;

  ThreadQueue(QueueType type=round_robin) 
	{
	queue_type=type;
	if (type == bias_weighted_priority)
	   max_proc_priority=1.0;
	else max_proc_priority=10000.0;
	}

  void update_priority(ThreadObj* thread, double time, double curr_time);
  void unblock(ThreadObj* thread, double time);

  double reset(double time);
};

} // namespace pebbl

#endif
