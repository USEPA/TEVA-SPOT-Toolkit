/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// stltest.cpp
//
// Test vector operations
//

#if !(defined(COUGAR) || defined(NON_ANSI))

#define USING_STL
#include <fstream>
#include <vector>
#include <utilib/stl_auxillary.h>

#include <utilib/IntVector.h>
#include <utilib/BitArray.h>
#include <utilib/CharString.h>
#include <utilib/CommonIO.h>

using namespace utilib;

int main()
{
vector<int> test_a(10);
test_a << 1;

IntVector a(10);

for (int i=0; i<10; i++) a[i] = i;
ucout << "a= " << a << " " << a.nrefs() << endl;

a.resize(15);
ucout << "a= " << a << " " << a.nrefs() << endl;

a.resize(5);
ucout << "a= " << a << " " << a.nrefs() << endl;

{
IntVector A;
A = a;
ucout << "a= " << a << " " << a.nrefs() << endl;
ucout << "A= " << A << " " << A.nrefs() << endl;

a[0] = -1;
ucout << "a= " << a << " " << a.nrefs() << endl;
ucout << "A= " << A << " " << A.nrefs() << endl;

A &= a;
A[0] = 10;
ucout << "a= " << a << " " << a.nrefs() << endl;
ucout << "A= " << A << " " << A.nrefs() << endl;

{
IntVector A2;
A2 &= A;
ucout << "a= " << a << " " << a.nrefs() << endl;
ucout << "A= " << A << " " << A.nrefs() << endl;
ucout << "A2= " << A2 << " " << A2.nrefs() << endl;

a.resize(8);
ucout << "a= " << a << " " << a.nrefs() << endl;
ucout << "A= " << A << " " << A.nrefs() << endl;

//int bar=0;
}

ucout << "a= " << a << " " << a.nrefs() << endl;
ucout << "A= " << A << " " << A.nrefs() << endl;

//int foo=01;
}

ucout << "a= " << a << " " << a.nrefs() << endl;

a -= 1;
ucout << "a-=1: " << a << " " << a.nrefs() << endl;
a = a.operator-();
ucout << "a=-a: " << a << " " << a.nrefs() << endl;

IntVector AA;
AA = a;
AA *= 2;
ucout << "AA: " << AA << " " << a.nrefs() << endl;
a += AA;
ucout << "a+=AA: " << a << " " << a.nrefs() << endl;

ucout << endl;
ucout << "a= " << a << " " << a.nrefs() << endl;
ofstream ofstr("testfile");
ofstr << a;
ofstr.close();
a << 0;
ifstream ifstr("testfile");
ifstr >> a;
ifstr.close();
ucout << "a= " << a << " " << a.nrefs() << endl;

a.append(a);
ucout << "a= " << a << " " << a.nrefs() << endl;
ucout << endl;




NumArray<int> foo1;
NumArray<int> foo2;
foo1 &= foo2;
foo1.resize(3);
foo1 << 1;
ucout << "These two arrays should be equal." << endl;
ucout << "foo1= " << foo1 << " " << foo1.nrefs() << endl;
ucout << "foo2= " << foo2 << " " << foo2.nrefs() << endl;


NumArray<int> bar1(10);
NumArray<int> bar2(10);
bar1 << 1;
bar2 << 2;
bar2[2]=0;
bar1.set_subvec(3,4,bar2,2);
ucout << "This array should contain a subsequence of 2 indeces, beginning with a zero." << endl;
ucout << "bar1= " << bar1 << " " << bar1.nrefs() << endl;
ucout << "bar2= " << bar2 << " " << bar2.nrefs() << endl;

BitArray b(10);
BitArray B;

{for (int i=0; i<10; i++) b.set(i);}
ucout << "b= " << b << endl;

b.resize(15);
ucout << "b= " << b << endl;

b.resize(5);
ucout << "b= " << b << endl;

B = b;
ucout << "b= " << b << endl;
ucout << "B= " << B << endl;

b.reset(0);
ucout << "b= " << b << endl;
ucout << "B= " << B << endl;

ucout << endl;


CharString str(10);
CharString STR(10);

{for (int i=0; i<10; i++) str[i] = (char)((int)'a' + i);}
ucout << "str= " << str << endl;

str.resize(15);
ucout << "str= " << str << endl;

str.resize(5);
ucout << "str= " << str << endl;

STR = str;
ucout << "str= " << str << endl;
ucout << "STR= " << STR << endl;

str[0] = 'A';
ucout << "str= " << str << endl;
ucout << "STR= " << STR << endl;

return 0;
}

#else
int main()
{
return 0;
}
#endif
