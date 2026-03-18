/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/* 
 *
 * seconds.cpp
 *
 * This file wraps architecture-dependent timer routines.  Both CPU time
 * and wall clock (elapsed) time are provided.
 * 
 * This file was derived from the seconds.C timing routines developed by
 * Scott Kohn for LPARX.
 *
 * Note:  this is now a C++ file to enable automatic initialization of 
 * the WallClock_init variable.  
 *
 * Michael S. Eldred
 *   Jan 2000   Added default returns to CPUSeconds & WallClockSeconds so that
 *              they would at least compile on unsupported platforms.  Also
 *              improved timer definitions for SGI and DEC.  Note that a 
 *              CPUParentChildSeconds function could be added which mimics 
 *              RWTimer's use of the C clock() function from <time.h>.  Then 
 *              CPUSeconds would be renamed to CPUParentSeconds.
 */

/*
 *
 * INCLUDES
 *
 */

#include <iostream>
#ifdef ANSI_HDRS
#include <cmath>
#else
#include <math.h>
#endif

#include <utilib/seconds.h>

#ifndef STDLIB_MPI
#include <time.h>

#ifdef UWIN
#include <utilib/_math.h>
#include <time.h>
#define CLK_TCK CLOCKS_PER_SEC
#endif

#include <utilib/utilib_dll.h>

#ifdef _MSC_VER
#include <time.h>
#else
#if !defined(MINGW)
#include <sys/time.h>
#include <sys/times.h>
#endif
#include <unistd.h>
#endif

#if defined(SUNOS) || defined(SOLARIS) || defined(PVM) || defined(SGI) || defined(OSF) || defined(LINUX) || (defined(CYGWIN) && !defined(MINGW)) || defined(DARWIN)
#include <sys/resource.h>
#include <sys/types.h>
#if !(defined(__GNUC__) && __GNUC__ >= 3)
extern "C" int getrusage(int who, struct rusage *rusage);
#endif
#endif

#if defined(SUNOS) || defined(SOLARIS) || defined(PVM) || defined(SGI) || defined(OSF) || defined(LINUX) || (defined(CYGWIN) && !defined(MINGW))
extern "C" int ftime(struct timeb *);
#include <sys/timeb.h>
static struct timeb WallClock_init;
// by providing an additional reference point, round-off can be minimized.  
// This is accomplished by differencing large, similar integers _before_ the 
// conversion to a double which avoids roundoff in the conversion.
static int dummy = ftime(&WallClock_init);
#endif

#if defined (COUGAR)
extern "C" {
#include <nx.h>
};
#endif

#if (defined(CRAYC90) && defined(MULTITASK))
#pragma _CRI taskcommon CPU_start_time
#pragma _CRI taskcommon WallClock_start_time
#endif

static double CPU_start_time=0.0;
static double WallClock_start_time=0.0;

#define MILLISECONDS (1.0e-3)
#define MICROSECONDS (1.0e-6)

#ifdef ANSI_NAMESPACES
using namespace std;
#endif

/*
 * Initialize Timing
 *
 * Initialize the timing routines.  We get the starting time and use this as a
 * baseline for elapsed time measurements.
 *
 * NOTE: time estimates can be corrupted if multiple calls (e.g., DAKOTA & 
 *       SGOPT) are made to this function and the Elapsed routines are used.
 *       For this reason, DAKOTA uses the absolute timing routines and tracks 
 *       elapsed times internally.
 */
extern "C" int InitializeTiming(void)
{
#ifdef CM5
   CMMD_node_timer_clear(CM5_MP_TIMER_NUMBER);
   CMMD_node_timer_start(CM5_MP_TIMER_NUMBER);
#endif

   CPU_start_time = CPUSeconds();
   WallClock_start_time = WallClockSeconds();
   return 0;
}


/*
 * CPUSeconds
 *
 */
extern "C" double CPUSeconds(void)
  {
#if defined(KSR)
   return(user_seconds());

#elif defined(SUNOS) || defined(SOLARIS) || defined(PVM) || defined(SGI) || defined(OSF) || defined(LINUX) || (defined(CYGWIN)&&!defined(MINGW)) || defined(DARWIN)
   struct rusage usage;
   long sec, usec;
   double res;
   (void) getrusage(RUSAGE_SELF, &usage);
   sec  = usage.ru_utime.tv_sec  + usage.ru_stime.tv_sec ;
   usec = usage.ru_utime.tv_usec + usage.ru_stime.tv_usec;
   /*printf("TIME %ld %ld %f\n", sec, usec, (sec+MICROSECONDS*usec));*/
   res = sec + MICROSECONDS*usec;
   return res;

/* Previously part of the WallClockSeconds routine, this routine has been moved
   here (as an alternate timer) since the SGI man pages state that this is in 
   fact a CPU timer.
#elif defined(SGI)
   struct tms usage;
   long wallclock = times(&usage);
   return(((double) wallclock)/((double) CLK_TCK));
*/

#elif defined(CM5)
   CMMD_node_timer_stop(CM5_MP_TIMER_NUMBER);
   double busy = CMMD_node_timer_busy(CM5_MP_TIMER_NUMBER);
   CMMD_node_timer_start(CM5_MP_TIMER_NUMBER);
   return(busy);

#elif defined(iPSC860)
#ifndef iPSC860SIMULATOR
   return(dclock());
#else
   return(MILLISECONDS*mclock());
#endif

#elif defined(nCUBE2)
   return(MICROSECONDS*amicclk());

#elif defined(PARAGON) || defined(COUGAR) 
   return(dclock());

#elif defined(SPX)
   struct tms usage;
   (void) times(&usage);
   return(((double) (usage.tms_utime+usage.tms_stime))/((double) CLK_TCK));

#elif defined(_MSC_VER) || defined(MINGW)
   clock_t t = clock();
   return ((double)t)/CLOCKS_PER_SEC; 
   /* WEH - I'm not exactly sure that this measures the CPU time 
	    for a process under NT. */

/* WEH - provide a default times utility???
   struct tms usage;
   (void) times(&usage);
   return(((double) (usage.tms_utime+usage.tms_stime))/((double) CLK_TCK));
*/

#else
   return 0.0; // so that function at least compiles on an unsupported platform
#endif
  }



/*
 * ElapsedCPUSeconds
 *
 */
extern "C" double ElapsedCPUSeconds(void)
{
return(CPUSeconds() - CPU_start_time);
}



/*
 * WallClockSeconds
 *
 */
extern "C" double WallClockSeconds(void)
  {
#if defined(CM5)
   CMMD_node_timer_stop(CM5_MP_TIMER_NUMBER);
   double elapsed = CMMD_node_timer_elapsed(CM5_MP_TIMER_NUMBER);
   CMMD_node_timer_start(CM5_MP_TIMER_NUMBER);
   return(elapsed);

#elif defined(iPSC860)
#ifndef iPSC860SIMULATOR
   return(dclock());
#else
   return(MILLISECONDS*mclock());
#endif

#elif defined(nCUBE2)
   return(MICROSECONDS*amicclk());

#elif defined(PARAGON) || defined(COUGAR)
   return(dclock());

#elif defined(SPX)
   struct tms usage;
   long wallclock = times(&usage);
   return(((double) wallclock)/((double) CLK_TCK));

#elif defined(_MSC_VER) || defined(DARWIN) || defined(MINGW)
   struct tm * tm_str;
   time_t t=0;
   long int nsecs;
   tm_str = localtime(&t);
   /* Compute seconds since Jan 1, 1990 */
   if (tm_str == 0) return -999.0;
   nsecs = tm_str->tm_sec + 60*(tm_str->tm_min + 
	   60*(tm_str->tm_hour + 24*(tm_str->tm_yday + 
	   365*(tm_str->tm_year - 90))));
   return (double) nsecs;

#elif defined(KSR)
   return(all_seconds());

#elif defined(SUNOS) || defined(SOLARIS) || defined(PVM) || defined(SGI) || defined(OSF) || defined(LINUX) || (defined(CYGWIN) && !defined(MINGW))
   struct timeb tb;
   double res;
   (void) ftime(&tb);
   res = (tb.time - WallClock_init.time)
     + MILLISECONDS*(tb.millitm - WallClock_init.millitm);
   return res;

/*
#if (defined(PVM))
   struct timeb tb;
   (void) ftime(&tb);
   return(tb.time + MILLISECONDS*tb.millitm); // since tb.time is a large
   // integer, the conversion to double can drop significant figures, which 
   // is compounded by the fact that 2 of these similar doubles are differenced
   // in the elapsed routines.
#endif
*/

#else
   return 0.0; // so that function at least compiles on an unsupported platform
#endif
  }


/*
 * ElapsedWallClockSeconds
 *
 */
extern "C" double ElapsedWallClockSeconds(void)
{
return(WallClockSeconds() - WallClock_start_time);
}


/*
 * CurrentTime
 *
 * Measures the number of seconds from some arbitrary starting point.
 * Useful for initializing RNGs
 */
extern "C" double CurrentTime(void)
  {
#if defined(CM5)
   CMMD_node_timer_stop(CM5_MP_TIMER_NUMBER);
   double elapsed = CMMD_node_timer_elapsed(CM5_MP_TIMER_NUMBER);
   CMMD_node_timer_start(CM5_MP_TIMER_NUMBER);
   return(elapsed);

#elif defined(iPSC860)
#ifndef iPSC860SIMULATOR
   return(dclock());
#else
   return(MILLISECONDS*mclock());
#endif

#elif defined(nCUBE2)
   return(MICROSECONDS*amicclk());

#elif defined(PARAGON) || defined(COUGAR)
   return(dclock());

#elif defined(SPX)
   struct tms usage;
   long wallclock = times(&usage);
   return(((double) wallclock)/((double) CLK_TCK));

#elif defined(_MSC_VER) || defined(DARWIN) || defined(MINGW)
   struct tm * tm_str;
   time_t t=0;
   long int nsecs;
   tm_str = localtime(&t);
   /* Compute seconds since Jan 1, 1990 */
   if (tm_str == 0) return -999.0;
   nsecs = tm_str->tm_sec + 60*(tm_str->tm_min + 
	   60*(tm_str->tm_hour + 24*(tm_str->tm_yday + 
	   365*(tm_str->tm_year - 90))));
   return (double) nsecs;

#elif defined(KSR)
   return(all_seconds());

#elif defined(SUNOS) || defined(SOLARIS) || defined(PVM) || defined(SGI) || defined(OSF) || defined(LINUX) || (defined(CYGWIN) && !defined(MINGW))
   struct timeb tb;
   double res;
   (void) ftime(&tb);
   res = tb.time + MILLISECONDS*tb.millitm;
   return res;

/*
#if (defined(PVM))
   struct timeb tb;
   (void) ftime(&tb);
   return(tb.time + MILLISECONDS*tb.millitm); // since tb.time is a large
   // integer, the conversion to double can drop significant figures, which 
   // is compounded by the fact that 2 of these similar doubles are differenced
   // in the elapsed routines.
#endif
*/

#else
   return 0.0; // so that function at least compiles on an unsupported platform
#endif
  }

/// The smallest amount of time (in seconds) that the timer on this
/// system can distinguish from zero (experimental).  In practice, it
/// would be good to call this at least twice (with a second between calls)
/// and use the minimum value returned.
/// Note: this assumes the granularity is no larger than a second.

double timerGranularitySeconds()
{
double secondHolder = CPUSeconds();
double granularity = 1.0;
while (secondHolder != floor(secondHolder))
  {
  secondHolder *= 10;
  granularity /= 10;
  }
return(granularity);
}



/*
 * ElapsedWallClockSeconds
 *
 */


#endif // STDLIB_MPI
