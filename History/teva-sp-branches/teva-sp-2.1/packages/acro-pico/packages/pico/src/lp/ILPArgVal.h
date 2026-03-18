/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file ILPArgVal.h
 *
 * Defines the pico::ILPArgVal class.
 */

#ifndef pico_ILPArgVal_h
#define pico_ILPArgVal_h

#include <acro_config.h>
#include <utilib/HashedSet.h>
#include <utilib/PackObject.h>


namespace pico {

using utilib::HashedSet;
using utilib::CharString;
using utilib::BasicArray;
using utilib::PackBuffer;
using utilib::UnPackBuffer;

class ILPArgVal : public utilib::PackObject
{
public:

  ///
  ILPArgVal() {string_set=NULL; int_set=NULL; list_set=NULL; index=-1;}

  /// Set of symbolic names
  HashedSet<CharString>* 	string_set;
  
  /// Set of integers
  HashedSet<int>*        	int_set;
  
  /// Set of lists
  HashedSet<BasicArray<int> >* 	list_set;
  
  /// Index into one of these sets
  int index;

  ///
  int operator==(const ILPArgVal& ) const {return 1;}
  
  ///
  int operator!=(const ILPArgVal& ) const {return 0;}
  
  ///
  int operator<(const ILPArgVal& ) const {return 0;}

  ///
  virtual void write(std::ostream& ) const {}
  
  ///
  virtual void read(std::istream& ) {}

  ///
  virtual void write(PackBuffer& pack) const
  	{
	if (string_set)
	   pack << true << *string_set;
	else
	   pack << false;
	if (int_set)
	   pack << true << *int_set;
	else
	   pack << false;
	if (list_set)
	   pack << true << *list_set;
	else
	   pack << false;
	pack << index;
	}
  
  ///
  virtual void read(UnPackBuffer& unpack)
  	{
	bool tmp;
	unpack >> tmp;
	if (tmp) {
	   string_set = new HashedSet<CharString>;
	   unpack >> *string_set;
	   }
	unpack >> tmp;
	if (tmp) {
	   int_set = new HashedSet<int>;
	   unpack >> *string_set;
	   }
	unpack >> tmp;
	if (tmp) {
	   list_set = new HashedSet<BasicArray<int> >;
	   unpack >> *string_set;
	   }
	unpack >> index;
  	}

};

} // namespace pico

#endif
