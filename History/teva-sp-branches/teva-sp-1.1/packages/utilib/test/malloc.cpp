
//
// Test the public domain malloc routines from within C++
//

#include <iostream>

#ifdef ANSI_NAMESPACES
using namespace std;
#endif

#ifdef USING_UTILIB_MALLOC
#include <utilib/utilib_malloc.h>
#endif

int main()
{
#ifndef USING_UTILIB_MALLOC

cout << "The UTILIB malloc utilities have not been configured." << endl;

#else

malloc_stats();
double* foo = new double [10];
malloc_stats();
malloc_trim(0);
malloc_stats();
#endif
}
