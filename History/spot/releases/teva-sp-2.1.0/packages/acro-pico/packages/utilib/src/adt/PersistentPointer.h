/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file PersistentPointer.h
 *
 * Defines the utilib::PersistentPointer class
 */

#ifndef utilib_PersistentPointer_h 
#define utilib_PersistentPointer_h

#include <utilib/utilib_config.h>

namespace utilib {

/// This class looks like a pointer, but it does not delete the memory
/// when it gets deleted!
template <typename PointerT>
class PersistentPointer
{
public:

  /// Constructor
  PersistentPointer(PointerT* ptr_) : ptr(ptr_) {}

  /// Returns a pointer to the data
  PointerT* operator ->() {return ptr;}

  /// Returns a reference to the data
  PointerT& operator*() {return *ptr;}

protected:

  /// A pointer to the data
  PointerT* ptr;

};

} // namespace utilib

#endif
