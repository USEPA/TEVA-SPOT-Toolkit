
#ifndef utilib_PackObject_h
#define utilib_PackObject_h

#include <utilib/utilib_config.h>
#ifdef UTILIB_HAVE_STD
#include <iostream>
#else
#include <iostream.h>
#endif

#include <utilib/PackBuf.h>
#include <utilib/exception_mngr.h>

namespace utilib {

///
class PackObject
{
public:

  ///
  virtual void write(std::ostream& os) const
	{EXCEPTION_MNGR(std::runtime_error, "Undefined method.");}

  ///
  virtual void read(std::istream& is)
	{EXCEPTION_MNGR(std::runtime_error, "Undefined method.");}

  ///
  virtual void write(PackBuffer& os) const
	{EXCEPTION_MNGR(std::runtime_error, "Undefined method.");}

  ///
  virtual void read(UnPackBuffer& is)
	{EXCEPTION_MNGR(std::runtime_error, "Undefined method.");}

};

} // namespace utilib

/// Out-stream operator for writing the contents of a PackObject
inline std::ostream& operator<<(std::ostream& os, const utilib::PackObject& obj)
{ obj.write(os); return(os); }

/// Out-stream operator for writing the contents of a PackObject
inline utilib::PackBuffer& operator<<(utilib::PackBuffer& os,
					const utilib::PackObject& obj)
{ obj.write(os); return(os); }

/// In-stream operator for reading the contents of a PackObject
inline std::istream& operator>>(std::istream& is, utilib::PackObject& obj)
{ obj.read(is); return(is); }

/// In-stream operator for reading the contents of a PackObject
inline utilib::UnPackBuffer& operator>>(utilib::UnPackBuffer& is,
					utilib::PackObject& obj)
{ obj.read(is); return(is); }


#endif
