/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/* shell_sort.c
 *
 * Adapted from Numerical Recipies in C
 */

#include <memory.h>
#include <math.h>
#include <utilib/c_sort.h>
#include <utilib/_generic.h>

#define ELT(x)          (void*)((char*)base + ((x)*width))
#define PUT(x,y)        memcpy(y,x,width)

#define ALN2I 1.442695022
#define TINY 1.0e-5


SORT_RETVAL shell_sort SORT_ARGS
{
int nn,m,j,i,lognb2;
SORT_VOID* t;

t = (SORT_VOID*) malloc((size_t)width);

lognb2= (int) (log((double) nel)*ALN2I+TINY);
m=nel;
for (nn=0; nn<lognb2; nn++) {
  m >>= 1;
  for (j=m; j<nel; j++) {
    i=j-m;
    PUT(ELT(j),t);
    while ((i >= 0) && ((*compare)(ELT(i),t) > 0)) {
      PUT(ELT(i),ELT(i+m));
      i -= m;
      }
    PUT(t, ELT(i+m));
    }
  }

free(t);
return;
}

#undef ALN2I
#undef TINY
