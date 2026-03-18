/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/* heap_sort.c
 *
 * Adapted from Numerical Recipies in C routine 'sort'
 */

#include <memory.h>
#include <utilib/c_sort.h>
#include <utilib/_generic.h>


#define ELT(x)          (void*)((char*)base + ((x)*width))
#define PUT(x,y)        memcpy(y,x,width)
/*
			{int putk; \
                        for (putk=0; putk<width; putk++) \
                          *((y)+putk) = *((x)+putk); \
                        }
*/


SORT_RETVAL heap_sort SORT_ARGS
{
long long int l,ir,i,j;
SORT_VOID* rra;

rra = (SORT_VOID*) malloc((size_t)width);

l=(nel >> 1);
ir=nel;

for (;;) {
  if (l > 0) {
     --l;
     PUT(ELT(l),rra);
     }
  else {
     --ir;
     PUT(ELT(ir),rra);
     PUT(ELT(0),ELT(ir));
     if (ir == 0) {
        PUT(rra,ELT(0));
        free(rra);
	return SORT_RETURN;
        }
     }

  /* We're doing the 'downheap' operation on elt rra */
  i=l;					/* The current location of rra */
  j=(l << 1)+1;				/* The left child of rra */
  while (j < ir) {
    if ((j < (ir-1)) && ((*compare)(ELT(j),ELT(j+1)) < 0))
       j++;
    if ((*compare)(rra, ELT(j)) < 0) {
       PUT(ELT(j),ELT(i));
       i = j;
       j = j*2 + 1;
       }
    else j=ir;
    }
  PUT(rra, ELT(i));
  }
}
