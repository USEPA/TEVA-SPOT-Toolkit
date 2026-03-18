/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file Parameter.h
 * 
 * Defines the utilib::Parameter class.
 *
 * \todo revise how strings are written/read
 *		empty strings do not generate anything!
 * \todo add parameter categories
 */

#ifndef utilib_Parameter_h
#define utilib_Parameter_h

#include <utilib/utilib_config_bool.h>
#include <utilib/std_headers.h>
#include <utilib/exception_mngr.h>
#include <utilib/ValidateParameter.h>
#include <utilib/SmartPtr.h>
#include <utilib/PackObject.h>

namespace utilib {


/**
 * Defines the information for a parameter included in a ParameterSet object.
 *
 * This class stores the value of a parameter using an AnyReference
 * object.  Thus when the value of a Parameter object is set,
 * the result is that the value of the \a referenced object is 
 * actually set.
 */
class Parameter : public PackObject
{
public:

  /// Constructor
  Parameter() 
	: initialized(false), disabled(false), referenced(false),
	  validator(0), callback(0) {}

  /// Copy Constructor
  Parameter(const Parameter& param)
	{copy(param);}

  /// Copy operator
  Parameter& operator=(const Parameter& param)
	{ copy(param); return *this; }

  /// Destructor
  virtual ~Parameter()
	{
	}

  /// Write a parameter
  void write(std::ostream& os) const
	{os << "Parameter: " << name <<
		" Value: " << info <<
		" Initialized: " << initialized <<
		" Disabled: " << disabled <<
		" Referenced: " << referenced << std::endl;}

  /// Read a parameter
  void read(std::istream& is)
	{
	string tmp;
	is >> tmp >> name >> tmp >> info;
	is >> tmp >> initialized;
	is >> tmp >> disabled;
	is >> tmp >> referenced;
	}

  /// Pack a parameter into a PackBuffer object
  void write(utilib::PackBuffer& os) const
	{os << info << initialized << disabled << referenced;}

  /// Unpack a parameter out of a PackBuffer object
  void read(utilib::UnPackBuffer& is)
	{is >> info >> initialized >> disabled >> referenced;}

  /// Returns true if the parameter is valid
  bool valid()
	{
	if (!validator)
	   return true;
	return validator->validate(info);
	}

  /// A reference to the value of the parameter
  AnyReference info;

  /// The parameter name
  string name;

  /// A textual description of the parameter type
  string syntax;

  /// A textual description of the default value
  string default_value;

  /// A general description of the parameter
  string description;

  /**
   * True if the parameter has been initialized after creation.
   * Default value: false.
   */
  bool initialized;

  /**
   * True if the parameter has been disabled.
   * Default value: false.
   */
  bool disabled;

  /**
   * True if the value of this parameter has been accessed.
   * Default value: false.
   */
  bool referenced;

  /**
   * The set of aliases that provide alternative names for the parameter.
   * \note A hash table would be a better choice here, but this is not part of
   * the STL yet.
   */
  std::set<string> aliases;

  /**
   * Pointer to an object that contains a unary_function that is
   * used to validate a parameter value.
   */
  SmartPtr<ParameterValidatorBase> validator;

  /**
   * Pointer to an object that contains a unary_function that performs a
   * callback when the parameter is set.
   */
  std::unary_function<AnyReference,void>* callback;

protected:

  /// Copy function
  void copy(const Parameter& param)
	{
	info = param.info;
	name = param.name;
	syntax = param.syntax;
	default_value = param.default_value;
	description = param.description;
	initialized = param.initialized;
	disabled = param.disabled;
	referenced = param.referenced;
        aliases = param.aliases;
	validator = param.validator;
	callback = param.callback;
	}

};

} // namespace utilib

#endif
