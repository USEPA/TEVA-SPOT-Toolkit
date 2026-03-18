/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// pyutilib.cpp
//

#include "pyutilib.h"

extern "C" void augment_pyutilib_modules()
{}


int main(int argc, char** argv)
{
return pyutilib_main(argc, argv);
}
