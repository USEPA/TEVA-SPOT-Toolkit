/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file paragon.c
 *
 * This file contains contains commands which should have been included
 * in the standard paragon libraries.
 *
 * \author William E. Hart
 */

#include <math.h>

/* isinf and isnan should be removed when paragon supplies their equivalents */

#ifdef PARAGON

#ifdef DBL_MAX
int isinf(double x) { return ((x > DBL_MAX) || (x < -DBL_MAX)); }
#else
int isinf(double x) { return ((x > HUGE) || (x < -HUGE)); }
#endif

int isnan(double x) {return (x != x);}


/*
void bzero(char* b, int length)
{
register int i;
for (i=0; i<length; i++, b++)
  *b = '\0';
}
*/

#endif


