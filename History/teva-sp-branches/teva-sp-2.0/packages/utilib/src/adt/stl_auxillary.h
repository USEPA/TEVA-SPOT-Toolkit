/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file stl_auxillary.h
 *
 * Defines auxillary functions for STL classes
 */

#ifndef utilib_stl_auxillary_h
#define utilib_stl_auxillary_h

#include <utilib/utilib_config_bool.h>

#ifdef UTILIB_HAVE_STD
#include <string>
#include <iostream>
#include <vector>
#include <list>
#else
#include <string.h>   
#include <iostream.h>
#include <vector.h>
#include <list.h>
#endif

#ifdef UTILIB_HAVE_EXCEPTIONS
#include <stdexcept>
#else
#error "TODO fix this file to work without exceptions"
#endif

#include <utilib/NumArray.h>
#include <utilib/Ereal.h>
#include <utilib/exception_mngr.h>

namespace utilib {

//using utilib::NumArray;

/// Compute the max of a vector
template <typename TYPE>
TYPE max(const std::vector<TYPE>& vec)
{
if (vec.size() == 0)
   EXCEPTION_MNGR(std::runtime_error, "max - applied to null vector")
typename std::vector<TYPE>::const_iterator start = vec.begin();
typename std::vector<TYPE>::const_iterator end = vec.end();
TYPE max_val = *start;
start++;
while (start < end) {
  if (max_val < (*start)) max_val = *start;
  start++;
  }
return max_val;
}

} // namespace utilib

///
/// Extensions to STL
///

/*
** TODO - find a test for this
*/
#if !defined(CYGWIN) && !defined(LINUX) && !defined(__GNUC__) && !defined(COUGAR) && !defined(OSF) && !defined(TFLOPS_SERVICE) && !defined(SGI) && !defined(RS6K) && !defined(_MSC_VER)
inline std::ostream& operator<< (std::ostream& os, bool& flag)
{
if (flag)
   os << "true";
else
   os << "false";
return os;
}

#if 0
template <class TYPE1, class TYPE2>
inline std::basic_ostream<TYPE1,TYPE2>& operator<<(std::basic_ostream<TYPE1,TYPE2>& os, bool& flag)
{
if (flag)
   os << "true";
else
   os << "false";
return os;
}
#endif


inline std::istream& operator>> (std::istream& is, bool& flag)
{
std::string str;
is >> str;
if ((str == "0") || (str == "false") || (str == "f") || (str == "False")
	         || (str == "FALSE") || (str == "F"))
   flag = false;
else
   flag = true;

return is;
}
#endif


/// Stream operator for copying one vector into another
template <class TYPE>
std::vector<TYPE>& operator<< ( std::vector<TYPE>& vec, const utilib::NumArray<TYPE>& old)
{
vec.resize(old.size());

for (size_type i=0; i<vec.size(); i++)
  vec[i] = old[i];

return vec;
}

/// Stream operator for copying one vector into another
template <class TYPE>
utilib::NumArray<TYPE>& operator<< ( utilib::NumArray<TYPE>& vec, const std::vector<TYPE>& old)
{
vec.resize(old.size());

for (size_type i=0; i<vec.size(); i++)
  vec[i] = old[i];

return vec;
}

/// Stream operator for copying one vector into another
template <class Type1, class Type2>
std::vector<Type1>& operator<< ( std::vector<Type1>& vec, const std::vector<Type2>& old)
{
vec.resize(old.size());
typename std::vector<Type2>::const_iterator oldval = old.begin();
typename std::vector<Type1>::iterator curr = vec.begin();
typename std::vector<Type1>::iterator last = vec.end();

while (curr != last) {
  *curr = *oldval;
  ++curr;
  ++oldval;
  }
return vec;
}

#if defined(SOLARIS_CC)
template <class Type1, class Type2>
std::vector<Type1>& operator<< ( std::vector<Type1>& vec, std::vector<Type2>& old)
{
vec.resize(old.size());
typename std::vector<Type2>::iterator oldval = old.begin();
typename std::vector<Type1>::iterator curr = vec.begin();
typename std::vector<Type1>::iterator last = vec.end();

while (curr != last) {
  *curr = *oldval;
  ++curr;
  ++oldval;
  }
return vec;
}
#endif


/// Stream operator for filling a vector with a given value
template <class TYPE>
std::vector<TYPE>& operator<< (std::vector<TYPE>& vec, const TYPE& val)
{
typename std::vector<TYPE>::iterator curr = vec.begin();
typename std::vector<TYPE>::iterator last = vec.end();
while (curr != last) {
  *curr = val;
  ++curr;
  }
return vec;
}


/// Stream operator for filling a vector with a given value
template <class TYPE>
std::vector<utilib::Ereal<TYPE> >& operator<< (std::vector<utilib::Ereal<TYPE> >& vec, const TYPE& val)
{
typename std::vector<TYPE>::iterator curr = vec.begin();
typename std::vector<TYPE>::iterator last = vec.end();
while (curr != last) {
  *curr = val;
  ++curr;
  }
return vec;
}


/// Stream operator for adding one vector with another
template <class TYPE>
std::vector<TYPE>& operator+= (std::vector<TYPE>& vec, const std::vector<TYPE>& val)
{
typename std::vector<TYPE>::iterator curr = vec.begin();
typename std::vector<TYPE>::iterator last = vec.end();
typename std::vector<TYPE>::const_iterator vals = val.begin();
while (curr != last) {
  *curr += *vals;
  ++curr;
  ++vals;
  }
return vec;
}


/// Ampersand-Equal operator for vectors
template <class TYPE>
std::vector<TYPE>& operator&= ( std::vector<TYPE>& vec, const std::vector<TYPE>& old)
{ vec = old; return vec; }



/// Write a list to an output stream
template <class TYPE>
std::ostream& operator<<(std::ostream& os, const std::list<TYPE>& obj)
{
os << obj.size();
if (obj.size() > 0) {
   os << " :";
   typename std::list<TYPE>::const_iterator curr = obj.begin();
   typename std::list<TYPE>::const_iterator last = obj.end();
   while (curr != last) {
     os << " " << *curr;
     ++curr;
     }
   }
return os;
}


/// Write a list to an output stream
template <class TYPE>
std::ostream& operator<<(std::ostream& os, std::list<TYPE>& obj)
{
os << obj.size();
if (obj.size() > 0) {
   os << " :";
   typename std::list<TYPE>::const_iterator curr = obj.begin();
   typename std::list<TYPE>::const_iterator last = obj.end();
   while (curr != last) {
     os << " " << *curr;
     ++curr;
     }
   }
return os;
}

//
/// Write a vector to an output stream
template <class TYPE>
std::ostream& operator<<(std::ostream& os, const std::vector<TYPE>& vec)
{
os << vec.size();
if (vec.size() > 0) {
   os << " :";
   typename std::vector<TYPE>::const_iterator curr = vec.begin();
   typename std::vector<TYPE>::const_iterator last = vec.end();
   while (curr != last) {
     os << " " << *curr;
     ++curr;
     }
   }
return os;
}


/// Write a vector to an output stream
template <class TYPE>
std::ostream& operator<<(std::ostream& os, std::vector<TYPE>& vec)
{
os << vec.size();
if (vec.size() > 0) {
   os << " :";
   typename std::vector<TYPE>::const_iterator curr = vec.begin();
   typename std::vector<TYPE>::const_iterator last = vec.end();
   while (curr != last) {
     os << " " << *curr;
     ++curr;
     }
   }
return os;
}


/// Read a vector from an input stream
template <class TYPE>
std::istream& operator>>(std::istream& is, std::vector<TYPE>& vec)
{
unsigned int tmp;
is >> tmp;
EXCEPTION_TEST( !is, std::runtime_error, "operator>> - istream problem.")

vec.resize(tmp);
if (vec.size() > 0) {
   char c;
   is >> c;
   typename std::vector<TYPE>::iterator curr = vec.begin();
   typename std::vector<TYPE>::iterator last = vec.end();
   while (curr != last) {
     EXCEPTION_TEST( !is, std::runtime_error, "operator>> - istream problem.")
     is >> *curr;
     ++curr;
     }
   }
return is;
}

#endif
