/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file parBCMipBase.h
 *
 * Defines the pico::parBCMipBase class.
 */

#ifndef pico_parBCMipBase_H
#define pico_parBCMipBase_H

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI
#include <pico/BCMipBase.h>
#include <pico/picoRowCut.h>

namespace pico {


class BCMipProblem;
typedef picoRowCut* (*cutConstructor)(BCMipProblem *);


/// A base class for parallel B&C.  This contains relevant static
/// members, parameters, enum declaration, etc.  The parallel MIP and
/// MIP node classes both derive from this.
class parBCMipBase: virtual public BCMipBase
{

 protected:

// During preprocessing a pointer to the picoRowCut makeNew method is
// placed into this array for all the built-in cut types (e.g. all the
// ones created by a CGL class).  Once we have custom cut types, each
// such cut class linked into the library will place a pointer to a
// constructor-like (static) method into this array at static
// initialization.  It will first have to get a cut type (int) from a
// static cut number "server".

 static BasicArray<cutConstructor>* makeNewCut;

};

} // namespace pico

#endif

#endif
