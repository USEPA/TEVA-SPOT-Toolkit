/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/* merge_sort.c
 *
 * Merge sort performed by sorting subsets between two internal buffers.
 * Note: this is a stable sorting algorithm, but it uses a lot of memory
 *
 * There be bugs in this code.
 */

#include <memory.h>
#include <stdio.h>
#include <math.h>
#include <utilib/c_sort.h>
#include <utilib/_generic.h>

#define MIN(x,y)	((x)<(y)? (x) : (y))
#define ELT(x)          (void*)((char*)base + ((x)*width))
#define SELT(x)		(void*)((char*)slist + ((x)*width))
#define DELT(x)		(void*)((char*)dlist + ((x)*width))
#define PUT(x,y)        memcpy(y,x,width)


static int merge(SORT_VOID* slist, SORT_VOID* dlist, int i, int ndx, int nel, 
			int width, int (*compare)(COMP_VOID*,COMP_VOID*))
{
int a,aa,b,bb,ii;

a = i;
aa = b = i + ndx;
bb = MIN(b + ndx, nel);
ii = i;

while ((a<aa) & (b<bb)) {
  if ((*compare)(SELT(a), SELT(b)) < 0) {
     PUT(SELT(a), DELT(ii));
     a++;
     }
  else {
     PUT(SELT(b),DELT(ii));
     b++;
     }
  ii++;
  }
while (a<aa) {
   PUT(SELT(a), DELT(ii));
   a++;
   ii++;
   }
while (b<bb) {
   PUT(SELT(b), DELT(ii));
   b++;
   ii++;
   }

return(OK);
}


SORT_RETVAL merge_sort SORT_ARGS
{
SORT_VOID *templist[2], *slist, *dlist;
int i,ndx,flag=0;

templist[0] = (SORT_VOID *) malloc((size_t) width*nel); 
templist[1] = (SORT_VOID *) malloc((size_t) width*nel);
slist = templist[0];
dlist = templist[1];

for(i=0; i<nel; i++) {			/* Read in values */
  PUT(ELT(i),SELT(i));
  }

ndx = 1;
while (ndx < nel) {
  i=0;
  while (i < nel) {
    merge(slist, dlist, i, ndx, nel, width, compare);
    i += ndx + ndx;
    }

  dlist = templist[flag];		/* Swap buffers */
  flag = (flag+1) % 2;
  slist = templist[flag];

  ndx += ndx;				/* Increment partition length */
  }

for(i=0; i<nel; i++) {			/* Write out values */
  PUT(SELT(i),ELT(i));
  }
free( templist[0] );
free( templist[1] );

return SORT_RETURN;
}


/** TEST ROUTINE FOR MERGESORTF
#define LEN 1000
main()
{
double list[LEN];
long  order[LEN];
int i;

for (i=0; i<LEN; i++) {
  list[i] = (double) cos((double) i);
  order[i] = (long) i;
  }
mergesortf(list, order, LEN);

for (i=0; i<LEN; i++) {
  printf("%.3f ", list[i]);
  }
printf("\n");
for (i=0; i<LEN; i++) {
  printf("%d ", order[i]);
  }
printf("\n");
}
*/

