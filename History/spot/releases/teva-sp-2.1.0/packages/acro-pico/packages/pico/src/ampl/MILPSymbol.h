/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file MILPSymbol.h
 *
 * Defines MILP symbol objects.
 */

//
// TODO: eliminate the static object symb_list (using a pointer to a
//    set that is defined within the MILPSymbInfo class).
//

#ifndef pico_MILPSymbol_h
#define pico_MILPSymbol_h

#include <acro_config.h>
#include <utilib/Tuple.h>
#include <utilib/HashedSet.h>
#include <utilib/BasicArray.h>
#include <utilib/QueueList.h>
#include <pico/ILPHashItem.h>

namespace pico {

using utilib::QueueList;
using utilib::HashedSet;
using utilib::Tuple1;
using utilib::Tuple2;
using utilib::Tuple3;
using utilib::Tuple4;
using utilib::Tuple5;
using utilib::Tuple6;
using utilib::Tuple7;

//
// Find the index of a given value in a vector
//
template <class T>
int find(std::vector<T>& vec, T& val)
{
int i=0;
typename std::vector<T>::const_iterator curr = vec.begin();
typename std::vector<T>::const_iterator last = vec.end();
while (curr != last) {
  if ((*curr) == val)
     return i;
  i++;
  curr++;
  }
return -1;
}


//
// Only append the vector if it is a unique element of the array
//
template <class T>
void push_back_unique(std::set<T>& vec, T& val)
{
vec.insert(val);
#if 0
if (find(vec,val) == -1)
   vec.push_back(val);
#endif
}



/**
 * Defines a symbol, which is mapped into a MILP variable.
 */
class MILPSymbolBase
{
public:
	/// Symbol's name
  CharString name;
	///
  virtual void add_valid_tuple(QueueList<CharString>& ) {}
	///
  static std::vector<CharString> symb_list;

protected:
	/// Constructor
  MILPSymbolBase(CharString& _name, HashedSet<ILPSymbItem>* _symb_index,
	         HashedSet<MILPVarMap>*  _symb_set) 
		: name(_name), symb_index(_symb_index), symb_set(_symb_set) {}
	///
  virtual ~MILPSymbolBase() {}

	///
  HashedSet<ILPSymbItem>* symb_index;
	///
  HashedSet<MILPVarMap>*  symb_set;
	///
  MILPVarMap& get_symbol(CharString& name_)
		{
		ILPSymbItem key(name_);
		return (symb_index->find(key))->map;
		}

	///
  HashedSet<MILPVarMap>::iterator get_index(MILPVarMap& varmap)
		{
		HashedSet<MILPVarMap>::iterator map_item =
				symb_set->find(varmap);
		if (map_item == symb_set->end()) {
		   throw std::runtime_error("MILPSymbolBase::get_index - bad varmap!");
		   }
		return(map_item);
		}
	///
  int find_symbol(CharString& str);
};


template <class T>
class MILPSymbol0 : public MILPSymbolBase
{
public:
	/// Constructor
  MILPSymbol0(CharString _name, HashedSet<ILPSymbItem>* _symb_index,
	         HashedSet<MILPVarMap>*  _symb_set) 
		: MILPSymbolBase(_name,_symb_index, _symb_set) {}
	///
  T operator()()
		{
		ILPSymbItem key(name);
		T tmp;
		(symb_index->find(key))->map.get_val(tmp);
		return tmp;
		}
};


template <class T1, class T>
class MILPSymbol1 : public MILPSymbolBase
{
public:
	/// Constructor
  MILPSymbol1(CharString _name, HashedSet<ILPSymbItem>* _symb_index,
	        HashedSet<MILPVarMap>*  _symb_set,
		std::set<T1>* _set1)
		: MILPSymbolBase(_name,_symb_index, _symb_set),
		  set1(_set1) {}
	/// Returns the index for a given tuple of arguments
  T operator()( const T1& arg1 )
		{
		MILPVarMap& varmap = get_symbol(name);
		varmap[2] = arg1;
		T tmp;
		get_index(varmap)->get_val(tmp);
		return tmp;
		}
	///
  T operator()(const Tuple1<T1>& tuple)
		{ return (*this)(tuple.val1);}
	///
  void add_valid_tuple(QueueList<CharString>& queue)
		{
		CharString str;
		queue.remove(str);
		T1 val1 = (T1)find_symbol(str);
		push_back_unique(*set1,val1);
		Tuple1<T1> tmp(val1);
		push_back_unique(valid,tmp);
		}
	/// Set of valid tuples for this symbol.
  std::set<Tuple1<T1> > valid;

protected:
	///
  std::set<T1>* set1;
};


template <class T1, class T2, class T>
class MILPSymbol2 : public MILPSymbolBase
{
public:
	/// Constructor
  MILPSymbol2(CharString _name, HashedSet<ILPSymbItem>* _symb_index,
	        HashedSet<MILPVarMap>*  _symb_set,
		std::set<T1>* _set1,
		std::set<T2>* _set2)
		: MILPSymbolBase(_name,_symb_index, _symb_set),
		  set1(_set1), set2(_set2) {}
	/// Returns the index for a given tuple of arguments
  T operator()( const T1& arg1, const T2& arg2 )
		{
		MILPVarMap& varmap = get_symbol(name);
		varmap[2] = arg1;
		varmap[4] = arg2;
		T tmp;
		get_index(varmap)->get_val(tmp);
		return tmp;
		}
  T operator()(const Tuple2<T1,T2>& tuple)
		{ return (*this)(tuple.val1,tuple.val2);}
	///
  void add_valid_tuple(QueueList<CharString>& queue)
		{
		CharString str;
		queue.remove(str);
		T1 val1 = (T1) find_symbol(str);
		push_back_unique(*set1,val1);
		queue.remove(str);
		T2 val2 = (T2) find_symbol(str);
		push_back_unique(*set2,val2);
		Tuple2<T1,T2> tmp(val1,val2);
		push_back_unique(valid,tmp);
		}
	/// Set of valid tuples for this symbol.
  std::set<Tuple2<T1,T2> > valid;

protected:
	///
  std::set<T1>* set1;
	///
  std::set<T2>* set2;
};


template <class T1, class T2, class T3, class T>
class MILPSymbol3 : public MILPSymbolBase
{
public:
	/// Constructor
  MILPSymbol3(CharString _name, HashedSet<ILPSymbItem>* _symb_index,
	        HashedSet<MILPVarMap>*  _symb_set,
		std::set<T1>* _set1,
		std::set<T2>* _set2,
		std::set<T3>* _set3) 
		: MILPSymbolBase(_name,_symb_index, _symb_set),
		  set1(_set1), set2(_set2), set3(_set3) {}
	/// Returns the index for a given tuple of arguments
  T operator()( const T1& arg1, const T2& arg2, const T3& arg3 )
		{
		MILPVarMap& varmap = get_symbol(name);
		varmap[2] = arg1;
		varmap[4] = arg2;
		varmap[6] = arg3;
		T tmp;
		get_index(varmap)->get_val(tmp);
		return tmp;
		}
  T operator()(const Tuple3<T1,T2,T3>& tuple)
		{ return (*this)(tuple.val1,tuple.val2,tuple.val3);}
	///
  void add_valid_tuple(QueueList<CharString>& queue)
		{
		CharString str;
		queue.remove(str);
		T1 val1 = (T1) find_symbol(str);
		push_back_unique(*set1,val1);
		queue.remove(str);
		T2 val2 = (T2) find_symbol(str);
		push_back_unique(*set2,val2);
		queue.remove(str);
		T3 val3 = (T3) find_symbol(str);
		push_back_unique(*set3,val3);
		Tuple3<T1,T2,T3> tmp(val1,val2,val3);
		push_back_unique(valid,tmp);
		}
	/// Set of valid tuples for this symbol.
  std::set<Tuple3<T1,T2,T3> > valid;

protected:
	///
  std::set<T1>* set1;
	///
  std::set<T2>* set2;
	///
  std::set<T3>* set3;
};


template <class T1, class T2, class T3, class T4, class T>
class MILPSymbol4 : public MILPSymbolBase
{
public:
	/// Constructor
  MILPSymbol4(CharString _name, HashedSet<ILPSymbItem>* _symb_index,
	        HashedSet<MILPVarMap>*  _symb_set,
		std::set<T1>* _set1,
		std::set<T2>* _set2,
		std::set<T3>* _set3,
		std::set<T4>* _set4) 
		: MILPSymbolBase(_name,_symb_index, _symb_set),
		  set1(_set1), set2(_set2), set3(_set3), set4(_set4) {}
	/// Returns the index for a given tuple of arguments
  T operator()( const T1& arg1, const T2& arg2, const T3& arg3, 
				const T4& arg4 )
		{
		MILPVarMap& varmap = get_symbol(name);
		varmap[2] = arg1;
		varmap[4] = arg2;
		varmap[6] = arg3;
		varmap[8] = arg4;
		T tmp;
		get_index(varmap)->get_val(tmp);
		return tmp;
		}
  T operator()(const Tuple4<T1,T2,T3,T4>& tuple)
		{ return (*this)(tuple.val1,tuple.val2,tuple.val3,tuple.val4);}
	///
  void add_valid_tuple(QueueList<CharString>& queue)
		{
		CharString str;
		queue.remove(str);
		T1 val1 = (T1) find_symbol(str);
		push_back_unique(*set1,val1);
		queue.remove(str);
		T2 val2 = (T2) find_symbol(str);
		push_back_unique(*set2,val2);
		queue.remove(str);
		T3 val3 = (T3) find_symbol(str);
		push_back_unique(*set3,val3);
		queue.remove(str);
		T4 val4 = (T4) find_symbol(str);
		push_back_unique(*set4,val4);
		Tuple4<T1,T2,T3,T4> tmp(val1,val2,val3,val4);
		push_back_unique(valid,tmp);
		}
	/// Set of valid tuples for this symbol.
  std::set<Tuple4<T1,T2,T3,T4> > valid;

protected:
	///
  std::set<T1>* set1;
	///
  std::set<T2>* set2;
	///
  std::set<T3>* set3;
	///
  std::set<T4>* set4;
};


template <class T1, class T2, class T3, class T4, class T5, class T>
class MILPSymbol5 : public MILPSymbolBase
{
public:
	/// Constructor
  MILPSymbol5(CharString _name, HashedSet<ILPSymbItem>* _symb_index,
	        HashedSet<MILPVarMap>*  _symb_set,
		std::set<T1>* _set1,
		std::set<T2>* _set2,
		std::set<T3>* _set3,
		std::set<T4>* _set4,
		std::set<T5>* _set5) 
		: MILPSymbolBase(_name,_symb_index, _symb_set),
		  set1(_set1), set2(_set2), set3(_set3),
		  set4(_set4), set5(_set5) {}
	/// Returns the index for a given tuple of arguments
  T operator()( const T1& arg1, const T2& arg2, const T3& arg3, 
				const T4& arg4, const T5& arg5 )
		{
		MILPVarMap& varmap = get_symbol(name);
		varmap[2] = arg1;
		varmap[4] = arg2;
		varmap[6] = arg3;
		varmap[8] = arg4;
		varmap[10] = arg5;
		T tmp;
		get_index(varmap)->get_val(tmp);
		return tmp;
		}
	///
  T operator()(const Tuple5<T1,T2,T3,T4,T5>& tuple)
		{ return (*this)(tuple.val1,tuple.val2,tuple.val3,tuple.val4,
				tuple.val5);}
	///
  void add_valid_tuple(QueueList<CharString>& queue)
		{
		CharString str;
		queue.remove(str);
		T1 val1 = (T1) find_symbol(str);
		push_back_unique(*set1,val1);
		queue.remove(str);
		T2 val2 = (T2) find_symbol(str);
		push_back_unique(*set2,val2);
		queue.remove(str);
		T3 val3 = (T3) find_symbol(str);
		push_back_unique(*set3,val3);
		queue.remove(str);
		T4 val4 = (T4) find_symbol(str);
		push_back_unique(*set4,val4);
		queue.remove(str);
		T5 val5 = (T5) find_symbol(str);
		push_back_unique(*set5,val5);
		Tuple5<T1,T2,T3,T4,T5> tmp(val1,val2,val3,val4,val5);
		push_back_unique(valid,tmp);
		}
	/// Set of valid tuples for this symbol.
  std::set<Tuple5<T1,T2,T3,T4,T5> > valid;

protected:
	///
  std::set<T1>* set1;
	///
  std::set<T2>* set2;
	///
  std::set<T3>* set3;
	///
  std::set<T4>* set4;
	///
  std::set<T5>* set5;
};


template <class T1, class T2, class T3, class T4, class T5, class T6, class T>
class MILPSymbol6 : public MILPSymbolBase
{
public:
	/// Constructor
  MILPSymbol6(CharString _name, HashedSet<ILPSymbItem>* _symb_index,
	        HashedSet<MILPVarMap>*  _symb_set,
		std::set<T1>* _set1,
		std::set<T2>* _set2,
		std::set<T3>* _set3,
		std::set<T4>* _set4,
		std::set<T5>* _set5,
		std::set<T6>* _set6) 
		: MILPSymbolBase(_name, _symb_index, _symb_set),
		  set1(_set1), set2(_set2), set3(_set3),
		  set4(_set4), set5(_set5), set6(_set6) {}
	/// Returns the index for a given tuple of arguments
  T operator()( const T1& arg1, const T2& arg2, const T3& arg3, 
				const T4& arg4, const T5& arg5, const T6& arg6 )
		{
		MILPVarMap& varmap = get_symbol(name);
		varmap[2] = arg1;
		varmap[4] = arg2;
		varmap[6] = arg3;
		varmap[8] = arg4;
		varmap[10] = arg5;
		varmap[12] = arg6;
		T tmp;
		get_index(varmap)->get_val(tmp);
		return tmp;
		}
	///
  T operator()(const Tuple6<T1,T2,T3,T4,T5,T6>& tuple)
		{ return (*this)(tuple.val1,tuple.val2,tuple.val3,tuple.val4,
				tuple.val5,tuple.val6);}
	///
  void add_valid_tuple(QueueList<CharString>& queue)
		{
		CharString str;
		queue.remove(str);
		T1 val1 = (T1) find_symbol(str);
		push_back_unique(*set1,val1);
		queue.remove(str);
		T2 val2 = (T2) find_symbol(str);
		push_back_unique(*set2,val2);
		queue.remove(str);
		T3 val3 = (T3) find_symbol(str);
		push_back_unique(*set3,val3);
		queue.remove(str);
		T4 val4 = (T4) find_symbol(str);
		push_back_unique(*set4,val4);
		queue.remove(str);
		T5 val5 = (T5) find_symbol(str);
		push_back_unique(*set5,val5);
		queue.remove(str);
		T6 val6 = (T6) find_symbol(str);
		push_back_unique(*set6,val6);
		Tuple6<T1,T2,T3,T4,T5,T6> tmp(val1,val2,val3,val4,val5,val6);
		push_back_unique(valid,tmp);
		}
	/// Set of valid tuples for this symbol.
  std::set<Tuple6<T1,T2,T3,T4,T5,T6> > valid;

protected:
	///
  std::set<T1>* set1;
	///
  std::set<T2>* set2;
	///
  std::set<T3>* set3;
	///
  std::set<T4>* set4;
	///
  std::set<T5>* set5;
	///
  std::set<T6>* set6;
};


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T>
class MILPSymbol7 : public MILPSymbolBase
{
public:
  /// Constructor
  MILPSymbol7(CharString _name, HashedSet<ILPSymbItem>* _symb_index,
	        HashedSet<MILPVarMap>*  _symb_set,
		std::set<T1>* _set1,
		std::set<T2>* _set2,
		std::set<T3>* _set3,
		std::set<T4>* _set4,
		std::set<T5>* _set5,
		std::set<T6>* _set6,
		std::set<T7>* _set7)
		: MILPSymbolBase(_name,_symb_index, _symb_set),
		  set1(_set1), set2(_set2), set3(_set3),
		  set4(_set4), set5(_set5), set6(_set6), set7(_set7) {}
  /// Returns the index for a given tuple of arguments
  T operator()( const T1& arg1, const T2& arg2, const T3& arg3, 
				const T4& arg4,
				const T5& arg5, const T6& arg6, const T7& arg7 )
		{
		MILPVarMap& varmap = get_symbol(name);
		varmap[2] = arg1;
		varmap[4] = arg2;
		varmap[6] = arg3;
		varmap[8] = arg4;
		varmap[10] = arg5;
		varmap[12] = arg6;
		varmap[14] = arg7;
		T tmp;
		get_index(varmap)->get_val(tmp);
		return tmp;
		}
  ///
  T operator()(const Tuple7<T1,T2,T3,T4,T5,T6,T7>& tuple)
		{ return (*this)(tuple.val1,tuple.val2,tuple.val3,tuple.val4,
				tuple.val5,tuple.val6,tuple.val7);}
  ///
  void add_valid_tuple(QueueList<CharString>& queue)
		{
		CharString str;
		queue.remove(str);
		T1 val1 = (T1) find_symbol(str);
		push_back_unique(*set1,val1);
		queue.remove(str);
		T2 val2 = (T2) find_symbol(str);
		push_back_unique(*set2,val2);
		queue.remove(str);
		T3 val3 = (T3) find_symbol(str);
		push_back_unique(*set3,val3);
		queue.remove(str);
		T4 val4 = (T4) find_symbol(str);
		push_back_unique(*set4,val4);
		queue.remove(str);
		T5 val5 = (T5) find_symbol(str);
		push_back_unique(*set5,val5);
		queue.remove(str);
		T6 val6 = (T6) find_symbol(str);
		push_back_unique(*set6,val6);
		queue.remove(str);
		T7 val7 = (T7) find_symbol(str);
		push_back_unique(*set7,val7);
		Tuple7<T1,T2,T3,T4,T5,T6,T7> tmp(val1,val2,val3,val4,val5,val6,val7);
		push_back_unique(valid,tmp);
		}
  /// Set of valid tuples for this symbol.
  std::set<Tuple7<T1,T2,T3,T4,T5,T6,T7> > valid;

protected:

  ///
  std::set<T1>* set1;
  ///
  std::set<T2>* set2;
  ///
  std::set<T3>* set3;
  ///
  std::set<T4>* set4;
  ///
  std::set<T5>* set5;
  ///
  std::set<T6>* set6;
  ///
  std::set<T7>* set7;
};

} // namespace pico

#endif
