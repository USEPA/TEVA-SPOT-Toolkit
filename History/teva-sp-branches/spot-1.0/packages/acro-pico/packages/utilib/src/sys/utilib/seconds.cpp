/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/*
 * Routines to return time used by processes and wallclock time.
 */

#include <utilib/utilib_config.h>

#include <utilib/seconds.h>

#if UTILIB_TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#if UTILIB_HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#if !defined(UTILIB_HAVE_GETTIMEOFDAY) && !defined(UTILIB_HAVE_LOCALTIME) 
#if defined(HAVE_TIMEB_H)
#include <sys/timeb.h>
#elif !defined(UTILIB_HAVE_CLOCK)
#error "This platform has no usable time functions"
#endif
#endif

#if UTILIB_HAVE_UNISTD_H
#include <unistd.h>
#endif
#if UTILIB_HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif

#include <math.h>

static double CPU_start_time=0.0;
static double WallClock_start_time=0.0;
static long clock_tick=0;

#define MILLISECONDS (1.0e-3)
#define MICROSECONDS (1.0e-6)

/*
 * Initialize Timing
 *
 * Initialize the timing routines.  We get the starting time and use this as a
 * baseline for elapsed time measurements.
 *
 * NOTE: time estimates can be corrupted if multiple calls
 *       are made to this function and the Elapsed routines are used.
 */
extern "C" int InitializeTiming(void)
{
#if !defined(UTILIB_HAVE_GETRUSAGE) && defined(UTILIB_HAVE_TIMES)
   clock_tick = sysconf(_SC_CLOCK_TICK);
#endif

   CPU_start_time = CPUSeconds();
   WallClock_start_time = WallClockSeconds();
   return 0;
}


/*
 * CPUSeconds - Returns sum of system and user time (in seconds)
 *   used by the process so far.
 */
extern "C" double CPUSeconds(void)
  {
#ifdef UTILIB_HAVE_GETRUSAGE
   struct rusage usage;
   long sec, usec;
   double res;
   (void)getrusage(RUSAGE_SELF, &usage);
   sec  = usage.ru_utime.tv_sec  + usage.ru_stime.tv_sec ;
   usec = usage.ru_utime.tv_usec + usage.ru_stime.tv_usec;
   res = sec + MICROSECONDS*usec;
   return res;
#elif defined(UTILIB_HAVE_TIMES) && defined(UTILIB_HAVE_SYSCONF)
   struct tms usage;
   (void)times(&usage);
   return(((double)(usage.tms_utime + usage.tms_stime))/((double) clock_tick));
#elif UTILIB_HAVE_CLOCK
   clock_t t = clock();
   return ((double)t)/CLOCKS_PER_SEC; 
#else
   return 0.0; // Should never get here, configure should have failed 
#endif
  }

/*
 * ElapsedCPUSeconds - Returns time in seconds used by process in the
 *  interval since InitializeTiming was called.
 */
extern "C" double ElapsedCPUSeconds(void)
{
  return(CPUSeconds() - CPU_start_time);
}

/*
 * WallClockSeconds -  Returns the number of seconds elasped since
 *  some date in the distant past.
 */
extern "C" double WallClockSeconds(void)
  {
#ifdef UTILIB_HAVE_GETTIMEOFDAY
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return ((double)tv.tv_sec + ((double)tv.tv_usec * MICROSECONDS));
#elif UTILIB_HAVE_LOCALTIME
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
#elif UTILIB_HAVE_FTIME
   struct timeb tb;
   double res;
   (void) ftime(&tb);
   res = tb.time + MILLISECONDS*tb.millitm;
   return res;
#else
  return 0.0;   /* Should never get here; configure should have failed. */
#endif
  }

/*
 * ElapsedWallClockSeconds - Returns the number of seconds elapsed since
 *  InitializeTiming was called.
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
  return WallClockSeconds();
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
