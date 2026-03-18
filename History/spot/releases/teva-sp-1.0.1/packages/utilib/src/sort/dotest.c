/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/* stest.c
 *
 * Code to test the sorts
 */

#include <math.h>
#include <utilib/c_sort.h>
#include <utilib/random.h>


#define LEN 5
main()
{
double list[LEN];
int  order[LEN];
int i;

sfrnd1(-1);
for (i=0; i<LEN; i++) {
  list[i] = frnd1();
  order[i] = (long) i;
  }
for (i=0; i<LEN; i++) {
  printf("%.3f ", list[i]);
  }
printf("\n");

/*orderd(order, LEN, list);*/
/*
quick_sort((SORT_VOID*)list,LEN,sizeof(double),compare_double);
*/
sortd(list, LEN);

for (i=0; i<LEN; i++) {
  printf("%.3f ", list[i]);
  }
printf("\n");
for (i=0; i<LEN; i++) {
  printf("%d ", order[i]);
  }
printf("\n");
}

