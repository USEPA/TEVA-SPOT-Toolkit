/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file ValidateParameter.h
 * 
 * Defines the utilib::Parameter class.
 */

#ifndef utilib_ValidateParameter_h
#define utilib_ValidateParameter_h

#include <functional>
#include <utilib/Any.h>


namespace utilib {

/**
 * The base class for a container object that can validate a parameter.
 */
class ParameterValidatorBase
{
public:

  /// Destructor
  virtual ~ParameterValidatorBase() {}

  /// Routine used to validate an AnyReference object
  virtual bool validate(AnyReference& info) = 0;

  /// Write information about the validation criteria
  virtual void write(ostream& os) const = 0;

};


/**
 * A generic base class for the function objects that are used by
 * ParameterValidator.  Deriving from this base class allows users
 * to simplify their create_option() declaration by omitting the second
 * template parameter.
 */
template <class Type>
class ParameterValidatorFunction : public std::unary_function<Type,bool>
{
};


/**
 * Defines a class that can validate a parameter given a 
 * unary_function object.
 */
template <class Type, class ValidatorFuncT=ParameterValidatorFunction<Type> >
class ParameterValidator : public ParameterValidatorBase
{
public:

  /// Constructor
  ParameterValidator(ValidatorFuncT func)
	: test_func(func) {}

  ///
  bool validate(AnyReference& info)
	{return test_func( any_cast<Type>(info) );}

  ///
  void write(ostream& os) const
	{ test_func.write(os); }

  /// Validation object
  ValidatorFuncT test_func;

};




/**
 * Class that validates a lower bound of a parameter.
 */
template <class Type>
class ParameterLowerBound : public std::unary_function<Type,bool>
{
public:

  /// Constructor
  ParameterLowerBound(Type lower_) : lower(lower_) {}

  ///
  void write(ostream& os) const
	{os << "Lower bound: " << lower;}

  ///
  bool operator()(const Type& x)
	{return (x >= lower);}

  ///
  Type lower;
};


/**
 * Class that validates an upper bound of a parameter.
 */
template <class Type>
class ParameterUpperBound : public std::unary_function<Type,bool>
{
public:

  /// Constructor
  ParameterUpperBound(Type upper_) : upper(upper_) {}

  ///
  void write(ostream& os) const
	{os << "Upper bound: " << upper;}

  ///
  bool operator()(const Type& x)
	{return (x <= upper);}

  ///
  Type upper;
};


/**
 * Class that validates a lower and upper bounds of a parameter.
 */
template <class Type>
class ParameterBounds : public std::unary_function<Type,bool>
{
public:

  /// Constructor
  ParameterBounds(Type lower_, Type upper_) : lower(lower_), upper(upper_) {}

  ///
  void write(ostream& os) const
	{os << "Lower bound: " << lower << "  Upper bound: " << upper;}

  ///
  bool operator()(const Type& x)
	{return ((x >= lower) && (x <= upper));}

  ///
  Type lower;

  ///
  Type upper;
};


/**
 * Class that validates a non-negative parameter.
 */
template <class Type>
class ParameterNonnegative : public std::unary_function<Type,bool>
{
public:

  /// Constructor
  ParameterNonnegative() {}

  ///
  void write(ostream& os) const
	{os << "Nonnegative";}

  ///
  bool operator()(const Type& x)
	{return (x >= static_cast<Type>(0));}
};


/**
 * Class that validates a positive parameter.
 */
template <class Type>
class ParameterPositive : public std::unary_function<Type,bool>
{
public:

  /// Constructor
  ParameterPositive() {}

  ///
  void write(ostream& os) const
	{os << "Positive";}

  ///
  bool operator()(const Type& x)
	{return (x > static_cast<Type>(0));}
};

} // namespace utilib


//============================================================================
//
///
inline std::ostream& operator<< (std::ostream& ostr, const utilib::ParameterValidatorBase&
 item)
{ item.write(ostr); return ostr; }


#endif
