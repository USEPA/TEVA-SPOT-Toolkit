/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

#include <utilib/std_headers.h>
#include <utilib/seconds.h>

#ifdef UTILIB_HAVE_NAMESPACES
using namespace std;
#endif

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
