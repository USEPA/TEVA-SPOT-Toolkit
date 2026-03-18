/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

//
// bbhConfig.cpp
//
// 
//
// 
//

#include <acro_config.h>
#include <pico/bbhConfig.h>

using namespace std;

namespace pico {

using utilib::ParameterSet;

BBHConfig::BBHConfig()
{
bbhDebug=0;
ParameterSet::create_categorized_parameter("bbhDebug",bbhDebug,"<int>","0",
                               "Debugging level for MIP heuristic",
	"BBH",
			       utilib::ParameterNonnegative<int>());

bbhObjectiveTol=1.0e-6;
ParameterSet::create_categorized_parameter("bbhObjectiveTol", bbhObjectiveTol,
	"<double>","1e-6",
	"Tolerance for comparing objective coefficients to zero",
	"BBH",
	utilib::ParameterNonnegative<double>());

bbhRayTol=1e-6;
ParameterSet::create_categorized_parameter("bbhRayTol",bbhRayTol,
	"<double>","1e-6",
	"Tolerance for comparing ray direction components to zero",
	"BBH",
	utilib::ParameterNonnegative<double>());

bbhFeasibilityTol=1e-8;
ParameterSet::create_categorized_parameter("bbhFeasibilityTol",bbhFeasibilityTol,
	"<double>","1e-8",
	"Tolerance for comparing ray direction components to zero",
	"BBH",
	utilib::ParameterNonnegative<double>());

bbhCanonicalOnly=false;
ParameterSet::create_categorized_parameter("bbhCanonicalOnly",bbhCanonicalOnly,
			       "<bool>","false",
			       "Always use canonical alpha=1/2 merit function"
			       " (otherwise random)",
	"BBH");

bbhRampUpUseCanonical=false;
ParameterSet::create_categorized_parameter("bbhRampUpUseCanonical",bbhRampUpUseCanonical,
			       "<bool>","false",
			       "Force one processor per group to use\n\t"
			       "canonical merit function during ramp-up.",
	"BBH");
 
bbhSeedAlpha=1;
ParameterSet::create_categorized_parameter("bbhSeedAlpha",bbhSeedAlpha,
	"<int>","1",
	"Seed number used when randomizing merit functions",
	"BBH",
	utilib::ParameterBounds<int>(0, 30000));

bbhShift=1e-5;
ParameterSet::create_categorized_parameter("bbhShift",bbhShift,
	"<double>","1e-5",
	"Shift parameter used when bbhAlphaType==1",
	"BBH",
	utilib::ParameterBounds<double>(0.0,1.0));

bbhRoundingMethod=1;
ParameterSet::create_categorized_parameter("bbhRoundingMethod",bbhRoundingMethod,
	"<int>","1",
	"Rounding method: 0 - ratio, 1 - sum (mfstructure)",
	"BBH",
	utilib::ParameterBounds<int>(0,1));

bbhWeight=1e-3;
ParameterSet::create_categorized_parameter("bbhWeight",bbhWeight,
	"<double>","1e-3",
	"Weight of an objective used when bbhRoundingMethod==1",
	"BBH",
	utilib::ParameterNonnegative<double>());

bbhMeritFunctionTolerance=1e-5;
ParameterSet::create_categorized_parameter("bbhMeritFunctionTolerance",
bbhMeritFunctionTolerance,
	"<double>","1e-5",
	"Tolerance for comparing merit function to zero",
	"BBH",
	utilib::ParameterNonnegative<double>());

bbhProbingComplexity=0.35;
ParameterSet::create_categorized_parameter("bbhProbingComplexity",bbhProbingComplexity,
	"<double>","0.35",
	"Relative complexity of the probing pivot",
	"BBH",
	utilib::ParameterNonnegative<double>());

bbhIterationComplexity=3.0;
ParameterSet::create_categorized_parameter("bbhIterationComplexity",bbhIterationComplexity,
	"<double>", "3.0",
	"Relative complexity of a regular main loop iteration",
	"BBH",
	utilib::ParameterNonnegative<double>());

bbhObjCutOn=false;
ParameterSet::create_categorized_parameter("bbhObjCutOn",bbhObjCutOn,
	"<bool>","false",
	"Handling of a priori value via objective cut",
	"BBH");

bbhSatQualityRatio=1.0;
ParameterSet::create_categorized_parameter("bbhSatQualityRatio", bbhSatQualityRatio,
	"<double>","1.0",
	"Ratio used to identify satisfactory quality",
	"BBH",
	utilib::ParameterNonnegative<double>());

bbhReqQualityRatio=0.0;
ParameterSet::create_categorized_parameter("bbhReqQualityRatio", bbhReqQualityRatio,
	"<double>","0.0",
	"Ratio used to identify required quality",
	"BBH",
	utilib::ParameterNonnegative<double>());

bbhTheta=1e-6;
ParameterSet::create_categorized_parameter("bbhTheta", bbhTheta,
	"<double>","1e-6",
	"Used in computing ordering of probing pivots (ratio method)",
	"BBH",
	utilib::ParameterLowerBound<double>(1e-10));

bbhMaxMFResets=0;
ParameterSet::create_categorized_parameter("bbhMaxMFResets",bbhMaxMFResets,
	"<int>","0",
	"Maximum number of MF resets",
	"BBH",
	utilib::ParameterBounds<int>(0,30000));

bbhReoptWeight=1e-5;
ParameterSet::create_categorized_parameter("bbhReoptWeight",bbhReoptWeight,
	"<double>","1e-5",
	"Weight of the original objective to be used in the\n\t"
        "repotimization solver calls",
	"BBH",
	utilib::ParameterNonnegative<double>());

// bbhConvCutPreference=1.0;
// ParameterSet::create_categorized_parameter("bbhConvCutPreference",bbhConvCutPreference,
// 	"<double>","1.0",
// 	"Preference coefficient between convexity and vertex cuts",
//	"BBH",
// 	utilib::ParameterNonnegative<double>());

bbhMaxTenure=24;
ParameterSet::create_categorized_parameter("bbhMaxTenure",bbhMaxTenure,
	"<int>","24",
	"Maximum tenure of an inactive cut",
	"BBH",
	utilib::ParameterBounds<int>(1, 30000));

bbhPeriodicity=24;
ParameterSet::create_categorized_parameter("bbhPeriodicity",bbhPeriodicity,
	"<int>","24",
	"Periodicity of the cut inactivity test",
	"BBH",
	utilib::ParameterBounds<int>(1,30000));

// bbhFWIter=500;
// ParameterSet::create_categorized_parameter("bbhFWIter",bbhFWIter,
// 	"<int>","500",
// 	"Maximum number of iterations per Frank-Wolfe call"
//      "\n\t(use 1 for regular sum method, does not affect ratio method)",
//	"BBH",
// 	utilib::ParameterBounds<int>(1, 30000));

bbhHeurTimeLimit=60.0;
ParameterSet::create_categorized_parameter("bbhHeurTimeLimit",bbhHeurTimeLimit,
	"<double>","60.0",
	"Maximum number of seconds alotted per heuristic call",
	"BBH",
	utilib::ParameterNonnegative<double>());

bbhMaxHeurPivots=0;
ParameterSet::create_categorized_parameter("bbhMaxHeurPivots",bbhMaxHeurPivots,
	"<int>","0",
	"Maximum number of simplex pivots per heuristic call.\n\t"
        "A value of zero indicates an unlimited number of iterations.",
	"BBH",
	utilib::ParameterBounds<int>(0, 30000));

bbhMasterIter=100000;
ParameterSet::create_categorized_parameter("bbhMasterIter",bbhMasterIter,
	"<int>","100000",
	"\"Master\" iteration limit: heuristic will not be\n\t"
        "called if the total number of heuristic pivots exceeds it.",
	"BBH",
	utilib::ParameterBounds<int>(0, 10000000));

bbhMaxFWIter=40;
ParameterSet::create_categorized_parameter("bbhMaxFWIter",bbhMaxFWIter,
	"<int>","40",
	"Maximum number of major FW iterations",
	"BBH",
	utilib::ParameterBounds<int>(0, 1000));

bbhMinProblemAbsGap=0.5;
ParameterSet::create_categorized_parameter("bbhMinProblemAbsGap",bbhMinProblemAbsGap,
	"<double>","0.5",
	"Minimum absolute problem gap when heuristic is still run",
	"BBH",
	utilib::ParameterNonnegative<double>());

// bbhMaxDepth=4;
// ParameterSet::create_categorized_parameter("bbhMaxDepth",bbhMaxDepth,
// 	"<int>","4",
// 	"Maximum tree depth at which heuristic is run for all subproblems",
//	"BBH",
// 	utilib::ParameterBounds<int>(1, 30000));

bbhMinCalls=5;
ParameterSet::create_categorized_parameter("bbhMinCalls",bbhMinCalls,
	"<int>","5",
	"Minimum number of problems in the beginning of the\n\t" 
        "run for which the heuristic is always called (unless master\n\t"
        "iteration limit is exceeded)",
	"BBH",
	utilib::ParameterBounds<int>(1,1000));

bbhRelGapCutoff=1e-2;
ParameterSet::create_categorized_parameter("bbhRelGapCutoff", bbhRelGapCutoff,
	"<double>","1e-2",
	"Minimum relative problem gap when the heuristic is still run",
	"BBH",
	utilib::ParameterBounds<double>(0.0, 1.0));

bbhGapRatioCutoff=0.0;
ParameterSet::create_categorized_parameter("bbhGapRatioCutoff",bbhGapRatioCutoff,
	"<double>","0.0",
	"Maximum gap ratio of the problem and global absolute\n\t"
        "gaps when the heuristic is still run",
	"BBH",
	utilib::ParameterBounds<double>(0.0, 1.0));

bbhIntMeasureCutoff=1.0;
ParameterSet::create_categorized_parameter("bbhIntMeasureCutoff",bbhIntMeasureCutoff,
	"<double>","1.0",
	"Maximum problem percentile of integrality measure in\n\t"
        "the population when the heuristic is still run",
	"BBH",
	utilib::ParameterBounds<double>(0.0,1.0));

bbhPMax=0.5;
ParameterSet::create_categorized_parameter("bbhPMax",bbhPMax,
	"<double>","0.5",
	"Standard maximum probability of the run",
	"BBH",
	utilib::ParameterBounds<double>(0.0,1.0));

bbhRelGapExp=1.0;
ParameterSet::create_categorized_parameter("bbhRelGapExp",bbhRelGapExp,
	"<double>","1.0",
	"A probability adjustment exponent",
	"BBH",
	utilib::ParameterBounds<double>(0.0,100.0));

bbhGapRatioExp=2.0;
ParameterSet::create_categorized_parameter("bbhGapRatioExp",bbhGapRatioExp,
	"<double>","2.0",
	"A probability adjustment exponent",
	"BBH",
	utilib::ParameterBounds<double>(0.0,100.0));

bbhIntMeasureExp=2.0;
ParameterSet::create_categorized_parameter("bbhIntMeasureExp",bbhIntMeasureExp,
	"<double>","2.0",
	"A probability adjustment exponent",
	"BBH",
	utilib::ParameterBounds<double>(0.0,100.0));

bbhScalingStatus=false;
ParameterSet::create_categorized_parameter("bbhScalingStatus",bbhScalingStatus,
	"<bool>","false",
	"If true, use scaling in BBH",
	"BBH");

bbhAPrioriObj=1e20;
ParameterSet::create_categorized_parameter("bbhAPrioriObj",bbhAPrioriObj,
	"<double>","1e20",
	"A priori bound on the objective (note: heuristic minimizes)",
	"BBH");

bbhOutStatus=false;
ParameterSet::create_categorized_parameter("bbhOutStatus",bbhOutStatus,
	"<bool>","false",
	"Output status",
	"BBH");

bbhMaxCuts=100;
ParameterSet::create_categorized_parameter("bbhMaxCuts",bbhMaxCuts,
	"<int>","100",
	"Maximum number of cuts",
	"BBH",
	utilib::ParameterBounds<int>(0, 30000));

bbhMaxPool=20;
ParameterSet::create_categorized_parameter("bbhMaxPool",bbhMaxPool,
	"<int>","20",
	 "Number of fractional solutions stored for rounding by "
	 " heuristic\n\t(parallel version only)",
	"BBH",
         utilib::ParameterLowerBound<int>(1));

bbhPoolRemove=false;
ParameterSet::create_categorized_parameter("bbhPoolRemove",bbhPoolRemove,
	"<bool>","false",
	"Remove solutions from heuristic pool after "
	"trying to round them\n\t(parallel version only)",
	"BBH");

bbhRampUpMFTarget=2;
ParameterSet::create_categorized_parameter("bbhRampUpMFTarget", bbhRampUpMFTarget,
	"<int>","2",
	"Number of different merit functions to try in rampUp heuristic "
				"\n\t(parallel version only)",
	"BBH",
	utilib::ParameterNonnegative<int>());

bbhPreSplitDepth=1;
ParameterSet::create_categorized_parameter("bbhPreSplitDepth", bbhPreSplitDepth,
	"<int>","1",
	"Depth of pre-splitting to try in ramp-up heuristic "
				"\n\t(parallel version only)",
	"BBH",
	utilib::ParameterNonnegative<int>());

bbhAddCols=3;
ParameterSet::create_categorized_parameter("bbhAddCols", bbhAddCols,
	"<int>","3",
	"Maximum number of duplicate columns (in each direction) "
	"around general\n\tinteger variables",
	"BBH",
	utilib::ParameterNonnegative<int>());
}

} // namespace pico
