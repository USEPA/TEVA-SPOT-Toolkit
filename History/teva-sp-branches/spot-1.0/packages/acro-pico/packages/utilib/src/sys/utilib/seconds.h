/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file seconds.h
 * 
 * Portable wrappers for system-dependent timing routines.  Both CPU time
 * and wall clock (elapsed) time are provided.  These routines were 
 * derived from similar routines developed by Scott Kohn for LPARX.
 */

#ifndef utilib_seconds_h
#define utilib_seconds_h

#include <utilib/utilib_config.h>
#ifdef __cplusplus
extern "C" {
#endif

/// The `absolute' number of CPU seconds (User+System CPU seconds).
double CPUSeconds();

/// The `absolute' number of wall-clock seconds.
double WallClockSeconds();

/// Resets the timing state for \c ElapsedCPUSeconds and \c ElapsedWallClockSeconds.
int InitializeTiming();

/// The number of CPU seconds elapsed since the code was started or since the last call to \c InitializdTiming.
double ElapsedCPUSeconds();

/// The number of elapsed wall-clock seconds since the code was started or since the last call to \c InitializeTiming.
double ElapsedWallClockSeconds();

/// This measures the number of seconds from some arbitrary starting point,
/// which is useful for initializing RNGs.  This is the same as WallClockSeconds().
double CurrentTime();

/// The smallest amount of time (in seconds) that the timer on this
/// system can distinguish from zero (experimental)
double timerGranularitySeconds();

#ifdef __cplusplus
}
#endif

#endif
