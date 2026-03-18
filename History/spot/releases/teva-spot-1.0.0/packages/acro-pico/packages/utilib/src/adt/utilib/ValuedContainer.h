/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file ValuedContainer.h
 *
 * Defines the utilib::ValuedContainer class. 
 */

#ifndef utilib_ValuedContainer_h
#define utilib_ValuedContainer_h

#include <utilib/utilib_config.h>
#include <utilib/std_headers.h>

namespace utilib {

/// A container class that contains a value that can be extracted.  For example,
/// this is useful to sort objects that have a value with auxillary 
/// information.
template <class ValueT, class InfoT>
class ValuedContainer
{
public:

  /// Empty constructor
  ValuedContainer() {}

  /// Constructor
  explicit ValuedContainer(ValueT& value_, InfoT& info_) : value(value_), info(info_) {}

  /// Copy constructor
  ValuedContainer(const ValuedContainer<ValueT,InfoT>& item)
  		{ value = item.value; info = item.info; }

  /// The value of this object.
  ValueT value;

  /// A data structure for auxillary data associated with this value.
  InfoT info;

  /// Returns the value of this object.
  operator ValueT() const
	  {return value;}

  /// Compares two ValuedContainer objects.
  int compare(const ValuedContainer<ValueT,InfoT>& item) const
	{
	if (value < item.value) return -1;
	if (value > item.value) return  1;
	return 0;
	}
};

}

/// A stream operator for writing a ValuedContainer object
template <class ValueT, class InfoT>
std::ostream& operator<<(std::ostream& os, const utilib::ValuedContainer<ValueT,InfoT>& item)
{
os << item.value << " " << item.info;
return os;
}


#endif
