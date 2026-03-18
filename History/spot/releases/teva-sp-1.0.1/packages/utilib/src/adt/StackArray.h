/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file StackArray.h
 *
 * Defines the utilib::StackArray class
 */

#ifndef utilib_StackArray_h
#define utilib_StackArray_h

#ifdef NON_ANSI
#include <iostream.h>
#else
#include <iostream>
#endif

#ifndef ANSI_HDRS
#include <stddef.h>
#include <stdlib.h>
#else
#include <cstddef>
#include <cstdlib>
#endif

#include <utilib/_math.h>
#include <utilib/BasicArray.h>

#ifdef ANSI_NAMESPACES
using namespace std;
#endif

namespace utilib {


/**
 * An array-based stack.
 */
template <class T>
class StackArray
{
public:

  /// Empty constructor that sets up an empty stack.
  StackArray();

  /// Constructor that sets up an empty stack using a shared array.
  StackArray(BasicArray<T>& data);

  /// Destructor.
  virtual ~StackArray() {}

  /// Returns the value at the top of the stack, which is the next element 
  /// that will be removed.
  virtual T& top()
		{
		if (Len == 0)
		   EXCEPTION_MNGR(runtime_error, "StackArray::top -- Empty stack.")
		return Data[Len-1];
		}

  /// Adds a item with data value \a data.
  virtual void add( const T& );

  /// Removes the next item, and returns the data value in \a data.
  virtual void remove( T& );

  /// Returns true if the stack is empty
  bool empty() const
		{return (Len == 0);}

  /// Returns true if the stack is not empty
  operator int() const
		{return (Len > 0);}

  /// Returns the number of items in the stack.
  size_type size() const
		{return Len;}

  /// Get the data from the stack
  BasicArray<T>& get_data()
		{Data.resize(Len); return(Data);}

protected:

  /// The number of items in the stack.
  size_type Len;

  /// The array used for storing the stack.
  BasicArray<T> Data;

};




template <class T>
StackArray<T>::StackArray()
 : Len(0)
{ }


template <class T>
StackArray<T>::StackArray(BasicArray<T>& data)
 : Len(0)
{ Data &= data; }


template <class T>
void StackArray<T>::remove( T& Value )
{
if (Len == 0)
   EXCEPTION_MNGR(runtime_error, "StackArray<T>::remove -- empty list!")

Value = Data[--Len];
}


template <class T>
void StackArray<T>::add( const T& val )
{
if (Data.size() == Len)
   Data.resize(max((Data.size() / 10),10));
Data[Len++] = val;
}

} // namespace utilib

/// Remove an element from the stack
template <class T>
inline utilib::StackArray<T>& operator>>(utilib::StackArray<T>& list, T& val)
{list.remove(val); return list;}

/// Add an element to the stack
template <class T>
inline utilib::StackArray<T>& operator<<(utilib::StackArray<T>& list, const T& val)
{list.add(val); return list;}


#endif
