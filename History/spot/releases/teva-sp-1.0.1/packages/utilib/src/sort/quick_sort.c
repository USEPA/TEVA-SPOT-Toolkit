/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/* quick_sort.c
 *
 * Adapted from Numerical Recipies in C routine 'qcksrt'
 */

#include <memory.h>
#include <utilib/c_sort.h>
#include <utilib/errmsg.h>


#define ELT(x)          (void*)((char*)base + ((x)*width))
#define PUT(x,y)        memcpy(y,x,width)

#define M 7		/* Min length for use of quick sort */
#define NSTACK 50
#define FM 7875
#define FA 211
#define FC 1663



SORT_RETVAL quick_sort SORT_ARGS
{
int l,jstack=0,j,ir,iq,i;
int istack[NSTACK+1];
long int fx=0L;
SORT_VOID* a;

a = (SORT_VOID*) malloc((size_t)width);

l=0;
ir=nel;

for (;;) {
  if ((ir - l + 1) < M) {	/* Do a straight insertion sort */
     for (j=l; j<ir; j++) {
       PUT(ELT(j),a);
       for (i=j-1; (((*compare)(ELT(i),a) > 0) && (i>-1)); i--) 
	 PUT(ELT(i), ELT(i+1));
       if (i != (j-1))
          PUT(a,ELT(i+1));
       }
     if (jstack == 0) {
        free(a);
	return SORT_RETURN;
        }
     ir=istack[jstack--];
     l=istack[jstack--];
     }
  else {
     i=l;
     j=ir-1;
     fx=(fx*FA+FC) % FM;	/* generate a random number in [l,ir-1> */
     iq=i+((j-i+1)*fx)/FM;
     PUT(ELT(iq),a);
     PUT(ELT(i),ELT(iq));
     for (;;) {
       while ((j > -1) && ((*compare)(a,ELT(j)) < 0))
         j--;
       if (j <= i) {
	  PUT(a,ELT(i));
	  break;
	  }
       PUT(ELT(j),ELT(i));
       i++;
       while (((*compare)(a,ELT(i)) > 0) && (i < nel))
         i++;
       if (j <= i) {
          i=j;
	  PUT(a,ELT(i));
	  break;
	  }
       PUT(ELT(i),ELT(j));
       j--;
       }
     if (ir-i >= i-l) {
        istack[++jstack]=i;
	istack[++jstack]=ir;
	ir=i;
	}
     else {
	istack[++jstack]=l;
	istack[++jstack]=i;
	l=i;
	}
     if (jstack > NSTACK)
        ErrExit("quick_sort.c -- NSTACK too small in QCKSRT");
     }
  }
}

#undef M
#undef NSTACK
#undef FM
#undef FA
#undef FC
