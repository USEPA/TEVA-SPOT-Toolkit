/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file Any.h
 *
 * Defines the utilib::AnyValue and utilib::AnyReference classes,
 * along with the utilib::bad_any_cast exception that these classes
 * might throw.
 */

#ifndef utilib_Any_h
#define utilib_Any_h

#include <algorithm>
#include <typeinfo>
#ifdef NON_ANSI
#include <iostream.h>
#else
#include <iostream>
#endif

#include <utilib/exception_mngr.h>
#include <utilib/PackBuf.h>

#ifdef ANSI_NAMESPACES
using namespace std;
#endif

namespace utilib {

#if !defined(COUGAR)
using std::ostream;
using std::istream;
//using std::typeid;
#endif

class AnyValue;
class AnyReference;

/// Cast an AnyValue pointer into a pointer of its value
template<typename ValueType>
ValueType * any_cast(AnyValue * operand);

/// Cat an AnyValue const pointer into a const pointer of its value
template<typename ValueType>
const ValueType * any_cast(const AnyValue * operand);

/// Cast an AnyValue reference into its value
template<typename ValueType>
ValueType any_cast(const AnyValue & operand);

/// Cast an AnyValue reference into its value
template<typename ValueType>
ValueType& any_cast(AnyValue & operand);

/// Cast an AnyReference pointer into a pointer of its value
template<typename ValueType>
ValueType * any_cast(AnyReference * operand);

/// Cat an AnyReference const pointer into a const pointer of its value
template<typename ValueType>
const ValueType * any_cast(const AnyReference * operand);

/// Cast an AnyReference reference into its value
template<typename ValueType>
ValueType any_cast(const AnyReference & operand);

/// Cast an AnyReference reference into a boolean
template<>
bool any_cast<bool>(const AnyReference & operand);

/// Cast an AnyReference reference into its value
template<typename ValueType>
ValueType& any_cast(AnyReference & operand);



/**
 * This class stores any object by value.
 * This class was adapted from the boost::any class, for UTILIB by Bill Hart.
 */ 
class AnyValue
{
public:

  /// Empty constructor
  AnyValue()
	: content(0)
	{}

  /// Templated constructor that takes a value reference
  template<typename ValueType>
  AnyValue(const ValueType & value)
	: content(new holder<ValueType>(value))
        { }

  /// Copy constructor
  AnyValue(const AnyValue & other)
	: content(other.content ? other.content->clone() : 0)
        { }

  /// Destructor
  ~AnyValue()
	{ delete content; }


  /// Swaps the values between the current object and the \a rhs
  AnyValue & swap(AnyValue & rhs)
        {
#if defined(COUGAR)
   	::swap(content, rhs.content);
#else
   	std::swap(content, rhs.content);
#endif
        return *this;
        }

  /// Sets the value of the AnyValue object equal to the value of \a rhs
  template<typename ValueType>
  AnyValue & operator=(const ValueType & rhs)
        {
        AnyValue(rhs).swap(*this);
        return *this;
        }

  /// Sets the value of the AnyValue object equal to the value of \a rhs
  AnyValue & operator=(const AnyValue & rhs)
        {
        AnyValue(rhs).swap(*this);
        return *this;
        }

  /// Returns true if the object value has not been set
  bool empty() const
        {
        return !content;
        }

  /// Returns the type informatio of the object
  const std::type_info & type() const
        {
        return content ? content->type() : typeid(void);
        }

  /// Write the object 
  void write(ostream& ostr) const
	{
	if (content) content->write(ostr);
	}

#ifndef NO_MEMBER_TEMPLATE_FRIENDS
    private: // types
#else
    public: // types (public so any_cast can be non-friend)
#endif

	///
        class placeholder
        {
        public: // structors
    
	    ///
            virtual ~placeholder()
            {
            }

        public: // queries

	    ///
            virtual const std::type_info & type() const = 0;

	    ///
            virtual placeholder * clone() const = 0;
    
	    ///
	    virtual void write(ostream& ) const = 0;

        };

	///
        template<typename ValueType>
        class holder : public placeholder
        {
        public: // structors

	    ///
            holder(const ValueType & value)
              : held(value)
            {
            }

        public: // queries

	    ///
            virtual const std::type_info & type() const
            {
                return typeid(ValueType);
            }

	    ///
            virtual placeholder * clone() const
            {
                return new holder(held);
            }

	    ///
	    virtual void write(ostream& ostr) const
            {
		ostr << held;
            }

        public: // representation

	    ///
            ValueType held;

        };

#ifndef NO_MEMBER_TEMPLATE_FRIENDS

private: // representation

  ///
  template<typename ValueType>
  friend ValueType * any_cast(AnyValue *);

#else

public: // representation (public so any_cast can be non-friend)

#endif

  /// Pointer to the content of the object
  placeholder * content;

};



/**
 * This class stores any object by reference.
 *
 * The use of this class tacitly assumes that the object to 
 * which this class maintains a reference has not been deleted.
 * This class does not delete this reference when it is deleted.
 *
 * This class was inspired by the boost::any class.
 */ 
class AnyReference
{
public:

  /// Constructor
  AnyReference()
	: content(0)
	{ }

  /// Templated constructor that takes a value reference
  template<typename RefType>
  AnyReference(RefType*  value)
	: content(new holder<RefType>(value))
        { }

  /// Copy constructor
  AnyReference(const AnyReference & other)
	: content(other.content ? other.content->clone() : 0)
        { }

  /// Destructor
  ~AnyReference()
	{ delete content; }

  /// Swaps the object references between the current object and the \a rhs
  AnyReference & swap(AnyReference & rhs)
        {
#if defined(COUGAR)
	::swap(content, rhs.content);
#else
	std::swap(content, rhs.content);
#endif
        return *this;
        }

  /// Set the value of the referenced object
  template<typename ValueType>
  AnyReference & operator=(const ValueType & rhs)
        {
	if (!content)
	   EXCEPTION_MNGR(runtime_error,"AnyReference::operator= - no reference defined!")

	AnyValue val(rhs);
	content->set_value(val);
        return *this;
        }

  /// Set the value of the referenced object when dealing with strings
  AnyReference & operator=(const char* rhs)
        {
	if (!content)
	   EXCEPTION_MNGR(runtime_error,"AnyReference::operator= - no reference defined!")

        string tmp = rhs;
	AnyValue val(tmp);
	content->set_value(val);
        return *this;
        }

  /// Set the value of the referenced object
  AnyReference & operator=(const AnyReference & rhs)
        {
        AnyReference(rhs).swap(*this);
        return *this;
        }

  /// Set the reference for a templated type
  template<typename RefType>
  AnyReference & set_ref(RefType & rhs)
	{
        AnyReference(&rhs).swap(*this);
        return *this;
	}

  /// Returns true if the object does not contain a reference pointer
  bool empty() const
        {
        return !content;
        }

  /// Returns the type of the referenced object
  const std::type_info & type() const
        {
        return content ? content->type() : typeid(void);
        }

  /// Write the referenced object
  void write(ostream& ostr) const
	{
	if (content) content->write(ostr);
	}

  /// Read the referenced object
  void read(istream& istr)
	{
	if (content) content->read(istr);
	}

  /// Write the referenced object to a PackBuffer
  void write(PackBuffer& ostr) const
	{
	if (content) content->write(ostr);
	}

  /// Read the referenced object from an UnPackBuffer
  void read(UnPackBuffer& istr)
	{
	if (content) content->read(istr);
	}

#if !defined(NO_MEMBER_TEMPLATE_FRIENDS) && !defined(SOLARIS)
    private: // types
#else
    public: // types (public so any_cast can be non-friend)
#endif

	///
        class placeholder
        {
        public: // structors
    
	    ///
            virtual ~placeholder()
            {
            }

        public: // queries

	    ///
            virtual const std::type_info & type() const = 0;

	    ///
            virtual placeholder * clone() const = 0;
    
	    ///
	    virtual void write(ostream& ) const = 0;

	    ///
	    virtual void read(istream& ) = 0;

	    ///
	    virtual void write(PackBuffer& ) const = 0;

	    ///
	    virtual void read(UnPackBuffer& ) = 0;

	    ///
	    virtual void set_value(const AnyValue& ) = 0;

	    ///
	    virtual void set_value(const AnyReference& ) = 0;

        };

	///
        template<typename ValueType>
        class holder : public placeholder
        {
        public: // structors

	    ///
            holder(ValueType * value)
              : held(value)
            {
            }

        public: // queries

	    ///
            virtual const std::type_info & type() const
            {
                return typeid(ValueType);
            }

	    ///
            virtual placeholder * clone() const
            {
                return new holder(held);
            }

	    ///
	    virtual void write(ostream& ostr) const
            {
		ostr << (*held);
            }

	    ///
	    virtual void read(istream& istr)
            {
		istr >> (*held);
            }

	    ///
	    virtual void write(PackBuffer& ostr) const
            {
		ostr << (*held);
            }

	    ///
	    virtual void read(UnPackBuffer& istr) {
		istr >> (*held);
	    }

	    ///
	    void set_value(const AnyValue& value)
	    {
	    *held = any_cast<ValueType>(value);
	    }

	    ///
	    void set_value(const AnyReference& value)
	    {
	    *held = any_cast<ValueType>(value);
	    }


        public: // representation

	    ///
            ValueType* held;

        };

#ifndef NO_MEMBER_TEMPLATE_FRIENDS

private: // representation

  ///
  template<typename ValueType>
  friend ValueType * any_cast(AnyReference *);

#else

public: // representation (public so any_cast can be non-friend)

#endif

  /// Pointer to the content of the object
  placeholder * content;

};

template<>
inline void AnyReference::holder<bool>::write(std::ostream& ostr) const
{
	if (*held)
		ostr << "true";
	else
		ostr << "false";
}

template<>
inline void AnyReference::holder<bool>::read(std::istream& istr)
		{
		string str;
		istr >> str;
		if ((str == "0") || (str == "false") || (str=="False") ||
				(str=="FALSE")||(str=="f")||(str=="F"))
		   *held=false;
		else if ((str == "1")||(str == "true")||(str=="True")||(str=="TRUE")||(str=="t")||(str=="T"))
		   *held=true;
		else
   		   EXCEPTION_MNGR(runtime_error,"AnyReference::holder<bool>::read - nonboolean value for read method: " << str)
		}


/// An exception class used by AnyValue and AnyReference classes
class bad_any_cast : public std::bad_cast
{
public:

  /// Constructor
  explicit bad_any_cast(const char* msg_) : msg(msg_) {}

  /// Return the exception description
  virtual const char * what() const throw()
	{ return msg; }

protected:

  /// The exception description
  const char* msg;

};


template<typename ValueType>
inline ValueType * any_cast(AnyValue * operand)
{
return (operand && (operand->type() == typeid(ValueType)))
         ? &(static_cast<AnyValue::holder<ValueType> *>(operand->content)->held)
         : static_cast<ValueType*>(0);
}


template<typename ValueType>
inline const ValueType * any_cast(const AnyValue * operand)
{
return any_cast<ValueType>(const_cast<AnyValue *>(operand));
}


template<typename ValueType>
inline ValueType any_cast(const AnyValue & operand)
{
const ValueType * result = any_cast<ValueType>(&operand);
if (!result)
   EXCEPTION_MNGR(utilib::bad_any_cast,"AnyValue::any_cast - failed conversion using boost::any_cast")

return *result;
}


template<typename ValueType>
inline ValueType& any_cast(AnyValue & operand)
{
ValueType * result = any_cast<ValueType>(&operand);
if (!result)
   EXCEPTION_MNGR(utilib::bad_any_cast,"AnyValue::any_cast - failed conversion using boost::any_cast")

return *result;
}



template<typename ValueType>
inline ValueType * any_cast(AnyReference * operand)
{
return (operand && (operand->type() == typeid(ValueType)))
       ? static_cast<AnyReference::holder<ValueType> *>(operand->content)->held
       : 0;
}


template<typename ValueType>
inline const ValueType * any_cast(const AnyReference * operand)
{
return any_cast<ValueType>(const_cast<AnyReference *>(operand));
}


template<typename ValueType>
inline ValueType any_cast(const AnyReference & operand)
{
const ValueType * result = any_cast<ValueType>(&operand);
if (!result)
   EXCEPTION_MNGR(utilib::bad_any_cast,"AnyReference::any_cast - failed conversion using boost::any_cast")

return *result;
}


template<>
inline bool any_cast<bool>(const AnyReference & operand)
{
const bool * result = any_cast<bool>(&operand);
if (!result)
   EXCEPTION_MNGR(utilib::bad_any_cast,"AnyReference::any_cast - failed conversion using boost::any_cast")

return *result;
}


template<typename ValueType>
inline ValueType& any_cast(AnyReference & operand)
{
ValueType * result = any_cast<ValueType>(&operand);
if (!result)
   EXCEPTION_MNGR(utilib::bad_any_cast,"AnyReference::any_cast - failed conversion using boost::any_cast")

return *result;
}



} // namespace utilib

/// Out-stream operator for AnyValue objects
inline ostream& operator<< (ostream& ostr, const utilib::AnyValue& item)
{ item.write(ostr); return ostr; }

/// Out-stream operator for AnyReference objects
inline ostream& operator<< (ostream& ostr, const utilib::AnyReference& item)
{ item.write(ostr); return ostr; }

/// In-stream operator for AnyValue objects
inline istream& operator>> (istream& istr, utilib::AnyReference& item)
{ item.read(istr); return istr; }

/// Out-stream PackBuffer operator for AnyReference objects
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& ostr, const utilib::AnyReference& item)
{ item.write(ostr); return ostr; }

/// In-stream UnPackBuffer operator for AnyReference objects
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& istr, utilib::AnyReference& item)
{ item.read(istr); return istr; }


// Copyright Kevlin Henney, 2000, 2001, 2002. All rights reserved.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose is hereby granted without fee, provided that this copyright and
// permissions notice appear in all copies and derivatives.
//
// This software is provided "as is" without express or implied warranty.

#endif
