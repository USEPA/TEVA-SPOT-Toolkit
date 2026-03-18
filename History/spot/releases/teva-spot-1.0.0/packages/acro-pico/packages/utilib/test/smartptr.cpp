/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

#include <utilib/std_headers.h>
#include <utilib/SmartPtr.h>

#ifdef UTILIB_HAVE_NAMESPACES
using namespace std;
using namespace utilib;
#endif

string str;

class A
{
public:

  A() {}

  ~A() {cout << str << endl << flush;}

};


int main()
{
str = "Pointer deleted when SmartPtr object is deleted";
{
SmartPtr<A> foo = new A;
}

str = "Pointer deleted when another pointer is set in the smart pointer";
SmartPtr<A> foo = new A;
foo = new A;

str = "Pointer deleted when one smart pointer is set to another";
{
SmartPtr<A> bar = new A;
foo = bar;
str = "Shared pointer deleted?";
}

str = "This pointer shouldn't be deleted when I reinsert it into the SmartPtr";
{
A* a = new A;
SmartPtr<A> bar  = a;
bar  = a;
str = "OK .. avoided deleteing a pointer when it was reinserted.";
}

str = "Finishing up";

return 0;
}

