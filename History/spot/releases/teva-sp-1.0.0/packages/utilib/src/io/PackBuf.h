/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file PackBuf.h
 *
 * Defines the utilib::PackBuffer and utilib::UnPackBuffer classes, 
 * which facilitate the use of stream operators to create MPI buffers.
 *
 * \author William E. Hart
 */

/**
 * \class utilib::PackBuffer
 *
 * A class that provides a facility for packing messages, using the MPI 
 * packing facilities. The PackBuffer class dynamically resizes the
 * internal buffer to contain enough memory to pack the entire object.  When 
 * deleted, the PackBuffer object deletes this internal buffer.
 */
/**
 * \class utilib::UnPackBuffer
 *
 * A class that provides a facility for unpacking messages, using the MPI 
 * packing facilities.
 */

 
#ifndef utilib_PackBuf_h
#define utilib_PackBuf_h

#include <vector>
#include <string>
#ifdef NON_ANSI
#include <iostream.h>
#else
#include <iostream>
#endif
#include <memory.h>

#include <utilib/mpi_utilib.h>
#include <utilib/mpiUtil.h>
#include <utilib/exception_mngr.h>

#ifdef ANSI_NAMESPACES
using namespace std;
#endif

namespace utilib {

using std::string;

#ifdef NO_MEMBER_TEMPLATES
typedef unsigned char unsigned_char;
typedef unsigned short unsigned_short;
typedef unsigned int unsigned_int;
typedef unsigned long unsigned_long;
typedef long long long_long;
#endif

#if !defined(COUGAR)
using std::vector;
#endif

//---------------------------------------------------------------------
//
// PackBuffer
//
//---------------------------------------------------------------------

class PackBuffer {
 
public:
 
  /// Constructor, which allows the default buffer size to be set.
  PackBuffer(int size_=1024) {Index=0; Size=size_; buffer = new char [size_];}

  /// Desctructor.
  ~PackBuffer() {if (buffer) delete [] buffer;}
 
  /// Returns a pointer to the internal buffer that has been packed.
  const char* buf() const {return buffer;}

  /// The number of bytes of packed data.
  size_type size() const {return Index;}

  /// Returns the number of bytes that have been packed into the buffer.
  size_type curr() {return Index;}

  /// Resets the buffer index in order to reuse the internal buffer.
  void reset() {Index=0;}

#ifndef NO_MEMBER_TEMPLATES
  /// Pack a list of objects
  template <class TYPE>
  void pack(const TYPE* data, const int num);

  /// Pack a single object
  template <class TYPE>
  void pack(const TYPE& data) {pack(&data,1);}

  /// Dump the buffer to ostr, interpreted as the specified datatype
  template <class TYPE>
  void dump(ostream& ostr);
  
#else

  ///
  void pack(const bool* data, const int num);

  ///
  void pack(const bool& data) {pack(&data,1);}

  ///
  void pack(const int* data, const int num);

  ///
  void pack(const int& data) {pack(&data,1);}

  ///
  void pack(const unsigned_int* data, const int num);

  ///
  void pack(const unsigned_int& data) {pack(&data,1);}

  ///
  void pack(const long* data, const int num);

  ///
  void pack(const long& data) {pack(&data,1);}

  ///
  void pack(const unsigned_long* data, const int num);

  ///
  void pack(const unsigned_long& data) {pack(&data,1);}

  ///
  void pack(const long long* data, const int num);

  ///
  void pack(const long long& data) {pack(&data,1);}

  ///
  void pack(const short* data, const int num);

  ///
  void pack(const short& data) {pack(&data,1);}

  ///
  void pack(const unsigned_short* data, const int num);

  ///
  void pack(const unsigned_short& data) {pack(&data,1);}

  ///
  void pack(const float* data, const int num);

  ///
  void pack(const float& data) {pack(&data,1);}

  ///
  void pack(const double* data, const int num);

  ///
  void pack(const double& data) {pack(&data,1);}

  ///
  void pack(const char* data, const int num);

  ///
  void pack(const char& data) {pack(&data,1);}

  ///
  void pack(unsigned const char* data, const int num);

  ///
  void pack(unsigned const char& data) {pack(&data,1);}
#endif

  /// Pack a string
  void pack(char* data);

  /// Pack a string
  void pack(const char* data);

  /// Write the buffer to a stream opened with the ios::binary flag.
  /// Also performs a reset.
  void writeBinary(ostream& ostr);

protected:
 
  /// The internal buffer for packing
  char* buffer;

  /// The index into the current buffer
  size_type Index;

  /// The total size that has been allocated for the buffer
  size_type Size;

  /// Resizes the internal buffer
  void resize(const size_type newsize);

};
 
 
#ifndef NO_MEMBER_TEMPLATES
template <>
inline void PackBuffer::pack<bool>(const bool* data, const int num)
{
for (int i=0; i<num; i++) {
  char c;
  if (data[i])
     c = 'T';
  else
     c = 'F';
  pack(c);
  }
}


template <class TYPE>
  inline void PackBuffer::dump(ostream& ostr)
  {
    const char* endPtr = (const char*) buf() + size();
    for(TYPE* ptr = (TYPE*) buf(); (const char*) ptr < endPtr; ptr++)
      ostr << *ptr << endl;
  }

#else

inline void PackBuffer::pack(const bool* data, const int num)
{
for (int i=0; i<num; i++) {
  char c;
  if (data[i])
     c = 'T';
  else
     c = 'F';
  pack(c);
  }
}

#endif



//---------------------------------------------------------------------
//
// UnPackBuffer
//
//---------------------------------------------------------------------

class UnPackBuffer {
 
public:
 
  /// Method that does the setup for the constructors
  void setup(char* buf_, size_type size_, bool flag=false);

  /// Method that does the setup for the constructors
  void setup(const char* buf_, size_type size_)
		{setup((char*)buf_,size_,false);}

  /// Default constructor.
  UnPackBuffer()
	: buffer(0), own_flag(false), status_flag(true)
	{setup(0,0,false);}

  /// Constructor that specifies the size of the buffer
  explicit UnPackBuffer(size_type size_) 
	: buffer(0), own_flag(false), status_flag(true)
	{setup(new char [size_],size_,true);}

  /// Constructor that sets the internal buffer to the given array
  UnPackBuffer(char* buf_, size_type size_, bool flag=false) 
	: buffer(0), own_flag(false), status_flag(true)
	{setup(buf_,size_,flag);}

  /// Constructor that sets the internal buffer to the given array
  UnPackBuffer(const char* buf_, size_type size_)
	: buffer(0), own_flag(false), status_flag(true)
	{setup((char*)buf_,size_,false);}
  ///
  explicit UnPackBuffer(PackBuffer& pack)
	: buffer(0), own_flag(false), status_flag(true)
	{setup(pack.buf(),pack.size());}

  /// Destructor.
  ~UnPackBuffer()
	{if (buffer && own_flag) delete [] buffer;}

  /// Resizes the internal buffer
  void resize(const size_type newsize);

  /// Returns a pointer to the internal buffer
  const char* buf() {return buffer;}

  /// Returns the length of the buffer.
  size_type size() {return Size;}

  /// Returns the current message size.
  size_type message_length() { return MessageLength; };

  /// Returns the number of bytes that have been 
  /// unpacked from the buffer.
  size_type curr() {return Index;}

  /// Resets the internal buffer and sets a new message length.
  void reset(size_type message_size);

  /// If there is no argument to reset(), it assumes that the whole
  /// buffer is in use.
  void reset() { reset(Size); };

#ifdef USING_MPI
  /// Reset the internal buffer from the information in an 
  /// MPI message status
  void reset(MPI_Status* statusP)
	{ reset(uMPI::getCount(statusP,MPI_PACKED)); };
#endif

  ///
  operator bool ()
	{return status_flag && (Size > 0) && (Index <= MessageLength);}

  /// Copy buffer out of pack buff into unpack buff.
  UnPackBuffer& operator=(const PackBuffer& pbuff);

#ifndef NO_MEMBER_TEMPLATES
  /// Unpack one or more objects
  template <class TYPE>
  void unpack(TYPE* data, const int num);

  /// Unpack a \b int
  template <class TYPE>
  void unpack(TYPE& data) {unpack(&data,1);}

  /// Dump the buffer to ostr, interpreted as the specified datatype
  template <class TYPE>
  void dump(ostream& ostr);

#else

  ///
  void unpack(bool* data, const int num);

  ///
  void unpack(bool& data) {unpack(&data,1);}

  ///
  void unpack(int* data, const int num);

  ///
  void unpack(int& data) {unpack(&data,1);}

  ///
  void unpack(unsigned_int* data, const int num);

  ///
  void unpack(unsigned_int& data) {unpack(&data,1);}

  ///
  void unpack(long* data, const int num);

  ///
  void unpack(long& data) {unpack(&data,1);}

  ///
  void unpack(long long* data, const int num);

  ///
  void unpack(long long& data) {unpack(&data,1);}

  ///
  void unpack(unsigned_long* data, const int num);

  ///
  void unpack(unsigned_long& data) {unpack(&data,1);}

  ///
  void unpack(short* data, const int num);

  ///
  void unpack(short& data) {unpack(&data,1);}

  ///
  void unpack(unsigned_short* data, const int num);

  ///
  void unpack(unsigned_short& data) {unpack(&data,1);}

  ///
  void unpack(float* data, const int num);

  ///
  void unpack(float& data) {unpack(&data,1);}

  ///
  void unpack(double* data, const int num);

  ///
  void unpack(double& data) {unpack(&data,1);}

  ///
  void unpack(char* data, const int num);

  /// Unpack a \b char
  void unpack(char& data) {unpack(&data,1);}

  ///
  void unpack(unsigned char* data, const int num);

  /// Unpack a \b char
  void unpack(unsigned char& data) {unpack(&data,1);}
#endif

  /// Unpack a \b char*
  void unpack(char* data);

  /// Read from an ios::binary stream into an UnPackBuffer.
  /// Automatically resizes and resets the buffer.
  void readBinary(istream& istr);

protected:
 
  /// The internal buffer for unpacking
  char* buffer;

  /// The index into the current buffer
  size_type Index;

  /// The total size that has been allocated for the buffer
  size_type Size;

  /// The actual size of the message that was just received
  size_type MessageLength;

  /// If \c true, then this class owns the internal buffer
  bool own_flag;

  /// If true, then the last operation was successful.
  bool status_flag;

};
 
 
#ifndef NO_MEMBER_TEMPLATES
///
template <>
inline void UnPackBuffer::unpack<bool>(bool* data, const int num)
{
if (num == 0) {
   status_flag = true;
   return;
   }
for (int i=0; i<num; i++) {
  char c;
  unpack(c);
  if (c == 'T')
     data[i] = true;
  else
     data[i] = false;
  }
}


template <class TYPE>
  inline void UnPackBuffer::dump(ostream& ostr)
  {
    const char* endPtr = buf() + message_length();
    for(TYPE* ptr = (TYPE*) buf(); (const char*) ptr < endPtr; ptr++)
      ostr << *ptr << endl;
  }

#else

///
inline void UnPackBuffer::unpack(bool* data, const int num)
{
if (num == 0) {
   status_flag = true;
   return;
   }
for (int i=0; i<num; i++) {
  char c;
  unpack(c);
  if (c == 'T')
     data[i] = true;
  else
     data[i] = false;
  }
}

#endif


#ifndef NO_MEMBER_TEMPLATES
#ifdef USING_MPI

///
template <class TYPE>
inline void PackBuffer::pack(const TYPE* data, const int num)
{
resize(PackSize(data[0],num));
int index = static_cast<int>(Index);
#ifndef CPLANT
MPI_Pack((void*)data, num, mpi_datatype<TYPE>(),
			buffer, Size, &index, MPI_COMM_WORLD);
#else
if (data)
   MPI_Pack((void*)data, num, mpi_datatype((TYPE*)data),
			buffer, Size, &index, MPI_COMM_WORLD);
#endif
Index = index;
}

#else

///
template <class TYPE>
#if !defined(SGI)
inline
#endif
void PackBuffer::pack(const TYPE* data, const int num)
{ 
size_t curr = sizeof(TYPE);
resize(curr*num);

memcpy(&buffer[Index], data, curr*num);
Index += curr;
}

#endif


#ifdef USING_MPI


///
template <class TYPE>
inline void UnPackBuffer::unpack(TYPE* data, const int num)
{
if (num == 0) {
   status_flag = true;
   return;
   }
if (Index < MessageLength) {
   data[0] = 0;		// A simple test to make sure this isn't const data
   int index=static_cast<int>(Index);
   int tmp = MPI_Unpack(buffer, Size, &index, (void*)data, num, 
#ifdef CPLANT
				mpi_datatype(data), MPI_COMM_WORLD);
#else
				mpi_datatype<TYPE>(), MPI_COMM_WORLD);
#endif
   Index = index;
   status_flag = (tmp == MPI_SUCCESS) && (Index <= MessageLength);
   if (Index > MessageLength)
      EXCEPTION_MNGR(runtime_error, "UnPackBuffer::unpack - Unpack operation started within the message but ended beyond it")
   }
else
  {
    status_flag = false;
  }
}


#else


///
template <class TYPE>
#if !defined(SGI)
inline
#endif
void UnPackBuffer::unpack(TYPE* data, const int num)
{ 
  if (num == 0) 
    {
      status_flag = true;
      return;
    }
  if (Index < MessageLength) 
    {
      data[0] = 0;
      size_t curr = num*sizeof(TYPE);
      memcpy((void*)data, &(buffer[Index]), curr);
      Index += curr;
      status_flag = (Index <= MessageLength);
      if (Index > MessageLength)
         EXCEPTION_MNGR(runtime_error, "UnPackBuffer::unpack - Unpack operation started within message length but ended beyond it")
    }
  else
    {
      status_flag = false;
    }
  
}

#endif

#else
// NO_MEMBER_TEMPLATES

#ifdef USING_MPI

#define PACKBUF(TYPE)\
inline void PackBuffer::pack(const TYPE* data, const int num)\
{\
resize(PackSize(data[0],num));\
MPI_Pack((void*)data, num, mpi_datatype<TYPE>(),\
			buffer, Size, &Index, MPI_COMM_WORLD);\
}

#else

#define PACKBUF(TYPE)\
inline void PackBuffer::pack(const TYPE* data, const int num)\
{ \
size_t curr = sizeof(TYPE);\
resize(curr*num);\
\
memcpy(&buffer[Index], data, curr*num);\
Index += curr;\
}

#endif

PACKBUF(int)
PACKBUF(unsigned_int)
PACKBUF(long)
PACKBUF(long long)
PACKBUF(unsigned_long)
PACKBUF(short)
PACKBUF(unsigned_short)
PACKBUF(char)
PACKBUF(unsigned_char)
PACKBUF(double)
PACKBUF(float)


#ifdef USING_MPI

#define UNPACKBUF(TYPE)\
inline void UnPackBuffer::unpack(TYPE* data, const int num)\
{\
if (num == 0) {\
   status_flag = true;\
   return;\
   }\
if (Index < MessageLength) {\
   data[0] = 0;\
   int tmp = MPI_Unpack(buffer, Size, &Index, (void*)data, num, \
				mpi_datatype<TYPE>(), MPI_COMM_WORLD);\
   status_flag = (tmp == MPI_SUCCESS) && (Index <= MessageLength);\\
   if (Index > MessageLength)\
      EXCEPTION_MNGR(runtime_error, "UnPackBuffer::unpack - Unpack operation started within message length but ended beyond it")\
   }\
else\
  {\
    status_flag = false;\
  }\
}

#else

#define UNPACKBUF(TYPE)\
inline void UnPackBuffer::unpack(TYPE* data, const int num)\
{ \
  if (num == 0) \
    {\
      status_flag = true;\
      return;\
    }\
  if (Index < MessageLength) \
    {\
      data[0] = 0;\
      size_t curr = num*sizeof(TYPE);\
      memcpy((void*)data, &(buffer[Index]), curr);\
      Index += curr;\
      status_flag = (Index <= MessageLength);\
      if (Index > MessageLength)\
         EXCEPTION_MNGR(runtime_error, "UnPackBuffer::unpack - Unpack operation started within message length but ended beyond it")\
    }\
  else\
    {\
      status_flag = false;\
    }\
}
#endif

UNPACKBUF(int)
UNPACKBUF(unsigned_int)
UNPACKBUF(long)
UNPACKBUF(unsigned_long)
UNPACKBUF(short)
UNPACKBUF(unsigned_short)
UNPACKBUF(char)
UNPACKBUF(unsigned_char)
UNPACKBUF(double)
UNPACKBUF(float)
UNPACKBUF(long long)

#endif 


} // namespace utilib

///
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& buff, const void* data)
	{return buff;}
///
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& buff, const bool& data)
	{buff.pack(data); return buff;}
///
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& buff, const double& data)
	{buff.pack(data); return buff;}
///
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& buff, const float& data)
	{buff.pack(data); return buff;}
///
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& buff, const long long int& data)
	{buff.pack(data); return buff;}
///
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& buff, const long int& data)
	{buff.pack(data); return buff;}
///
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& buff, const int& data)
	{buff.pack(data); return buff;}
///
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& buff, const short int& data)
	{buff.pack(data); return buff;}
///
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& buff, const char& data)
	{buff.pack(data); return buff;}
///
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& buff, const unsigned long int& data)
	{buff.pack(data); return buff;}
///
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& buff, const unsigned int& data)
	{buff.pack(data); return buff;}
///
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& buff, const unsigned short int& data)
	{buff.pack(data); return buff;}
///
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& buff, const unsigned char& data)
	{buff.pack(data); return buff;}
///
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& buff, const char* data)
	{buff.pack(data); return buff;}

///
utilib::PackBuffer& operator<< (utilib::PackBuffer& buff, istream& istr);

///
inline utilib::PackBuffer& operator<< (utilib::PackBuffer& buff, const string& str)
{
size_type len = str.size();
buff << len;
for (size_type i=0; i<len; i++)
  buff << str[i];
return buff;
}


///
template <class TYPE>
utilib::PackBuffer& operator<<(utilib::PackBuffer& os, const vector<TYPE>& vec)
{
os << vec.size();
if (vec.size() > 0) {
   typename vector<TYPE>::const_iterator curr = vec.begin();
   typename vector<TYPE>::const_iterator last = vec.end();
   while (curr != last) {
     os << *curr;
     ++curr;
     }
   }
return os;
}


///
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buff, void* data)
	{return buff;}
///
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buff, bool& data)
	{buff.unpack(data); return buff;}
///
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buff, double& data)
	{buff.unpack(data); return buff;}
///
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buff, float& data)
	{buff.unpack(data); return buff;}
///
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buff, long long int& data)
	{buff.unpack(data); return buff;}
///
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buff, long int& data)
	{buff.unpack(data); return buff;}
///
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buff, int& data)
	{buff.unpack(data); return buff;}
///
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buff, short int& data)
	{buff.unpack(data); return buff;}
///
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buff, char& data)
	{buff.unpack(data); return buff;}
///
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buff, unsigned long int& data)
	{buff.unpack(data); return buff;}
///
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buff, unsigned int& data)
	{buff.unpack(data); return buff;}
///
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buff, unsigned short int& data)
	{buff.unpack(data); return buff;}
///
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buff, unsigned char& data)
	{buff.unpack(data); return buff;}
///
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buff, char* data)
	{buff.unpack(data); return buff;}

///
utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buff, ostream& str);

///
inline utilib::UnPackBuffer& operator>> (utilib::UnPackBuffer& buff, string& str)
{
size_type len;
buff >> len;
str.resize(len);
for (size_type i=0; i<len; i++)
  buff >> str[i];
return buff;
}

///
template <class TYPE>
utilib::UnPackBuffer& operator>>(utilib::UnPackBuffer& is, vector<TYPE>& vec)
{
size_type tmp;
is >> tmp;
EXCEPTION_TEST( !is, runtime_error, "operator>> - UnPackBuffer problem.")
vec.resize(tmp);
if (vec.size() > 0) {
   typename vector<TYPE>::iterator curr = vec.begin();
   typename vector<TYPE>::iterator last = vec.end();
   while (curr != last) {
     EXCEPTION_TEST( !is, runtime_error, "operator>> - UnPackBuffer problem.")
     is >> *curr;
     ++curr;
     }
   }
return is;
}



#endif // __PackBuf_h
