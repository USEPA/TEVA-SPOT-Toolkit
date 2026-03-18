/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file chunkAlloc.h
 *
 * This is some simple code that can be #include'd into a class to make
 * it use the chunk allocator.
 */

#ifndef pebbl_chunkAlloc_h
#define pebbl_chunkAlloc_h

#include <acro_config.h>
#define INSERT_CHUNKALLOC \
\
public:\
  void*  operator new(size_t size)       { return memory.allocate(); };\
  void   operator delete(void* p)        { memory.putBack(p); };\
  static void memClear(bool check=false) { memory.wipe(check); };\
private:\
  static chunkAllocator memory;\

#endif
