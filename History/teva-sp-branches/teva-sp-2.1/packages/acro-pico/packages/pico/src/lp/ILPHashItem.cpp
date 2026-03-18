/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// ILPHashItem.cpp
//

#include <acro_config.h>
#include <utilib/std_headers.h>
#include <utilib/hash_fn.h>
#include <pico/ILPHashItem.h>

using namespace std;

namespace pico {


void ILPHashItem::write(ostream& os) const
{ os << name << " " << index << endl; }


void ILPHashItem::read(istream& is)
{ is >> name >> index; }


size_type ILPHashItem::hash(size_type size) const
{ return utilib::hash_fn(name,size); }


int ILPHashItem::compare(const ILPHashItem* key) const
{ return strcmp(name.data(),key->name.data()); }


void ILPSymbItem::write(ostream& os) const
{
ILPHashItem::write(os);
for (size_type i=0; i<args.size(); i++) {
  os << "  arg" << i << " ";
  if (args[i].string_set) 
     os << "sym(" << args[i].index << "):" << (*args[i].string_set)  << endl;
  else
     os << "num(" << args[i].index << "):" << (*args[i].int_set)  << endl;
  }
os << "  RC    " << rc_flag << endl;
os << "  MAP   " << map << endl;
}

} // namespace pico
