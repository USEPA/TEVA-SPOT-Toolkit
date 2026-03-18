/* gennf.c
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <utilib/Random.h>


REAL gennf(REAL dfn,REAL dfd,REAL xnonc)
/*
**********************************************************************
     REAL gennf(REAL dfn,REAL dfd,REAL xnonc)
           GENerate random deviate from the Noncentral F distribution
                              Function
     Generates a random deviate from the  noncentral F (variance ratio)
     distribution with DFN degrees of freedom in the numerator, and DFD
     degrees of freedom in the denominator, and noncentrality parameter
     XNONC.
                              Arguments
     dfn --> Numerator degrees of freedom
             (Must be >= 1.0)
     dfd --> Denominator degrees of freedom
             (Must be positive)
     xnonc --> Noncentrality parameter
               (Must be nonnegative)
                              Method
     Directly generates ratio of noncentral numerator chisquare variate
     to central denominator chisquare variate.
**********************************************************************
*/
{
static REAL gennf,xden,xnum;
static long qcond;

    qcond = dfn <= 1.0 || dfd <= 0.0 || xnonc < 0.0;
    if(!qcond) goto S10;
    puts("In GENNF - Either (1) Numerator DF <= 1.0 or");
    puts("(2) Denominator DF < 0.0 or ");
    puts("(3) Noncentrality parameter < 0.0");
    printf("DFN value: %16.6EDFD value: %16.6EXNONC value: \n%16.6E\n",dfn,dfd,
      xnonc);
    exit(1);
S10:
    xnum = gennch(dfn,xnonc)/dfn;
/*
      GENNF = ( GENNCH( DFN, XNONC ) / DFN ) / ( GENCHI( DFD ) / DFD )
*/
    xden = genchi(dfd)/dfd;
    if(!(xden <= 9.999999999998E-39*xnum)) goto S20;
    puts(" GENNF - generated numbers would cause overflow");
    printf(" Numerator %16.6E Denominator %16.6E\n",xnum,xden);
    puts(" GENNF returning 1.0E38");
    gennf = 1.0E38;
    goto S30;
S20:
    gennf = xnum/xden;
S30:
    return gennf;
}

