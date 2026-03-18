/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file math_basic.h
 *
 * Defines and constants for basic numerical operations.
 */

#ifndef utilib_math_basic_h
#define utilib_math_basic_h

#if defined(__cplusplus) && defined(ANSI_HDRS)
#include <cmath>
#else
#include <math.h>
#endif

#if defined(__cplusplus) && !defined(NON_ANSI)
#include <algorithm>
#ifdef ANSI_NAMESPACES
using namespace std;
#endif
#endif


/*
 *
 * DEFINE MATHEMATICAL CONSTANTS
 *
 */
#if defined(CYGWIN)
#if defined(__cplusplus) && defined(ANSI_HDRS)
#include <climits>
#else
#include <limits.h>
#endif
#include <float.h>
#define MAXINT INT_MAX
#ifndef MAXFLOAT
#define MAXFLOAT FLT_MAX
#endif
#ifndef MAXDOUBLE
#define MAXDOUBLE DBL_MAX
#endif

#elif defined _MSC_VER
#if defined(__cplusplus) && defined(ANSI_HDRS)
#include <climits>
#include <cfloat>
#else
#include <limits.h>
#include <float.h>
#endif
#define MAXINT INT_MAX
#ifndef MAXFLOAT
#define MAXFLOAT FLT_MAX
#endif
#ifndef MAXDOUBLE
#define MAXDOUBLE DBL_MAX
#endif

#elif defined DARWIN
#include <limits.h>
#include <float.h>
#define MAXINT INT_MAX
#ifndef MAXDOUBLE
#define MAXDOUBLE DBL_MAX
#endif

#else
#include <values.h>

#endif

#if defined(SOLARIS) && !defined(__GNUC__)
#include <sunmath.h>
#endif

#include <utilib/utilib_dll.h>
#include <utilib/_generic.h>

#if 0
#if defined(DOXYGEN)
/* A definition for pi. */
const long double M_PI = 3.1415926535897932384626433832795029L;
/* A definition for e. */
const long double M_E  = 2.71828
#endif
#endif

#if defined(DARWIN)
const double PI = 3.1415926535897932384626433832795029L;

#elif !defined(RS6K)
const long double PI = 3.1415926535897932384626433832795029L;
#endif

#ifndef MAXINT
#define MAXINT (int)(~((int)0) ^ (1 << (sizeof(int)*8-1)))
#endif

/**
 * \def MAXINT
 *
 * The maximum integer value, which is system dependent.
 */
#if defined(DOXYGEN)
#ifdef MAXINT
#undef MAXINT
#endif
#define MAXINT
#endif

/**
 * \def PI
 *
 * The value of pi, which may be defined by the system.
 */
#if defined(DOXYGEN)
#ifdef PI
#undef PI
#endif
#define PI
#endif

/**
 * \def MAXFLOAT
 *
 * The maximum double value, which is system dependent.
 */
#if defined(DOXYGEN)
#ifdef MAXFLOAT
#undef MAXFLOAT
#endif
#define MAXFLOAT
#endif

/**
 * \def MAXDOUBLE
 *
 * The maximum double value, which is system dependent.
 */
#if defined(DOXYGEN)
#ifdef MAXDOUBLE
#undef MAXDOUBLE
#endif
#define MAXDOUBLE
#endif


#if defined(__cplusplus)
namespace utilib {

#ifdef ANSI_NAMESPACES
using std::max;
using std::min;
using std::swap;
#endif

/*
 * 
 * OPERATIONS ON SIMPLE VALUES
 *
 */


/* Returns +1 if argument is positive, -1 if it is negative, and 0 otherwise. */
template<class T>
inline int sgn(const T& thing)
{
  if (thing > 0)
    return 1;
  if (thing < 0)
    return -1;
  return 0;
}

}
#endif

/*
 *
 * MISCELLANEOUS 
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Compute the number of lines in file filename. */
int calc_filesize(char* filename);

#if defined(DOXYGEN) | defined(REDSTORM) | (!defined(CYGWIN) && !defined(DARWIN) && !defined(LINUX))
/**
 * Return the integer value of the rounded value of x.  
 * If the fractional part of x is less than 0.5, then x is
 * rounded down.  Otherwise, x is rounded up.
 */
#ifndef DOXYGEN
long int lround _((double x));
#else
long int lround(double x);
#endif
#endif

  /* We hope this is a portable method for rounding a double to num_digits
     number of decimal digits after the decimal point. */

double d_round(double to_round, unsigned int num_digits);

/* Setup the data structures for bufexp. */
#ifndef DOXYGEN
void setup_bufexp _((int tabsz, double xmin, double xmax));
#else
void setup_bufexp(int tabsz, double xmin, double xmax);
#endif

/* Compute the exponential function using fast buffered interpolation. */
#ifndef DOXYGEN
double bufexp _((double x));
#else
double bufexp(double x);
#endif

#if defined(DOXYGEN) | defined(PARAGON) | defined(nCUBE2) | defined(SGI) | defined(COUGAR) 
/* Returns \c TRUE if \a x is infinite. */
int isinf(double x);

/* Returns \c TRUE if \a x is not-a-number. */
int isnan(double x);

/* Returns the value of 2^x. */
double exp2(double x);
#endif

#if defined(DOXYGEN) | defined(PARAGON) | defined(nCUBE2) | defined(COUGAR)
/* Sets a char array b to null values. */
void bzero(char* b, int length);
#endif

#ifdef __cplusplus
}
#endif

/**
 * \def Exp(x)
 *
 * A macro masks the 'exp' function, possibly replacing it with bufexp to 
 * accelerate the calculation of 'exp'.
 */
#ifdef BUFFERED_EXP
#define Exp(x)	bufexp(x)
#else
#define Exp(x)	exp(x)
#endif

#endif
