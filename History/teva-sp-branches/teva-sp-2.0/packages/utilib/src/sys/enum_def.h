
#ifndef utilib_enum_def_h
#define utilib_enum_def_h

#include <utilib/utilib_config.h>
#include <utilib/PackBuf.h>

#define ENUM_STREAMS(Etype)\
\
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& buff, const Etype& data)\
{buff.pack( static_cast<int>(data) ); return buff;}\
\
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buff, Etype& data)\
{int tmp; buff.unpack(tmp); data = static_cast< Etype >(tmp); return buff;}\
\
inline std::ostream& operator<< (std::ostream& ostr, const Etype data)\
{ostr << static_cast<int>(data); return ostr;}\
\
inline std::istream& operator>> (std::istream& istr, Etype& data)\
{int tmp; istr >> tmp; data = static_cast< Etype >(tmp); return istr;}

#endif
