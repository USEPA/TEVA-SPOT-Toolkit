/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file HashedSet.h
 *
 * Defines the utilib::HashedSet class
 */
#ifndef utilib_HashedSet_h
#define utilib_HashedSet_h

#include <utilib/utilib_config_bool.h>
#include <utilib/SimpleHashTable.h>
#include <utilib/GenericHashTable.h>

namespace utilib {

template <class _Key, bool simple_flag>
class HashedSet;

}


template <class _Key, bool simple_flag>
std::ostream& operator<<(std::ostream& os, const typename utilib::HashedSet<_Key,simple_flag>& obj);

template <class _Key, bool simple_flag>
utilib::PackBuffer& operator<<(utilib::PackBuffer& os, const utilib::HashedSet<_Key,simple_flag>& obj);

template <class _Key, bool simple_flag>
std::istream& operator>>(std::istream& is, utilib::HashedSet<_Key,simple_flag>& obj);

template <class _Key, bool simple_flag>
utilib::UnPackBuffer& operator>>(utilib::UnPackBuffer& is, utilib::HashedSet<_Key,simple_flag>& obj);


namespace utilib {

/**
 * Base class for __LinkedList_iterator that a LinkedList can use
 * as an iterator for the array
 */
template<typename _Ref, typename _Node>
class __LinkedList_HashedSet_OpClass
{
public:

  ///
  _Ref value(_Node* node) const
        { 
        if (!node)
	   EXCEPTION_MNGR(runtime_error, "Accessing an invalid iterator.");
	return node->Data->key();
	}

};


///
template <class _Key, bool simple_flag>
class HashedSetBase
{
public:

  ///
  typedef SimpleHashTable<_Key,char> _HT_type;

  ///
  typedef ListItem<SimpleHashTableItem<_Key,char>*> _Node;

  ///
  typedef __LinkedList_iterator<_Key,_Key&,_Key*,_Node,__LinkedList_HashedSet_OpClass<_Key&,_Node> > iterator;

  ///
  typedef __LinkedList_iterator<_Key,const _Key&,const _Key*,_Node,__LinkedList_HashedSet_OpClass<_Key&,_Node> > const_iterator;

#if !defined(UTILIB_NO_MEMBER_TEMPLATE_FRIENDS)
protected:
#else
public:
#endif

  ///
  SimpleHashTable<_Key,char> ht;

};


///
template <class _Key>
class HashedSetBase<_Key,false>
{
public:

  ///
  typedef GenericHashTable<_Key,char> _HT_type;

  ///
  typedef ListItem<GenericHashTableItem<_Key,char>*> _Node;

  ///
  typedef __LinkedList_iterator<_Key,_Key&,_Key*,_Node,__LinkedList_HashedSet_OpClass<_Key&,_Node> > iterator;

  ///
  typedef __LinkedList_iterator<_Key,const _Key&,const _Key*,_Node,__LinkedList_HashedSet_OpClass<_Key&,_Node> > const_iterator;

#if !defined(UTILIB_NO_MEMBER_TEMPLATE_FRIENDS)
protected:
#else
public:
#endif

  ///
  GenericHashTable<_Key,char> ht;

};


///
template <class _Key, bool simple_flag=true>
class HashedSet : public HashedSetBase<_Key,simple_flag>
{
#if !defined(UTILIB_NO_MEMBER_TEMPLATE_FRIENDS)
  friend std::ostream& ::operator<< <_Key,simple_flag>(std::ostream& os, const utilib::HashedSet<_Key,simple_flag>& obj);

  friend utilib::PackBuffer& ::operator<< <_Key,simple_flag>(utilib::PackBuffer& os, const utilib::HashedSet<_Key,simple_flag>& obj);

  friend std::istream& ::operator>> <_Key,simple_flag>(std::istream& is, utilib::HashedSet<_Key,simple_flag>& obj);

  friend utilib::UnPackBuffer& ::operator>> <_Key,simple_flag>(utilib::UnPackBuffer& is, utilib::HashedSet<_Key,simple_flag>& obj);
#endif

public:

  typedef          _Key     key_type;
  typedef          _Key     value_type;
  typedef 	   HashedSetBase<_Key,simple_flag> base;
  typedef typename base::iterator iterator;
  typedef typename base::const_iterator const_iterator;
  typedef typename base::_HT_type _Rep_type;
  typedef typename _Rep_type::size_type size_type;

  ///
  HashedSet() 
	{}

  ///
  HashedSet(const HashedSet<_Key,simple_flag>& __x) 
	{ this->ht = __x.ht; }

  ///
  HashedSet<_Key,simple_flag>& operator=(const HashedSet<_Key,simple_flag>& __x)
  	{
    	this->ht = __x.ht;
    	return *this;
  	}

  ///
  const_iterator find(const value_type& __x) const
	{
	typename _Rep_type::const_iterator item = this->ht.find(__x);
	return item._M_node;
	}

  ///
  iterator find(const value_type& __x)
	{
	typename _Rep_type::iterator item = this->ht.find(__x);
	return item._M_node;
	}

  /// Added by JE to make calling code easier to understand.
  /// I sure hope I did this right...
  bool contains(const value_type& __x) const { return find(__x) != end(); }

  /// insert/erase
  std::pair<iterator,bool> insert(const value_type& __x)
	{
	typename _Rep_type::iterator item = this->ht.find(__x);
	if (item != this->ht.end()) {
	   iterator tmp(item._M_node);
	   return std::pair<iterator,bool>(tmp,false);
           }
	char dummy='a';
	typename _Rep_type::iterator tmpitem = this->ht.add(__x,dummy,true);
	iterator tmp(tmpitem._M_node);
        return std::pair<iterator, bool>(tmp,true);
  	}

  ///
  size_type erase(const key_type& __x)
	{
	typename _Rep_type::iterator item = this->ht.find(__x);
	if (item == this->ht.end()) return 0;
	bool status;
	this->ht.remove(item,status);
    	return 1;
  	}

  ///
  size_type erase(const iterator& curr)
	{
	if (curr == end()) return 0;
	bool status;
  	typename _Rep_type::iterator tmp(curr._M_node);
	this->ht.remove(tmp,status);
    	return 1;
  	}

  ///
  iterator begin() { return this->ht.begin()._M_node; }

  ///
  const_iterator begin() const { return this->ht.begin()._M_node; }

  ///
  iterator end() { return this->ht.end()._M_node; }

  ///
  const_iterator end() const { return this->ht.end()._M_node; }

  ///
  size_type size() const { return this->ht.size(); }

  ///
  bool empty() const { return (this->ht.size() == 0); }

  ///
  operator bool() const { return (this->ht.size() != 0); }

  ///
  void clear() { this->ht.clear(); }

};


} // namespace utilib


///
template <class _Key, bool simple_flag>
bool is_disjoint(const utilib::HashedSet<_Key,simple_flag>& set1, 
		 const utilib::HashedSet<_Key,simple_flag>& set2)
{
if (set2.size() < set1.size()) {
   typename utilib::HashedSet<_Key,simple_flag>::const_iterator curr = set2.begin();
   typename utilib::HashedSet<_Key,simple_flag>::const_iterator end  = set2.end();
   while (curr != end) {
     if (set1.contains(*curr)) return false;
     curr++;
     }
   }
else {
   typename utilib::HashedSet<_Key,simple_flag>::const_iterator curr = set1.begin();
   typename utilib::HashedSet<_Key,simple_flag>::const_iterator end  = set1.end();
   while (curr != end) {
     if (set2.contains(*curr)) return false;
     curr++;
     }
   }
return true;
}

/// Out-stream operator for writing the contents of a HashedSet
template <class _Key, bool simple_flag>
inline std::ostream& operator<<(std::ostream& os, const utilib::HashedSet<_Key,simple_flag>& obj)
{ os << obj.ht; return os; }

/// Out-stream operator for writing the contents of a HashedSet
template <class _Key, bool simple_flag>
inline utilib::PackBuffer& operator<<(utilib::PackBuffer& os,
                                        const utilib::HashedSet<_Key,simple_flag>& obj)
{ os << obj.ht; return os; }

/// In-stream operator for reading the contents of a HashedSet
template <class _Key, bool simple_flag>
inline std::istream& operator>>(std::istream& is, utilib::HashedSet<_Key,simple_flag>& obj)
{ is >> obj.ht; return is; }

/// In-stream operator for reading the contents of a HashedSet
template <class _Key, bool simple_flag>
inline utilib::UnPackBuffer& operator>>(utilib::UnPackBuffer& is,
                                        utilib::HashedSet<_Key,simple_flag>& obj)
{ is >> obj.ht; return is; }

#endif
