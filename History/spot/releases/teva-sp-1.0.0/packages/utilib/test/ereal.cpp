/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */


#ifdef NON_ANSI
#include <iostream.h>
#else
#include <iostream>
#endif

#include <utilib/Ereal.h>


template <class T, class V>
void compare(T& x, V& y)
{
cout << "Comparing: " << x << " " << y << endl;
cout << "x==y\t" << (x == y) << endl;
cout << "x!=y\t" << (x != y) << endl;
cout << "x<y\t" << (x < y) << endl;
cout << "x<=y\t" << (x <= y) << endl;
cout << "x>y\t" << (x > y) << endl;
cout << "x>=y\t" << (x >= y) << endl;
cout << "x+y\t" << (x+y) << endl;
cout << "x-y\t" << (x-y) << endl;
cout << "x*y\t" << (x*y) << endl;
cout << "x/y\t" << (x/y) << endl;
cout << endl;
}


bool dotest(double tmp)
{
if (tmp < 0.0) return true;
else return false;
}


int main()
{
utilib::Ereal<double> foo = -6;
utilib::Ereal<double> bar = 7;
utilib::Ereal<double> infhi = utilib::Ereal<double>::positive_infinity;
utilib::Ereal<double> inflo = utilib::Ereal<double>::negative_infinity;

cout << ((double)foo) << " " << ((double)bar) << " " << ((double)infhi) << " " << ((double)inflo) << endl;
cout << dotest(foo) << " " << dotest(bar) << " " << dotest(infhi) << " " << dotest(inflo) << endl;

compare(foo,foo);
compare(bar,bar);
compare(infhi,infhi);
compare(inflo,inflo);

compare(foo,bar);
compare(bar,foo);

compare(foo,infhi);
compare(infhi,foo);

compare(bar,infhi);
compare(infhi,bar);


compare(inflo,foo);
compare(foo,inflo);

compare(inflo,bar);
compare(bar,inflo);

compare(inflo,infhi);
compare(infhi,inflo);

double tmp = 20;

compare(tmp,foo);
compare(foo,tmp);

compare(tmp,bar);
compare(bar,tmp);

compare(tmp,infhi);
compare(infhi,tmp);

compare(tmp,inflo);
compare(inflo,tmp);


{
double tmp2 = -20;

compare(tmp2,foo);
compare(foo,tmp2);

compare(tmp2,bar);
compare(bar,tmp2);

compare(tmp2,infhi);
compare(infhi,tmp2);

compare(tmp2,inflo);
compare(inflo,tmp2);
}

{
double tmp2 = -6;

compare(tmp2,foo);
compare(foo,tmp2);

compare(tmp2,bar);
compare(bar,tmp2);

compare(tmp2,infhi);
compare(infhi,tmp2);

compare(tmp2,inflo);
compare(inflo,tmp2);
}

{
utilib::Ereal<double> tmp2 = -6;

compare(tmp2,foo);
compare(foo,tmp2);

compare(tmp2,bar);
compare(bar,tmp2);

compare(tmp2,infhi);
compare(infhi,tmp2);

compare(tmp2,inflo);
compare(inflo,tmp2);
}

{
int tmp2 = -6;

compare(tmp2,foo);
compare(foo,tmp2);

compare(tmp2,bar);
compare(bar,tmp2);

compare(tmp2,infhi);
compare(infhi,tmp2);

compare(tmp2,inflo);
compare(inflo,tmp2);
}

ucout.precision(15);
ucout << "lround(infinity):      " << lround(infhi) << " " << infhi << endl;
ucout << "lround(-infinity):    " << lround(inflo) << " " << inflo << endl;
utilib::Ereal<double> tmp2 = MAXDOUBLE;
ucout << "lround(MAXDOUBLE):     " << lround(tmp2) << " " << tmp2 << endl;
tmp2 = -MAXDOUBLE;
ucout << "lround(-MAXDOUBLE):   " << lround(tmp2) << " " << tmp2 << endl;
tmp2 = LONG_MAX;
ucout << "lround(LONG_MAX):      " << lround(tmp2) << " " << tmp2 << endl;
tmp2 = LONG_MIN;
ucout << "lround(LONG_MIN):     " << lround(tmp2) << " " << tmp2 << endl;
tmp2 = LONG_MAX-1.5;
ucout << "lround(LONG_MAX-1.5):  " << lround(tmp2) << " " << tmp2 << endl;
tmp2 = LONG_MIN+1.5;
ucout << "lround(LONG_MIN+1.5): " << lround(tmp2) << " " << tmp2 << endl;

}
