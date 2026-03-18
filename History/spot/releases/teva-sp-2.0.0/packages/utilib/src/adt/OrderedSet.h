/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file OrderedSet.h
 *
 * Defines the utilib::OrderedSet class
 */

#ifndef utilib_OrderedSet_h
#define utilib_OrderedSet_h

#include <utilib/utilib_config_bool.h>

#ifndef UTILIB_HAVE_STD
#include <stddef.h>
#include <stdlib.h>
#else
#include <cstddef>
#include <cstdlib>
#endif

#include <utilib/BasicArray.h>

namespace utilib {

typedef size_type OrderedSetIndex;


/**
 * An array-based set.
 *
 * \todo Use a hash table to make the 'index' operation efficient.
 */
template <class T>
class OrderedSet
{
public:

  /// Empty constructor that sets up an empty set.
  OrderedSet();

  /// Destructor.
  virtual ~OrderedSet() {}

  /// Adds an element to the set.
  virtual void add( const T& val , const bool unique=false );

  /// Adds all of the elements from another set.
  virtual void add( const OrderedSet<T>&, const bool unique=false );

  /// Removes the first element, and returns the data value in \a data.
  virtual void remove( T& val );

  /// Returns true if the set is empty
  bool empty() const
		{return (Len == 0);}

  /// Returns true if the set is not empty
  operator int() const
		{return (Len > 0);}

  /// Returns the number of items in the set.
  size_type size() const
		{return Len;}

  /// Returns the (zero-based) index of \c val (and -1 if not a member)
  int index(const T& val);

  /// Return the first element of the set, updating \c index.
  T& first( OrderedSetIndex& index, bool& status);

  /// Return the next element of the set, updating \c index.
  T& next( OrderedSetIndex& index, bool& status);

  /// Returns true if val is a member of the set
  bool is_member(const T& val)
	{return (index(val) != -1);}

  /// Returns true if set is disjoint from the current set.
  bool is_disjoint(const OrderedSet<T>& set);

protected:

  /// The number of items in the set.
  size_type Len;

  /// The array used for storing the set.
  BasicArray<T> Data;

};





template <class T>
OrderedSet<T>::OrderedSet()
 : Len(0), Data(1)
{ }


//
// Note: in order to make 'removes' effective, you need to be able to deal with
// 'holes' in the array.  This is messy, and I don't need this capability right
// now!  -- WEH
//
template <class T>
void OrderedSet<T>::remove( T& Value )
{
EXCEPTION_MNGR(runtime_error, "OrderedSet<T>::remove -- unimplemented method!")
}


template <class T>
void OrderedSet<T>::add( const T& val, const bool add_unique )
{
int ndx = index(val);
if (add_unique && (ndx != -1))
   return;	// OLD:  ndx;

if (Data.size() == Len)
   Data.resize(Len + std::max((Data.size() / 10),(size_type)10));

Data[Len] = val;
Len++;
return; 	// OLD:  (Len-1);
}



template <class T>
void OrderedSet<T>::add( const OrderedSet<T>& set, const bool add_unique )
{
for (size_type i=0; i<set.Len; i++)
  if (!add_unique || !is_member(set.Data[i]))
     add(set.Data[i]);
}



template <class T>
int OrderedSet<T>::index(const T& val)
{
for (size_type i=0; i<Len; i++)
  if (val == Data[i])
     return i;
return -1;
}


template <class T>
T& OrderedSet<T>::first( OrderedSetIndex& index, bool& status)
{
if (Len == 0) {
   status = false;
   return Data[0];
}
index = 1;
status=true;
return Data[0];
}


template <class T>
T& OrderedSet<T>::next( OrderedSetIndex& index, bool& status)
{
if (status == false) return Data[0];
if (index >= Len) {
   status = false;
   return Data[0];
   }
return Data[index++];
}

template <class T>
bool OrderedSet<T>::is_disjoint(const OrderedSet<T>& set)
{
for (size_type i=0; i<Len; i++)
  for (size_type j=0; j<set.Len; j++)
    if (Data[i] == set.Data[j]) return false;
return true;
}

} // namespace utilib

/// Stream operator for removing a set member
template <class T>
inline utilib::OrderedSet<T>& operator>>(utilib::OrderedSet<T>& list, T& val)
{list.remove(val); return list;}

/// Stream operator for adding a set member
template <class T>
inline utilib::OrderedSet<T>& operator<<(utilib::OrderedSet<T>& list, const T& val)
{list.add(val); return list;}

/// Out-stream operator for writing the contents of a OrderedSet<T>
template <class T>
inline std::ostream& operator<<(std::ostream& os, const utilib::OrderedSet<T>& obj)
{
os << "{ ";
if (obj.Len > 0) {
   os << obj.Data[0];
   for (size_type i=1; i<obj.Len; i++)
     os << ", " << obj.Data[i];
   os << " ";
   }
os << "}";
return os;
}

/// Out-stream operator for writing the contents of a OrderedSet<T>
template <class T>
inline utilib::PackBuffer& operator<<(utilib::PackBuffer& os,
                                        const utilib::OrderedSet<T>& obj)
{
os << obj.Len;
for (size_type i=0; i<obj.Len; i++)
  os << obj.Data[i];
return os;
}

/// In-stream operator for reading the contents of a OrderedSet<T>
template <class T>
inline std::istream& operator>>(std::istream& is, utilib::OrderedSet<T>& obj)
{ EXCEPTION_MNGR(std::runtime_error, "Undefined operator>>"); return is; }

/// In-stream operator for reading the contents of a OrderedSet<T>
template <class T>
inline utilib::UnPackBuffer& operator>>(utilib::UnPackBuffer& is,
                                        utilib::OrderedSet<T>& obj)
{
is >> obj.Len;
if (obj.Data.size() <= obj.Len)
   obj.Data.resize(std::max((size_type)1,obj.Len));
for (size_type i=0; i<obj.Len; i++)
  is >> obj.Data[i];
return is;
}

////
//// Define an iterator macro for ordered sets
////
#define ForAllElements(index,set,elt_type)\
if (set.size() > 0) {\
bool index ## __first=true, index ## __status=true;\
OrderedSetIndex index ## __ndx;\
while (index ## __status == true) {\
  const elt_type& index = (index ## __first == true ? set.first(index ## __ndx, index ## __status) : set.next(index ## __ndx, index ## __status));\
  index ## __first = false;\
  if (index ## __status == false) break;

#endif
