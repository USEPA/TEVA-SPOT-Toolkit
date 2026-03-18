/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file bbhConfig.h
 *
 * Defines the pico::BBHConfig class.
 */

#ifndef pico_bbhConfig_h
#define pico_bbhConfig_h

#include <acro_config.h>
#include <pebbl/pebblParams.h> 

namespace pico {

///
class BBHConfig : virtual public utilib::ParameterSet
{
public:

  ///
  BBHConfig();
  
  ///
  virtual ~BBHConfig() {}

  //Standard heuristic parameters:
  int bbhDebug;
  
  /// EPS = tolerance for comparing objective coefficients to 0
  double bbhObjectiveTol;
  
  /// EPS1 = tolerance for comparing ray direction components to 0 
  double bbhRayTol;
  
  /// EPS2 = feasibility tolerance
  double bbhFeasibilityTol;
  
  /// Use canonical merit function only
  bool bbhCanonicalOnly;

  /// Use canoncial merit function in at least one processor per group
  /// during ramp up.
  bool bbhRampUpUseCanonical;
  
  /// Seed number for the case bbhAlphaType==2
  int bbhSeedAlpha;
  
  /// shift parameter for the case bbhAlphaType==1
  double bbhShift;
  
  /// Rounding method: 0 - ratio, 1 - sum (mfstructure)
  int bbhRoundingMethod;
  
  /// Weight of an objective for the case bbhRoundingMethod==1
  double bbhWeight;
  
  /// mftol = tolerance for comparing merit function to 0
  double bbhMeritFunctionTolerance;
  
  /// Relative complexity of the probing pivot
  double bbhProbingComplexity;
  
  /// Relative complexity of a regular main loop iteration
  double bbhIterationComplexity;
  
  /// Handling of a priori value via objective cut
  bool bbhObjCutOn;
  
  // For probing:
  
  /// "Satisfactory quality" ratio (qualityRatio)
  double bbhSatQualityRatio;
  
  /// "Required quality" ratio (pessimisticQualityRatio)
  double bbhReqQualityRatio;
  
  /// Parameter used in computing ordering of probing pivots (ratio method)
  double bbhTheta;
  
  /// Maximum number of MF resets
  int bbhMaxMFResets;
  
  /// Weight of the original objective to be used in the
  /// reoptimization solver calls
  double bbhReoptWeight;
  //Preference coefficient between convexity and vertex cuts
  double bbhConvCutPreference;
  //For cut management:
  //Maximum tenure of inactive cut
  int bbhMaxTenure;
  //Periodicity of the cut inactivity test
  int bbhPeriodicity;
  //Maximum number of iterations per Frank-Wolfe call (use 1 for
  //regular sum method, does not affect ratio method)
  //int bbhFWIter;
  //Maximum time alotted per heuristic call
  double bbhHeurTimeLimit;
  //Maximum number of pivots per heuristic call
  int bbhMaxHeurPivots;

  //"Master" iteration limit: heuristic will not be called if
  //the total number of heuristic pivots exceeds it
  int bbhMasterIter;
  //Maximum number of major FW iterations
  int bbhMaxFWIter;

  //The following are parameters associated with heuristic, but
  //used outside of IPHeuristic object:
  //Mimimum absolute problem gap when heuristic is still run
  double bbhMinProblemAbsGap;
  //Maximum tree depth at which heuristic is run for all subproblems
  //int bbhMaxDepth;
  //Minimum number of problems in the beginning of the run for which the
  //heuristic is always called (unless master iteration limit is exceeded)
  int bbhMinCalls;

  //Minimum relative problem gap when the heuristic is still run
  double bbhRelGapCutoff;
  //Maximum gap ratio of the problem and global absolute gaps
  //when the heuristic is still run
  double bbhGapRatioCutoff;
  //Maximum problem percentile of integrality measure in the
  //population when the heuristic is still run
  double bbhIntMeasureCutoff;
  //Standard maximum probability of the run
  double bbhPMax;
  //Probability adjustment exponents
  double bbhRelGapExp;
  double bbhGapRatioExp;
  double bbhIntMeasureExp;
  //Scaling status
  bool bbhScalingStatus;
  //A priori bound on the objective (note: heuristic maximizes)
  double bbhAPrioriObj;
  
  /// Output status
  bool bbhOutStatus;
  ///Maximum number of cuts
  int bbhMaxCuts;

  /// For parallel stuff -- max number of incomplete solutions to keep around
  int bbhMaxPool;

  /// The number of different merit functions to try in ramp-up heuristic
  int bbhRampUpMFTarget;
  /// Maximum depth of presplitting allowed in ramp-up heuristic
  int bbhPreSplitDepth;

  /// In parallel heuristic, remove things from pool after trying them
  /// once.
  bool bbhPoolRemove;

  /// Number of artificial columns, in each direction, to add 
  /// duplicating a general integer variable.  If zero, general
  /// integers are constrained to the "current" interval.
  int bbhAddCols;

};
  
} // namespace pico

#endif

