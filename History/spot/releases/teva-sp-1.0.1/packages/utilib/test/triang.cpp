/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// Test of the triangular distributions
//

#include <utilib/Triangular.h>
#include <utilib/IntVector.h>
#include <utilib/PM_LCG.h>
#include <utilib/CommonIO.h>

using namespace utilib;

void test_crandvar(SimpleRandomVariable<double>* rv, 
		double lower, double upper, IntVector& vec, int num)
{
vec << 0;
double total=0.0;

for (int i=0; i<num; i++) {
  double tmp = (*rv)();
  if (tmp < lower)
     EXCEPTION_MNGR(runtime_error, "rv " << tmp << " is less than lower bound " << lower)
  if (tmp > upper)
     EXCEPTION_MNGR(runtime_error, "rv " << tmp << " is greater than upper bound " << upper)
  total += tmp;
  vec[(int)((tmp-lower)/(upper-lower)*vec.size())]++;
  }
ucout << "Expectation= " << (total/num) << endl;
}

void test_triang(Triangular* rv, double lower, double upper, IntVector& vec,
				int num)
{
vec << 0;
double total=0.0;

for (int i=0; i<num; i++) {
  double tmp = (*rv)(lower,upper);
  if (tmp < lower)
     EXCEPTION_MNGR(runtime_error, "rv " << tmp << " is less than lower bound " << lower)
  if (tmp > upper)
     EXCEPTION_MNGR(runtime_error, "rv " << tmp << " is greater than upper bound " << upper);
  total += tmp;
  vec[(int)((tmp-lower)/(upper-lower)*vec.size())]++;
  }
ucout << "Expectation= " << (total/num) << endl;
}

int main()
{
PM_LCG rng;
Triangular trv(&rng);
Uniform urv(&rng);
IntVector vec(20);

ucout << "Test Uniform" << endl;
test_crandvar(&urv,0.0,1.0,vec,10000);
ucout << vec << endl;

ucout << "Test Triangular" << endl;
test_crandvar(&trv,-1.0,1.0,vec,10000);
ucout << vec << endl;

vec.resize(21);
ucout << "Test Truncated Triangular" << endl;
test_triang(&trv,-0.5,1.0,vec,10000);
ucout << vec << endl;

ucout << "Test Probability-Balanced Triangular" << endl;
trv.truncation(PROBABILITY_BALANCED_TRIANGULAR);
test_triang(&trv,-0.5,1.0,vec,10000);
ucout << vec << endl;

ucout << "Test Expectation-Balanced Triangular" << endl;
trv.truncation(EXPECTATION_BALANCED_TRIANGULAR);
test_triang(&trv,-0.5,1.0,vec,10000);
ucout << vec << endl;
return OK;
}
