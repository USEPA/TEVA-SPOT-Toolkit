/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file BCMipBase.h
 *
 * Defines the pico::BCMipBase class.
 */

#ifndef pico_BCMipBase_h
#define pico_BCMipBase_h

#include <acro_config.h>
#include <utilib/_math.h>
#include <utilib/ParameterSet.h>

namespace pico {

using utilib::BasicArray;

class cutFinder;
class BCMip;

typedef cutFinder* (*cutMaker)(BCMip *);

///
/// THis class serves a similar function as the pebblBase class, but defines 
/// static data, functions, enums, parameters, etc, used only for branch and 
/// cut.  All the branch-and-cut classes (cut, search, subproblem, problem) 
/// will derive from this class, sharing the declarations.
///
/// This class defines all the parameters used for branch and cut, even
/// if they could be confined to a subclass like BCMip
///
class BCMipBase : virtual public utilib::ParameterSet
{
public:


  ///
  BCMipBase();

  // For tracking performance.  The set of items we want to track
  // accessible only at the B&C MIP level.  The printing/recording of
  // this information in BCMip.cpp depends upon otherTime being the
  // last member of this enum.
enum BCMIPTiming
  {
    initialBound = 0,
    resolving = 1,
    cutGeneration = 2,
    cutOverhead = 3,
    otherTime = 4, // really a catch-all for anything not tracked
    numBCMIPTimingStates = 5
  };

 const char* BCMIPTimingString(int timingObject)
   {
     return BCMIPTimingStringArray[timingObject];
   }


  // TODO: perhaps some of these tolerances can be reused/combined?

  /// tolerance for determining if a point violates a cut.

  double violationTolerance;

  ///  If a cut has a slack variable less than this, it's considered binding.
  double BindTolerance;

  /// Round to this many decimal places after the decimal point when computing canonical
  /// form for cuts. The maximum absolute value for any entry is 1.
  /// So this is the number of digits used to represent each entry.
  /// "canonical cut decimal point tolerance"

  int canonicalCutDPTol;

  /// Two vectors are considered aligned if the cosine between them is
  // 1 within this tolerance.  Current default set so two vectors are
  // considered aligned if the angle between them is less than 1
  // degree.

  double alignmentTolerance;

  // General documentation for the next few parameters:
  // Max number of cuts in the loaded pool (max number to be loaded)
  // is K_{L,0} + K_{L,r} * (# rows) + K_{L,c} * (# cols), where #rows,cols
  // is in the original LP instance.  The loaded pool will not be larger
  // than this unless all loaded cuts are binding.

  /// Base constant for the maximum number of loaded cuts (K_{L,0} above)
  int loadedPoolSizeConstant;

  /// K_{L,r}
  double loadedPoolSizeRowFactor;

  /// K_{L,c}
  double loadedPoolSizeColFactor;

  /// Base constant for the usual number of cuts to incorporate at once
  int cutBlockSizeConstant;

  /// Increase usual # of cuts to incorporate at once by this times the original # columns
  double cutBlockSizeColFactor;

  /// Increase usual # of cuts to incorporate at once by this times the original # rows
  double cutBlockSizeRowFactor;

  /// Number of consecutive times a cut can be nonbinding before it's unloaded.
  int NonBindLimit;

  /// Used in updating cut-finder quality measures.  The last call is
  /// weighted by this, while previous average is weighted by
  /// (1-this)

  double qualSmoothFactor;

  /// Simliar parameter for the first time a cut finder is called for subproblems

  double firstQualSmoothFactor;

  // Quality of a cut finder determining a subproblem is infeasible is
  // this times some estimate of bound movement (usually the gap if there's
  // an incumbent.

  double infeasibilityDetectionBonus;

  ///Similar parameter for estimate of time to solve
  /// the LP the first time on a subproblem

  double firstSPSolveSmoothFactor;

  // When we pass from the stage when all cut finders get a crack at
  // each new subproblem into competition-only mode, all cut finders
  // that didn't find any cuts in the first phase (zero quality) 
  // have quality set to this times the smallest positive quality among
  // all finders.

  double finderCompetitionQualInitFactor;

  // A weighting factor that helps give all finders a chance to (eventually) run
  // again, even if their performance has been poor.  When we start to work on a new
  // subproblem, those with quality lower than the current branchingReadyMeasure, have
  // their quality incremented by a small term that uses this weighting.  In essence, they'll
  // get a chance to run every 1/(this param) subproblems or so.

double cutFinderRunPossibleFactor;

// Give branching a positive quality, even when we expect every branch
// to have no movement.  This is multiplied by an estimate of the optimal
// objective function magnitude and added to other measures of branching
// quality.

  ///
  double branchPossibleFactor;


// Parameters for determining readiness to branch.  There is a negative
// exponential factor that damps readiness (so goes to zero quickly as
// the number of LP solves increases, thus moving readiness to 1).
// We have a parameter to weight the exponent, one to weight the subtraction
// of this exponential factor, and one to weight the whole thing.

  ///
  double totalBranchReadyFactor;

  ///
  // This weights the subtraction (how long we'll wait to branch)
  double branchWaitFactor;

  ///
  double branchExpFactor;

// branch when the desirability of branching is this factor
// greater than the desirability of each cut finder

  double branchBias;

// Number of initial subproblems for which we try all finders.

  int tryAllFindersLimit;

  // During finder competition, one way to suppress a finder is if its effective quality
  // is less than this factor times the best effective quality at the last resolve.

  double finderCompetitiveFactor;

// Make timings pseudorandom.  We hope this will make behavior
// deterministic (at least in serial) and allow a lot of variety.

  bool makeTimingsPseudoRandom;

// Seed for random number generator when using (pseudo)random numbers for timings.

  int TimingSeed;

  ///

  bool using_cuts;

  /// If this is true, run the root LP solve, immediately apply the incumbent
  /// heuristic (before full bounding), and return

  bool onlyRootHeuristic;

  /// Parameters for the SNL knapsack cuts class

  double initial_snlks_bound;
  double snlks_reduction_rate;

protected:

  // Printing strings for the operations for which we're tracking timing

 static const char *BCMIPTimingStringArray[numBCMIPTimingStates];

 /// Each (non-abstract) cut finder class linked into the library will place a
 /// pointer to it's (static) makeCutFinder method here on static initialization
 static BasicArray<cutMaker>* available;

 /// An array of documentation strings for cuts, indexed by cut types.  A
 /// custom cut class will have to get its type from a static cut number
 /// "server" (like messageID) during static initialization, and set this string.
 static BasicArray<char *> *cutTypeStrings;

};

} // namespace pico

#endif
