/* genunf.c
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <utilib/Random.h>


REAL genunf(REAL low,REAL high)
/*
**********************************************************************
     REAL genunf(REAL low,REAL high)
               GeNerate Uniform Real between LOW and HIGH
                              Function
     Generates a real uniformly distributed between LOW and HIGH.
                              Arguments
     low --> Low bound (exclusive) on real value to be generated
     high --> High bound (exclusive) on real value to be generated
**********************************************************************
*/
{
static REAL genunf;

    if(!(low > high)) goto S10;
    printf("LOW > HIGH in GENUNF: LOW %16.6E HIGH: %16.6E\n",low,high);
    puts("Abort");
    exit(1);
S10:
    genunf = low+(high-low)*ranf();
    return genunf;
}

