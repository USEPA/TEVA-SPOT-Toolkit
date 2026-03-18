
#include <utilib/HashedSet.h>
#include <utilib/BasicArray.h>

using namespace utilib;


int main()
{
BasicArray<int> tmp(3);
tmp << 0;
HashedSet<BasicArray<int> > set1;

int ndx=0;
for (int i=0; i<10; i++) {
  tmp[ndx]++;
  ndx = (ndx+1) % 3;
  set1.insert(tmp);
  }

HashedSet<BasicArray<int> >::iterator curr = set1.begin();
HashedSet<BasicArray<int> >::iterator end  = set1.end();
while (curr != end) {
  cout << (*curr) << endl;
  curr++;
  };

HashedSet<BasicArray<int>,true> set2;

return 0;
}
