#include <utilib/utilib_config.h>
#ifdef UTILIB_HAVE_STD
#include <iostream>
#else
#include <iostream.h>
#endif

#include <utilib/LPHashTable.h>
#include <utilib/stl_auxillary.h>
#include <utilib/BasicArray.h>

using namespace std;
using namespace utilib;

void test()
{
LPHashTable<BasicArray<double>, double > htable;
BasicArray<double> foo(3);
foo[0] = 1.0;
foo[1] = 2.0;
foo[2] = 3.0;
double value = 4.0;
htable.add(foo, value);

if (htable.find(foo) == htable.end())
   cerr << "THIS IS AN ERROR!" << endl;

LPHashTable<BasicArray<double>, double>::iterator item = htable.find(foo);
cerr << "Value: " << item->info() << endl;
}


int main()
{
test();

utilib::LPHashTable< BasicArray<double>, double > ht;

BasicArray<double> tmp(3);
tmp << 1.0;

int i=0;
int ndx=0;

while (ndx < 100) {
  tmp[i] /= 10.0;
  double val = ndx;
  cout << val << " " << tmp << endl;
  ht.add(tmp,val);
  i = (i+1) % 3;
  ndx++;
  }

BasicArray<BasicArray<double> > data(21);
LPHashTable<BasicArray<double>, double>::iterator curr = ht.begin();
LPHashTable<BasicArray<double>, double>::iterator end  = ht.end();
while (curr != end) {
  data[ static_cast<int>(curr->info())] = curr->key();
  curr++;
  }

for (size_type i=0; i<data.size(); i++)
  cout << i << "  " << data[i] << endl;

return 0;
}
