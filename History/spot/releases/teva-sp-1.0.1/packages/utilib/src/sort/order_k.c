/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/* order_k.c
 *
 * order_k	Move the k smallest elements to the front of the 
 *			list (in sorted order)
 * order_ki	Modify index list to point to the k smallest elements of
 *			the list (in sorted order)
 *
 * Note: this is inefficient for k > N/2, if you don't care about the
 *	sorder order of the k smallest elements.  To make this efficient,
 *	it sufficies to swap what we think we are doing.
 */

#include <memory.h>
#include <utilib/c_sort.h>
#include <utilib/_generic.h>


#define ELT(x)          (void*)((char*)base + ((nel-1-(x))*width))
#define PUT(x,y)        memcpy(y,x,width)



/**********************************************************************
 *
 * order_k
 *
 */

void order_k(SORT_VOID* base, int nel, int width, 
				int (*compare)(COMP_VOID*,COMP_VOID*),
				int k)
{
int l,j,ir,i;
SORT_VOID* tmp;

tmp = (SORT_VOID*) malloc((size_t)width);

l=(nel >> 1);
ir=nel;

for (;;) {
  if (l > 0) {			/* Construct heap with nel Mod 2 downheaps */
     --l;
     PUT(ELT(l),tmp);
     }
  else {
     --ir;			/* Remove k largest using k downheaps */
     PUT(ELT(ir),tmp);
     PUT(ELT(0),ELT(ir));
     if (ir == (nel-k)) {
        PUT(tmp,ELT(0));
        free(tmp);
        return;
        }
     }

  /* We're doing the 'downheap' operation on elt tmp */
  i=l;					/* The current location of tmp */
  j=(l << 1)+1;				/* The left child of tmp */
  while (j < ir) {
    if ((j < (ir-1)) && ((*compare)(ELT(j),ELT(j+1)) > 0))
       j++;
    if ((*compare)(tmp, ELT(j)) > 0) {
       PUT(ELT(j),ELT(i));
       i = j;
       j = j*2 + 1;
       }
    else j=ir;
    }
  PUT(tmp, ELT(i));
  }
}



/**********************************************************************
 *
 * order_ki
 *
 */

#undef ELT
#define ELT(x)          (void*)((char*)base + ((x)*width))
#define NDX(x)		(ndx[nel-1-(x)])

void order_ki(SORT_VOID* base, int nel, int width, 
				int (*compare)(COMP_VOID*,COMP_VOID*),
				int k, int* ndx)
{
int l,j,ir,i;
int tmp;

for (i=0; i<nel; i++)		/* Initialize index */
  ndx[i] = i;

l=(nel >> 1);
ir=nel;

for (;;) {
  if (l > 0) {			/* Construct heap with nel Mod 2 downheaps */
     --l;
     tmp = NDX(l);
     }
  else {
     --ir;			/* Remove k largest using k downheaps */
     tmp = NDX(ir);
     NDX(ir) = NDX(0);
     if (ir == (nel-k)) {
        NDX(0) = tmp;
        return;
        }
     }

  /* We're doing the 'downheap' operation on elt tmp */
  i=l;					/* The current location of tmp */
  j=(l << 1)+1;				/* The left child of tmp */
  while (j < ir) {
    if ((j < (ir-1)) && ((*compare)(ELT(NDX(j)),ELT(NDX(j+1))) > 0))
       j++;
    if ((*compare)(ELT(tmp), ELT(NDX(j))) > 0) {
       NDX(i) = NDX(j);
       i = j;
       j = j*2 + 1;
       }
    else j=ir;
    }
  NDX(i) = tmp;
  }
}


/*
int main()
{
int ndx[10];
double vec[10];
int i;

for (i=9; i>=0; i--)
  vec[i] = (double)(i*i*10 % 17);
for (i=0; i<10; i++)
  printf("%lf ",vec[i]);
printf("\n");

order_k(vec, 10, sizeof(double), compare_double, 4);
for (i=0; i<10; i++)
  printf("%lf ",vec[i]);
printf("\n");



for (i=9; i>=0; i--)
  vec[i] = (double)(i*i*10 % 17);
for (i=0; i<10; i++)
  printf("%lf ",vec[i]);
printf("\n");

order_ki(vec, 10, sizeof(double), compare_double, 4, ndx);
for (i=0; i<10; i++)
  printf("%d ",ndx[i]);
printf("\n");
for (i=0; i<10; i++)
  printf("%lf ",vec[ndx[i]]);
printf("\n");
}
*/
