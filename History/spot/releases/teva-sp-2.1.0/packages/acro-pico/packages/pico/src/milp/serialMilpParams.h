/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file serialMilpParams.h
 *
 * Defines the pico::serialMilpParams class.
 */

#ifndef pico_serialMilpParams_h
#define pico_serialMilpParams_h

#include <acro_config.h>
#include <utilib/ParameterSet.h>

// These are just some of the milp-based params.  In particular these are the ones
// that multiple classes such as milp and milpNode need.

namespace pico {

/// Defines parameters, etc, used for general mixed-integer linear programming. These
/// are parameters shared by the milp classes and the problem classes.  The node classes
/// don't have parameters (they get them from milp since the milp class gets parameter
/// changes at the start).
class serialMilpParams : virtual public utilib::ParameterSet
{
public:

  ///
  serialMilpParams();

  /// Use special sets for branching.  For now PICO infers these sets, only sums
  /// of binary variables set equal to 1.  Default ordering is by variable 
  /// number. This will become adjustable later.
  bool useSets;

  /// Give branching on sets priority by default (this can be overridden by an 
  /// application).  Sets are
  /// generally stronger branches than single variables.
  bool giveSetsPriority;

  bool rootLP;
  bool preprocessOnly;
  bool preprocessIP;
  bool onlyRootLP;
  bool saveRootBoundingInfo;
  bool allowPseudoEnumeration;
};

} // namespace pico

#endif
