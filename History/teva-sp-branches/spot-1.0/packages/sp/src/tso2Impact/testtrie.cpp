#include <iostream>
#include <vector>

#include "VecTrie.h"

using namespace std;

main() {

  VecTrie<int, int> a;
  VecTrieKey<int> key1, key2, key3;
  vector<int> v1, v2;
  int d0 = 10;
//  int d1 = 11;
//  int d2 = 12;

  v1.push_back(1);
  v1.push_back(2);
  v1.push_back(3);
  v1.push_back(4);	// 1234
  key1 = v1;		// 1234

  v1[0] = 7;		// 7234
  key2 = v1;		// 7234

  v2 = key1.getkey();	// 1234
  v2[2] = 7;		// 1274
  key3 = v2;		// 1274

  cout << "v1:" << v1[0] << v1[1] << v1[2] << v1[3] << endl;
  cout << "v2:" << v2[0] << v2[1] << v2[2] << v2[3] << endl;
  cout << "k1:" << key1[0] << key1[1] << key1[2] << key1[3] << endl;
  cout << "k2:" << key2[0] << key2[1] << key2[2] << key2[3] << endl;
  cout << "k3:" << key3[0] << key3[1] << key3[2] << key3[3] << endl;

  a.insert(key1, d0);
  d0 = 11;
  a.insert(key2, d0);
  d0 = 12;
  a.insert(key3, d0);

  cout << "key1 data: " << a.getdata(key1) << endl;
  cout << "key2 data: " << a.getdata(key2) << endl;
  cout << "key3 data: " << a.getdata(key3) << endl;

  
  a.dumpnode(a.headnode());

  return 0;
}
