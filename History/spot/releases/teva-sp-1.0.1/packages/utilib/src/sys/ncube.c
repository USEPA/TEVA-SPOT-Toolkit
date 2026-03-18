/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file ncube.c
 *
 * This file contains defines commands which should have been included
 * in the standard libraries for nCUBE, RS6K and SGI.
 *
 * \author William E. Hart
 */


/* isinf and isnan should be removed when NCUBE supplies their equivalents */

#if defined(nCUBE2) || defined(RS6K) || defined(SGI)

typedef unsigned long long ull;

typedef union {
    double d;           /* put the double-precision number in here... */
    ull l;              /* ... and examine the bits here!             */
} dorl;


int isinf( double x ) {
    register dorl y;
    y.d = x;
    return y.l<<1 == ((ull)1023*2+1)<<52<<1;            /* e=2b+1 and f==0 */
}

int isnan( double x ) {
    register dorl y;
    y.d = x;
    return y.l<<1 > ((ull)1023*2+1)<<52<<1;             /* e=2b+1 and f!=0 */
}
#endif

#if defined(nCUBE2)
void bzero(char* b, int length)
{
int i;
for (i=0; i<length; i++, b++)
  *b = '\0';
}

#else
int dummy=0;   /* avoids an empty translation unit */
#endif

