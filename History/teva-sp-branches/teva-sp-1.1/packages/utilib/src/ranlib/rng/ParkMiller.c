/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/*
 * ParkMiller.c
 *
 * William Hart
 * Dec, 1999
 *
 * A C implementation of ParkMiller
 *
    Presented below is a FORTRAN 77 adaptation of one of the Pascal codes
    that appears in [8].  This code is portable to any machine that has a
    maximum integer greater than, or equal to, 2**31-1.  Thus, this code
    should run on any 32-bit machine.  The code meets all of the
    requirements of the "minimal standard" as described in [8].  <p>
 
    We ran it on all of the systems mentioned in this article.  In general, 
    the execution times for running the portable code were 2 to 50 times
    slower  than the random number generator supplied by the vendor.
    For versions of  random number generators that implement the minimal
    standard on machines  with a maximum integer less than 2**31-1, or
    generators that are more  specific to machines with 64-bit arithmetic,
    see [8].
*/

#include <math.h>

static int jseed=123456789;
static int ifrst=0;


void PMsrand(int iseed)
{
jseed = iseed;
ifrst = 0;
}

#define MPLIER 16807
#define MODLUS 2147483647
#define MOBYMP 127773
#define MOMDMP 2836

double PMrand()
{
/*
 * This function returns a pseudo-random number for each invocation.
 * It is a FORTRAN 77 adaptation of the "Integer Version 2" minimal
 * standard number generator whose Pascal code appears in the article:
 *
 *    Park, Steven K. and Miller, Keith W., "Random Number Generators:
 *    Good Ones are Hard to Find", Communications of the ACM,
 *    October, 1988.
 */

register int hvlue, lvlue, testv;
static int nextn;

if (ifrst == 0) {
   nextn = jseed;
   ifrst = 1;
   }

hvlue = nextn / MOBYMP;
lvlue = mod(nextn, MOBYMP);
testv = MPLIER*lvlue - MOMDMP*hvlue;
if (testv > 0) 
   nextn = testv;
else
   nextn = testv + MODLUS;
return nextn/MODLUS;
}
