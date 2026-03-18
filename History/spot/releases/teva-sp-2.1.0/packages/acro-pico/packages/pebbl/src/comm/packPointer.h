/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file packPointer.h
 * \author Jonathan Eckstein
 * 
 * Generic code for putting pointer in and out of (Un)PackBuffers.
 */

#ifndef pebbl_packPointer_h
#define pebbl_packPointer_h

#include <acro_config.h>
#include <utilib/PackBuf.h>

namespace pebbl {


#define unpackAs(buf,what,kind,castType) \
   { \
       castType unpackTemp_; \
       buf >> unpackTemp_; \
       kind* tp = (kind*) &unpackTemp_; \
       what = *tp; \
   }


inline void packPointer(PackBuffer& outBuffer,void* ptr)
{
  if (sizeof(void*) == sizeof(int))
    outBuffer << *((int*) &ptr);
  else if (sizeof(void*) == sizeof(long))
    outBuffer << *((long*) &ptr);
  else if (sizeof(void*) == sizeof(double))
    outBuffer << *((double*) &ptr);
  else
    EXCEPTION_MNGR(std::runtime_error, "Can't figure out how to pack " << sizeof(void*) << "-byte pointers");
}


inline void* unpackPointer(UnPackBuffer& inBuffer)
{
  void* temp = 0;
  if (sizeof(void*) == sizeof(int))
    unpackAs(inBuffer,temp,void*,int)
  else if (sizeof(void*) == sizeof(long))
    unpackAs(inBuffer,temp,void*,long)
  else if (sizeof(void*) == sizeof(double))
    unpackAs(inBuffer,temp,void*,double)
  else
    EXCEPTION_MNGR(std::runtime_error, "Can't figure out how to unpack " << sizeof(void*) << "-byte pointers");
  return (void*) temp;
}
  
} // namespace pebbl

#endif
