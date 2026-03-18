/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file NumArray.h
 *
 * Defines the utilib::NumArray class
 */

#ifndef utilib_NumArray_h
#define utilib_NumArray_h

#if defined(__cplusplus) && defined(ANSI_HDRS)
#include <cmath>
#else
#include <math.h>
#endif
#include <utilib/BasicArray.h>

namespace utilib {

template <class T>
class NumArray;

template <class T>
class Num2DArray;


/**
 * An array data type for numerical data.  NumArray extends the 
 * definition of BasicArray to include numerical vector operations.
 */
template <class T>
class NumArray: public BasicArray<T> {

  typedef BasicArray<T> base_t;
  #if !defined(SOLARIS)
  using base_t::Data;
  using base_t::Len;
  #endif

#if defined (SOLARIS) && !defined(__GNUC__)
  friend Num2DArray<T>;
#else
  template <class U> friend class Num2DArray;
#endif

public:

  /// Empty constructor.
  NumArray() : BasicArray<T>() {}

  /**
   * Constructor that initializes the length and possibly data.
   * The \a len parameter specifies the length of the array.
   * If the \a d parameter is null, then the array is initialized to
   * have a zero value. Otherwise, this array is
   * assumed to have length \a len.  The initialization of the
   * array then depends upon the value of the \a o parameter. If
   * \a o is \b DataNotOwned (the default), then the data is
   * copied from \a d.  Otherwise, the internal point is set to
   * \a d and the internal ownership flag is set to \a o.
   */
#ifndef NON_ANSI
  explicit 
#endif
  NumArray(const size_type len, 
	   T *d=((T*)0), 
	   const EnumDataOwned o=DataNotOwned) : BasicArray<T>()
    { construct(len,d,o); };
  
  /// Copy constructor.
  NumArray(const NumArray& array) : BasicArray<T>(array) {};
  
  /// Default destructor.
  virtual ~NumArray() {}

  /// Copies the \b array object by constructing a new \b NumArray
  NumArray<T>& operator=(const BasicArray<T>& array)
                {BasicArray<T>::operator=(array); return *this;}

  /// Copies the \b array object by constructing a new \b NumArray
  NumArray<T>& operator=(const NumArray<T>& array)
                {BasicArray<T>::operator=(array); return *this;}

  /// Sets the array equal to the pair-wise value \a x + \a y.
  void plus  (const NumArray<T>& x, const NumArray<T>& y);

  /// Sets the array equal to the pair-wise value \a x - \a y.
  void minus (const NumArray<T>& x, const NumArray<T>& y);

  /// Sets the array equal to the pair-wise value \a x * \a y.
  void times (const NumArray<T>& x, const NumArray<T>& y);

  /// Sets the array equal to the pair-wise value \a x / \a y.
  void divide(const NumArray<T>& x, const NumArray<T>& y);

  /// Sets the array equal to the value \a x[i] + z.
  void plus  (const NumArray<T>& x, const T& z);

  /// Sets the array equal to the value \a x[i] - z.
  void minus (const NumArray<T>& x, const T& z);

  /// Sets the array equal to the value \a x[i] * z.
  void times (const NumArray<T>& x, const T& z);

  /// Sets the array equal to the value \a x[i] / z.
  void divide(const NumArray<T>& x, const T& z);

  /// Changes the sign of elements in the array
  NumArray<T> operator-    ();
 
  /// Adds the values of \a x elementwise to the current array.
  NumArray<T>& operator+= (const NumArray<T>& x);

  /// Subtracts the values of \a x elementw to the current array.ise
  NumArray<T>& operator-= (const NumArray<T>& x);

  /// Multiplies the values of \a x elementwise to the current array.
  NumArray<T>& operator*= (const NumArray<T>& x);

  /// Divides the values of \a x elementwise to the current array.
  NumArray<T>& operator/= (const NumArray<T>& x);
 
  /// Adds \a z to the elements of the current array.
  NumArray<T>& operator+=  (const T& z);

  /// Subtracts \a z to the elements of the current array.
  NumArray<T>& operator-=  (const T& z);

  /// Multiplies \a z to the elements of the current array.
  NumArray<T>& operator*=  (const T& z);

  /// Divides \a z to the elements of the current array.
  NumArray<T>& operator/=  (const T& z);

protected:

  /// Initializes the array to zero from \a start to \a stop.
  void initialize(T* data, const size_type start, const size_type stop);

};
 


#ifndef SWIG
#define BINARYOP(opname,pseudonym)\
template <class T>\
inline UTILIB_API NumArray<T> opname (const NumArray<T>& a1, const NumArray<T>& a2)\
{\
NumArray<T> res;\
res.pseudonym(a1,a2);\
return res;\
}\
\
template <class T>\
inline UTILIB_API NumArray<T> opname (const NumArray<T>& a1, const T& val)\
{\
NumArray<T> res;\
res.pseudonym(a1,val);\
return res;\
}\
\
template <class T>\
inline UTILIB_API NumArray<T> opname (const T& val, const NumArray<T>& a1)\
{\
NumArray<T> res;\
res.pseudonym(a1,val);\
return res;\
}

BINARYOP( operator+ , plus )
BINARYOP( operator- , minus )
BINARYOP( operator* , times )
BINARYOP( operator/ , divide )

#undef BINARYOP
#endif // SWIG


/*
template <class T>
T NumArray<T>::operator%(const NumArray<T>& a1, const NumArray<T>& a2)
{
T cum;
T *ptr1, *ptr2;
ptr1 = a1.a->Data;
ptr2 = a2.a->Data;
cum = *ptr1 * *ptr2;
ptr1++; ptr2++;
for (size_type i=1; i<a1.a->len; i++, ptr1++, ptr2++)
  cum += *ptr1 * *ptr2;
return cum;
}
*/


#define BINARYOP(opname,op1,pseudonym, op)\
template <class T>\
void NumArray<T>::pseudonym(const NumArray<T>& a1, const NumArray<T>& a2)\
{\
if (a1.size() != a2.size())\
   EXCEPTION_MNGR(runtime_error, "NumArray<T>::pseudonym : Unequal array lengths " << a1.size() << " and " << a2.size())\
resize(a1.size());\
T* tmp = Data;\
size_type mylen = base_t::size();\
for (size_type i=0; i<mylen; i++)\
  tmp[i] = a1[i] op a2[i];\
}\
\
template <class T>\
void NumArray<T>::pseudonym(const NumArray<T>& a1, const T& val)\
{\
resize(a1.size());\
T* tmp = Data;\
size_type mylen = base_t::size();\
for (size_type i=0; i<mylen; i++)\
  tmp[i] = a1[i] op val;\
}\
\
template <class T>\
NumArray<T>& NumArray<T>::opname(const T& val)\
{\
T* tmp = Data;\
size_type mylen = base_t::size();\
for (size_type i=0; i<mylen; i++) \
  tmp[i] op1 val;\
return(*this); \
}\
\
template <class T>\
NumArray<T>& NumArray<T>::opname(const NumArray<T>& array)\
{\
T* tmp = Data;\
T* arraytmp = array.Data;\
size_type mylen = base_t::size();\
for (size_type i=0; i<mylen; i++) \
  tmp[i] op1 arraytmp[i]; \
return(*this); \
}


BINARYOP(operator+=,+=,plus, + )
BINARYOP(operator-=,-=,minus, - )
BINARYOP(operator*=,*=,times, * )
BINARYOP(operator/=,/=,divide, / )

#undef BINARYOP


template <class T>
NumArray<T> NumArray<T>::operator-    ()
{
size_type mylen = base_t::size();
NumArray<T> res(mylen);
T* tmp = Data;
T* restmp = res.Data;
for (size_type i=0; i<mylen; i++)
  restmp[i] = - tmp[i];
return res;
}


template <class T>
void NumArray<T>::initialize(T* data, const size_type start, 
							const size_type stop)
{
for (size_type i=start; i < stop; i++)
  data[i] = ( T ) 0;
}

} // namespace utilib

#if 0
//============================================================================
//
//
template <class TYPE>
utilib::NumArray<TYPE>& operator<< ( utilib::NumArray<TYPE>& vec, const utilib::NumArray<TYPE>& old)
{
vec.resize(old.size());

for (size_type i=0; i<vec.size(); i++)
  vec[i] = old[i];

return vec;
}


//============================================================================
//
//
template <class TYPE>
utilib::BasicArray<TYPE>& operator<< ( utilib::BasicArray<TYPE>& vec, const utilib::NumArray<TYPE>& old)
{
vec.resize(old.size());

for (size_type i=0; i<vec.size(); i++)
  vec[i] = old[i];

return vec;
}
#endif

#endif
