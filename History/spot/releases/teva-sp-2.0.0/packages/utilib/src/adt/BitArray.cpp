/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// BitArray.cpp
//
// All internal arrays are indexed 0..n-1
//

#include <utilib/utilib_config.h>
#ifndef UTILIB_HAVE_STD
#ifdef UTILIB_HAVE_STRING_H
#include <string.h>
#endif
#ifdef UTILIB_HAVE_STRINGS_H
#include <strings.h>
#endif
#else
#include <cstring>
#endif

#include <utilib/BitArray.h>

#ifdef UTILIB_HAVE_NAMESPACES
using namespace std;
#endif

namespace utilib {


size_type BitArray::nbits() const
{
  size_type counter = 0;

  // Do all the full words of data

  size_type numFullWords = Len >> index_shift;
  size_type i = 0;
  for (; i<numFullWords; i++)
    count_bits(counter,Data[i]);

  // Check the last bitword, if any

  int leftover = (Len & index_mask);
  if (leftover > 0)
    {
      workingBitData mask = (1 << leftover) - 1;
      count_bits(counter,Data[i] & mask);
    }
  
  return counter;
}


void BitArray::set()
{ 
  size_type nwords = alloc_size(Len);
  for(size_type i=0; i<nwords; i++)
    Data[i] = ~0;
}


void BitArray::reset()
{
  size_type nwords = alloc_size(Len);
  for(size_type i=0; i<nwords; i++)
    Data[i] = 0;
}


void BitArray::flip()
{
  size_type nwords = alloc_size(Len);
  for(size_type i=0; i<nwords; i++)
    Data[i] = ~Data[i];
}


void bitwise_or(BitArray& a1, BitArray& a2, BitArray& result)
{
  if ((a1.size() != a2.size()) || (a1.size() != result.size()    ))
     EXCEPTION_MNGR(runtime_error, "bitwise_or : inconsistent array lengths: a1=" << a1.size() << " a2=" << a2.size() << " result=" << result.size())
  size_type nwords = a1.alloc_size(a1.size());
  for (size_type i=0; i<nwords; i++)
    result.Data[i] = a1.Data[i] | a2.Data[i];
}


void bitwise_xor(BitArray& a1, BitArray& a2, BitArray& result)
{
  if ((a1.size() != a2.size()) || (a1.size() != result.size()    ))
     EXCEPTION_MNGR(runtime_error, "bitwise_xor : inconsistent array lengths: a1=" << a1.size() << " a2=" << a2.size() << " result=" << result.size())
  size_type nwords = a1.alloc_size(a1.size());
  for (size_type i=0; i<nwords; i++)
    result.Data[i] = a1.Data[i] ^ a2.Data[i];
}


void bitwise_and(BitArray& a1, BitArray& a2, BitArray& result)
{
  if ((a1.size() != a2.size()) || (a1.size() != result.size()    ))
     EXCEPTION_MNGR(runtime_error, "bitwise_and : inconsistent array lengths: a1=" << a1.size() << " a2=" << a2.size() << " result=" << result.size())
  size_type nwords = a1.alloc_size(a1.size());
  for (size_type i=0; i<nwords; i++)
    result.Data[i] = a1.Data[i] & a2.Data[i];
}


// return true (1) if the two BitArrays both have a 1 in at least one
// bit.  Equivalent to a bitwise AND and then comparing nbits() on the result
// to 0, but much more efficient (e.g. can return as soon as find agreement)

int BitArray::shared_one(BitArray& other) const
{
size_type min_length = other.size();
if (size() < min_length)
  min_length = size();

  // Do all the full words of data

  size_type numFullWords = Len >> index_shift;
  size_type i = 0;
  for (; i<numFullWords; i++)
    if (Data[i] & other.Data[i])
      return 1;

  // Check the last word, if any

  int leftover = (Len & index_mask);
  if (leftover > 0)
    {
      workingBitData mask = (1 << leftover) - 1;
      if (Data[i] & other.Data[i] & mask)
	return 1;
    }
  
  return 0;
}

}
