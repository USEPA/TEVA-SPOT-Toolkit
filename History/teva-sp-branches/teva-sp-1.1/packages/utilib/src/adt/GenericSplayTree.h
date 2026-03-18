/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file GenericSplayTree.h
 *
 * Defines the utilib::GenericSplayTreeItem and utilib::GenericSplayTree
 * classes
 */

#ifndef utilib_GenericSplayTree_h
#define utilib_GenericSplayTree_h

#include <utilib/utilib_dll.h>
#include <utilib/_generic.h>
#include <utilib/AbstractSplayTree.h>
 
namespace utilib {


///
template <class T>
class UTILIB_API GenericSplayTree;


/**
 * An item in a utilib::GenericSplayTree
 */
template <class T>
class UTILIB_API GenericSplayTreeItem
{
  friend class AbstractSplayTree<GenericSplayTreeItem<T>,T>;
  friend class GenericSplayTree<T>;

public:

  /// Return the size of the subtree
  int Size() {return size;}

  /// Write the key to an output stream.
  void write(ostream& os) {Keyptr->write(os);}

  /// Return the key.
  T& key() {return *Keyptr;}

  ///
  /// Return the key.
  const T& key() const {return *Keyptr;}

  ///
  int compare(const T& key) const
		{return Keyptr->compare(key);}

  ///
  GenericSplayTreeItem<T>* clone()
        {
        GenericSplayTreeItem<T>* newtree = new GenericSplayTreeItem<T>(Keyptr);
        newtree->Keyptr = Keyptr;
        newtree->ctr = ctr;
        newtree->size = size;
	if (left)
           newtree->left = left->clone();
	if (right)
           newtree->right = right->clone();
        return newtree;
        }

private:

  /// A pointer to a key object.
  T* Keyptr;

  /// The number of instances of \c Key that have been inserted into this splay tree.
  int ctr;

  /// Pointer to the left subtree.
  GenericSplayTreeItem<T> *left;

  /// Pointer to the right subtree.
  GenericSplayTreeItem<T> *right;

  /// The size of the subtree below this item and including this item.
  int size;

  /// Constructor, which requires a key.
  GenericSplayTreeItem(T* key_ptr_) : Keyptr(key_ptr_), ctr(1), left(0),
				right(0), size(0) {}
};



/**
 * A splay tree class that uses a general set of classes for keys.
 * The GenericSplayTree class is derived from AbstractSplayTree,
 * which defines the basic operations of the splay tree. The keys
 * are assumed to be classes for which the following operations are
 * defined:
 *	int compare(const KEY* key) const
 *	int write(ostream& os) const
 *	int read(istream& is)
 *
 * \sa SimpleSplayTree
 */
template <class T>
class UTILIB_API GenericSplayTree : public AbstractSplayTree<GenericSplayTreeItem<T>,T>
{
public:

  ///
  typedef AbstractSplayTree<GenericSplayTreeItem<T>,T> base_t;

  /// Constructor, which specifies a name for the tree.
  explicit GenericSplayTree(const char* nameBuff = "Unnamed")
	: AbstractSplayTree<GenericSplayTreeItem<T>,T>(nameBuff), 
		duplicate_flag(true), check_duplicates_flag(true) {}

  ///
  GenericSplayTree(const GenericSplayTree<T>& tree)
	{ *this = tree; }

  ///
  GenericSplayTree<T>& operator=(const GenericSplayTree<T>& tree)
                {
		base_t::operator=(tree);
		duplicate_flag = tree.duplicate_flag;
		check_duplicates_flag = tree.check_duplicates_flag;
		return *this;
		}

  /// Returns the value of \c duplicate_flag.
  bool& duplicate() {return duplicate_flag;}

  /// Returns the value of \c check_duplicates_flag.
  bool& check_duplicates() {return check_duplicates_flag;}
 
protected:
 
  /// TODO
  bool duplicate_flag;

  /// TODO
  bool check_duplicates_flag;

  /**
   * Insert \a key into the splay tree and return the item used for 
   * it in the tree.
   */
  GenericSplayTreeItem<T>* insert(T* key);

  /**
   * Remove \a item from the tree.  Set the value of \a status to true
   * if the item existed in the tree.
   */
  void extract(GenericSplayTreeItem<T>* item, bool& status);

};


template <class T>
GenericSplayTreeItem<T>* GenericSplayTree<T>::insert(T* key)
{
this->tree = exec_splay(key,this->tree);
GenericSplayTreeItem<T>* treeitem = this->top();

duplicate_flag = false;
if (check_duplicates_flag && (treeitem != NULL)) {
   if (treeitem->compare(*key)==0) {     // it's already there
      treeitem->ctr++;
      duplicate_flag = true;
      return treeitem;
      }
   }
return AbstractSplayTree<GenericSplayTreeItem<T>,T>::insert(key);
}


template <class T>
void GenericSplayTree<T>::extract(GenericSplayTreeItem<T>* item, bool& status)
{
if (check_duplicates_flag) {
   this->tree = exec_splay(&(item->key()),this->tree);
   GenericSplayTreeItem<T>* treeitem = this->top();
   if (treeitem && (treeitem->compare(item->key()) == 0)) {
      treeitem->ctr--;
      if (treeitem->ctr > 0) {
         status=true;
         return;
         }
      }
   }
AbstractSplayTree<GenericSplayTreeItem<T>,T>::extract(item,status);
}

} // namespace utilib

#endif
