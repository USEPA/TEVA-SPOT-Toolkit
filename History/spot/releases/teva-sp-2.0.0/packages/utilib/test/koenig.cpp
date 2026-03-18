
#include <utilib/utilib_config.h>
#ifdef UTILIB_HAVE_STD
#include <vector>
#else
#include <vector.h>
#endif
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
