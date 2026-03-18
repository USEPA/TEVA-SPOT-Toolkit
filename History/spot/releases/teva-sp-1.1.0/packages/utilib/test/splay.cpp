/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// splay.cpp
//

#ifdef NON_ANSI
#include <fstream.h>
#else
#include <fstream>
#endif

#ifndef ANSI_HDRS
#include <math.h>
#else
#include <cmath>
using namespace std;
#endif

#include <utilib/_math.h>
#include <utilib/CharString.h>
#include <utilib/SimpleSplayTree.h>
#include <utilib/GenericSplayTree.h>
#include <utilib/ParameterSet.h>

using namespace utilib;

bool debug=false;

#define LIMIT 100

template <class T>
class A {

public:

  A() {}

  int compare(const A<T>& tmp) const;

  T val;

  int write(ostream& os) const
	{os << val; return OK;}

  int read(istream& is)
	{is >> val; return OK;}

  A<T>& operator=(A<T>& tmp)
	{val = tmp.val; return *this;}
};


template <class T>
int A<T>::compare(const A<T>& tmp) const
{
int foo = utilib::compare(val,tmp.val);
return foo;
}



template <class T>
double test_fn(BasicArray<T>& x)
{
double ans=0.0;
for (size_type i=0; i<x.size(); i++)
  ans += x[i]*x[i];
return ans;
}


template <class T>
void fill_table(SimpleSplayTree<BasicArray<T> >& table)
{
BasicArray<T> tmp(5);
tmp << (T) (sqrt(2.0)*100);
T delta= (T) 1.0;
table.add(tmp);

int j=0; 
while (j < LIMIT) {
  if (debug)
     cerr << j << "\t" << tmp << "\t" << delta << endl;
  bool flag=false;
  int i=0;
  double curr = test_fn(tmp);
  while ((i<5) && !flag) {
    T foo=tmp[i];
    tmp[i] = foo-delta;
    if (test_fn(tmp) < curr) {
       table.add(tmp);
       flag=true;
       j++;
       break;
       }
    tmp[i] = foo+delta;
    if (test_fn(tmp) < curr) {
       table.add(tmp);
       flag=true;
       j++;
       break;
       }
    tmp[i] = foo;
    i++;
    }
  if (!flag)
     delta = (T) (delta*0.9);
  if (delta < 1e-16)
     break;
  }
}



#define FN(x) (fabs(x)*fabs(x))

void fill_table(SimpleSplayTree<double>& table)
{
double tmp = sqrt(2.0);
double delta=1.0;
table.add(tmp);

int j=0; 
while (j < LIMIT) {
  double curr = FN(tmp);
  if (FN(tmp-delta) < curr) {
     tmp = tmp-delta;
     table.add(tmp);
     j++;
     }
  else if (FN(tmp+delta) < curr) {
     tmp = tmp+delta;
     table.add(tmp);
     j++;
     }
  else {
     delta *= 0.9;
     }
  }
}


void fill_table(SimpleSplayTree<int>& table)
{
int tmp;
for (int i=0; i<LIMIT; i++) {
  tmp = i*i*i;
  table.add(tmp);
  }
}


void fill_table(GenericSplayTree<A<int> >& table)
{
A<int>* tmp;
for (int i=0; i<LIMIT; i++) {
  tmp = new A<int>;
  tmp->val = i*i*i;
  table.add(*tmp);

  tmp = new A<int>;
  tmp->val = i*i*i;
  table.add(*tmp);
  }
}


void fill_table(GenericSplayTree<A<CharString> >& table)
{
ifstream hash_data("hash.data");
A<CharString>* tmp;
tmp = new A<CharString>;
hash_data >> tmp->val;
while (hash_data) {
  table.add(*tmp);
  tmp = new A<CharString>;
  hash_data >> tmp->val;
  }
}


template <class T>
void do_stest(const CharString& msg, SimpleSplayTree<T>& tree, ostream& os)
{
os << "\\" << endl;
os << "\\" << endl;
os << "\\" << endl;
os << msg << endl;
os << "\\" << endl;
os << "\\" << endl;
os << "\\" << endl;
fill_table(tree);
SimpleSplayTree<T> mytree;
mytree = tree;

os << "Sending the splay tree to 'os'" << endl;
os << mytree << endl;
}


template <class T>
void do_test(const CharString& msg, GenericSplayTree<A<T> >& tree, ostream& os)
{
os << "\\" << endl;
os << "\\" << endl;
os << "\\" << endl;
os << msg << endl;
os << "\\" << endl;
os << "\\" << endl;
os << "\\" << endl;
fill_table(tree);
GenericSplayTree<A<T> > mytree;
mytree = tree;

os << "Sending the splay tree to 'os'" << endl;
os << mytree << endl;
}



int main(int argc, char** argv)
{
  ParameterSet params;
  params.create_parameter("debug",debug,"","","");

  if (!params.process_parameters(argc,argv)) {
     cerr << "Unknown option: " << argv[1] << endl;
     return -1;
     }
  if (params.get_parameter<bool>("help")) {
     params.write_parameters(cout);
     return -1;
     }

//
// HASHING BasicArray<double>
//
{
//ofstream ofstr("splay-array-double");
SimpleSplayTree<BasicArray<double> > test_double;
do_stest("Testing BasicArray<double> simpleST",test_double,cout);
}


//
// HASHING BasicArray<int>
//
{
//ofstream ofstr("splay-array-int");
SimpleSplayTree<BasicArray<int> > test_int;
do_stest("Testing BasicArray<int> simpleST",test_int,cout);
}


//
// SPLAY TREE OF DOUBLES
//
{
//ofstream ofstr("splay-double");
SimpleSplayTree<double> test_double;
do_stest("Testing double simpleST",test_double,cout);
}


//
// SPLAY TREE OF INTEGERS
//
{
//ofstream ofstr("splay-int");
SimpleSplayTree<int> test_int;
do_stest("Testing int simpleST",test_int,cout);
}


//
// HASHING STRINGS
//
{
//ofstream ofstr("splay-str");
GenericSplayTree<A<CharString> > foo;
do_test("Testing char* HT with default function",foo,cout);
}

return OK;
}
