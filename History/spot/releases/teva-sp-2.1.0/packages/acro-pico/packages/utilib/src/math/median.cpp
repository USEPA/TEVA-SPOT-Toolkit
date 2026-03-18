/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// median.cpp
//
// Developed using ideas from Corman, Lieseron and Rivest
//
// This code uses a (user supplied) work space to avoid modifying the
// original array.
//

#include <utilib/utilib_config.h>
#include <utilib/_math.h>
#include <utilib/DUniform.h>
#include <utilib/default_rng.h>
#include <utilib/math_array.h>

namespace utilib {



int rand_select(double* x, int p, int r, int i, int* ws, DUniform& drnd);

namespace {

int partition(double* x, int p, int r, int* ws)
{
double X = x[ws[p]];
int i = p-1;
int j=r+1;
while (1) {
  while (X< x[ws[--j]]);
  while (x[ws[++i]] < X);
  if (i<j) {
     int tmp = ws[i];
     ws[i] = ws[j];
     ws[j] = tmp;
     }
  else
     return j;
  }
}


int rand_partition(double* x, int p, int n, int* ws, DUniform& drnd)
{
drnd.low(p);
drnd.high(n);
int i = drnd();

int tmp = ws[i];
ws[i] = ws[p];
ws[p] = tmp;

return partition(x,p,n,ws);
}

} // static namespace


int rand_select(double* x, int p, int r, int i, int* ws, DUniform& drnd)
{
if (p == r) return ws[p];

int q = rand_partition(x,p,r,ws,drnd);
int k = q-p+1;				// Size of the LHS of the partition
if (i <= k)
   return rand_select(x,p,q,i,ws,drnd);
else
   return rand_select(x,q+1,r,i-k,ws,drnd);
}


size_type argmedian(double* x, size_type n, int* ws, RNG* rng)
{
int delete_flag=FALSE;
if (!ws) {
   ws = new int [n];
   delete_flag=TRUE;
   }
for (size_type i=0; i<n; i++)
  ws[i] = i;

if (!rng)
   rng = &default_rng;
DUniform drnd(rng);
int ans = rand_select(x,0,n-1,(n+1)/2,ws,drnd);

if (delete_flag)
   delete [] ws;

return ans;
}


double median(double* x, size_type n, int* ws, RNG* rng)
{ return x[argmedian(x,n, ws, rng)]; }





#ifdef USING_OLD_CODE
//
// OLD CODE
//

#ifdef OLD_MEDIAN

/* median.cpp
 *
 * Adapted from Numerical Recipies routine mdian2.
 * 
 * The mediand command sorts the vector!
 *
 * The argmedd routine does not work!
 */

#include <math.h>
#include <stdio.h>
#include "_sort.h"


#define BIG 1.0e30
#define AFAC 1.5
#define AMP 1.5


double mediand(double* x, int n)
{
int med;

if (n == 1)
   return x[0];

sortd(x,n);

med = n/2;
if (n%2 == 0)
   return x[med];
else
   return ((x[med]+x[med+1])*0.5);
}



int argmedd(double* x, int n)
{
int np,nm,ne,j;
double xx,sumx,sum,eps,stemp,dum,aa;
int xm_ndx, xp_ndx;	/* Indeces of points closest to the median */
double ap, am;		/* Lower and upper bounds on the median */
double a;		/* value of the answer */
int ans;		/* Index to the answer */

a = x[n/2];
am = -(ap=BIG);
eps = fabs(a);

for (;;) {
/* printf("here %lf\n",a); */
  sum = sumx = 0.0;
  np = nm = ne = 0;
  ans = xm_ndx = xp_ndx = -1;

  for (j=0; j<n; j++) {
    xx=x[j];
    if (xx == a) {
/* printf("Equal %d\n",j); */
       ne++;
       ans = j;
       }
    else {
/* printf("NEqual %d %le\n",j,xx); */
       if (xx > a) {
          ++np;
          if ((xp_ndx == -1) || (xx < x[xp_ndx])) xp_ndx=j;
          }
       else {  /* xx < a */
          ++nm;
          if ((xm_ndx == -1) || (xx > x[xm_ndx])) xm_ndx=j;
          }
       sum += dum=1.0/(eps+fabs(xx-a));
       sumx += xx*dum;
       }
    }

/* printf("np %d  nm %d  ne %d\n",np,nm,ne); */
fflush(stdout);
  stemp=(sumx/sum)-a;
  if ( ((ne == 0) && ((np-nm) >= 2)) || ((np-nm) >= (1+ne)) ) {
     am=a;
     aa =  stemp < 0.0 ? x[xp_ndx] : x[xp_ndx]+stemp*AMP;
     if (aa > ap) 
        aa=0.5*(a+ap);
     eps=AFAC*fabs(aa-a);
     a=aa;
     }
  else if ( ((ne == 0) && (nm-np >= 2)) || (nm-np >= 1+ne) ){
     ap=a;
     aa = stemp > 0.0 ? x[xm_ndx] : x[xm_ndx]+stemp*AMP;
     if (aa < am)
        aa=0.5*(a+am);
     eps=AFAC*fabs(aa-a);
     a=aa;
     }
  else {
     if (n % 2 == 0) {
        if ((ne > 0) && (np > nm))
           return ans;
 	else
	   return xm_ndx;
	}
     else {
	if (ne > 0)
	   return ans;
	else
	   return (np > nm ? xp_ndx : xm_ndx);
	}
     }
  }

}


#undef BIG
#undef AFAC
#undef AMP
#endif


#endif

} // namespace utilib
