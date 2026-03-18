/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/* math.c
 *
 */


#include <stdlib.h>
#include <utilib/math_basic.h>
#include <math.h>
#include <utilib/errmsg.h>

#if !(defined(CYGWIN) || defined(DARWIN))
extern long int lround(double x);
#endif
void setup_bufexp(int tabsz, double xmin, double xmax);
double bufexp(double x);


#if !defined(DARWIN)
long int lround(double x)
{
double temp;

temp = floor(x);
if ((x - temp) < 0.5)
   return( (long int) temp );
else
   return( (long int) ceil(x) );
}
#endif

/* We hope this is a portable method for rounding a double to num_digits
   number of decimal digits after the decimal point. We may want to add
   some tolerances. */

double d_round(double to_round, unsigned int num_digits)
{
double intpart;
double shift_factor;
double val;

intpart = floor(to_round);
to_round -= intpart;
shift_factor = pow(10, num_digits);

val = to_round * shift_factor;
val += 0.5;
val = floor(val);
val /= shift_factor;
val += intpart;

return(val);
}




/*  Table for 'exp'
 *
 *  I don't know if this is a good way to do this.
 */

static int exp_tabsz=0;
static double exp_xmin=-999.0;
static double exp_xmax=-999.0;
static double exp_step=-999.0;
static double* exp_tab=0;

void setup_bufexp(int tabsz, double xmin, double xmax)
{
abort();
#if 0
static int flag=1;

if (tabsz < 1) return;

if (flag == 1) {
   double temp;
   int i;

   exp_tabsz = tabsz;
   exp_xmin = xmin;
   exp_xmax = xmax;

   exp_step = (xmax - xmin)/(double)(tabsz-1.0);

   exp_tab = dvector(0,tabsz-1);
   for (i=0, temp=exp_xmin; i<tabsz; i++, temp+=exp_step) {
     exp_tab[i] = exp(temp);
/*
     printf("%d\t%lf\t%lf\n",i,temp, exp_tab[i]);
*/
     }
   }
#endif
}


double bufexp(double x)
{
double ndx;
int lndx;

if (exp_tabsz == 0)
   setup_bufexp(5000,-70.0,70.0);

ndx = (x - exp_xmin)/exp_step;
if (ndx <= 0)
   return exp_tab[0];
if (ndx >= exp_tabsz-1)
   return exp_tab[exp_tabsz-1];
lndx = (int) floor(ndx);
return exp_tab[lndx] + (ndx - (double)lndx)*(exp_tab[lndx+1] - exp_tab[lndx]);
}
