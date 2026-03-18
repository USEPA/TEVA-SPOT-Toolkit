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

#ifndef ANSI_HDRS
#include <stddef.h>
#include <stdlib.h>
#else
#include <cstddef>
#include <cstdlib>
#endif

#include <utilib/PackBuf.h>
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

/*
        /// Accesss the i-th element of the array and returns a const.
  const T & operator[](const int i) const
		{return Data[i];}
*/

  /// Write out the set to an output stream.
  void write(ostream& os) const;

  /// Read the set from an input stream.
  void read(istream& is);

  /// Write out the set to a pack buffer.
  void write(PackBuffer& os) const;

  /// Read the set from an unpack buffer.
  void read(UnPackBuffer& is);

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


template <class T>
void OrderedSet<T>::write(ostream& os) const
{
os << "{ ";
if (Len > 0) {
   os << Data[0];
   for (size_type i=1; i<Len; i++)
     os << ", " << Data[i];
   os << " ";
   }
os << "}";
}


template <class T>
void OrderedSet<T>::read(istream& /*is*/)
{
}


template <class T>
void OrderedSet<T>::write(PackBuffer& os) const
{
os << Len;
for (size_type i=0; i<Len; i++)
  os << Data[i];
}


template <class T>
void OrderedSet<T>::read(UnPackBuffer& is)
{
is >> Len;
if (Data.size() <= Len)
   Data.resize(std::max((size_type)1,Len));
for (size_type i=0; i<Len; i++)
  is >> Data[i];
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


} // namespace utilib

/// Stream operator for removing a set member
template <class T>
inline utilib::OrderedSet<T>& operator>>(utilib::OrderedSet<T>& list, T& val)
{list.remove(val); return list;}

/// Stream operator for adding a set member
template <class T>
inline utilib::OrderedSet<T>& operator<<(utilib::OrderedSet<T>& list, const T& val)
{list.add(val); return list;}

/// Stream operator for writing
template <class T>
inline ostream& operator<<(ostream& output, const utilib::OrderedSet<T>& set)
{ set.write(output); return(output); }

/// Stream operator for reading
template <class T>
inline istream& operator>>(istream& input, utilib::OrderedSet<T>& set)
{ set.read(input); return(input); }

/// Stream operator for packing
template <class T>
inline utilib::PackBuffer& operator<<(utilib::PackBuffer& output, const utilib::OrderedSet<T>& set)
{ set.write(output); return(output); }

/// Stream operator for unpacking
template <class T>
inline utilib::UnPackBuffer& operator>>(utilib::UnPackBuffer& input, utilib::OrderedSet<T>& set)
{ set.read(input); return(input); }

#endif
