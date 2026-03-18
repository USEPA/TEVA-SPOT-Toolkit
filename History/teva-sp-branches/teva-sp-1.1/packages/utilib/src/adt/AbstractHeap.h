/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file AbstractHeap.h
 *
 * Defines the utilib::AbstractHeap class
 */

#ifndef utilib_AbstractHeap_h
#define utilib_AbstractHeap_h

#ifdef NON_ANSI
#include <iostream.h>
#else
#include <iostream>
#endif

#include <utilib/compare.h>
#include <utilib/_generic.h>
#include <utilib/exception_mngr.h>

#ifdef ANSI_NAMESPACES
using namespace std;
#endif

namespace utilib {


/**
 * Implements an abstract class for defining the core operations of a heap.
 *
 * This class is based on a generic heap class initially develop by
 * Jonathan Eckstein (Rutgers).
 *
 * \todo evaluate how this class relates to the STL heap class.
 */
template <class T, class KEY, class Compare>
class AbstractHeap 
{
public:

  ///
  typedef T item_t;

  /// Constructor, which allows the heap to be named.
  explicit AbstractHeap(const char* nameBuff = "Unnamed",
       int initSize = -1, 
       int quantumChoice = -1);

  /// Destructor.
  virtual ~AbstractHeap();
  
  /// Returns \c true if the list is empty and \c false otherwise.
  bool empty() const {return (used == 0);}

  /// Returns \c true if the list is empty and \c false otherwise.
  operator bool() const {return (used != 0);}

  /// Returns the number of items in the heap.
  int size() const { return used; }

  /// Return the top of the heap.
  T* top() const
		{ 
      		if (used <= 0) 
	   	   EXCEPTION_MNGR(runtime_error, "Empty heap: " << name)
        	return tree[1]; 
        	}

  /// Return a specific element of the heap.
  T* member(int elem) const
		{ return tree[elem]; }
  /**
   * Find the element with the given key.
   * Returns null if no such key exists.
   */
  T* find(KEY& key);

  /// Add a key to the heap.
  virtual void add(KEY& key)
                {insert(&key);}

  /**
   * Add a key to the heap and return the heap item
   * that contains it.
   */
  virtual void add(KEY& key, T*& item)
                {item = insert(&key);}

  /**
   * Remove a heap item with the given key.
   * The status flag is \c true if the key was found and \c false
   * otherwise.
   */
  virtual void remove(KEY& key, bool& status)
                {
                T* tmp = find(key);
                if (tmp) extract(tmp,status);
                else     status = false;
                }

  /**
   * Remove a heap item.
   * The status flag is \c true if the item was found and \c false
   * otherwise.
   */
  virtual void remove(T*& item, bool& status)
                {
                if (!item)
                   status = false;
                else {
                   extract(item,status);
                   item = 0;
                   }
                }

  /**
   * Remove a heap and return the item's key.
   * The status flag is \c true if the item was found and \c false
   * otherwise.
   */
  virtual void remove(T*& item, KEY& key, bool& status)
                {
                if (!item)
                   status = false;
                else {
                   key = item->key();
                   extract(item,status);
                   item = 0;
                   }
                }

#if 0
//#if !defined(SOLARIS) 
// || (defined(__SUNPRO_CC) && (__SUNPRO_CC < 0x550 ))
  /**
   * Remove a heap and return the item's key.
   * The status flag is \c true if the item was found and \c false
   * otherwise.
   */
  virtual void remove(T* item, KEY& key, bool& status)
                {
                if (!item)
                   status = false;
                else {
                   key = item->key();
                   extract(item,status);
                   }
                }
#endif

  /** 
   * Makes sense when KEY is a class.  If the way this key compares to
   * others in the heap has changed, call this to repair the heap ordering
   */
  virtual void refloat(KEY &key, bool& status)
    {
    T* item = find(key);
    if (!item)
      {
      status = false;
      return;
      }
    status = true;
    refloatElement(element(item));
    }

  /** 
   * As with refloat, but for the case when you know the direction of
   * of the comparison changes (e.g. only decreasing the key in a min heap)
   */
  virtual void floatUp(KEY &key, bool&status)
    {
    T* item = find(key);
    if (!item)
      {
      status = false;
      return;
      }
    status = true;
    floatUp(element(item));
    }

  /// Companion to floatUp (other direction)
  virtual void sinkDown(KEY &key, bool&status)
    {
    T* item = find(key);
    if (!item)
      {
      status = false;
      return;
      }
    status = true;
    sinkDown(element(item));
    }

  /// Write the heap to an output stream.
  int write(ostream& os) const;

  /// Read a heap from an input stream.
  int read(istream& is);

  /// TODO
  int prune();

  /// Default size of the heap.
  static const int defaultSize;

  /// The default quantum used to resize the heap.
  static const int defaultQuantum;

  /// Empty the heap.
  void clear();

  /// Signal an error if the heap is out of order
  void validate();

  /// TODO.
  int refloatElement(int elem);

  /// Validate that the elements form a heap by re-sinking each element
  void reheapify()
	{ for (int i = used>>1; i>0; i--) sinkDown(i); }

protected:

  /// Add an item to the heap.
  virtual T* insert(KEY* item);

  /**
   * Remove a heap item.
   * The status flag is \c true if the item was found and \c false
   * otherwise.
   */
  virtual void extract(T* item, bool& status);

  /// TODO
  virtual int& element(T* item) = 0;

  /// TODO.
  virtual int  prunable(T* /*item*/) {return 0;}

  /// TODO.
  virtual void moveEffect(T* /*item*/) {}

  /// TODO.
  virtual void addEffect(T* /*item*/) {}

  /// TODO.
  virtual void removeEffect(T* item)
		{element(item)=0;}

  /// TODO.
  virtual void pruneEffect(T* /*item*/) {}

  /// The number of items in the heap.
  int    heap_size;

  /// The number of indeces in \c used for the heap.
  int    used;

  /// The size of the \c tree array.
  int    allocated;

  /// The quantity by which the \c tree array is increased.
  int    quantum;

  /// The array that contains the heap.
  T** tree;

  /// The name of the heap.
  const char*  name;

  /// TODO.
  void swap(int a,int b);

  /// TODO.
  int floatUp(int elem);

  /// TODO.
  int sinkDown(int elem);

  /// A comparison class
  Compare compare;
};


template <class T, class KEY, class Compare>
const int AbstractHeap<T,KEY,Compare>::defaultSize    = 256;

template <class T, class KEY, class Compare>
const int AbstractHeap<T,KEY,Compare>::defaultQuantum = 256;



template <class T, class KEY, class Compare>
AbstractHeap<T,KEY,Compare>::AbstractHeap(const char* nameBuff,
	   int initSize, 
	   int quantumChoice) :
	   heap_size(0),
	   used(0),
	   allocated(initSize),
	   quantum(quantumChoice),
	   name(nameBuff)
{
if (initSize == -1)
   allocated = defaultSize;
if (quantumChoice == -1)
   quantum=defaultQuantum;

if (!name)
   name = "";

tree = new T*[allocated+1];
}


template <class T, class KEY, class Compare>
AbstractHeap<T,KEY,Compare>::~AbstractHeap()
{
delete[] tree;
}



template <class T, class KEY, class Compare>
void AbstractHeap<T,KEY,Compare>::swap(int a,int b) 
{
register T* t1 = tree[a];
register T* t2 = tree[b];
tree[a] = t2;
tree[b] = t1;
element(t1)=b;
element(t2)=a;
moveEffect(t1);
moveEffect(t2);
}


// Signals an error if the heap isn't in valid heap order.

template <class T, class KEY, class Compare>
void AbstractHeap<T,KEY,Compare>::validate()
{
int parent;
// Other code implies this array isn't zero based (first element at 1)
for (int i = used; i > 1; i--)
  {
  parent = i >> 1;
  if (compare(tree[i]->key(),tree[parent]->key()) == Comparison::better)
    EXCEPTION_MNGR(runtime_error, "Heap failed validation, parent index "
		<< parent << ", child index " << i)
  }
}

template <class T, class KEY, class Compare>
int AbstractHeap<T,KEY,Compare>::floatUp(int elem)
{
int next;
while (((next = elem >> 1) >= 1) && 
       (compare(tree[elem]->key(),tree[next]->key()) == Comparison::better))
  {
  swap(elem,next);
  elem = next;
  }
return elem;
}


template <class T, class KEY, class Compare>
int AbstractHeap<T,KEY,Compare>::sinkDown(int elem)
{
int child;
while ((child = elem << 1) <= used) 
  {
  if ((child < used) && 
	(compare(tree[child+1]->key(),tree[child]->key()) == Comparison::better))
     child++;
  if (compare(tree[elem]->key(),tree[child]->key()) == Comparison::better)
     return(elem);
  swap(elem,child);
  elem = child;
  }
return elem;
}


template <class T, class KEY, class Compare>
int AbstractHeap<T,KEY,Compare>::refloatElement(int elem)
{
int newLocation = floatUp(elem);
if (newLocation == elem)
   return sinkDown(elem);
else
   return elem;
}


template <class T, class KEY, class Compare>
T* AbstractHeap<T,KEY,Compare>::insert(KEY* newkey) 
{
if (used == allocated) {
   if (quantum > 0) {
      allocated += quantum;
      T** oldTree = tree;
      tree = new T* [allocated+1]; 
      for (int i=1; i<=used; i++)
	tree[i] = oldTree[i];
      delete [] oldTree;
      }
   else 
      EXCEPTION_MNGR(runtime_error, name << "heap overflowed at " << (used+1) << " elements")
   }

T* item = new T(newkey);
tree[++used] = item;
heap_size++;
addEffect(item);
element(item)=used;
moveEffect(item);
floatUp(used);
return item;
}


template <class T, class KEY, class Compare>
void AbstractHeap<T,KEY,Compare>::extract(T* item, bool& status) 
{
int elem = element(item);
if ((elem < 1) || (elem > used)) {
   status=false;
   return;
   }

removeEffect(item);
delete item;
if (elem < used) {
   T* t = tree[used];
   tree[elem] = t;
   used--;
   heap_size--;
   element(t)=elem;
   moveEffect(t);
   refloatElement(elem);
   }
else {
   used--;
   heap_size--;
   }

status=true;
}


//
// TODO - check that this works well with SimpleHeaps, which
// have their own internal counter
//
template <class T, class KEY, class Compare>
int AbstractHeap<T,KEY,Compare>::prune() 
{
int n = used;
while (n > (used >> 1)) {
  if (prunable(tree[n])) {
     T* item = tree[n];
     pruneEffect(item);
     //this->deleteElement(n);
     if (n > used)
	n = used;
     }
  else 
     n--;
  }
return used;
}


template <class T, class KEY, class Compare>
int AbstractHeap<T,KEY,Compare>::write(ostream& os) const
{
os << heap_size << endl;
for (int i=1; i<=used; i++) {
  member(i)->write(os);
  os << endl;
  }
os << endl;
return OK;
}

             
template <class T, class KEY, class Compare>
int AbstractHeap<T,KEY,Compare>::read(istream& is)
{
clear();
int Size;
is >> Size;
for (int i=1; i<=Size; i++) {
  T* item;
  item->read(is);
  add(item);
  }
return OK;
}          



//
// For right now, this is done with a linear search through the
// heap array.  A more efficient method could be designed, but I don't
// expect to use this method much, so...
//
template <class T, class KEY, class Compare>
T* AbstractHeap<T,KEY,Compare>::find(KEY& key)
{
for (int i=1; i<= used; i++)
  if (compare(member(i)->key(),key) == Comparison::equal)
     return member(i);

return 0;
}


template <class T, class KEY, class Compare>
void AbstractHeap<T,KEY,Compare>::clear()
{
bool status;
while (used>0) {
  T *foo = top();
  remove(foo,status);
  if (!status)
     EXCEPTION_MNGR(runtime_error,"AbstractHeap<T,KEY,Compare>::clear - problem removing the root of the heap.")
  }
}

} // namespace utilib

/// Out-stream for AbstractHeap objects
template <class T, class KEY, class Compare>
inline ostream& operator<<(ostream& output, const utilib::AbstractHeap<T,KEY,Compare>& heap)
{ heap.write(output); return(output); }

/// In-stream for AbstractHeap objects
template <class T, class KEY, class Compare>
inline istream& operator>>(istream& input, utilib::AbstractHeap<T,KEY,Compare>& heap)
{ heap.read(input); return(input); }

#endif
