#include <utilib/utilib_config.h>
#ifdef UTILIB_HAVE_NAMESPACES
using namespace std;
#endif
#ifndef UTILIB_HAVE_STD
#include <iostream.h>
#else
#include <iostream>
#endif

template <class T>
class vector { public: vector() {} };

template <class T>
class set { public: set() {} };

template <class T>
class map { public: map() {} };

template <class T>
class list { public: list() {} };

template <class T>
class queue { public: queue() {} };

int main()
{
#ifdef UTILIB_HAVE_NAMESPACES
cout << "OK" << endl;
#else
std::cout << "OK" << std::endl;
#endif
return 0;
}
