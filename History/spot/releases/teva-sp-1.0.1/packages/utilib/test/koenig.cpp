
#include <vector>

#include <utilib/stl_auxillary.h>

using namespace std;
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
