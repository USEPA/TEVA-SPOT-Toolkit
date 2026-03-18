/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

#include <utilib/utilib_config.h>
#ifdef UTILIB_HAVE_STD
#include <iostream>
#else
#include <iostream.h>
#endif


#include <utilib/Ereal.h>

using namespace std;

template <class T, class V>
void compare(T& x, V& y)
{
  ucout << "Comparing: " << x << " " << y << endl;

  //test x==y
  ucout << "x==y\t";
  try {
    ucout << (x == y) << endl;
  }
  catch (std::invalid_argument) {
     ucout << "Caught invalid_argument" << endl;
  }
  catch (std::runtime_error) {
    ucout << "Caught std::runtime_error" << endl;
  }
  catch (...) {
    ucout << "Caught unknown exception!" << endl;
  }

  //test x!=y
  ucout << "x!=y\t";
  try {
    ucout << (x != y) << endl;
  }
  catch (std::invalid_argument) {
     ucout << "Caught invalid_argument" << endl;
  }
  catch (std::runtime_error) {
    ucout << "Caught std::runtime_error" << endl;
  }
  catch (...) {
    ucout << "Caught unknown exception!" << endl;
  }

  //test x<y
  ucout << "x<y\t";
  try {
    ucout << (x < y) << endl;
  }
  catch (std::invalid_argument) {
     ucout << "Caught invalid_argument" << endl;
  }
  catch (std::runtime_error) {
    ucout << "Caught std::runtime_error" << endl;
  }
  catch (...) {
    ucout << "Caught unknown exception!" << endl;
  }

  //test x<=y
  ucout << "x<=y\t";
  try {
    ucout << (x <= y) << endl;
  }
  catch (std::invalid_argument) {
     ucout << "Caught invalid_argument" << endl;
  }
  catch (std::runtime_error) {
    ucout << "Caught std::runtime_error" << endl;
  }
  catch (...) {
    ucout << "Caught unknown exception!" << endl;
  }

  //test x>y
  ucout << "x>y\t";
  try {
    ucout << (x > y) << endl;
  }
  catch (std::invalid_argument) {
     ucout << "Caught invalid_argument" << endl;
  }
  catch (std::runtime_error) {
    ucout << "Caught std::runtime_error" << endl;
  }
  catch (...) {
    ucout << "Caught unknown exception!" << endl;
  }

  //test x>=y
  ucout << "x>=y\t";
  try {
    ucout << (x >= y) << endl;
  }
  catch (std::invalid_argument) {
     ucout << "Caught invalid_argument" << endl;
  }
  catch (std::runtime_error) {
    ucout << "Caught std::runtime_error" << endl;
  }
  catch (...) {
    ucout << "Caught unknown exception!" << endl;
  }

  //test x+y
  ucout << "x+y\t";
  try {
    ucout << (x+y) << endl;
  }
  catch (std::range_error) {
    ucout << "Caught std::range_error" << endl;
  }
  catch (std::runtime_error) {
    ucout << "Caught std::runtime_error" << endl;
  }
  catch (...) {
    ucout << "Caught unknown exception!" << endl;
  }

  //test x-y
  ucout << "x-y\t";
  try {
    ucout << (x-y) << endl;
  }
  catch (std::range_error) {
    ucout << "Caught std::range_error" << endl;
  }
  catch (std::runtime_error) {
    ucout << "Caught std::runtime_error" << endl;
  }
  catch (...) {
    ucout << "Caught unknown exception!" << endl;
  }

  //test x*y
  ucout << "x*y\t";
  try {
    ucout << (x*y) << endl;
  }
  catch (std::range_error) {
    ucout << "Caught std::range_error" << endl;
  }
  catch (std::runtime_error) {
    ucout << "Caught std::runtime_error" << endl;
  }
  catch (...) {
    ucout << "Caught unknown exception!" << endl;
  }

  //test x/y
  ucout << "x/y\t";
  try {
    ucout << (x/y) << endl;
  }
  catch (std::domain_error& err) {
    ucout << "Caught std::domain_error" << endl;
  }
  catch (std::range_error& err) {
    ucout << "Caught std::range_error" << endl;
  }
  catch (std::runtime_error& err) {
    ucout << "Caught std::runtime_error" << endl;
  }
  catch (...) {
    ucout << "Caught unknown exception!" << endl;
  }

  ucout << endl;
}

template <class T>
void test_cast(T& x)
{

  ucout << endl;
  //cast Ereal to double
  ucout << "Casting " << x << " to double: ";
  try {
    ucout << (double)x << endl;
  }
  catch (std::invalid_argument) {
    ucout << "Caught std::invalid_argument" << endl;
  }
  catch (std::runtime_error& err) {
    ucout << "Caught std::runtime_error" << endl;
  }
  catch (...) {
    ucout << "Caught unknown exception!" << endl;
  }

  //cast Ereal to int
  ucout << "Casting " << x << " to int: ";
  try {
    ucout << (int)x << endl;
  }
  catch (std::invalid_argument) {
    ucout << "Caught std::invalid_argument" << endl;
  }
  catch (std::runtime_error& err) {
    ucout << "Caught std::runtime_error" << endl;
  }
  catch (...) {
    ucout << "Caught unknown exception!" << endl;
  }
}

template <class T>
void test_round(const string& s, T& x, long int val)
{
  ucout << "lround(" << s << "):\t";
  long int ans;
  try {
    ans= lround(x);
  }
  catch (std::invalid_argument) {
    ucout << "Caught std::invalid_argument" << endl;
    return;
  }
  catch (std::runtime_error& err) {
    ucout << "Caught std::runtime_error" << endl;
    return;
  }
  catch (...) {
    ucout << "Caught unknown exception!" << endl;
    return;
  }
  //ucout << ans << '\t' << x;
  ucout << '\t' << (ans == val) << endl;
}

template <class T>
void test_unary(T& x)
{
  ucout << endl;
  //test unary -
  ucout <<  "testing -( " << x << " ): ";
  try {
    ucout << -x << endl;
  }
  catch (std::invalid_argument) {
    ucout << "caught std::invalid_argument" << endl;
  }
  catch (std::runtime_error& err) {
    ucout << "caught std::runtime_error" << endl;
  }
  catch (...) {
    ucout << "caught unknown exception!" << endl;
  }

  //test unary +
  ucout <<  "testing +( " << x << " ): ";
  try {
    ucout << +x << endl;
  }
  catch (std::invalid_argument) {
    ucout << "caught std::invalid_argument" << endl;
  }
  catch (std::runtime_error& err) {
    ucout << "caught std::runtime_error" << endl;
  }
  catch (...) {
    ucout << "caught unknown exception!" << endl;
  }

}

void run_tests()
{
utilib::Ereal<double> foo = -6;
utilib::Ereal<double> bar = 7;
utilib::Ereal<double> infhi = utilib::Ereal<double>::positive_infinity;
utilib::Ereal<double> inflo = utilib::Ereal<double>::negative_infinity;
utilib::Ereal<double> zero = utilib::Ereal<double>(0.0);
utilib::Ereal<double> nan = utilib::Ereal<double>::NaN;
utilib::Ereal<double> ind = utilib::Ereal<double>::indeterminate;
utilib::Ereal<double> invalid = utilib::Ereal<double>::invalid;

//test casting
ucout << endl << "=========================================================================" << endl;
ucout << "Testing casting:" << endl;
test_cast(foo);
test_cast(bar);
test_cast(infhi);
test_cast(inflo);
test_cast(zero);
test_cast(nan);
test_cast(ind);
test_cast(invalid);

//test rounding
ucout << endl << "=========================================================================" << endl;
ucout << "Testing rounding:" << endl << endl;
test_round("Infinity",infhi,LONG_MAX);
test_round("-Infinity",inflo,LONG_MIN);
test_round("NaN",nan,0);
test_round("Indeterminate",ind,0);
test_round("Invalid",invalid,0);
utilib::Ereal<double> rtmp = MAXDOUBLE;
test_round("MAXDOUBLE",rtmp,LONG_MAX);
rtmp = -MAXDOUBLE;
test_round("-MAXDOUBLE",rtmp,LONG_MIN);
rtmp = LONG_MAX;
test_round("LONG_MAX",rtmp,LONG_MAX);
rtmp = (LONG_MAX + 1.5);
test_round("LONG_MAX+1.5",rtmp,LONG_MAX);
rtmp = (LONG_MAX - 1.5);
test_round("LONG_MAX-1.5",rtmp,LONG_MAX-1);
rtmp = LONG_MIN;
test_round("LONG_MIN",rtmp,LONG_MIN);
rtmp = (LONG_MIN + 1.5);
test_round("LONG_MIN+1.5",rtmp,LONG_MIN+2);
rtmp = (LONG_MIN - 1.5);
test_round("LONG_MIN-1.5",rtmp,LONG_MIN);


//test unary operators
ucout << endl << "=========================================================================" << endl;
ucout << "Testing unary operations:" << endl;
test_unary(foo);
test_unary(bar);
test_unary(infhi);
test_unary(inflo);
test_unary(zero);
test_unary(nan);
test_unary(ind);
test_unary(invalid);

// test Ereal & Ereal
ucout << endl << "=========================================================================" << endl;
ucout << "Testing Ereal, Ereal:" << endl << endl;

//tests with finite values
compare(foo,foo);
compare(bar,bar);
compare(zero,zero);

compare(foo,bar);
compare(bar,foo);

compare(zero,foo);
compare(foo,zero);

compare(zero,bar);
compare(bar,zero);

// tests with one finite, one non-finite
compare(foo,inflo);
compare(inflo,foo);

compare(bar,inflo);
compare(inflo,bar);

compare(zero,inflo);
compare(inflo,zero);

compare(foo,infhi);
compare(infhi,foo);

compare(bar,infhi);
compare(infhi,bar);

compare(zero,infhi);
compare(infhi,zero);

compare(foo,ind);
compare(ind,foo);

compare(bar,ind);
compare(ind,bar);

compare(zero,ind);
compare(ind,zero);

compare(foo,nan);
compare(nan,foo);

compare(bar,nan);
compare(nan,bar);

compare(zero,nan);
compare(nan,zero);

compare(foo,invalid);
compare(invalid,foo);

compare(bar,invalid);
compare(invalid,bar);

compare(zero,invalid);
compare(invalid,zero);

// tests with both non-finite
compare(inflo,inflo);
compare(infhi,infhi);
compare(ind,ind);
compare(nan,nan);
compare(invalid,invalid);

compare(inflo,infhi);
compare(infhi,inflo);

compare(inflo,ind);
compare(ind,inflo);

compare(inflo,nan);
compare(nan,inflo);

compare(inflo,invalid);
compare(invalid,inflo);

compare(infhi,ind);
compare(ind,infhi);

compare(infhi,nan);
compare(nan,infhi);

compare(infhi,invalid);
compare(invalid,infhi);

compare(ind,nan);
compare(nan,ind);

compare(ind,invalid);
compare(invalid,ind);

compare(nan,invalid);
compare(invalid,nan);

// test Ereal & double
{
  ucout << endl << "=========================================================================" << endl;
  ucout << "Testing Ereal, double" << endl << endl;
  double tmp = 20;
  
  compare(tmp,foo);
  compare(foo,tmp);
  
  compare(tmp,bar);
  compare(bar,tmp);
  
  compare(tmp,zero);
  compare(zero,tmp);
  
  compare(tmp,infhi);
  compare(infhi,tmp);
  
  compare(tmp,inflo);
  compare(inflo,tmp);
  
  compare(tmp,ind);
  compare(ind,tmp);
  
  compare(tmp,nan);
  compare(nan,tmp);
  
  compare(tmp,invalid);
  compare(invalid,tmp);
}

{
  double tmp = -20;
  
  compare(tmp,foo);
  compare(foo,tmp);
  
  compare(tmp,bar);
  compare(bar,tmp);
  
  compare(tmp,zero);
  compare(zero,tmp);
  
  compare(tmp,infhi);
  compare(infhi,tmp);
  
  compare(tmp,inflo);
  compare(inflo,tmp);
  
  compare(tmp,ind);
  compare(ind,tmp);
  
  compare(tmp,nan);
  compare(nan,tmp);
  
  compare(tmp,invalid);
  compare(invalid,tmp);
}

// test Ereal & int
{
  ucout << endl << "=========================================================================" << endl;
  ucout << "Testing Ereal, int" << endl << endl;
  
  int tmp = -6;
  compare(tmp,foo);
  compare(foo,tmp);
  
  compare(tmp,bar);
  compare(bar,tmp);
  
  compare(tmp,zero);
  compare(zero,tmp);
  
  compare(tmp,infhi);
  compare(infhi,tmp);
  
  compare(tmp,inflo);
  compare(inflo,tmp);
  
  compare(tmp,ind);
  compare(ind,tmp);
  
  compare(tmp,nan);
  compare(nan,tmp);
  
  compare(tmp,invalid);
  compare(invalid,tmp);
}


} //end run_tests

void read_tests()
{
  string test = "0 6.00 -7.2 1.23456e+12 -8.76543e-4 -Infinity -Inf -infinity -inf +Infinity +Inf +infinity +inf Infinity Inf infinity inf Indeterminate Ind indeterminate ind NaN nan Invalid invalid Inv inv gibberish";
  istringstream all_tests(test);
  string foo;
  utilib::Ereal<double> bar;
  
  ucout << "String\tValue" << endl;
  
  try {
    while ( all_tests >> foo) {
      ucout << foo << '\t';
      istringstream test(foo);
      test >> bar;
      ucout << bar << endl;
    }
  }
  catch (runtime_error) {
    ucout << "Caught std::runtime_error" << endl;
  }
  catch (...) {
    ucout << "Caught unknown error" << endl;
  }
}
      
      

int main() {
  ucout << "=========================================================================" << endl;
  ucout << "Testing default error handling." << endl;
  run_tests();

  ucout << endl << "=========================================================================" << endl;
  ucout << "Testing conservative error handling." << endl;
  utilib::Ereal<double>::setConservativeError( !(utilib::Ereal<double>::getConservativeError()) );
  run_tests();
  utilib::Ereal<double>::setConservativeError( !(utilib::Ereal<double>::getConservativeError()) );

  ucout << endl << "=========================================================================" << endl;
  ucout << endl << "Testing input stream reader." << endl;
  read_tests();
}
