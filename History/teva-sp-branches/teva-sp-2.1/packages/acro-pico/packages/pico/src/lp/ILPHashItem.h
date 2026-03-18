/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file ILPHashItem.h
 *
 * Defines the pico::ILPHashItem class.
 */

#ifndef pico_ILPHashItem_h
#define pico_ILPHashItem_h

#include <acro_config.h>
#include <utilib/compare.h>
#include <utilib/_math.h>
#include <utilib/CharString.h>
#include <utilib/OrderedList.h>
#include <utilib/Ereal.h>
#include <utilib/PackObject.h>
#include <pico/ILPArgList.h>
#include <pico/MILPVarMap.h>

namespace pico {

using utilib::OrderedList;
using utilib::Ereal;

/// An item used to store information in a hash table about the row/column
/// symbols, both those read in LP and MPS files, as well as symbolic 
/// variable representations.
class ILPHashItem : public utilib::PackObject
{
public:

  /// Constructor
  ILPHashItem() : index(-1) {}
  
  /// Constructor
  ILPHashItem(const CharString& name_, int index_=-1)
		: name(name_), index(index_) {}

  ///
  virtual ~ILPHashItem() {}

  ///
  virtual void write(std::ostream& os) const;
  
  ///
  virtual void read(std::istream& is);
  
  ///
  virtual void write(PackBuffer& os) const
  	{ os << name << index; }
  
  ///
  virtual void read(UnPackBuffer& is)
  	{ is >> name >> index; }
  
  ///
  size_type hash(size_type table_size) const;
  
  ///
  int compare(const ILPHashItem* key) const;
  
  /// row/col name
  CharString name;
  
  /// row/col index
  int index;

};



class ILPRowItem : public ILPHashItem
{
public:

	/// Constructor
  ILPRowItem() : constraintType('U')
		{
		constraintLHS=Ereal<double>::negative_infinity; 
		constraintRHS=Ereal<double>::positive_infinity;
		}
	/// Constructor
  ILPRowItem(const CharString& name_, int index_=-1) 
		: ILPHashItem(name_,index_), constraintType('U')
		{
		constraintLHS=Ereal<double>::negative_infinity; 
		constraintRHS=Ereal<double>::positive_infinity;
		}

	/// Type of the constraint
  char constraintType;
	/// The LHS constraint
  Ereal<double> constraintLHS;
	/// The RHS constraint
  Ereal<double> constraintRHS;
};


class ILPColItem : public ILPHashItem
{
public:

	/// Constructor
  ILPColItem() :  intflag(0)
		{
		lbound=Ereal<double>::negative_infinity; 
		ubound=Ereal<double>::positive_infinity;
		}
	/// Constructor
  ILPColItem(const CharString& name_, int index_=-1) 
		: ILPHashItem(name_,index_), 
		  intflag(0)
		{
		lbound=Ereal<double>::negative_infinity; 
		ubound=Ereal<double>::positive_infinity;
		}

	/// Coefficient list for columns
  OrderedList<double,int> coef;

	/// Row lower bound
  Ereal<double> lbound;
	/// Row upper bound;
  Ereal<double> ubound;
	/// If true, then this variable is integer
  int intflag;
};



class ILPSymbItem : public ILPHashItem
{
public:

  /// Constructor
  ILPSymbItem() : ILPHashItem(), rc_flag(-1), symb_type(0) {}
  
  /// Constructor
#ifdef ANSI_CPP
  explicit
#endif
  ILPSymbItem(const CharString& name_, int index_=-1) 
		: ILPHashItem(name_,index_), rc_flag(-1), symb_type(0) {}
  
  ///
  void write(std::ostream& os) const;
  
  ///
  void write(PackBuffer& os) const
 	{
	ILPHashItem::write(os);
	os << args << rc_flag << symb_type << map;
	}
  
  ///
  void read(UnPackBuffer& is)
  	{
	ILPHashItem::read(is);
	is >> args >> rc_flag >> symb_type >> map;
  	}
  ///
  void read(std::istream& is)
	{ ILPHashItem::read(is); }

  ///
  ILPArgList args;
  
  /// TRUE if a column, FALSE if a row, -1 if neither
  int rc_flag;
  
  /// Type of symbol: 0-constant, 1-array, 2-set
  int symb_type;
  
  /// Provides a numeric representation of a given variable
  MILPVarMap map;
  
};

} // namespace pico

namespace utilib {

template <>
inline int compare(const pico::ILPRowItem& a, const pico::ILPRowItem& b)
{ return a.compare(&b); }

template <>
inline int compare(const pico::ILPColItem& a, const pico::ILPColItem& b)
{ return a.compare(&b); }

template <>
inline int compare(const pico::ILPSymbItem& a, const pico::ILPSymbItem& b)
{ return a.compare(&b); }


inline size_type hash_fn( const pico::ILPRowItem& a, size_type tmp)
{ return a.hash(tmp); }

inline size_type hash_fn( const pico::ILPColItem& a, size_type tmp)
{ return a.hash(tmp); }

inline size_type hash_fn( const pico::ILPSymbItem& a, size_type tmp)
{ return a.hash(tmp); }

} // namespace utilib

#endif
