/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// class Any test
//
#include <utilib/utilib_config_bool.h>
#include <utilib/std_headers.h>
#include <utilib/Any.h>

#ifdef UTILIB_HAVE_NAMESPACES
using namespace utilib;
using namespace std;
#endif

int main()
{
try {
int i=-1;
utilib::AnyValue tmp(i);
std::vector<utilib::AnyValue> vec(2);

vec[0] = i;
vec[1] = (char*)"333";

i = utilib::any_cast<int>(vec[0]);
cout << i << endl;
char* cstr = utilib::any_cast<char*>(vec[1]);
cout << cstr << endl;
 }
catch(const utilib::bad_any_cast& err) {
 cerr << "Catch 1: " <<  err.what() << endl;
 }

try {
 int i=-1;
 utilib::AnyReference tmp(&i);
 tmp = 123456;
 cout << tmp << endl;
 cout << i << endl;
 }
catch (const utilib::bad_any_cast& err) {
 cerr << "Catch 2: " <<  err.what() << endl;
 } 

utilib::AnyReference aref;
//aref = "abc";

bool foo=false;
aref.set_ref(foo);
//cerr << aref << endl;
//cerr << "Type a boolean value" << endl;
//cin >> aref;
//cerr << aref << endl;
}
