
#ifdef COUGAR
// Work around a namespace bug...!
#include <fstream.h>
#endif

#include <utilib/CachedAllocator.h>

class A {
int i;
};


int main()
{
utilib::CachedAllocator<A*> tmp;
return 0;
}
