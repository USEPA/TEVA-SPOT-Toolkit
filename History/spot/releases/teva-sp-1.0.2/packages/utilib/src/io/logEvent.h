/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file logEvent.h
 *
 * Definess the utilib::logEvent class.
 *
 * \author Jonathan Eckstein
 */

#ifndef utilib_logEvent_H
#define utilib_logEvent_H

#if defined(DOXYGEN) || (defined(USING_MPI) && defined(DEBUGGING) && defined(USING_MPE)) 

#include <values.h>
#include <mpi.h>
#include <mpe.h>

#include <utilib/ParameterSet.h>

namespace utilib {


/**
 * Routines for doing event logging/tracing of parallel codes using MPE.
 */
class logEvent : public ParameterSet
{
public:

  /// Constructor
  logEvent()
	{
	create_option("eventLog",eventLog,
		"<int>","0",
		"The logging level",ParameterLowerBound<int>(0));
	}

  ///
  enum { undefinedState = -MAXINT };

  /// Initializes the MPI event logging capability.
  static void init()
	{ MPE_Init_log(); }

  /**
   * Opens an event file (event.alog or event.clog in the current default
   * directory. Call after calling \c init() but before any
   * \c log_xxx calls.
   */
  static void open();

  /**
   * Writes and closes the log file.
   * This can be quite slow, depending on the system.
   */
  static void close();

  /**
   * Defines a state to desplay in the log file.
   * \p descrip is the name.
   * \p color is an X windows color for the visualizer to use.
   */
  static int defineState(const char* description,const char* color);

  /// Indicate the start of a state (states can be nested).
  static void log_start(int stateNumber)
	{ event(stateNumber*2); }

  /// Indicate the end of a state.
  static void log_end(int stateNumber)
	{ event(stateNumber*2 + 1); }

  /**
   * Equivalent to \c log_start(stateNumber) followed by 
   * \c log_end(stateNumber).
   */
  static void log_point(int stateNumber)
	{
      	log_start(stateNumber);
      	log_end(stateNumber);
    	}
  
protected:

  ///
  int eventLog;

  /// Log event \c eventNumber.
  static void event(int eventNumber) 
    	{ 
      	MPE_Log_event(eventNumber,0,(char *) ""); 
    	}

  /// Number of states that can be logged.
  static int numberOfStates;

};


/// Code that is executed if MPE logging is in use.
#define IF_LOGGING_COMPILED(arbitraryCode) arbitraryCode

/// Executes \p action if the event log is greater or equal to \p level.
#define IF_LOGGING_EVENTS(level,action) \
   if (eventLog >= level) action

/**
 * Wraps a call to \c log_<how> using \p state if the logging level is 
 * greater or equal to \p level.
 */
#define LOG_EVENT(level,how,state) \
   IF_LOGGING_EVENTS(level,logEvent::log_##how(state);)

/// Wraps a call to \c log_<how> if \p cond is satisfied.
#define LOG_EVENT_CONDITIONAL(level,cond,how,state) \
   IF_LOGGING_EVENTS(level,if (cond) logEvent::log_##how(state);)

/// Indicates that the logging macros are not empty.
#define EVENT_LOGGING_PRESENT 1

} // namespace utilib

#else


#define IF_LOGGING_COMPILED(arbitraryCode)          /* Nothing */
#define IF_LOGGING_EVENTS(level,action)             /* Nothing */
#define LOG_EVENT(level,how,state)                  /* Nothing */
#define LOG_EVENT_CONDITIONAL(level,cond,how,state) /* Nothing */
#undef  EVENT_LOGGING_PRESENT


#endif


#endif
