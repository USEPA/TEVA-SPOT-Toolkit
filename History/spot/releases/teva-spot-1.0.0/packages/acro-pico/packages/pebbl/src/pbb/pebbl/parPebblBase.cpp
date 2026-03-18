/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// parPebblBase.cpp
//
// A base class for the parallel part of PEBBL.  Like the pebblBase class, 
// it contains only static members (parameter definitions) and enum 
// declarations.  The parallelBranching and parallelBranchSub classes both 
// derive from this class, so they effectively share these declarations.
//
// Jonathan Eckstein
//

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <pebbl/parPebblBase.h>

#ifdef EVENT_LOGGING_PRESENT
using utilib::logEvent;
#endif

using namespace std;

namespace pebbl {

#ifdef EVENT_LOGGING_PRESENT
int parallelPebblBase::workerLogState      = logEvent::undefinedState;
int parallelPebblBase::boundLogState       = logEvent::undefinedState;
int parallelPebblBase::foundIncLogState    = logEvent::undefinedState;
int parallelPebblBase::pruneLogState       = logEvent::undefinedState;
int parallelPebblBase::hubPruneLogState    = logEvent::undefinedState;
int parallelPebblBase::hubLogState         = logEvent::undefinedState;
int parallelPebblBase::statusPrintLogState = logEvent::undefinedState;
int parallelPebblBase::pCreateLogState     = logEvent::undefinedState;
int parallelPebblBase::pBoundedLogState    = logEvent::undefinedState;
int parallelPebblBase::pDeadLogState       = logEvent::undefinedState;
int parallelPebblBase::pSplitLogState      = logEvent::undefinedState;
int parallelPebblBase::rampUpLogState      = logEvent::undefinedState;
int parallelPebblBase::rebalLogState       = logEvent::undefinedState;
int parallelPebblBase::preprocLogState     = logEvent::undefinedState;
int parallelPebblBase::rampUpState         = logEvent::undefinedState;
int parallelPebblBase::checkpointLogState  = logEvent::undefinedState;
#endif

} // namespace pebbl

#endif
