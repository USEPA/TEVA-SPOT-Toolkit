/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */


#include <utilib/Tuple.h>
#include <utilib/CharString.h>
#include <utilib/CommonIO.h>

using namespace utilib;

int main()
{
utilib::Tuple1<int> a(3);
utilib::Tuple2<int,utilib::CharString> b(4,"boo");
utilib::Tuple7<int,double,utilib::CharString,char*,int,int,int> e(1,2,"aa","b",3,4,5);
ucout << a << endl;
ucout << b << endl;
ucout << e << endl;
}


