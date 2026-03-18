/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

#include "nr.double.h"
#include "IntVector.h"

int main()
{
DoubleVector vec(101);
IntVector ws(101);
LCG* rng = new LCG(100);

for (int i=0; i<10; i++) {
  int ans=0;
  for (int j=0; j<vec.len(); j++)
    vec[j] = j;
  shuffle(vec,rng);
  ans = argmedian(vec,vec.len(),ws);
  cout << "Problem:\n" << vec << endl << ans << " " << vec[ans] << endl;
  //if (vec[ans] != 49)
  //   cout << vec[ans] << endl;
  }
return 0;
}
