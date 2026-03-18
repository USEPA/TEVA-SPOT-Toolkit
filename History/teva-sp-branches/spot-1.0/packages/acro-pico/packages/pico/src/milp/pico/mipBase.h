/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file mipBase.h
 *
 * Defines the pico::mipBase class.
 */

#ifndef pico_mipBase_h
#define pico_mipBase_h

#include <acro_config.h>

namespace pico {

///
/// A base class for PICO MIP data types.  This is a way to share some declarations and
/// data objects among all the classes related to MIP: MILP, MILPNode, MILPProblem, etc.
///
class mipBase
{
 public:

  // For tracking performance.  The set of items we want to track
  // accessible only at the MIP level
enum MIPTiming
  {
    preprocessTime = 0,
    PCInit = 1,
    MIPHeur = 2,
    numMIPTimingStates = 3
  };

 const char* MIPTimingString(int timingObject)
   {
     return MIPTimingStringArray[timingObject];
   }


 mipBase() {};

 protected:

  // Printing strings for the operations for which we're tracking timing

 static const char *MIPTimingStringArray[numMIPTimingStates];

}; // class mipBase

}

#endif

