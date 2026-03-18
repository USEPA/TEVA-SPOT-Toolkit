/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file StackList.h
 *
 * Defines the utilib::StackList class
 */

#ifndef utilib_StackList_h
#define utilib_StackList_h

#include <utilib/utilib_config.h>
#ifndef UTILIB_HAVE_STD
#include <stddef.h>
#else
#include <cstddef>
#endif

#include <utilib/LinkedList.h>

namespace utilib {


/**
 * A data type that defines a stack using a doubly-linked
 * list data structure.  This class is a simple derivation of 
 * LinkedList that sets the stack mode.
 */
template <class T>
class StackList : public LinkedList<T>
{
public:

  /// Empty constructor.
  StackList() : LinkedList<T>() {this->mode = this->stackLL;}

};

} // namespace utilib

#endif
