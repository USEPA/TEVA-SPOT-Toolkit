/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file gRandom.h
 * \author Jonathan Eckstein
 *
 * A global uniform random number -- coding convenience.
 */

#ifndef pebbl_gRandom_h
#define pebbl_gRandom_h

#include <acro_config.h>
#include <utilib/Uniform.h>
//#include <utilib/paramTable.h>

namespace pebbl {


utilib::RNG*    gRandomRNG();
extern utilib::Uniform gRandom;
extern int randomSeed;

// The following commented-out code is the functionality we want.
// Work around for gcc compiler is below

// void gRandomReSeed(int seed=(int) randomSeed(),int processorVariation=1);

void gRandomReSeed(int seed, int processorVariation=1);

void gRandomReSeed();

} // namespace pebbl

#endif
