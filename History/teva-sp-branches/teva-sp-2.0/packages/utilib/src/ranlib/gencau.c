/* gencau.c
 *
 */

#include <utilib/utilib_config.h>
#include <math.h>
#include <utilib/Random.h>


double rcauchy(double alpha, double beta)
/*
**********************************************************************
     double rcauchy(double alpha, double beta)
         GENerate random deviate from a CAUchy distribution
                              Function
     Generates a single random deviate from a Cauchy distribution
     with mean, alpha, and parameter, beta.
                              Arguments
**********************************************************************
*/
{
static double gencau;

    gencau = beta*scauchy1()+alpha;
    return gencau;
}
