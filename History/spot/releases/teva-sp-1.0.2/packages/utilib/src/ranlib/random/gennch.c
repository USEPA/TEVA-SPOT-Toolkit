/* gennch.c
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <utilib/Random.h>


REAL gennch(REAL df,REAL xnonc)
/*
**********************************************************************
     REAL gennch(REAL df,REAL xnonc)
           Generate random value of Noncentral CHIsquare variable
                              Function
     Generates random deviate  from the  distribution  of a  noncentral
     chisquare with DF degrees  of freedom and noncentrality  parameter
     xnonc.
                              Arguments
     df --> Degrees of freedom of the chisquare
            (Must be > 1.0)
     xnonc --> Noncentrality parameter of the chisquare
               (Must be >= 0.0)
                              Method
     Uses fact that  noncentral chisquare  is  the  sum of a  chisquare
     deviate with DF-1  degrees of freedom plus the  square of a normal
     deviate with mean XNONC and standard deviation 1.
**********************************************************************
*/
{
static REAL gennch;

    if(!(df <= 1.0 || xnonc < 0.0)) goto S10;
    puts("DF <= 1 or XNONC < 0 in GENNCH - ABORT");
    printf("Value of DF: %16.6E Value of XNONC%16.6E\n",df,xnonc);
    exit(1);
S10:
    gennch = genchi(df-1.0)+pow(gennor(sqrt(xnonc),1.0),2.0);
    return gennch;
}

