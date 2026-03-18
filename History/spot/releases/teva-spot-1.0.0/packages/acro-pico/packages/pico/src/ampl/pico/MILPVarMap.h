/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file MILPVarMap.h
 * 
 * Defines the pico::MILPVarMap class.
 */

#ifndef pico_MILPVarMap_h
#define pico_MILPVarMap_h

#include <acro_config.h>
#include <utilib/BasicArray.h>
#include <utilib/hash_fn.h>
#include <utilib/PackObject.h>

namespace pico {


///
class MILPVarMap : public utilib::PackObject
{
public:

  ///
  MILPVarMap() {}
  
  ///
  virtual ~MILPVarMap() {}

  ///
#ifdef ANSI_CPP
  explicit 
#endif
  MILPVarMap(BasicArray<int>& map, int ndx=-1)
		{varmap = map; set_val(ndx); range_type=-1;}
  
  ///
  int& operator[](const int i)
		{return varmap[i];}
  
  ///
  const int& operator[](const int i) const
		{return varmap[i];}

  /// Computes the type of a token
  static int compute_type(CharString& token);

  ///
  void set_val(double val) {dval = val; range_type = 1;}
  
  ///
  void get_val(double& val) const {val = dval;}

  ///
  void set_val(int val) {ival = val; range_type = 0;}
  
  ///
  void get_val(int& val) const {val = ival;}

  ///
  void set_val(CharString& str);

  ///
  BasicArray<int> varmap;
  
  ///
  int range_type;

  ///
  void write(std::ostream& os) const
	{
	int tmp;
	get_val(tmp);
	os << varmap << tmp; 
	}

  ///
  void read(std::istream& unpack)
  	{
	int tmp;
	unpack >> varmap >> tmp; 
	set_val(tmp);
	}

  ///
  void write(PackBuffer& pack) const
	{ pack << varmap << range_type << dval << ival; }

  ///
  void read(UnPackBuffer& unpack)
  	{ unpack >> varmap >> range_type >> dval >> ival; }

protected:

  ///
  double dval;
  
  ///
  int ival;
};




inline int MILPVarMap::compute_type(CharString& token)
{
int ans;
int status;
utilib::aslong(token,status);
if (status == OK)
   ans = 0;
else {
   utilib::asdouble(token,status);
   if (status == OK)
      ans = 1;
   else
      ans = 2;
   }
return ans;
}

inline void MILPVarMap::set_val(CharString& token)
{
int status;
if (range_type == 0)
   ival = utilib::aslong(token,status);
if (range_type == 1)
   dval = utilib::asdouble(token,status);
//if (range_type == 2)
   //sval = token;
}

} // namespace pico


namespace utilib {

/// A hash function
inline size_type hash_fn(const pico::MILPVarMap& obj, size_type table_size)
{ return hash_fn(obj.varmap, table_size);}

/// Compare two MILPVarMap objects
template <>
inline int compare(const pico::MILPVarMap& a, const pico::MILPVarMap& b)
{ return a.varmap.compare(b.varmap); }

} // namespace utilib

#endif
