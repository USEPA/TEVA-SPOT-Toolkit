/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */


//
// Test the public domain malloc routines from within C++
//
#include <utilib/std_headers.h>

#ifdef UTILIB_HAVE_NAMESPACES
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
