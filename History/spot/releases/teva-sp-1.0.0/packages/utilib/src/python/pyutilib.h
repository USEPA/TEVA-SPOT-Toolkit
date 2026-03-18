/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// pyutilib.h
//
// This header file defines the basic hooks needed to add modules
// that augment the module defined by pyutilib
//

#ifndef __pyutilib_h
#define __pyutilib_h

#ifdef __cplusplus
extern "C" {
#endif

void swig_add_module(char *name, void (*initfunc)());
void augment_pyutilib_modules();
int python_main(int argc, char **argv);

#ifdef __cplusplus
};
#endif
#endif
