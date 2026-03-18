//
// string.cpp
//

#include <utilib/utilib.h>
//#include <utilib/string_ops.h>

#include <iostream>

using namespace utilib;


int main()
{
CharString str = "a.bb.ccc.dddd.eeee.";
cout << str << endl;

BasicArray<CharString> strs = split(str,'.');
cout << strs << endl;

str = join(strs,'-');
cout << str << endl;

long int a;
int status;
a = aslong("  1",status);
if ((a != 1) || (status != OK)) cout << "Error: aslong test 1" << endl;
a = aslong("  2e1",status);
if ((a != 20)|| (status != OK))  cout << "Error: aslong test 2" << endl;
a = aslong("  3e+1",status);
if ((a != 30) || (status != OK)) cout << "Error: aslong test 3" << endl;
a = aslong("  4.1e+1",status);
if ((a != 41)|| (status != OK))  cout << "Error: aslong test 4" << endl;
a = aslong("  5.1e+2",status);
if ((a != 510)|| (status != OK))  cout << "Error: aslong test 5" << endl;
a = aslong("2.4e+7",status);
if ((a != 24000000)|| (status != OK))  cout << "Error: aslong test 6" << endl;
//cin >> str;
//cout << str;

return 0;
}
