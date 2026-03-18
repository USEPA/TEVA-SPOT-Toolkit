/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

#include <pico/PicoLPSolver.h>

using namespace pico;
using namespace std;

int main()
{

BasicArray<int> intlist;
PicoLPSolver lp;
lp.init();
lp()->loadProblem("../etc/mpsfiles/miplib/bm23",intlist);
lp()->solve();
lp()->solve();

{
PicoLPSolver lp2(lp);
//lp2()->reset();
lp2()->solve();
}
cout << "HERE" << endl;

return 0;
}
