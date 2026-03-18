/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// BCMipBase.cpp
//
// Serves a similar function as the pebblBase class, but defines static
// data, functions, enums, etc, used only for branch and cut.  All the
// branch-and-cut classes (cut, search, subproblem, problem) will
// derive from this class, sharing the declarations.

#include <acro_config.h>
#include <pico/BCMipBase.h>

using namespace std;

namespace pico {

const char* BCMipBase::BCMIPTimingStringArray[BCMipBase::numBCMIPTimingStates] =
{
  "1st subprob bounding",
  "Resolves",
  "Cut generation",
  "Some cut overhead",
  "Other",
  };

BCMipBase::BCMipBase()
  : violationTolerance(1e-7),
    BindTolerance(1e-7),
    canonicalCutDPTol(2),
    alignmentTolerance(1e-4),
    loadedPoolSizeConstant(100),
    loadedPoolSizeRowFactor(20.0),
    loadedPoolSizeColFactor(20.0),
    cutBlockSizeConstant(20),
    cutBlockSizeColFactor(0.1),
    cutBlockSizeRowFactor(0.1),
    NonBindLimit(10),
    qualSmoothFactor(0.5),
    firstQualSmoothFactor(0.1),
    infeasibilityDetectionBonus(10.0),
    firstSPSolveSmoothFactor(0.1),
    finderCompetitionQualInitFactor(0.3),
    cutFinderRunPossibleFactor(.01),
    branchPossibleFactor(1e-5),
    totalBranchReadyFactor(1.0),
    // Chose this so we aren't very ready to branch when we've done few solves
    branchWaitFactor(2.0),
    branchExpFactor(1.0),
    // Default now is # children generated (ie. we prefer not to branch if
    // we can make progress cutting).  Have to be careful we are always able
    // to branch though
    branchBias(2.0),
    tryAllFindersLimit(10),
    finderCompetitiveFactor(0.5),
    makeTimingsPseudoRandom(false),
    TimingSeed(0),
    initial_snlks_bound(0.6),
    snlks_reduction_rate(0.25)
{
// TODO These really need to be checked (and all defaults in general)
// The quality measure that controls an individual subproblem (1st) should
// change more rapidly than the 2nd two, which are global for the whole
// search.

create_parameter("violationTolerance",violationTolerance,
	"<double>","1e-7",
	"Tolerance for determining if a point violates (doesn't satisfy) a cut",
	utilib::ParameterLowerBound<double>(0.0));

create_parameter("loadedPoolSizeConstant",loadedPoolSizeConstant,
	"<int>","100",
	"Base constant for the maximum number of loaded cuts",
	utilib::ParameterLowerBound<int>(0));

create_parameter("loadedPoolSizeRowFactor",loadedPoolSizeRowFactor,
	"<double>","20.0",
	"Maximum number of loaded cuts is increased from the base by this times the original # of rows",
	utilib::ParameterLowerBound<double>(0.0));

create_parameter("loadedPoolSizeColFactor",loadedPoolSizeColFactor,
	"<double>","20",
	"Maximum number of loaded cuts is increased from the base by this times the original # of columns",
	utilib::ParameterLowerBound<double>(0.0));

create_parameter("cutBlockSizeConstant",cutBlockSizeConstant,
	"<int>","20",
	"Base constant for the usual number of cuts to incorporate at once.",
	utilib::ParameterLowerBound<int>(0));

create_parameter("cutBlockSizeRowFactor",cutBlockSizeRowFactor,
	"<double>","0.1",
	"Increase usual # of cuts to incorporate at once by this times the original # rows",
	utilib::ParameterLowerBound<double>(0.0));

create_parameter("cutBlockSizeColFactor",cutBlockSizeColFactor,
	"<double>","0.1",
	"Increase usual # of cuts to incorporate at once by this times the original # columns",
	utilib::ParameterLowerBound<double>(0.0));

create_parameter("BindTolerance",BindTolerance,
	"<double>","1e-7",
	"If a cut has a slack variable less than this, it's considered binding.",
	utilib::ParameterLowerBound<double>(0.0));

 create_parameter("canonicalCutDPTol",canonicalCutDPTol,
	"<int>","2",
	"Round to this many decimal places after the decimal point when computing canonical form for cuts.",
	utilib::ParameterLowerBound<int>(0));

 // This will consider two vectors aligned if they angle between them is less than 1 degree.
 create_parameter("alignmentTolerance", alignmentTolerance,
	"<double>","1e-4",
	"Two vectors are considered aligned if the cosine between them is 1 within this tolerance",
	utilib::ParameterLowerBound<double>(0.0));

create_parameter("NonBindLimit",NonBindLimit,
	"<int>","10",
	"Number of times a cut can be nonbinding before it's unloaded.",
	utilib::ParameterLowerBound<int>(0));

create_parameter("qualSmoothFactor",qualSmoothFactor,
	"<double>","0.5",
	"Weight given to the last call in an exponential smoothing when updating cut finder quality measures",
	utilib::ParameterBounds<double>(0.0,1.0));

create_parameter("firstQualSmoothFactor",firstQualSmoothFactor,
	"<double>","0.1",
	"Weight given to the last call in an exponential smoothing when updating cut finder quality measures for the first call to a subproblem",
	utilib::ParameterBounds<double>(0.0,1.0));

 create_parameter("infeasibilityDetectionBonus",infeasibilityDetectionBonus,
	"<double>","10.0",
		  "The quality for a cut finder run that detects subproblem infeasibility is this times an estimate of bound movement (usually the gap)",
	utilib::ParameterLowerBound<double>(0.0));

create_parameter("firstSPSolveSmoothFactor",firstSPSolveSmoothFactor,
	"<double>","0.1",
	"Weight given to the last call in an exponential smoothing when updating time to run a cut finder on a subproblem for the first time.",
	utilib::ParameterBounds<double>(0.0,1.0));

create_parameter("finderCompetitionQualInitFactor", finderCompetitionQualInitFactor,
	"<double>","0.3",
	"Minimum quality for a cut finder at the start of the competition-only phase is this times the smallest positive quality among all finders.",
	utilib::ParameterBounds<double>(0.0,1.0));

create_parameter("cutFinderRunPossibleFactor", cutFinderRunPossibleFactor,
	"<double>","0.01",
	"Even very poor cut finders will have a chance to run about every 1/this subproblems",
	utilib::ParameterBounds<double>(0.0,1.0));

create_parameter("branchPossibleFactor",branchPossibleFactor,
	"<double>","1e-5",
	"Used to make sure branching always has a positive quality measure.",
	utilib::ParameterBounds<double>(0.0,1.0));

create_parameter("totalBranchReadyFactor",totalBranchReadyFactor,
	"<double>","1.0",
	"Weight used to calculate branching readiness (see documentation)",
	utilib::ParameterBounds<double>(0.0,1.0));

create_parameter("branchWaitFactor",branchWaitFactor,
	"<double>","2.0",
	"Weight used to calculate branching readiness (see documentation)",
	utilib::ParameterLowerBound<double>(0.0));

create_parameter("branchExpFactor",branchExpFactor,
	"<double>","1.0",
	"Weight used to calculate branching readiness (see documentation)",
	utilib::ParameterLowerBound<double>(0.0));

create_parameter("branchBias",branchBias,
	"<double>","2.0",
	"branch when the desirability of branching is this factor greater than the desirability of each cut finder",
	utilib::ParameterLowerBound<double>(0.0));

create_parameter("tryAllFindersLimit",tryAllFindersLimit,
	"<int>","10",
	"Number of initial subproblems for which we try all finders (rather than just use competition).\nMust be at least one.  If you don't want to run a solver once, disable it.",
	utilib::ParameterLowerBound<int>(1));

 create_parameter("finderCompetitiveFactor", finderCompetitiveFactor,
	"<double>","0.5",
	"Finders with effective quality less than this times the best (among all finders after last resolve) cannot compete in till the next resolve",
	utilib::ParameterBounds<double>(0.0, 1.0));


create_parameter("makeTimingsPseudoRandom", makeTimingsPseudoRandom,
	"<bool>","false",
	"Use pseudorandom values for all timings used for B&C scheduling are this value.\n Try to make behavior more deterministic.");

create_parameter("TimingSeed", TimingSeed,
	"<int>","0",
	"(Integer) Seed used for pseudorandom generator when using pseudorandom timings for B&C scheduling (see makeTimingsPseudoRandom\n");

using_cuts=true;
create_parameter("usingCuts",using_cuts,
	"<bool>","true",
	"If true, then use predefined cut classes");

onlyRootHeuristic=false;
create_parameter("onlyRootHeuristic",onlyRootHeuristic,
	"<bool>","false",
	"If true, then after solving the root LP, apply the incubent heuristic and exit");

// TODO: check bounds.  Since variables are binary and this is used to check a solution value,
// 0 to 1 seems reasonable
 create_parameter("initial_snlks_bound", initial_snlks_bound,
	"<double>","0.6",
		  "Used in SNL knapsack cut finder.  TODO: find more complete explanation",
	utilib::ParameterBounds<double>(0.0, 1.0));

 // TODO: check bounds, get more information
 create_parameter("snlks_reduction_rate", snlks_reduction_rate,
	"<double>","0.25",
		  "Used in SNL knapsack cut finder (initial_snlks_bound reduced to this percent each iteration).  TODO: find out more",
	utilib::ParameterBounds<double>(0.0, 1.0));


}

BasicArray<cutMaker>* BCMipBase::available = 0;

BasicArray<char *>*   BCMipBase::cutTypeStrings = 0;

} // namespace pico
