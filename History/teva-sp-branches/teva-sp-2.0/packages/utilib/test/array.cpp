/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

#include <utilib/utilib_config.h>
#ifndef UTILIB_HAVE_STD
#include <fstream.h>
#include <string.h>
#else
#include <fstream>
#include <cstring>
#endif

#include <utilib/IntVector.h>
#include <utilib/CharString.h>

#ifdef UTILIB_HAVE_NAMESPACES
using namespace std;
#endif
using utilib::BasicArray;
using utilib::CharString;
using utilib::IntVector;

//#define DEBUG

void dump(BasicArray<IntVector>& a,int max_elements=0)
{
#if defined(DEBUG)
  for(size_type i=0; i<a.size(); i++)
    {
      cout << i << ": ";
      a[i].debug_print(cout,max_elements);
    }
  cout << endl;
#endif
}


class thang 
{
public:
  
  static int counter;
  int myNum;
  int copyNum;
  
  thang() 
  {
    myNum = ++counter;
    copyNum = myNum;
    cout << "Thang " << myNum << '/' << copyNum << " created.\n";
  };

  ~thang()
  {
    cout << "Thang " << myNum << '/' << copyNum << " destroyed.\n";
  };

  thang& operator=(thang& otherThang)
  {
    copyNum = otherThang.copyNum;
    cout << "Thang " << myNum << " copied from thang " << copyNum << endl;
    return *this;
  };
  
};


ostream& operator<<(ostream& s, const thang& t)
{
s << t.myNum << '/' << t.copyNum; return s;
}



int thang::counter = 0;


int main(int argc, char* argv[])
{
  size_type i;
  
  // Check basic operations...

  BasicArray<IntVector> a(5);

  cout << "Initial a:\n"; dump(a);

#if 1
  BasicArray<IntVector>::iterator curr = a.begin();
  BasicArray<IntVector>::iterator end  = a.end();
  while (curr != end)
    {
      *curr &= a[0];
      cout << "Linked a:\n"; dump(a);
    curr++;
    }
#else
   for (unsigned int i=1; i<a.size(); i++)
    {
      a[i] &= a[0];
      cout << "Linked a:\n"; dump(a);
    }
#endif

  a[0].resize(10);

  cout << "Resized a:\n"; dump(a,100);

  cout << "Assigning...\n";
  a[0] << 1;

  cout << "After assignment a:\n"; dump(a,100);

  IntVector b(2);
  b << 2;

  cout << "b: ";
  #if defined(DEBUG)
  b.debug_print(cout,100);
  #endif

  a[1] &= b;

  cout << "\na after a[1] &= b:\n"; dump(a,100);
  cout << "b: ";
  #if defined(DEBUG)
  b.debug_print(cout,100);
  #endif

  int cdata[3];
  IntVector c(3,cdata,utilib::DataNotOwned);
  c << 3;

  cout << "\nShould be 4 2 1: " << a[0].nrefs() << ' '
       << b.nrefs() << ' ' << c.nrefs() << endl;

  cout << "Should be 1 1 0: " << a[0].shared_mem() << ' '
       << b.shared_mem() << ' ' << c.shared_mem() << endl;

  cout << "\nc: ";
  #if defined(DEBUG)
  c.debug_print(cout,100);
  #endif

  a[0].resize(5);
  
  cout << "\na after a[0].resize(5):\n"; dump(a,100);

  a[2] &= c;
  
  cout << "\na after a[2] &= c:\n"; dump(a,100);
  cout << "b: ";
  #if defined(DEBUG)
  b.debug_print(cout,100);
  #endif
  cout << "\nc: ";
  #if defined(DEBUG)
  c.debug_print(cout,100);
  #endif

  a[0] << c;

  cout << "\na after a[0] = c:\n"; dump(a,100);
  cout << "b: ";
  #if defined(DEBUG)
  b.debug_print(cout,100);
  #endif
  cout << "\nc: ";
  #if defined(DEBUG)
  c.debug_print(cout,100);
  #endif

  c[1] = -12;
  cout << "\nc: ";
  #if defined(DEBUG)
  c.debug_print(cout,100);
  #endif

  b.resize(3);
  cout << "\na after b.resize(3):\n"; dump(a,100);
  cout << "b: ";
  #if defined(DEBUG)
  b.debug_print(cout,100);
  #endif

  b << c;
  cout << "\na after b << c:\n"; dump(a,100);
  cout << "b: ";
  #if defined(DEBUG)
  b.debug_print(cout,100);
  #endif
  cout << "\nc: ";
  #if defined(DEBUG)
  c.debug_print(cout,100);
  #endif

  // Test memory allocation and deletion.

  cout << "\nMaking 5 thangs in t1:\n";
  BasicArray<thang> t1(5);

  cout << "\nResizing t1 to 6 thangs:\n";
  t1.resize(6);

  cout << "\nMaking another 3 thangs in t2:\n";
  BasicArray<thang> t2(3);
  
  cout << "\nt2 &= t1\n";
  t2 &= t1;

  cout << "\nMaking a pointer array of 4 thangs:\n";
  thang* data = new thang[4];
  
  cout << "\nCreating t3 to point to it:\n";
  BasicArray<thang> t3(4,data,utilib::DataNotOwned);

  cout << "\nt3 &= t1:\n";
  t3 &= t1;

  cout << "\nt2.resize(8):\n";
  t2.resize(8);

  cout << "\nDeleting the first pointer array:\n";
  delete[] data;

  cout << "\nMaking a new pointer array of 7 thangs:\n";
  data = new thang[7];

  cout << "\nSetting t1 data to the new 7 thangs with assumption:\n";
  t1.set_data(7,data,utilib::AssumeOwnership);

  // Test chaining of objects.

  BasicArray<thang>* d[4];

  for(i=0; i<4; i++)
    d[i] = new BasicArray<thang>;
  
  cout << "\nd's:\n";
  #if defined(DEBUG)
  for(i=0; i<4; i++)
    {
      cout << i << ": ";
      d[i]->debug_print(cout,0);
    }
  #endif

  cout << "\nChaining and resizing:\n";
  *d[0] &= *d[1];
  *d[2] &= *d[3];
  d[0]->resize(2);
  d[2]->resize(3);

  cout << "\nd's after first chaining:\n";
  #if defined(DEBUG)
  for(i=0; i<4; i++)
    {
      cout << i << ": ";
      d[i]->debug_print(cout,0);
    }
  #endif

  cout << "\nChaining again:\n";
  *d[1] &= *d[2];
  
  cout << "\nd's after second chaining:\n";
  #if defined(DEBUG)
  for(i=0; i<4; i++)
    {
      cout << i << ": ";
      d[i]->debug_print(cout,0);
    }
  #endif

  cout << "\nChaining yet again:\n";
  *d[0] &= *d[1];
  
  cout << "\nd's after third chaining:\n";
  #if defined(DEBUG)
  for(i=0; i<4; i++)
    {
      cout << i << ": ";
      d[i]->debug_print(cout,0);
    }
  #endif

  cout << "\nDeleting d[1]:\n";
  delete d[1];
  d[1] = NULL;
  #if defined(DEBUG)
  for(i=0; i<4; i++)
    {
      cout << i << ": ";
      if (d[i])
	d[i]->debug_print(cout,0);
      else
	cout << "NULL\n";
    }
  #endif

  cout << "\nDeleting d[0]:\n";
  delete d[0];
  d[0] = NULL;
  #if defined(DEBUG)
  for(i=0; i<4; i++)
    {
      cout << i << ": ";
      if (d[i])
	d[i]->debug_print(cout,0);
      else
	cout << "NULL\n";
    }
  #endif

  cout << "\nDeleting d[3]:\n";
  delete d[3];
  d[3] = NULL;
  #if defined(DEBUG)
  for(i=0; i<4; i++)
    {
      cout << i << ": ";
      if (d[i])
	d[i]->debug_print(cout,0);
      else
	cout << "NULL\n";
    }
  #endif

  cout << "\nDeleting d[2]:\n";
  delete d[2];
  d[2] = NULL;
  #if defined(DEBUG)
  for(i=0; i<4; i++)
    {
      cout << i << ": ";
      if (d[i])
	d[i]->debug_print(cout,0);
      else
	cout << "NULL\n";
    }
  #endif

  // Test CharStrings

  CharString s1("Hello there");
  cout << "\nMaking strings:\n";
  CharString s2(s1);
  CharString s3(20);

  #if defined(DEBUG)
  cout << "s1: ";
  s1.debug_print(cout,100);
  cout << "s2: ";
  s2.debug_print(cout,100);
  cout << "s3: ";
  s3.debug_print(cout,100);
  #endif

  cout << "\nResized s2 to 5:\n";
  s2.resize(5);

  #if defined(DEBUG)
  cout << "s1: ";
  s1.debug_print(cout,100);
  cout << "s2: ";
  s2.debug_print(cout,100);
  cout << "s3: ";
  s3.debug_print(cout,100);
  #endif

  cout << "\ns3 = s2:\n";
  s3 = s2;
  
  #if defined(DEBUG)
  cout << "s1: ";
  s1.debug_print(cout,100);
  cout << "s2: ";
  s2.debug_print(cout,100);
  cout << "s3: ";
  s3.debug_print(cout,100);
  #endif

  cout << "\nResized s2 to 4:\n";
  s2.resize(4);

  #if defined(DEBUG)
  cout << "s1: ";
  s1.debug_print(cout,100);
  cout << "s2: ";
  s2.debug_print(cout,100);
  cout << "s3: ";
  s3.debug_print(cout,100);
  #endif

  cout << "\ns3 &= s1:\n";
  s3 &= s1;

  #if defined(DEBUG)
  cout << "s1: ";
  s1.debug_print(cout,100);
  cout << "s2: ";
  s2.debug_print(cout,100);
  cout << "s3: ";
  s3.debug_print(cout,100);
  #endif

  cout << "\ns1 = Woof\n";
  s1 = "Woof";

  #if defined(DEBUG)
  cout << "s1: ";
  s1.debug_print(cout,100);
  cout << "s2: ";
  s2.debug_print(cout,100);
  cout << "s3: ";
  s3.debug_print(cout,100);
  #endif

  cout << "\nCharString comparison tests:\n";
  s1 = "aaaa";
  s2 = "zzz";
  cout << s1.compare(s2) << ' ' << s2.compare(s1) << ' ';
  cout << (s1 < s2) << (s2 < s1) << (s1 == s2) << (s2 == s1) 
       << (s1 > s2) << (s2 > s1) << (s1 != s2) << (s2 != s1) << endl;
  s2 = "aaa";
  cout << s1.compare(s2) << ' ' << s2.compare(s1) << ' ';
  cout << (s1 < s2) << (s2 < s1) << (s1 == s2) << (s2 == s1) 
       << (s1 > s2) << (s2 > s1) << (s1 != s2) << (s2 != s1) << endl;
  s2 = "aaaa";
  cout << s1.compare(s2) << ' ' << s2.compare(s1) << ' ';
  cout << (s1 < s2) << (s2 < s1) << (s1 == s2) << (s2 == s1) 
       << (s1 > s2) << (s2 > s1) << (s1 != s2) << (s2 != s1) << endl;
  s2 &= s1;
  cout << s1.compare(s2) << ' ' << s2.compare(s1) << ' ';
  cout << (s1 < s2) << (s2 < s1) << (s1 == s2) << (s2 == s1) 
       << (s1 > s2) << (s2 > s1) << (s1 != s2) << (s2 != s1) << endl;
  s1 = "abcdefg";
  cout << s1.compare("bcd") << ' ';
  cout << (s1 < (const char*) "bcd") << (s1 == "bcd")
       << (s1 > (const char*) "bcd") << (s1 != "bcd") << endl;
  cout << s1.compare("abc") << ' ';
  cout << (s1 < (const char*) "abc") << (s1 == "abc")
       << (s1 > (const char*) "abc") << (s1 != "abc") << endl;
  cout << s1.compare("aaaaaaaaaa") << ' ';
  cout << (s1 < (const char*) "aaaaaaaaaa") << (s1 == "aaaaaaaaaa")
       << (s1 > (const char*) "aaaaaaaaaa") << (s1 != "aaaaaaaaaa") << endl;
  cout << s1.compare("abcdefg") << ' ';
  cout << (s1 < (const char*) "abcdefg") << (s1 == "abcdefg")
       << (s1 > (const char*) "abcdefg") << (s1 != "abcdefg") << endl;
  s2.resize(0);
  cout << s1.compare(s2) << ' ' << s2.compare(s1) << ' ';
  cout << (s1 < s2) << (s2 < s1) << (s1 == s2) << (s2 == s1) 
       << (s1 > s2) << (s2 > s1) << (s1 != s2) << (s2 != s1) << endl;
  s1.resize(0);
  cout << s1.compare(s2) << ' ' << s2.compare(s1) << ' ';
  cout << (s1 < s2) << (s2 < s1) << (s1 == s2) << (s2 == s1) 
       << (s1 > s2) << (s2 > s1) << (s1 != s2) << (s2 != s1) << endl;
  

  // Tests to see if the array can be indexed by different types of data;
  BasicArray<int> foo(1);
  foo[0] = -99;
  int val;

  int i1=0;
  unsigned int i2=0;
  short int i3=0;
  unsigned short int i4=0;
  long int i5=0;
  unsigned long int i6=0;
  long long int i7=0;
  unsigned long long int i8=0;

  val = foo[i1];
  val = foo[i2];
  val = foo[i3];
  val = foo[i4];
  val = foo[i5];
  val = foo[i6];
  val = foo[i7];
  val = foo[i8];

  BasicArray<int> bar(3);
  bar << 0;

  cout << "\nDone.\n";
  return 0;
}
