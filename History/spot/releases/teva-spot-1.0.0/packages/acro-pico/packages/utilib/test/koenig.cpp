/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */


#include <utilib/std_headers.h>
#include <utilib/stl_auxillary.h>

#ifdef UTILIB_HAVE_NAMESPACES
using namespace std;
#endif
using namespace utilib;


namespace tester {

template <class TYPE>
class A {
public:

  void bar()
	{
	TYPE d;
	vector<TYPE> a,b;
	a << d;
	}
};


}



using namespace tester;

void foo ()
{
A<int> a;
a.bar();
}
