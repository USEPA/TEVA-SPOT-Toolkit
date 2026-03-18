/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/* orderx.c
 *
 */

#include <utilib/c_sort.h>
#include <utilib/_generic.h>


#define ELT(x)	(void*)((char*)index_base + ((x)*index_width))

static int	(*index_compar)(COMP_VOID*,COMP_VOID*);
static SORT_VOID* 	index_base;
static int 	index_width;
static int index_compare(COMP_VOID*,COMP_VOID*);


void orderx(int *indx, int n, SORT_VOID* base, int width, 
		int (*compare)(COMP_VOID*,COMP_VOID*),
		SORT_RETVAL (*sort_alg) SORT_ARGS )
{
int i;

for (i=0; i<n; i++) indx[i] = i;

index_compar = compare;
index_base = base;
index_width = width;

(*sort_alg)((SORT_VOID*)indx, n, sizeof(int), index_compare);
}



static int index_compare(COMP_VOID* x, COMP_VOID* y)
{
return (*index_compar)(ELT(*(int*)x), ELT(*(int*)y));
}
