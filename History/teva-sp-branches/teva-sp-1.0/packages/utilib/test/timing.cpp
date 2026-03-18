/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

#ifndef ANSI_HDRS
#include <stdio.h>
#include <time.h>
#include <math.h>
#else
#include <cstdio>
#include <ctime>
#include <cmath>
using namespace std;
#endif

#include <utilib/seconds.h>


int main()
{
long int start;
double startme;
double startw;
double x=10.0;
int i;
FILE* foo;

start = clock();
startme = CPUSeconds();
startw  = WallClockSeconds();



for (i=0; i<10000; i++) {
  foo = fopen("timing.temp","w+");
  x += 1/x;
  fprintf(foo,"Val = %g\n",x);
  fclose(foo);
  }

start = clock() - start;
startme = CPUSeconds() - startme;
startw  = WallClockSeconds();

printf("clock time %g\n", ((double)(start)/CLOCKS_PER_SEC));
printf("CPUSeconds time %g\n", startme);
printf("WallClockSeconds time %g\n", startw);

return 0;
}
