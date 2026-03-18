/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file GenericHeap.h
 *
 * Defines the utilib::GenericHeapCompare, utilib::GenericHeapItem,
 * utilib::GenericHeapBase, utilib::GenericHeap and 
 * utilib::LocatedGenericHeap classes.
 */

#ifndef utilib_GenericHeap_h
#define utilib_GenericHeap_h

#include <utilib/utilib_config_explicit.h>
#include <utilib/_generic.h>
#include <utilib/AbstractHeap.h>
 
namespace utilib {

template <class T>
class GenericHeapBase;


/**
 * The comparison class used for utilib::GenericHeap objects
 */
template <class Type>
struct GenericHeapCompare : public ComparisonBase<Type>, public Comparison
{
public:

  ///
  compare_type operator()(const Type& _arg1, const Type& _arg2) const
        {
	int ans = _arg1.compare(_arg2);
        if (ans < 0) return better;
        if (ans > 0) return worse;
        return equal;
        }
};



/**
 * An item in a utilib::GenericHeap
 */
template <class T>
class GenericHeapItem 
{
  friend class GenericHeapBase<GenericHeapItem<T> >;

public:

  /// Constructor, which requires a key.
  explicit GenericHeapItem(T* Key_) : Keyptr(Key_), element(0) {}

  /// Write the key to an output stream.
  void write(std::ostream& os) {Keyptr->write(os);}

  /// Return the key.
  T& key() {return *Keyptr;}

private:

  /// A pointer to a key object.
  T* Keyptr;

  ///
  int element;
};



/** 
 *  This class includes methods that access private data within
 *  a GenericHeapItem.  This is a base class of GenericHeap, and using
 *  this class gets around having to make GenericHeapItem templated on
 *  the comparison class.
 */
template <class ItemType>
class GenericHeapBase
{
protected:

  /// Returns the element id of a heap item
  int& element(ItemType* item)
                {return item->element;}
};


/**
 * A heap class that uses a general set of classes for keys.
 * The GenericHeap class is derived from AbstractHeap,
 * which defines the basic operations of the heap. The keys
 * are assumed to be classes for which the following operations are
 * defined:
 *	int compare(const KEY& key) const
 *	int write(ostream& os) const
 *	int read(istream& is)
 *
 * \sa SimpleHeap
 */
template <class T, class Compare=GenericHeapCompare<T> >
class GenericHeap : public AbstractHeap<GenericHeapItem<T>,T,Compare>,
			public GenericHeapBase<GenericHeapItem<T> >
{
public:

  ///
  typedef typename AbstractHeap<GenericHeapItem<T>,T,Compare>::item_t item_t;

  /// Constructor, which specifies the name for the hash table.
  explicit GenericHeap(const char* nameBuff = "Unnamed", 
		int initSize=-1, int quantumChoice = -1)
	: AbstractHeap<GenericHeapItem<T>,T,Compare>(
					nameBuff,initSize,quantumChoice) {}

protected:

  ///
  int& element(GenericHeapItem<T>* item)
                {return GenericHeapBase<GenericHeapItem<T> >::element(item);}

};


/**
 * A located generic heap, for which elements know their location in the 
 * heap.
 */
template <class T, class Compare=GenericHeapCompare<T> >
class LocatedGenericHeap : public GenericHeap<T,Compare>
{
public:

  /// Constructor
  explicit LocatedGenericHeap(const char* nameBuff = "Unnamed", 
		int initSize=-1, int quantumChoice = -1)
          : GenericHeap<T,Compare>(nameBuff,initSize,quantumChoice) {}
			
protected:

  ///
  void moveEffect(GenericHeapItem<T>* item)
	{item->key().location=element(item);};

};

} // namespace utilib

#endif
