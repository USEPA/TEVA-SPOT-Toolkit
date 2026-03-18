/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file Ereal.h
 *
 * Defines the utilib::Ereal class and associated operators
 */

#ifndef utilib_Ereal_h
#define utilib_Ereal_h

#include <iostream>
#ifndef ANSI_HDRS
#include <math.h>
#include <ctype.h>
#else
#include <cmath>
#include <cctype>
#endif
#include <utilib/math_basic.h>

#include <utilib/math_basic.h>
#include <utilib/PackBuf.h>

#ifdef ANSI_NAMESPACES
using namespace std;
#endif

namespace utilib {

template <class Type> class Ereal;

template <class Type> Ereal<Type>
operator+ (const Ereal<Type>& x, const Ereal<Type>& y);

template <class Type, class NType> Ereal<Type>
operator+ (const Ereal<Type>& x, const NType& y);

template <class Type, class NType> Ereal<Type>
operator+ (const NType& x, const Ereal<Type>& y);

template <class Type> Ereal<Type>
operator- (const Ereal<Type>& x, const Ereal<Type>& y);

template <class Type, class NType> Ereal<Type>
operator- (const Ereal<Type>& x, const NType& y);

template <class Type, class NType> Ereal<Type>
operator- (const NType& x, const Ereal<Type>& y);

template <class Type> Ereal<Type>
operator* (const Ereal<Type>& x, const Ereal<Type>& y);

template <class Type, class NType> Ereal<Type>
operator* (const Ereal<Type>& x, const NType& y);

template <class Type, class NType> Ereal<Type>
operator* (const NType& x, const Ereal<Type>& y);

template <class Type> Ereal<Type>
operator/ (const Ereal<Type>& x, const Ereal<Type>& y);

template <class Type, class NType> Ereal<Type>
operator/ (const Ereal<Type>& x, const NType& y);

template <class Type, class NType> Ereal<Type>
operator/ (const NType& x, const Ereal<Type>& y);

template <class Type> Ereal<Type>
operator+ (const Ereal<Type>& x);

template <class Type> Ereal<Type> 
operator- (const Ereal<Type>& num);

template <class Type> bool
operator== (const Ereal<Type>& x, const Ereal<Type>& y);

template <class Type, class NType> bool
operator== (const Ereal<Type>& x, const NType& y);

template <class Type, class NType> bool
operator== (const NType& x, const Ereal<Type>& y);

template <class Type> bool
operator!= (const Ereal<Type>& x, const Ereal<Type>& y);

template <class Type, class NType> bool
operator!= (const Ereal<Type>& x, const NType& y);

template <class Type, class NType> bool
operator!= (const NType& x, const Ereal<Type>& y);

template <class Type> bool
operator> (const Ereal<Type>& x, const Ereal<Type>& y);

template <class Type, class NType> bool
operator> (const Ereal<Type>& x, const NType& y);

template <class Type, class NType> bool
operator> (const NType& x, const Ereal<Type>& y);

template <class Type> bool
operator>= (const Ereal<Type>& x, const Ereal<Type>& y);

template <class Type, class NType> bool
operator>= (const Ereal<Type>& x, const NType& y);

template <class Type, class NType> bool
operator>= (const NType& x, const Ereal<Type>& y);

template <class Type> bool
operator< (const Ereal<Type>& x, const Ereal<Type>& y);

template <class Type, class NType> bool
operator< (const Ereal<Type>& x, const NType& y);

template <class Type, class NType> bool
operator< (const NType& x, const Ereal<Type>& y);

template <class Type> bool
operator<= (const Ereal<Type>& x, const Ereal<Type>& y);

template <class Type, class NType> bool
operator<= (const Ereal<Type>& x, const NType& y);

template <class Type, class NType> bool
operator<= (const NType& x, const Ereal<Type>& y);

template <class Type>
#if defined(RS6K)
bool Ereal_isinf(const Ereal<Type>& num);
#else
bool isinf(const Ereal<Type>& num);
#endif

template <class Type>
#if defined(RS6K)
bool Ereal_isnan(const Ereal<Type>& num);
#else
bool isnan(const Ereal<Type>& num);
#endif

template <class Type>
bool isind(const Ereal<Type>& num);

template <class Type>
bool finite(const Ereal<Type>& num);



/**
 * Defines an extension of 'real' (e.g. double, float, long double)  
 * that can 'assume' all 'real' values, as well as -infinity and +infinity.
 */
template <class Type>
class Ereal
{
public:

  /// Constructor that initializes the Ereal with a value.
  Ereal(const Type num=0.0);

  /// Copy constructor
  Ereal(const Ereal& num)
	{
	val    = num.val;
	Finite = num.Finite;
	}

  /// The plus-equals operator.
  const Ereal& operator+= (const Ereal<Type>& num)
	{
	plus(val,Finite,num.val,num.Finite,val,Finite);
	return *this;
	}

  /// Thus minus-equals operator.
  const Ereal& operator-= (const Ereal<Type>& num)
	{
	minus(val,Finite,num.val,num.Finite,val,Finite);
	return *this;
	}

  /// The times-equals operator.
  const Ereal& operator*= (const Ereal<Type>& num)
	{
	mult(val,Finite,num.val,num.Finite,val,Finite);
	return *this;
	}

  /// The divide-equals operator.
  const Ereal& operator/= (const Ereal<Type>& num)
	{
	div(val,Finite,num.val,num.Finite,val,Finite);
	return *this;
	}

  /// The plus-equals operator.
  template <class NType>
  const Ereal& operator+= (const NType& num)
	{
	Type xval = num;
	bool xFinite = true;
	Ereal<Type>::check_if_infinite(val,Finite);

	plus(xval,xFinite,val,Finite,val,Finite);
	return *this;
	}

  /// Thus minus-equals operator.
  template <class NType>
  const Ereal& operator-= (const NType& num)
	{
	Type xval = num;
	bool xFinite = true;
	Ereal<Type>::check_if_infinite(val,Finite);

	minus(xval,xFinite,val,Finite,val,Finite);
	return *this;
	}

  /// The times-equals operator.
  template <class NType>
  const Ereal& operator*= (const NType& num)
	{
	Type xval = num;
	bool xFinite = true;
	Ereal<Type>::check_if_infinite(val,Finite);

	mult(xval,xFinite,val,Finite,val,Finite);
	return *this;
	}

  /// The divide-equals operator.
  template <class NType>
  const Ereal& operator/= (const NType& num)
	{
	Type xval = num;
	bool xFinite = true;
	Ereal<Type>::check_if_infinite(val,Finite);

	div(xval,xFinite,val,Finite,val,Finite);
	return *this;
	}

  /// Coerces the Ereal to type \a Type.
  operator Type () const;

#if defined(NON_ANSI) || defined(COUGAR) || defined(SGI) || defined(OSF) || defined(CPLANT) || defined(DOXYGEN)
  /// Write the Ereal to a stream.
  ostream& write(ostream& os) const;

  /// Read an Ereal from a stream.
  istream& read(istream& is);
#else
  /// Write the Ereal to a stream.
  std::ostream& write(std::ostream& os) const;

  /// Read an Ereal from a stream.
  std::istream& read(std::istream& is);
#endif

  /// Pack an Ereal into a \a PackBuffer.
  PackBuffer& write(PackBuffer& os) const;

  /// Unpack an Ereal from a \a PackBuffer.
  UnPackBuffer& read(UnPackBuffer& is);

  template <class LType>
  friend Ereal<LType> operator+ (const Ereal<LType>& x, const Ereal<LType>& y);

  template <class LType, class NType>
  friend Ereal<LType> operator+ (const Ereal<LType>& x, const NType& y);

  template <class LType, class NType>
  friend Ereal<LType> operator+ (const NType& x, const Ereal<LType>& y);

  template <class LType>
  friend Ereal<LType> operator- (const Ereal<LType>& x, const Ereal<LType>& y);

  template <class LType, class NType>
  friend Ereal<LType> operator- (const Ereal<LType>& x, const NType& y);

  template <class LType, class NType>
  friend Ereal<LType> operator- (const NType& x, const Ereal<LType>& y);

  template <class LType>
  friend Ereal<LType> operator* (const Ereal<LType>& x, const Ereal<LType>& y);

  template <class LType, class NType>
  friend Ereal<LType> operator* (const Ereal<LType>& x, const NType& y);

  template <class LType, class NType>
  friend Ereal<LType> operator* (const NType& x, const Ereal<LType>& y);

  template <class LType>
  friend Ereal<LType> operator/ (const Ereal<LType>& x, const Ereal<LType>& y);

  template <class LType, class NType>
  friend Ereal<LType> operator/ (const Ereal<LType>& x, const NType& y);

  template <class LType, class NType>
  friend Ereal<LType> operator/ (const NType& x, const Ereal<LType>& y);

  friend Ereal<Type> operator+ <> (const Ereal<Type>& x);

  friend Ereal<Type> operator- <>(const Ereal<Type>& num);

  template <class LType, class NType>
  friend bool operator== (const NType& x, const Ereal<LType>& y);

  friend bool operator== <Type> (const Ereal<Type>&, const Ereal<Type>&);

  template <class LType, class NType>
  friend bool operator== (const Ereal<LType>&, const NType&);

  template <class LType>
  friend bool operator< (const Ereal<LType>&, const Ereal<LType>&);

  template <class LType, class NType>
  friend bool operator< (const Ereal<LType>&, const NType&);

  template <class LType, class NType>
  friend bool operator< (const NType& x, const Ereal<LType>& y);

  template <class LType>
  friend bool operator<= (const Ereal<LType>&, const Ereal<LType>&);

  template <class LType, class NType>
  friend bool operator<= (const Ereal<LType>&, const NType&);

  template <class LType, class NType>
  friend bool operator<= (const NType& x, const Ereal<LType>& y);

#if defined (RS6K)
  friend bool Ereal_isinf<> (const Ereal<Type>& num);
#else
  friend bool isinf<> (const Ereal<Type>& num);
#endif

#if defined (RS6K)
  friend bool Ereal_isnan<> (const Ereal<Type>& num);
#else
  friend bool isnan<> (const Ereal<Type>& num);
#endif

#if defined(COUGAR) || defined(SGI)
  friend bool utilib::isind<> (const Ereal<Type>& num);
#else
  friend bool isind<> (const Ereal<Type>& num);
#endif

  friend bool finite<> (const Ereal<Type>& num);

  /// A standard definition of positive infinity.
  static Ereal<Type> positive_infinity;

  /// A standard definition of negative infinity.
  static Ereal<Type> negative_infinity;

  /// A standard definition of negative infinity.
  static Ereal<Type> NaN;

#ifndef DARWIN 
#if ( (__GNUC__ != 3) || (__GNUC_MINOR__ != 3) ) 
  // JPW: under gcc 3.3, protection of this constructor prevents initialization
  //      of the static positive_infinity and negative_infinity members (the 
  //      initializer can't call the constructor even though it's in the same
  //      class). This is a compiler bug - I think.
protected:
#endif
#endif

  /// A constructor that specifies the internal states of the Ereal.
  Ereal(const Type num, const bool f_flag) : val(num), Finite(f_flag) {}

#ifdef DARWIN
 protected:
#endif

  /// The double value of the Ereal.
  Type val;

  /// A flag that indicates whether or not the Ereal is finite.
  bool Finite;

  /// A plus operator.
  static void plus(const Type xval, const bool xFinite, 
		const Type yval, const bool yFinite, 
		Type& rval, bool& rFinite);

  /// A minus operator.
  static void minus(const Type xval, const bool xFinite, 
		const Type yval, const bool yFinite, 
		Type& rval, bool& rFinite);

  /// A multiplication operator.
  static void mult(const Type xval, const bool xFinite, 
		const Type yval, const bool yFinite, 
		Type& rval, bool& rFinite);

  /// A division operator.
  static void div(const Type xval, const bool xFinite, 
		const Type yval, const bool yFinite, 
		Type& rval, bool& rFinite);

  /**
   * Set the flag \a Finite to true if \a val is large enough to be 
   * infinite.
   */
  static void check_if_infinite(Type& val, bool& Finite);

  /// The value of \a Type that represents positive infininity
  static Type positive_infinity_val;

  /// The value of \a Type that represents negative infininity
  static Type negative_infinity_val;

};


template <class Type>
Ereal<Type> Ereal<Type>::positive_infinity(1.0,false);


template <class Type>
Ereal<Type> Ereal<Type>::negative_infinity(-1.0,false);


template <class Type>
Ereal<Type> Ereal<Type>::NaN(2.0,false);


template <class Type>
inline void Ereal<Type>::check_if_infinite(Type& val, bool& Finite)
{
if (Finite) {
   if (val <= negative_infinity_val) {
      Finite = false;
      val = -1.0;
      }
   else if (val >= positive_infinity_val) {
      Finite = false;
      val = 1.0;
      }
   }
}


template <class Type>
inline Ereal<Type>::Ereal(const Type num)
{
val = num;
Finite = true;
check_if_infinite(val,Finite);
}


template <class Type>
inline Ereal<Type>::operator Type () const
{
if (Finite) 
   return val;

if (val == -1.0)
   return negative_infinity_val;
else if (val == 1.0)
   return positive_infinity_val;
else if (val == 2.0)
   return 0.0;			// TODO: exception - NaN
else 
   return 0.0;			// TODO: exception - NaN
}


template <class Type> 
inline void Ereal<Type>::plus(const Type xval, const bool xFinite, 
			const Type yval, const bool yFinite, 
			Type& rval, bool& rFinite)
{
if (xFinite) {
   if (yFinite) {
      rval = xval + yval;
      rFinite = true;
      check_if_infinite(rval,rFinite);
      }
   else {
      rFinite = false;
      rval = yval;
      }
   }

else {
   rFinite = false;
   if (!yFinite) {
      if ((xval == 2.0) || (yval == 2.0))
         rval = 2.0;				// NaN
      else if (xval*yval == 0.0)
         rval = 0.0;				// Indeterminate
      else if (xval*yval == -1.0)
         rval = 0.0;				// Indeterminate: infty - infty
      else
	 rval = xval;
      }
   else
      rval = xval;
   }
}


template <class Type> 
inline void Ereal<Type>::minus(const Type xval, const bool xFinite, 
			const Type yval, const bool yFinite, 
			Type& rval, bool& rFinite)
{
if (xFinite) {
   if (yFinite) {
      rval = xval - yval;
      rFinite = true;
      check_if_infinite(rval,rFinite);
      }
   else {
      rFinite = false;
      if (yval == 2.0)
         rval = 2.0;
      else
         rval = - yval;
      }
   }

else {
   rFinite = false;
   if (!yFinite) {
      if ((xval == 2.0) || (yval == 2.0))
         rval = 2.0;				// NaN
      else if (xval*yval == 0.0)
         rval = 0.0;				// Indeterminate
      else if (xval*yval == 1.0)
         rval = 0.0;				// Indeterminate: infty - infty
      else
	 rval = xval;
      }
   else
      rval = xval;
   }
}


template <class Type> 
inline void Ereal<Type>::mult(const Type xval, const bool xFinite, 
			const Type yval, const bool yFinite, 
			Type& rval, bool& rFinite)
{
if (xFinite) {
   if (yFinite) {
      rval = xval * yval;
      rFinite = true;
      check_if_infinite(rval,rFinite);
      }
   else {
      rFinite = false;
      if (yval == 2.0)
         rval = 2.0;
      else if (xval*yval == 0.0)		// Indeterminate: infty * 0
         rval = 0.0;				//             OR Ind * num
      else if (xval*yval > 0.0)
         rval = 1.0;
      else 
         rval = -1.0;
      }
   }

else {
   rFinite = false;
   if (yFinite) {
      if (xval == 2.0)
         rval = 2.0;
      else if (xval*yval == 0.0)		// Indeterminate: infty * 0
         rval = 0.0;				//             OR Ind * num
      else if (xval*yval > 0.0)
         rval = 1.0;
      else 
         rval = -1.0;
      }
   else {
      if ((xval == 2.0) || (yval == 2.0))
         rval = 2.0;
      else 
	 rval = xval*yval;			// Note: handles cases where
						//   either is Ind.
      }
   }
}


template <class Type>
inline void Ereal<Type>::div(const Type xval, const bool xFinite, 
		const Type yval, const bool yFinite, 
		Type& rval, bool& rFinite)
{
if (xFinite) {
   if (yFinite) {
      if (yval == 0.0) {
         rFinite = false;
         if (xval > 0.0)
            rval = 1.0;
	 else if (xval < 0.0)
	    rval = -1.0;
	 else 
	    rval = 0.0;			   // Indeterminate: 0 / 0
         }
      else {
         rFinite = true;
         rval = xval / yval;
         check_if_infinite(rval,rFinite);
         }
      }
   else {
      rval = 0.0;
      rFinite = true;
      }
   }

else {
   rFinite = false;
   if (yFinite) {
      if (yval < 0.0)
         rval = - xval;
      else if (yval == 0.0)
         rval = 0.0;				// Indeterminate: infty / 0
      else
         rval = xval;
      }
   else
      rval = 0.0;				// Indeterminate: 
						//        +/- infty / +/- infty
   }
}


template <class Type> inline
#if defined(NON_ANSI) || defined(COUGAR) || defined(SGI) || defined(OSF) || defined(CPLANT) || defined(DOXYGEN)
ostream& Ereal<Type>::write(ostream& os) const
#else
std::ostream& Ereal<Type>::write(std::ostream& os) const
#endif
{
if (Finite)
   os << val;
else {
   if (val == -1.0)
      os << "-Infinity";
   else if (val == 1.0)
      os << "Infinity";
   else if (val == 2.0)
      os << "NaN";
   else if (val == 0.0)
      os << "Indeterminate";
   else
      os << "Ereal_Bad_Value=" << val;		// Generate error
   }
return os;
}


template <class Type> inline
#if defined(NON_ANSI) || defined(COUGAR) || defined(SGI) || defined(OSF) || defined(CPLANT) || defined(DOXYGEN)
istream& Ereal<Type>::read(istream& is)
#else
std::istream& Ereal<Type>::read(std::istream& is)
#endif
{
char c;
is.get(c);

bool neg_flag=false;
if (c == '-') {
   neg_flag = true;
   is.get(c);
   }

if (isdigit(c)) {
   is.putback(c);
   is >> val;
   if (neg_flag)
      val = -val;
   Finite = true;
   check_if_infinite(val,Finite);
   }
else if (isalpha(c)) {
   is.get(c);
   if (c == 'I') {
      is.get(c);
      if (c != 'n') 
         EXCEPTION_MNGR(runtime_error, "Ereal::read - expected \'n\' character after \'I\'")
      is.get(c);
      if (c == 'd') {
         EXCEPTION_MNGR(runtime_error, "Ereal::read - processed \'d\' character for a negative value")
         // Read "Indeterminate" here...
         }
      else if (c == 'f') {
         // Read "Infinity" here...
         }
      else
         EXCEPTION_MNGR(runtime_error, "Ereal::read - unknown error")
      }
   else if (c == 'N') {
      if (neg_flag) 
         EXCEPTION_MNGR(runtime_error, "Ereal::read - processed \'N\' character")
      // Read "NaN" here...
      }
   // else generate an exception
   }

return is;
}


template <class Type> inline
PackBuffer& Ereal<Type>::write(PackBuffer& os) const
{ os << Finite << val;  return os; }


template <class Type> inline
UnPackBuffer& Ereal<Type>::read(UnPackBuffer& is)
{ is >> Finite >> val; return is; }


/// Plus operator: Ereal + Ereal
template <class Type> inline Ereal<Type>
operator+ (const Ereal<Type>& x, const Ereal<Type>& y)
{
Type val;
bool Finite;
Ereal<Type>::plus(x.val, x.Finite, y.val, y.Finite, val, Finite);
return Ereal<Type>(val,Finite);
}


/// Plus operator: Ereal + Type
template <class Type, class NType> inline Ereal<Type>
operator+ (const Ereal<Type>& x, const NType& y)
{
Type yval = y;
bool yFinite = true;
Ereal<Type>::check_if_infinite(yval,yFinite);

Type val;
bool Finite;
Ereal<Type>::plus(x.val, x.Finite, yval, yFinite, val, Finite);
return Ereal<Type>(val,Finite);
}


/// Plus operator: Type + Ereal
template <class Type, class NType> inline Ereal<Type>
operator+ (const NType& x, const Ereal<Type>& y)
{
Type xval = x;
bool xFinite = true;
Ereal<Type>::check_if_infinite(xval,xFinite);

Type val;
bool Finite;
Ereal<Type>::plus(xval, xFinite, y.val, y.Finite, val, Finite);
return Ereal<Type>(val,Finite);
}


/// Minus operator: Ereal - Ereal
template <class Type> inline Ereal<Type>
operator- (const Ereal<Type>& x, const Ereal<Type>& y)
{
Type val;
bool Finite;
Ereal<Type>::minus(x.val, x.Finite, y.val, y.Finite, val, Finite);
return Ereal<Type>(val,Finite);
}


/// Minus operator: Ereal - Type
template <class Type, class NType> inline Ereal<Type>
operator- (const Ereal<Type>& x, const NType& y)
{
Type yval = y;
bool yFinite = true;
Ereal<Type>::check_if_infinite(yval,yFinite);

Type val;
bool Finite;
Ereal<Type>::minus(x.val, x.Finite, yval, yFinite, val, Finite);
return Ereal<Type>(val,Finite);
}


/// Minus operator: Type - Ereal
template <class Type, class NType> inline Ereal<Type>
operator- (const NType& x, const Ereal<Type>& y)
{
Type xval = x;
bool xFinite = true;
Ereal<Type>::check_if_infinite(xval,xFinite);

Type val;
bool Finite;
Ereal<Type>::minus(xval, xFinite, y.val, y.Finite, val, Finite);
return Ereal<Type>(val,Finite);
}


/// Times operator: Ereal * Ereal
template <class Type> inline Ereal<Type>
operator* (const Ereal<Type>& x, const Ereal<Type>& y)
{
Type val;
bool Finite;
Ereal<Type>::mult(x.val, x.Finite, y.val, y.Finite, val, Finite);
return Ereal<Type>(val,Finite);
}


/// Times operator: Ereal * Type
template <class Type, class NType> inline Ereal<Type>
operator* (const Ereal<Type>& x, const NType& y)
{
Type yval = y;
bool yFinite = true;
Ereal<Type>::check_if_infinite(yval,yFinite);

Type val;
bool Finite;
Ereal<Type>::mult(x.val, x.Finite, yval, yFinite, val, Finite);
return Ereal<Type>(val,Finite);
}


/// Times operator: Type * Ereal
template <class Type, class NType> inline Ereal<Type>
operator* (const NType& x, const Ereal<Type>& y)
{
Type xval = x;
bool xFinite = true;
Ereal<Type>::check_if_infinite(xval,xFinite);

Type val;
bool Finite;
Ereal<Type>::mult(xval, xFinite, y.val, y.Finite, val, Finite);
return Ereal<Type>(val,Finite);
}


/// Division operator: Ereal / Ereal
template <class Type> Ereal<Type>
operator/ (const Ereal<Type>& x, const Ereal<Type>& y)
{
Type val;
bool Finite;
Ereal<Type>::div(x.val, x.Finite, y.val, y.Finite, val, Finite);
return Ereal<Type>(val,Finite);
}


/// Division operator: Ereal / Type
template <class Type, class NType> Ereal<Type>
operator/ (const Ereal<Type>& x, const NType& y)
{
Type yval = y;
bool yFinite = true;
Ereal<Type>::check_if_infinite(yval,yFinite);

Type val;
bool Finite;
Ereal<Type>::div(x.val, x.Finite, yval, yFinite, val, Finite);
return Ereal<Type>(val,Finite);
}


/// Division operator: Type / Ereal
template <class Type, class NType> Ereal<Type>
operator/ (const NType& x, const Ereal<Type>& y)
{
Type xval = x;
bool xFinite = true;
Ereal<Type>::check_if_infinite(xval,xFinite);

Type val;
bool Finite;
Ereal<Type>::div(xval, xFinite, y.val, y.Finite, val, Finite);
return Ereal<Type>(val,Finite);
}


/// Plus operator: + Ereal
template <class Type> inline Ereal<Type>
operator+ (const Ereal<Type>& x)
{ return x; }


/// Minus operator: - Ereal
template <class Type> inline 
Ereal<Type> operator- (const Ereal<Type>& num)
{
if (num.Finite)
   return Ereal<Type>(-num.val, num.Finite);
else if (num.val == 1.0)
   return Ereal<Type>(-1.0, num.Finite);
else if (num.val == -1.0)
   return Ereal<Type>(1.0, num.Finite);
else 
   return num;
}


/// Equality operator: Ereal == Ereal
template <class Type> inline
bool operator== (const Ereal<Type>& x, const Ereal<Type>& y)
{
if (x.Finite) {
   if (y.Finite)
      return (x.val == y.val);
   else
      return false; 
   }
else {
   if (y.Finite)
      return false;
   else
      return (x.val == y.val);			// Correct?
   }
}


/// Equality operator: Ereal == Type
template <class Type, class NType> inline
bool operator== (const Ereal<Type>& x, const NType& y)
{
if (x.Finite)
   return (x.val == y);
return false;
}


/// Equality operator: Type == Ereal
template <class Type, class NType> inline
bool operator== (const NType& x, const Ereal<Type>& y)
{ return (y == x); }


/// Not-equal operator: Ereal != Ereal
template <class Type> inline bool
operator!= (const Ereal<Type>& x, const Ereal<Type>& y)
{ return (!(y == x)); }


/// Not-equal operator: Ereal != Type
template <class Type, class NType> inline bool
operator!= (const Ereal<Type>& x, const NType& y)
{ return (!(y == x)); }


/// Not-equal operator: Type != Ereal
template <class Type, class NType> inline bool
operator!= (const NType& x, const Ereal<Type>& y)
{ return (y != x); }


/// Greater-Than operator: Ereal > Ereal
template <class Type> inline
bool operator> (const Ereal<Type>& x, const Ereal<Type>& y)
{ return (! (x <= y)); }


/// Greater-Than operator: Ereal > Type
template <class Type, class NType> inline
bool operator> (const Ereal<Type>& x, const NType& y)
{ return (! (x <= y)); }


/// Greater-Than operator: Type > Ereal
template <class Type, class NType> inline 
bool operator> (const NType& x, const Ereal<Type>& y)
{ return (!(x <= y)); }


/// Greater-Than-Or-Equal operator: Ereal >= Ereal
template <class Type> inline 
bool operator>= (const Ereal<Type>& x, const Ereal<Type>& y)
{ return (!(x < y)); }


/// Greater-Than-Or-Equal operator: Type >= Ereal
template <class Type, class NType> inline 
bool operator>= (const NType& x, const Ereal<Type>& y)
{ return (!(x < y)); }


/// Greater-Than-Or-Equal operator: Ereal >= Type
template <class Type, class NType> inline 
bool operator>= (const Ereal<Type>& x, const NType& y)
{ return (!(x < y)); }


/// Less-Than operator: Ereal < Ereal
template <class Type> inline
bool operator< (const Ereal<Type>& x, const Ereal<Type>& y)
{
if (x.Finite) {
   if (y.Finite)
      return (x.val < y.val);
   else if (y.val == 1.0)
      return true;
   else if (y.val == -1.0)
      return false; 					// TODO: errors
   return false;					// ERRORS!
   }

else {
   if (y.Finite) {
      if (x.val == 1.0)
         return false;
      else if (x.val == -1.0)
         return true;
      return false;					// TODO: errors
      }
   else {
      if ((x.val == -1.0) && (y.val == 1.0))
         return true;
      return false; 					// TODO: errors
      }
   }
}


/// Less-Than operator: Ereal < Type
template <class Type, class NType> inline
bool operator< (const Ereal<Type>& x, const NType& y)
{
if (x.Finite)
   return (x.val < y);

else {
   if (x.val == 1.0)
      return false;
   else if (x.val == -1.0)
      return true;
   return false;					// TODO: errors
   }
}


/// Less-Than operator: Type < Ereal
template <class Type, class NType> inline bool
operator< (const NType& x, const Ereal<Type>& y)
{
if (y.Finite)
   return (x < y.val);

else {
   if (y.val == 1.0)
      return true;
   else if (y.val == -1.0)
      return false;
   return false;					// TODO: errors
   }
}


/// Less-Than-Or-Equal operator: Ereal <= Ereal
template <class Type> inline
bool operator<= (const Ereal<Type>& x, const Ereal<Type>& y)
{
if (x.Finite) {
   if (y.Finite)
      return (x.val <= y.val);
   else if (y.val == 1.0)
      return true;
   else if (y.val == -1.0)
      return false;
   return false; 					// TODO: errors
   }

else {
   if (y.Finite) {
      if (x.val == 1.0)
         return false;
      else if (x.val == -1.0)
         return true; 
      return false;					// TODO: errors
      }
   else {
      if ((x.val == 1.0) && (y.val == -1.0))
         return false;
      return true; 					// TODO: errors
      }
   }
}


/// Less-Than-Or-Equal operator: Ereal <= Type
template <class Type, class NType> inline
bool operator<= (const Ereal<Type>& x, const NType& y)
{
if (x.Finite)
   return (x.val <= y);

else {
   if (x.val == 1.0)
      return false;
   else if (x.val == -1.0)
      return true;
   return false;					// TODO: errors
   }
}


/// Less-Than-Or-Equal operator: Type <= Ereal
template <class Type, class NType> inline bool
operator<= (const NType& x, const Ereal<Type>& y)
{
if (y.Finite)
   return (x <= y.val);
else {
   if (y.val == 1.0)
      return true;
   else if (y.val == -1.0)
      return false;
   return false;
   }
}



/// Returns true if the Ereal object represents infinity
template <class Type> inline
#if defined(RS6K)
bool Ereal_isinf(const Ereal<Type>& num)
#else
bool isinf(const Ereal<Type>& num)
#endif
{ return (!num.Finite && ((num.val == 1.0) || (num.val == -1.0))); }


/// Returns true if the Ereal object represents not-a-number
template <class Type> inline
#if defined(RS6K)
bool Ereal_isnan(const Ereal<Type>& num)
#else
bool isnan(const Ereal<Type>& num)
#endif
{ return (!num.Finite && num.val == 2.0); }


/// Returns true if the Ereal object represents an indeterminant value
template <class Type> inline
bool isind(const Ereal<Type>& num)
{ return (!num.Finite && num.val == 0.0); }


/// Returns true if the Ereal object is a finite value
template <class Type> inline
bool finite(const Ereal<Type>& num)
{ return (num.Finite); }


template<class Type>
Type Ereal<Type>::positive_infinity_val = (Type) 1e16;


template<class Type>
Type Ereal<Type>::negative_infinity_val = (Type) -1e16;

/// Rounds an Ereal to an integer
template<class Type>
long int lround(Ereal<Type>& val)
{
if (val == Ereal<Type>::positive_infinity)
   return LONG_MAX;
if (val == Ereal<Type>::negative_infinity)
   return LONG_MIN;
/// TODO - handle NaN and Ind states
double tmp = val;
if (tmp > LONG_MAX)
   return LONG_MAX;
if (tmp < LONG_MIN)
   return LONG_MIN;
return ::lround(tmp);
}


} // namespace utilib

/// In-Stream operator: istream >> Ereal
template <class Type> inline
istream& operator>> (istream& is, utilib::Ereal<Type>& num)
{ num.read(is); return is; }


/// Out-Stream operator: ostream << Ereal
template <class Type> inline
ostream& operator<< (ostream& os, const utilib::Ereal<Type>& num)
{ num.write(os); return os; }

/// In-Stream operator: UnPackBuffer >> Ereal
template <class Type> inline
utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& is, utilib::Ereal<Type>& num)
{ return num.read(is); }


/// Out-Stream operator: PackBuffer << Ereal
template <class Type> inline
utilib::PackBuffer& operator<< (utilib::PackBuffer& os, const utilib::Ereal<Type>& num)
{ return num.write(os); }

#endif
