/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file AnyRNG.h
 *
 * Defines the utilib::AnyRNG class.
 */

#ifndef utilib_AnyRNG_h
#define utilib_AnyRNG_h

#ifdef NON_ANSI
#include <iostream.h>
#else
#include <iostream>
#endif
#include <algorithm>

#ifdef ANSI_NAMESPACES
using namespace std;
#endif

namespace utilib {

#if !defined(COUGAR) && !defined(ANSI_NAMESPACES)
using std::ostream;
using std::istream;
#endif

/**
 * Templated container that contains a pointer to any RNG-compatible random
 * number generator, which does <i>not</i> need to be a subclass of RNG.
 */
class AnyRNG
{
public:

  /// Constructor
  AnyRNG()
	: content(0)
	{}

  /**
   * Templated constructor that takes a pointer to a RNG-compatible 
   * random number generator
   */
  template <typename RNGT>
  AnyRNG(RNGT* rng)
	: content(rng ? new holder<RNGT>(rng) : 0)
	{}

  /// Constructor that takes a pointer to a AnyRNG object
  explicit AnyRNG(AnyRNG* rng)
	: content(rng && rng->content ? rng->content->clone() : 0)
	{}

  /// Copy constructor
  AnyRNG(const AnyRNG& other)
	: content(other.content ? other.content->clone() : 0)
	{}

  /// Destructor
  ~AnyRNG()
	{ if (content) delete content; }

  /// Equal operator
  template<typename RNGT>
  AnyRNG& operator=(RNGT* rhs)
	{
	if (content) delete content;
	content = (rhs ? new holder<RNGT>(rhs) : 0);
	return *this;
	}

  /// Equal operator
  AnyRNG& operator=(const AnyRNG& rhs)
	{
	if (content) delete content;
	content = (rhs.content ? rhs.content->clone() : 0);
	return *this;
	}

  /// Equals operator
  template<typename RNGT>
  bool operator==(RNGT* rhs) const
	{
	AnyRNG rng(rhs);
	return operator==(rng);
	}

  /// Equals operator
  bool operator==(const AnyRNG& rhs) const
	{
	if (content) {
	   if (rhs.content)
	      return (content->voidptr() == rhs.content->voidptr());
	   else
	      return false;
           }
	return !(rhs.content);
	}

  /// Not-equals operator
  template<typename RNGT>
  bool operator!=(RNGT* rhs) const
	{
	AnyRNG rng(rhs);
	return operator!=(rng);
	}

  /// Not-equals operator
  bool operator!=(const AnyRNG& rhs) const
	{
	if (content) {
	   if (rhs.content)
	      return (content->voidptr() != rhs.content->voidptr());
	   else
	      return true;
           }
	return (rhs.content);
	}

  /// Returns true if the object is not pointing to an object
  bool empty() const
	{
	return !content;
	}

  /// Returns true if the object is pointing to an object
  operator bool () const
	{
	return content;
	}

  /// Return float in [0,1[
  float asFloat()
	{
	return content ? content->asFloat() : 0.0;
	}

  /// return double in [0,1[
  double asDouble()
	{
	return content ? content->asDouble() : 0.0;
        }

  /// Return long in 0...LONG_MAX
  unsigned long asLong()
	{
	return content ? content->asLong() : 0;
	}

  /// Reset the state of the random number generator
  void reset()
	{
	if (content) content->reset();
	}

  /// Write the random number generator's state to a stream
  void write(ostream& output) const
	{
	if (content) content->write(output);
	}

  /// Read the random number generator's state from a stream
  void read(istream& input)
	{
	if (content) content->read(input);
	}

#ifndef NO_MEMBER_TEMPLATE_FRIENDS
    private: // types
#else
    public: // types (public so any_cast can be non-friend)
#endif

	/// Private class that defines the base class of the
	/// objects to which an AnyRNG points
        class placeholder
        {
        public: // structors

            virtual ~placeholder() { }

        public: // queries

            virtual placeholder * clone() const = 0;

            virtual float asFloat() = 0;

            virtual double asDouble() = 0;

            virtual long asLong() = 0;

            virtual void reset() = 0;

            virtual void write(ostream& ) const = 0;

            virtual void read(istream& ) = 0;

	    virtual const void* voidptr() const = 0;

        };

	/// Private templated class to which an AnyRNG points
        template<typename ValueType>
        class holder : public placeholder
        {
        public: // structors

            holder(ValueType* value)
              : held(value) { }

        public: // queries

            placeholder * clone() const
              { return new holder(held); }

            float asFloat()
              { return held->asFloat(); }

            double asDouble()
              { return held->asDouble(); }

            long asLong()
              { return held->asLong(); }

            void reset()
              { held->reset(); }

            void write(ostream& ostr) const
              { ostr << *held; }

            void read(istream& istr)
              { istr >> *held; }

	    const void* voidptr() const
	      { return held; }

        public: // representation

            ValueType* held;

        };

protected: // representation

  /// The random number generator to which the object points
  placeholder * content;

};
   
} // namespace utilib


/// Out-stream operator for AnyRNG objects
inline ostream& operator << (ostream& output, const utilib::AnyRNG& rng)
{
rng.write(output);
return output;
}


/// In-stream operator for AnyRNG objects
inline istream& operator >> (istream& input, utilib::AnyRNG& rng)
{
rng.read(input);
return input;
}

#endif
