/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// loadBalPair.cpp
//
// A little class useful in pairing up processors to do load balancing.
// Had to be separated from the rest of the load balancing code so it
// could be used in a template.
//
// Jonathan Eckstein
//

#include <acro_config.h>
#include <pebbl/loadBalPair.h>
