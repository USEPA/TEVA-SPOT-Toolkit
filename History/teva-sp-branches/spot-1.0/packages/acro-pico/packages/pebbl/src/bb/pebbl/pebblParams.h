/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file pebblParams.h
 *
 * Defines the pebbl::pebblParams class.
 */

#ifndef pebbl_pebblParams_h
#define pebbl_pebblParams_h

#include <acro_config.h>
#include <utilib/ParameterSet.h>

namespace pebbl {


/// A base class for PEBBL parameters. 
class pebblParams : virtual public utilib::ParameterSet, virtual public
				utilib::CommonIO
{
public:

  ///
  pebblParams();

  ///
  int statusPrintCount;

  ///
  double statusPrintSeconds;

  ///
  bool depthFirst;

  ///
  bool breadthFirst;

  ///
  bool initialDive;

  ///
  bool integralityDive;

  ///
  bool lazyBounding;

  ///
  bool eagerBounding;

  ///
  double relTolerance;

  ///
  double absTolerance;

  ///
  double earlyOutputMinutes;

  ///
  double startIncumbent;

  ///
  bool validateLog;

  ///
  bool heurLog;

  ///
  int maxSPBounds;

  ///
  double maxCPUMinutes;

  ///
  double maxWallMinutes;

  ///
  bool printAbortMessage;

  ///
  bool printIntMeasure;

  ///
  bool printDepth;

  ///
  int debugPrecision;

  ///
  bool suppressWarnings;

  ///
  int randomSeed;

  ///
  int loadMeasureDegree;

  ///
  double EnumerationRelTolerance;

  ///
  double EnumerationAbsTolerance;

  ///
  int EnumerationCount;

  ///
  bool debug_solver_params;

  ///
  bool use_abort;

  ///
  bool version_flag;

  ///
  bool printFullSolution;

};

} // namespace pebbl

#endif
