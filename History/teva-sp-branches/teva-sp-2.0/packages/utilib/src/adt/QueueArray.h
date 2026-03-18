/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file QueueArray.h
 *
 * Defines the utilib::QueueArray class
 */

#ifndef utilib_QueueArray_h
#define utilib_QueueArray_h

#include <utilib/utilib_config.h>
#ifndef UTILIB_HAVE_STD
#include <stddef.h>
#include <stdlib.h>
#else
#include <cstddef>
#include <cstdlib>
#endif

#include <utilib/BasicArray.h>

namespace utilib {


/**
 * An array-based queue.
 */
template <class T>
class QueueArray
{
public:

  /// Empty constructor that sets up an empty queue.
  QueueArray();

  /// Destructor.
  virtual ~QueueArray() {}

  /// Returns the value at the top of the queue, which is the next element 
  /// that will be removed.
  virtual T& top()
		{
		if (Len == 0)
		   EXCEPTION_MNGR(runtime_error, "QueueArray::top -- Empty stack.")
		return Data[First];
		}

  /// Adds a list item with data value \a data.
  virtual void add( const T& );

  /// Removes the next item, and returns the data value in \a data.
  virtual void remove( T& val );

  /// Returns true if the queue is empty
  int empty() const
		{return (Len == 0);}

  /// Returns true if the queue is not empty
  operator int() const
		{return (Len > 0);}

  /// Returns the number of items in the queue.
  size_type size() const
		{return Len;}

  /// Empties the QueueArray
  void clear();

protected:

  /// The number of items in the queue.
  size_type Len;

  /// The index of the first element in the queue.
  size_type First;

  /// The index of the last element in the queue.
  size_type Last;

  /// The array used for storing the queue.
  BasicArray<T> Data;

  /// The array of next elements in the queue.
  BasicArray<size_t> next;

};


//
// Note, we are using internal indeces that are 1-based, indexing into
// zero based arrays.  This is because the smallest value that the 'next'
// array can represent is zero.
//
template <class T>
QueueArray<T>::QueueArray()
 : Len(0), First(1), Last(1)
{ Data.resize(10); next.resize(10); next[0] = 0;}


template <class T>
void QueueArray<T>::clear()
{
Len=0;
First = 1;
Last = 1;
next[0] = 0;
}


template <class T>
void QueueArray<T>::remove( T& Value )
{
if (Len == 0)
   EXCEPTION_MNGR(runtime_error, "QueueArray<T>::remove -- empty list!")

size_type curr = First;
Value = Data[curr];

//
// Move the First pointer down the list
//
if (next[curr] == 0) {
   //
   // The 'next' value is empty, so return to initial conditions for
   // code.
   //
   Len=0;
   First = 1;
   Last = 1;
   }
else {
   Len--;
   First = next[curr];
   }

//
// Setup the 'free list'
//
size_type tmp = next[0];
next[0] = curr;
next[curr] = tmp;
}


template <class T>
void QueueArray<T>::add( const T& val )
{
Len++;
size_type curr;
if (next[0] == 0)
   curr = Len;
else {
   //
   // Remove an element from the free list
   //
   curr = next[0];
   next[0] = next[curr];
   }
if (Data.size() == curr) {
   Data.resize(curr + std::max((Data.size() / 10),(size_type)10));
   next.resize(curr + std::max((next.size() / 10),(size_type)10));
   }

if (Len == 1)
   First = curr;
next[curr] = 0;
if (Len > 1)
   next[Last] = curr;
Data[curr] = val;
Last = curr;
}



} // namespace utilib

/// Stream operator for removing element
template <class T>
inline utilib::QueueArray<T>& operator>>(utilib::QueueArray<T>& list, T& val)
{list.remove(val); return list;}

/// Stream operator for adding element
template <class T>
inline utilib::QueueArray<T>& operator<<(utilib::QueueArray<T>& list, const T& val)
{list.add(val); return list;}

/// Out-stream operator for writing the contents of a QueueArray<T>
template <class T>
inline std::ostream& operator<<(std::ostream& os, const utilib::QueueArray<T>& obj)
{
os << obj.Len << " [ ";
if (obj.Len > 0) {
   size_type curr = obj.First;
   os << obj.Data[curr];
   for (size_type i=1; i<obj.Len; i++) {
     curr = obj.next[curr] ;
     os << ", " << obj.Data[curr];
     }
   os << " ";
   }
os << "]";
return os;
}

/// Out-stream operator for writing the contents of a QueueArray<T>
template <class T>
inline utilib::PackBuffer& operator<<(utilib::PackBuffer& os,
                                        const utilib::QueueArray<T>& obj)
{
os << obj.Len;
size_type curr = obj.First;
for (size_type i=0; i<obj.Len; i++) {
  os << obj.Data[curr];
  curr = obj.next[curr];
  }
return os;
}

/// In-stream operator for reading the contents of a QueueArray<T>
template <class T>
inline std::istream& operator>>(std::istream& is, utilib::QueueArray<T>& obj)
{ EXCEPTION_MNGR(std::runtime_error, "Undefined operator>>"); return(is); }

/// In-stream operator for reading the contents of a QueueArray<T>
template <class T>
inline utilib::UnPackBuffer& operator>>(utilib::UnPackBuffer& is,
                                        utilib::QueueArray<T>& obj)
{
is >> obj.Len;
if (obj.Data.size() <= (obj.Len+1))
   obj.Data.resize(std::max((size_type)1,(obj.Len+1)));
obj.Len=0;
obj.First = 1;
obj.Last = 1;
for (size_type i=0; i<obj.Len; i++) {
  is >> obj.Data[obj.Len];
  obj.add(obj.Data[obj.Len]);
  }
return is;
}
#endif
