/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/* ins_sort.c
 *
 * Insertion Sort
 */

#include <memory.h>
#include <stdio.h>
#include <stddef.h>
#include <utilib/c_sort.h>

#define ELT(x)          (void*)((char*)base + ((x)*width))
#define PUT(x,y)        memcpy(y,x,width)

/*
#define ELT(x)		base + ((x)*width)
#define PUT(x,y)	{int putk; \
			for (putk=0; putk<width; putk++) \
			  *((y)+putk) = *((x)+putk); \
			}
*/


SORT_RETVAL ins_sort SORT_ARGS
{
int i,j;
SORT_VOID* a;

a = (SORT_VOID*)malloc((size_t)width);	/* Create buffer */

for (j=1; j<nel; j++) {
  PUT(ELT(j),a);
  i=j-1;
  while ((i > -1) && ((*compare)(ELT(i),a) > 0)) {
    PUT(ELT(i), ELT(i+1));
    i--;
    }
  if (i != (j-1))
     PUT(a,ELT(i+1));
  }

free(a);

return SORT_RETURN;
}
