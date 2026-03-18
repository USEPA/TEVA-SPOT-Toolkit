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

#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>

#include <utilib/NumArray.h>
#include <utilib/Ereal.h>
#include <utilib/exception_mngr.h>

#ifdef ANSI_NAMESPACES
using namespace std;
#endif

namespace utilib {

//using utilib::NumArray;

/// Compute the max of a vector
template <typename TYPE>
TYPE max(const std::vector<TYPE>& vec)
{
if (vec.size() == 0)
   EXCEPTION_MNGR(runtime_error, "max - applied to null vector")
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

#if !defined(CYGWIN) && !defined(LINUX) && !defined(__GNUC__) && !defined(COUGAR) && !defined(OSF) && !defined(TFLOPS_SERVICE) && !defined(SGI) && !defined(RS6K) && !defined(_MSC_VER)
inline ostream& operator<< (ostream& os, bool& flag)
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


inline istream& operator>> (istream& is, bool& flag)
{
string str;
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
vector<TYPE>& operator<< ( vector<TYPE>& vec, const utilib::NumArray<TYPE>& old)
{
vec.resize(old.size());

for (size_type i=0; i<vec.size(); i++)
  vec[i] = old[i];

return vec;
}

/// Stream operator for copying one vector into another
template <class TYPE>
utilib::NumArray<TYPE>& operator<< ( utilib::NumArray<TYPE>& vec, const vector<TYPE>& old)
{
vec.resize(old.size());

for (size_type i=0; i<vec.size(); i++)
  vec[i] = old[i];

return vec;
}

/// Stream operator for copying one vector into another
template <class TYPE>
vector<TYPE>& operator<< ( vector<TYPE>& vec, const vector<TYPE>& old)
{
vec.resize(old.size());
typename vector<TYPE>::const_iterator oldval = old.begin();
typename vector<TYPE>::iterator curr = vec.begin();
typename vector<TYPE>::iterator last = vec.end();

while (curr != last) {
  *curr = *oldval;
  ++curr;
  ++oldval;
  }
return vec;
}

#if defined(SOLARIS)
template <class TYPE>
vector<TYPE>& operator<< ( vector<TYPE>& vec, vector<TYPE>& old)
{
vec.resize(old.size());
typename vector<TYPE>::iterator oldval = old.begin();
typename vector<TYPE>::iterator curr = vec.begin();
typename vector<TYPE>::iterator last = vec.end();

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
vector<TYPE>& operator<< (vector<TYPE>& vec, const TYPE& val)
{
typename vector<TYPE>::iterator curr = vec.begin();
typename vector<TYPE>::iterator last = vec.end();
while (curr != last) {
  *curr = val;
  ++curr;
  }
return vec;
}


/// Stream operator for filling a vector with a given value
template <class TYPE>
vector<utilib::Ereal<TYPE> >& operator<< (vector<utilib::Ereal<TYPE> >& vec, const TYPE& val)
{
typename vector<TYPE>::iterator curr = vec.begin();
typename vector<TYPE>::iterator last = vec.end();
while (curr != last) {
  *curr = val;
  ++curr;
  }
return vec;
}


/// Stream operator for adding one vector with another
template <class TYPE>
vector<TYPE>& operator+= (vector<TYPE>& vec, const vector<TYPE>& val)
{
typename vector<TYPE>::iterator curr = vec.begin();
typename vector<TYPE>::iterator last = vec.end();
typename vector<TYPE>::const_iterator vals = val.begin();
while (curr != last) {
  *curr += *vals;
  ++curr;
  ++vals;
  }
return vec;
}


/// Ampersand-Equal operator for vectors
template <class TYPE>
vector<TYPE>& operator&= ( vector<TYPE>& vec, const vector<TYPE>& old)
{ vec = old; return vec; }



/// Write a list to an output stream
template <class TYPE>
ostream& operator<<(ostream& os, const list<TYPE>& obj)
{
os << obj.size();
if (obj.size() > 0) {
   os << " :";
   typename list<TYPE>::const_iterator curr = obj.begin();
   typename list<TYPE>::const_iterator last = obj.end();
   while (curr != last) {
     os << " " << *curr;
     ++curr;
     }
   }
return os;
}


/// Write a list to an output stream
template <class TYPE>
ostream& operator<<(ostream& os, list<TYPE>& obj)
{
os << obj.size();
if (obj.size() > 0) {
   os << " :";
   typename list<TYPE>::const_iterator curr = obj.begin();
   typename list<TYPE>::const_iterator last = obj.end();
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
ostream& operator<<(ostream& os, const vector<TYPE>& vec)
{
os << vec.size();
if (vec.size() > 0) {
   os << " :";
   typename vector<TYPE>::const_iterator curr = vec.begin();
   typename vector<TYPE>::const_iterator last = vec.end();
   while (curr != last) {
     os << " " << *curr;
     ++curr;
     }
   }
return os;
}


/// Write a vector to an output stream
template <class TYPE>
ostream& operator<<(ostream& os, vector<TYPE>& vec)
{
os << vec.size();
if (vec.size() > 0) {
   os << " :";
   typename vector<TYPE>::const_iterator curr = vec.begin();
   typename vector<TYPE>::const_iterator last = vec.end();
   while (curr != last) {
     os << " " << *curr;
     ++curr;
     }
   }
return os;
}


/// Read a vector from an input stream
template <class TYPE>
istream& operator>>(istream& is, vector<TYPE>& vec)
{
unsigned int tmp;
is >> tmp;
EXCEPTION_TEST( !is, runtime_error, "operator>> - istream problem.")

vec.resize(tmp);
if (vec.size() > 0) {
   char c;
   is >> c;
   typename vector<TYPE>::iterator curr = vec.begin();
   typename vector<TYPE>::iterator last = vec.end();
   while (curr != last) {
     EXCEPTION_TEST( !is, runtime_error, "operator>> - istream problem.")
     is >> *curr;
     ++curr;
     }
   }
return is;
}

#if 0
/// COMMENTED OUT
//
#if !defined(COUGAR) && !defined(TFLOPS_SERVICE)
template <class A, class B>
A& apply_stream(A& a, const B& b)
{
#if defined(SOLARIS) 
operator<<(a,b);
return a;
#else
a << b;
return a;
#endif
}

template <>
inline vector<double>& apply_stream(vector<double>& a, const utilib::NumArray<double>& b)
{
a.resize(b.size());
for (size_type i = 0; i<a.size(); i++)
  a[i] = b[i];
return a;
}

template <>
inline utilib::NumArray<double>& apply_stream(utilib::NumArray<double>& a, const vector<double>& b)
{
a.resize(b.size());
for (size_type i = 0; i<a.size(); i++)
  a[i] = b[i];
return a;
}

#else
//
// For COUGAR, create a similar apply_stream method, with overloaded
// definitions for STL classes
//

template <class A, class B>
A& apply_stream(A& a, const B& b)
{ return operator<<(a,b); }

template <>
vector<int>& apply_stream(vector<int>& a, const int& b)
{ return ::operator<<(a,b); }

template <>
vector<double>& apply_stream(vector<double>& a, const double& b)
{ return ::operator<<(a,b); }

template <>
ostream& apply_stream(ostream& a, const vector<int>& b)
{ return ::operator<<(a,b); }

template <>
ostream& apply_stream(ostream& a, const vector<double>& b)
{ return ::operator<<(a,b); }

template <>
vector<int>& apply_stream(vector<int>& a, const vector<int>& b)
{ return ::operator<<(a,b); }

template <>
vector<double>& apply_stream(vector<double>& a, const vector<double>& b)
{ return ::operator<<(a,b); }

template <>
utilib::NumArray<double>& apply_stream(utilib::NumArray<double>& a, const vector<double>& b)
{ return ::operator<<(a,b); }

#if defined(colin_ColinReal_h)
template <>
vector<colin::real>& apply_stream(vector<colin::real>& a, const vector<colin::real>& b)
{ return ::operator<<(a,b); }
#endif

#endif

#endif

#endif
