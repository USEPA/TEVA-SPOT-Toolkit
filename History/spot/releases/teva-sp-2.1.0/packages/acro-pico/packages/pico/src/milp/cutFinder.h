/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file cutFinder.h
 *
 * Defines the pico::finderQualityObject class.
 */

#ifndef pico_cutFinder_h
#define pico_cutFinder_h

#include <acro_config.h>
#include <utilib/BasicArray.h>
#include <utilib/DoubleVector.h>
#include <pico/BCMipBase.h>

namespace pico {

using utilib::DoubleVector;

class PicoLPInterface;
class picoRowCut;
class BCMip;
class BCMipNode;

/// Used to schedule the cut finders
class finderQualityObject
{
friend class BCMip;
 protected:

// An average, maintained with exponential smoothing over time
// q_f in the design doc.  Used directly by the scheduler
  double AvgQuality;

  // A similar measure (also maintained with exponential smoothing) for
  // the first time a finder is called on a new subproblem.  Used by the
  // scheduler for new subproblems.

double newSPAvgQuality;

// Number of consecutive times the quality on new subproblems has been so
// low it cannot compete against branching.  As this goes up, it increases the
// likelihood the finder will get a chance anyway.

int UnderThresholdStreakSize;

  // For statistics
int numTimesCalled;
int numCutsFound;
int numInfeasibilityDetections;
// If multiple cuts are found at once, each contributes to this based on its
// dual variable and violation.  Actual effect may not be as good as the sum.
double totalObjectiveGain;
// This includes time to find the cuts and these cuts' share of the resolve time.
double totalTime;

 public:
  finderQualityObject()
    {
    AvgQuality = 0.0;
    newSPAvgQuality = 0.0;
    UnderThresholdStreakSize = 0;
    numTimesCalled = 0;
    numCutsFound = 0;
    numInfeasibilityDetections = 0;
    totalObjectiveGain = 0.0;
    totalTime = 0.0;
    }
};

// do we need to define debug as a global parameter?

/* Decided we didn't need this for now
class cutFinderType
{
 public:
  cutFinderType()
    {typeID = next_id++;};
  int ID()
    {return(typeID);};
 private:
  static int next_id;
  int typeID;
}; */

class cutFinder: public BCMipBase
{
   friend class BCMip;

public:

 /// For CGL finders: knapsack cover, odd hole, simple rounding,
 /// Gomory, lift and project, probing, mixed-integer rounding (MIR,
 /// MIR2, and 2MIR), flow cover, clique, duplicate row, preprocess.
   /// PARINO's lifted knapsack cover.
 /// Then SNL knapsack and
/// cuts added for enumeration (to cut off a current feasible/optimal solution).
 
   enum cutTypeID {ANYCUT, CGL_kc, CGL_oh, CGL_sr, CGL_gom, CGL_LAP, CGL_P, CGL_MIR, CGL_MIR2, CGL_2MIR, CGL_FC, CGL_C, CGL_DR, CGL_Pre, PAR_LKC, SNL_KC, ENUM, FIRST_APPLICATION_SPECIFIC};

 /// This should return NULL if the cuts don't apply to this problem type.
 cutFinder(BCMip *searcher)
	:MIPptr(searcher)
	{}

  /// CGL classes need the LP interface.  Later PICO-specific
  /// classes will probably only need the LP solution.  Have to
  /// have a uniform interface though, so pass them both
  virtual void generateCuts(const PicoLPInterface &lp,
			    BasicArray<picoRowCut *> &new_cuts,
			    int &num_cuts,
			    DoubleVector &lp_solution,
			    bool &problemInfeasible) = 0;

  /// Documentation string for each derived class
  virtual const char *typeString() = 0;

  /// The type of cut returned from this finder
  virtual int cutType() = 0;

  /// Documentation string for type of cut produced.  Don't make this const
  /// because cuts' documentation string is set after construction in parallel
  /// versions (and cannot be set from a const char *)
  virtual char* cutName() = 0;

  /// Eventually, we'll store a pointer to the BCMip instance
  /// used to create the problem.  From there we'll get the
  /// current problem once that's implemented (and won't need
  /// to pass it in)
  virtual double readiness(BCMipNode *current);

  // Really need the BCMipProblem, but we can get that through the
  // BCMip, and we'll eventually need to store the BCMip to access
  // the current problem.
  // We'd like to enforce that this be defined.  Unfortunately, then
  // it cannot be a static method in the derived classes (and cannot be
  // stored in our available array).  Just have to remember to do this.
  // TODO: replace this in the new system
  //virtual cutFinder *makeCutFinder(BCMip *instance) = 0;

  ///
  int type()
    {return(finderType);};

  // True if the cut finder alters the LP solver that gives the
  // problem and the solution. For example, this is true if the cut
  // finder uses the LP solver object to solve a new LP when searching
  // for cuts and it cannot guarentee the the solver is fully
  // restored.  Probably the PICO restoration method is the safest.
  // Even if the solver restores the solution, basis, and even dual
  // solution, it can be hard to guarantee the solver will behave as
  // expected if another cut finder is called before resolve.

virtual bool LPNeedsResolve() = 0;
  ///

  // True if the cut finder guarantees that all cuts are always globally
  // valid.  For now, one can assume that we do only simple branching
  // that modifies the bounds on variables only.

  virtual bool globallyValid() = 0;

  // Return an estimate of finder quality (bound movement/unit time).
  // With the default of 0.0, this is initialized from the first run
  // of the finder.

  virtual double qualityEstimate()
    {return(0.0);}

protected:


  /// This will be an index into the array of valid cut
  /// finders.
  int finderType;

  ///
  BCMip *MIPptr;
};

} // namespace pico

#endif
