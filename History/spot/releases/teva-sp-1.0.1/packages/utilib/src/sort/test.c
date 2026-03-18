/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/*
   This file does nothing but verify that the parameter format for qsort is
   the same as that defined in _sort.h
*/

#include <stdlib.h>
#include <utilib/c_sort.h>

void sort_test_utility(void);

void sort_test_utility(void)
{
int *ndx=0;
int n=0;
SORT_VOID *base=0;
int width=0;

orderx (ndx, n, base, width, compare_int, qsort);
}
