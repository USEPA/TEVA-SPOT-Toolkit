/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/* rank.c
 *
 * The jth entry of a rank table gives the rank of the jth element of the
 * 	original array.
 */

/* This function useful if 'order' has been used to generate a vector of
	indexes into the data */

#include <utilib/c_sort.h>
#include <utilib/utilib_dll.h>

void rank(int* ndx_vec, int* rank_vec, int n)
{
register int j;

for (j=0; j<n; j++)
  rank_vec[ndx_vec[j]]=j;
}


