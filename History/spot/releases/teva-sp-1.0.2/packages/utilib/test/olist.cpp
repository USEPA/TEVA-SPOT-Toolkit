/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// olist.cpp
//
// Test routine for ordered lists
//

#include <utilib/CommonIO.h>
#include <utilib/OrderedList.h>

using namespace utilib;


void do_write(const OrderedList<void*,double>& olist)
{
ucout << "LIST: " << Flush;
OrderedListItem<void*,double>* item=olist.head();
while (item) {
      ucout << "(" << item->key()  << ") ";
      item = olist.next(item);
      }
ucout << endl;
}


int main()
{
int i=0;

while (i++ < 100) {
OrderedList<void*,double>* olist = new OrderedList<void*,double>();
void* ptr;
double key;

key=1.0;
olist->add(ptr,key);
do_write(*olist);

key=2.0;
olist->add(ptr,key);
do_write(*olist);

key=3.0;
olist->add(ptr,key);
do_write(*olist);

delete olist;
}

return 0;
}
