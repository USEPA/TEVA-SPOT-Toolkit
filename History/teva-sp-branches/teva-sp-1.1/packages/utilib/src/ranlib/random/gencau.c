/* gencau.c
 *
 */

#include <math.h>
#include <utilib/Random.h>


REAL rcauchy(REAL alpha, REAL beta)
/*
**********************************************************************
     REAL rcauchy(REAL alpha, REAL beta)
         GENerate random deviate from a CAUchy distribution
                              Function
     Generates a single random deviate from a Cauchy distribution
     with mean, alpha, and parameter, beta.
                              Arguments
**********************************************************************
*/
{
static REAL gencau;

    gencau = beta*scauchy1()+alpha;
    return gencau;
}
