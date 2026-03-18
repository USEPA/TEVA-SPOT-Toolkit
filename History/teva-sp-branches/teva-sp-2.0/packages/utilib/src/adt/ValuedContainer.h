//
// ValuedContainer.h
//
// Container class that contains a value that can be extracted (e.g. to sort
//

#ifndef utilib_ValuedContainer_h
#define utilib_ValuedContainer_h

#include <utilib/utilib_config_explicit.h>
#ifndef UTILIB_HAVE_STD
#include <iostream.h>
#else
#include <iostream>
#endif

namespace utilib {

///
template <class ValueT, class InfoT>
class ValuedContainer
{
public:

  ///
  ValuedContainer() {}

  ///
  explicit ValuedContainer(ValueT& value_, InfoT& info_) : value(value_), info(info_) {}

  ///
  ValuedContainer(const ValuedContainer<ValueT,InfoT>& item)
  		{ value = item.value; info = item.info; }

  ///
  ValueT value;

  ///
  InfoT info;

  ///
  operator ValueT() const
	  {return value;}

  ///
  int compare(const ValuedContainer<ValueT,InfoT>& item) const
	{
	if (value < item.value) return -1;
	if (value > item.value) return  1;
	return 0;
	}
};

}

template <class ValueT, class InfoT>
std::ostream& operator<<(std::ostream& os, const utilib::ValuedContainer<ValueT,InfoT>& item)
{
os << item.value << " " << item.info;
return os;
}


#endif
