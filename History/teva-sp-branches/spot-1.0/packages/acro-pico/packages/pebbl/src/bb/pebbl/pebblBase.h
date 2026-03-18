/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file pebblBase.h
 *
 * Defines the pebbl::pebblBase class.
 */

#ifndef pebbl_pebblBase_h
#define pebbl_pebblBase_h

#include <acro_config.h>
#include <utilib/_math.h>
#include <pebbl/memUtil.h>


namespace pebbl {


///
/// A base class for PEBBL data types.  The branching and branchSub
/// classes are both derived from this class, so they effectively share these 
/// declarations.
///
class pebblBase
{
public:

  ///
  pebblBase();

  ///
  enum optimType 
    { 
      minimization = 1, 
      maximization = -1 
    };

  ///
  enum { maxLoadDegree = 3 };
  
  ///
  enum subState
    { 
      boundable      = 0, 
      beingBounded   = 1, 
      bounded        = 2,
      beingSeparated = 3,
      separated      = 4, 
      dead           = 5,
      numStates      = 6
    };

  ///
  enum { anyChild = -1 };  // Children are numbered 0...(k-1).

  ///
  static const char* stateString(int stateCode)
	{
	return stateStringArray[stateCode];
	}

  ///
  static bool isInteger(double val);

  ///
  static bool isZero(double val);

  ///
  static double integerTolerance;

protected:

  ///
  static const char* stateStringArray[numStates];

  ///
  static std::ostream* vout;

  ///
  static std::ostream* hlog;
};

} // namespace pebbl

#endif
