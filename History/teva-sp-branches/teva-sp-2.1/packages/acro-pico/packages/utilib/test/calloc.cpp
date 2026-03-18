/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

#include <utilib/std_headers.h>

#include <utilib/CachedAllocator.h>

class A {
int i;
};


int main()
{
utilib::CachedAllocator<A*> tmp;
return 0;
}
