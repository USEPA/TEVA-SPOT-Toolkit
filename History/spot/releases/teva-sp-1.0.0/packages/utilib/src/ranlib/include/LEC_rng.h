/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file LEC_rng.h
 *
 * Provide headers and externs for the routines in RANLIB.C
 */

#ifndef utilib_LEC_rng_h
#define utilib_LEC_rng_h

#include <utilib/real.h>

#if defined(__cplusplus)
extern "C" {
#endif

extern long mltmod(long a,long s,long m);
extern void advnst(long k);
extern void phrtsd(char* phrase,long* seed1,long* seed2);

extern REAL LEC_runif(void);

extern void setall(long iseed1,long iseed2);
extern void setant(long qvalue);
extern void setgmn(REAL *meanv,REAL *covm,long p,REAL *parm);
extern void setsd(long iseed1,long iseed2);
extern void getsd(long *iseed1,long *iseed2);

extern void initgn(long isdtyp);
extern void gscgn(long getset,long *g);

#ifdef __cplusplus
};
#endif

#endif
