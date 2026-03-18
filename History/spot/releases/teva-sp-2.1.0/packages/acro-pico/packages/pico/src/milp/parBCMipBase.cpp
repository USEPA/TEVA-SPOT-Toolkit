/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

//
// parBCMipBase.cpp
//
// A base class for parallel B&C.  This contains relevant static
// members, parameters, enum declaration, etc.  The parallel MIP and
// MIP node classes will both derive this.

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI
#include <pico/parBCMipBase.h>

using namespace std;

utilib::BasicArray<pico::cutConstructor>* pico::parBCMipBase::makeNewCut=0;

#endif
