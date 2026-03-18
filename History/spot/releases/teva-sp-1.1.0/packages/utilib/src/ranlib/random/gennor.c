/* gennor.c
 *
 */

#include <math.h>
#include <utilib/Random.h>


REAL gennor(REAL av,REAL sd)
/*
**********************************************************************
     REAL gennor(REAL av,REAL sd)
         GENerate random deviate from a NORmal distribution
                              Function
     Generates a single random deviate from a normal distribution
     with mean, AV, and standard deviation, SD.
                              Arguments
     av --> Mean of the normal distribution.
     sd --> Standard deviation of the normal distribution.
**********************************************************************
*/
{
static REAL gennor;

    gennor = sd*snorm1()+av;
    return gennor;
}

