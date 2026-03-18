/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file traits.h
 *
 * Defines a macro used to set object traits.
 */

#ifndef utilib_traits_h
#define utilib_traits_h

#include <utilib/utilib_config_bool.h>

/// Set a trait in a given namespace with a given type and value.
#define SetTrait(nspace,trait,type,val)\
namespace nspace { \
template <> \
struct trait < type > { \
public: \
static const bool value = val; \
}; }

#endif
