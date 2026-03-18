/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file Random.h
 *
 * Provide externs and headers for the routines in RANLIB.C
 */

#ifndef utilib_Random_h
#define utilib_Random_h

#if defined(__STDC__) || defined(__cplusplus)
#define _(args) args
#else
#define _(args) ()
#endif

#ifdef __cplusplus
extern "C" {
#endif


#include <utilib/real.h>


/* #define OLDSTYLE_RANLIB 	Define this in the Makefile */

/* Routines which generate random deviates */
extern REAL genbet _((REAL aa,REAL bb));
extern REAL genchi _((REAL df));
extern REAL genexp _((REAL av));
extern REAL sexpo _((void));
extern REAL genf _((REAL dfn, REAL dfd));
extern REAL gengam _((REAL a,REAL r));
extern REAL sgamma _((REAL a));
extern void genmn _((REAL *parm,REAL *x,REAL *work));
extern REAL gennch _((REAL df,REAL xnonc));
extern REAL gennf _((REAL dfn, REAL dfd, REAL xnonc));
extern REAL gennor _((REAL av,REAL sd));
extern REAL snorm1 _((void));
extern REAL snorm2 _((void));
extern REAL snorm3 _((void));
extern REAL genunf _((REAL low,REAL high));
extern long ignbin _((long n,REAL pp));
extern long ignpoi _((REAL mu));
extern long ignuin _((long low,long high));
extern REAL rcauchy _((REAL alpha, REAL beta));
extern REAL scauchy1 _((void));

/* #ifndef OLDSTYLE_RANLIB */
#if 0
#define rbeta(a,b)	genbet(a,b)
#define rchi(a)		genchi(a)
#define rexp(a)		genexp(a)
#define rF(a,b)		genf(a,b)
#define rgamma(a,b)	gengam(a,b)
#define rmnorm(a,b,c)	genmn(a,b,c)
#define rnchi(a,b)	gennch(a,b)
#define rnF(a,b,c)	gennf(a,b,c)
#define rnorm(a,b)	gennor(a,b)
#define runif(a,b)	genunf(a,b)
#define rbinomial(a,b)	ignbin(a,b)
#define rpoisson(a)	ignpoi(a)
#define runifint(a,b)	ignuin(a,b)
#endif


#ifdef __cplusplus
};
#endif


#define snorm() snorm1()

#define ABS(x) ((x) >= 0 ? (x) : -(x))
#ifndef MIN
#define MIN(a,b) ((a) <= (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) ((a) >= (b) ? (a) : (b))
#endif


/* Setup stuff for the global rng */
#include <utilib/LEC_rng.h>

#ifdef OLDSTYLE_RANLIB
#define ranf()	LEC_runif()
#else
#define ranf()	(*global_runif)()
#ifdef __cplusplus
extern "C" {
#endif
extern REAL (*global_runif) _((void));
#ifdef __cplusplus
};
#endif
#define set_global_runif(func)	global_runif = func
#endif

#endif
