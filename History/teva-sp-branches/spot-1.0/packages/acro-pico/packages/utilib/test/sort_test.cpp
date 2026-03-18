/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// sort_test.cpp
//
// Test sort/order/rank operations
//
#include <utilib/std_headers.h>
#include <utilib/sort.h>
#include <utilib/stl_auxillary.h>

using namespace utilib;
using namespace std;

class A
{
public:
  int a;
};


template <class VTYPE>
void reorder(VTYPE& vec)
{
for (unsigned int i=1; i<=vec.size(); i++) vec[i-1] = 5*i*i - i*i*i;
}

int main()
{
try {

#ifdef  UTILIB_YES_COMMONIO
CommonIO::begin();
#endif

{
vector<int> tmp(10);
reorder(tmp);
ucout << "a " << tmp << endl;
}

//
// SORT
//
{
vector<int> tmp(10);
reorder(tmp);
sort(tmp);
ucout << "a " << tmp << endl;
}
{
BasicArray<int> tmp(10);
reorder(tmp);
sort(tmp);
ucout << "a " << tmp << endl;
}

{
vector<int> tmp(10);
reorder(tmp);
sort(tmp.begin(),tmp.end());
ucout << "a " << tmp << endl;
}
{
BasicArray<int> tmp(10);
reorder(tmp);
sort(tmp.begin(),tmp.end());
ucout << "a " << tmp << endl;
}

{
vector<int> tmp(10);
reorder(tmp);
sort(tmp,less<int>());
ucout << "a " << tmp << endl;
}
{
BasicArray<int> tmp(10);
reorder(tmp);
sort(tmp,less<int>());
ucout << "a " << tmp << endl;
}

{
vector<int> tmp(10);
reorder(tmp);
sort(tmp.begin(),tmp.end(),less<int>());
ucout << "a " << tmp << endl;
}
{
BasicArray<int> tmp(10);
reorder(tmp);
sort(tmp.begin(),tmp.end(),less<int>());
ucout << "a " << tmp << endl;
}

//
// STABLE_SORT
//
{
vector<int> tmp(10);
reorder(tmp);
stable_sort(tmp);
ucout << "a " << tmp << endl;
}
{
BasicArray<int> tmp(10);
reorder(tmp);
stable_sort(tmp);
ucout << "a " << tmp << endl;
}

{
vector<int> tmp(10);
reorder(tmp);
stable_sort(tmp.begin(),tmp.end());
ucout << "a " << tmp << endl;
}
{
BasicArray<int> tmp(10);
reorder(tmp);
stable_sort(tmp.begin(),tmp.end());
ucout << "a " << tmp << endl;
}

{
vector<int> tmp(10);
reorder(tmp);
stable_sort(tmp,less<int>());
ucout << "a " << tmp << endl;
}
{
BasicArray<int> tmp(10);
reorder(tmp);
stable_sort(tmp,less<int>());
ucout << "a " << tmp << endl;
}

{
vector<int> tmp(10);
reorder(tmp);
stable_sort(tmp.begin(),tmp.end(),less<int>());
ucout << "a " << tmp << endl;
}
{
BasicArray<int> tmp(10);
reorder(tmp);
stable_sort(tmp.begin(),tmp.end(),less<int>());
ucout << "a " << tmp << endl;
}

//
// ORDER
//
{
vector<int> tmp(10);
vector<int> ord;
reorder(tmp);
order(ord,tmp);
ucout << "a " << ord << endl;
}
{
BasicArray<int> tmp(10);
BasicArray<int> ord;
reorder(tmp);
order(ord,tmp);
ucout << "a " << ord << endl;
}

{
vector<int> tmp(10);
vector<int> ord;
reorder(tmp);
order(ord,tmp,less<int>());
ucout << "a " << ord << endl;
}
{
BasicArray<int> tmp(10);
BasicArray<int> ord;
reorder(tmp);
order(ord,tmp,less<int>());
ucout << "a " << ord << endl;
}

//
// STABLE_ORDER
//
{
vector<int> tmp(10);
vector<int> ord;
reorder(tmp);
stable_order(ord,tmp);
ucout << "a " << ord << endl;
}
{
BasicArray<int> tmp(10);
BasicArray<int> ord;
reorder(tmp);
stable_order(ord,tmp);
ucout << "a " << ord << endl;
}

{
vector<int> tmp(10);
vector<int> ord;
reorder(tmp);
stable_order(ord,tmp,less<int>());
ucout << "a " << ord << endl;
}
{
BasicArray<int> tmp(10);
BasicArray<int> ord;
reorder(tmp);
stable_order(ord,tmp,less<int>());
ucout << "a " << ord << endl;
}

//
// RANK
//
{
vector<int> tmp(10);
vector<int> ord;
reorder(tmp);
rank(ord,tmp);
ucout << "a " << ord << endl;
}
{
BasicArray<int> tmp(10);
BasicArray<int> ord;
reorder(tmp);
rank(ord,tmp);
ucout << "a " << ord << endl;
}

{
vector<int> tmp(10);
vector<int> ord;
reorder(tmp);
rank(ord,tmp,less<int>());
ucout << "a " << ord << endl;
}
{
BasicArray<int> tmp(10);
BasicArray<int> ord;
reorder(tmp);
rank(ord,tmp,less<int>());
ucout << "a " << ord << endl;
}

//
// STABLE_RANK
//
{
vector<int> tmp(10);
vector<int> ord;
reorder(tmp);
stable_rank(ord,tmp);
ucout << "a " << ord << endl;
}
{
BasicArray<int> tmp(10);
BasicArray<int> ord;
reorder(tmp);
stable_rank(ord,tmp);
ucout << "a " << ord << endl;
}

{
vector<int> tmp(10);
vector<int> ord;
reorder(tmp);
stable_rank(ord,tmp,less<int>());
ucout << "a " << ord << endl;
}
{
BasicArray<int> tmp(10);
BasicArray<int> ord;
reorder(tmp);
stable_rank(ord,tmp,less<int>());
ucout << "a " << ord << endl;
}

#ifdef UTILIB_YES_COMMONIO 
CommonIO::end();
#endif

}
STD_CATCH(;)

return 0;
}
