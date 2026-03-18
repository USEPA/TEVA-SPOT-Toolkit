/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file parPebblBase.h
 * \author Jonathan Eckstein
 *
 * A base class for the parallel part of PEBBL.  Like the pebblBase class, 
 * it contains only static members (parameter definitions) and enum 
 * declarations.  The parallelBranching and parallelBranchSub classes both 
 * derive from this class, so they effectively share these declarations.
 *
 * Augmented to contain log event types.
 */

#ifndef pebbl_parPebblBase_h
#define pebbl_parPebblBase_h

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <utilib/std_headers.h>
#include <pebbl/pebblBase.h>

namespace pebbl {


class parallelPebblBase
{
public:

  /// Needed when you start having tokens.
  enum { self = -2 };

  /// These signals specify subtypes of messages passed from
  /// hubs to workers
  enum 
    { 
      loadInfoSignal, 
      terminateCheckSignal, 
      terminateSignal,
      startCheckpointSignal,
      writeCheckpointSignal,
      startAbortSignal
    };

  /// Codes that go at the beginning of subproblem delivery messages
  enum
    {
      spDeliverSignal = 2513,       // Subproblems follow
      spBufferWarningSignal = 18202 // Just a warning to enlarge receive buffer
    };

#ifdef EVENT_LOGGING_PRESENT
  static int preprocLogState;
  static int workerLogState;
  static int boundLogState;
  static int foundIncLogState;
  static int pruneLogState;
  static int hubPruneLogState;
  static int hubLogState;
  static int statusPrintLogState;
  static int rebalLogState;
  static int pCreateLogState;
  static int pBoundedLogState;
  static int pDeadLogState;
  static int pSplitLogState;
  static int rampUpLogState;
  static int checkpointLogState;
#endif
  
};


//  This class is a shorthand for parallelPebblBase combined with pebblBase

class fullPebblBase : virtual public pebblBase, public parallelPebblBase
{
  // Nothing new outside the two base classes.
};


} // namespace pebbl

#endif

#endif
