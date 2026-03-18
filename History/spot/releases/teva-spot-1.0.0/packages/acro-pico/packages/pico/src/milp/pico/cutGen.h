/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file cutGen.h
 *
 * Cut generators for PICO: CGL wrapper classes and the scan-pool cut generator
 */

#ifndef pico_cutGen_h
#define pico_cutGen_h

#include <acro_config.h>
#include <utilib/exception_mngr.h>
#include <pico/cutFinder.h>
#include <pico/BCMip.h>
#include <CglKnapsackCover.hpp>
#include <CglOddHole.hpp>
#include <CglSimpleRounding.hpp>
#include <pico/PicoCglGomory.hpp>
#include <CglLiftAndProject.hpp>
#include <CglProbing.hpp>
#include <CglClique.hpp>
#include <CglDuplicateRow.hpp>
#include <CglFlowCover.hpp>
#include <CglMixedIntegerRounding.hpp>
#include <CglMixedIntegerRounding2.hpp>
#include <CglTwomir.hpp>


namespace pico {

template <class T>
class PicoCGL: public cutFinder, public T
{
 public:

  PicoCGL<T>(BCMip *searcher) : cutFinder(searcher)
    {
    globalValidityComputation(searcher);
    solverInfinity = searcher->lp->getInfinity();
    FinderSpecificInit();
    };

  // This allows us to set up specific CGL solvers if necessary.
  // Initially this is motivated by the need to set finder-specific
  // parameters to suppress output.

  virtual void FinderSpecificInit()
    {};

  // Let the finder make some global validity decisions
  // based on the problem structure (e.g. set the globallyValidAnswer
  // boolean)

  virtual void globalValidityComputation(BCMip *searcher)
    {};

  // Actual classes will give a more informative string
virtual const char *typeString()
  {return("CGLWrapper");};

  // The type of cut returned from this finder.
// Derived classes have to override.
  virtual int cutType() {return(999);};

  // Documentation string for type of cut produced.
  // Derived classes have to override.

  virtual char* cutName() {return("Shouldn't get this");};

  // Be conservative.  For now, if the CGL routine pulls variable bound information
  // from the solver interface, then we have to assume it's used to compute cuts, and
  // therefore the cuts generated are not globally valid.  Customize this for each solver.

  virtual bool globallyValid()
    {return false;};

  // This has to be decided for each cut solver.  For example, if the solver runs
  // an LP (currently initialSolve() or resolve() calls to the OsiSolverInterface),
  // then a safe answer here would be true.  The default answer will be expensive, but
  // safe.  Be sure to override this if a cut finder can answer false!
  //TODO make sure this is overridden for all CGL solvers.

  virtual bool LPNeedsResolve()
    {return true;};

  // For now, we assume all CGL cut methods are always applicable.
  // Eventually we should probably look at them more closely and
  // check.  We don't want to call expensive cut generators if
  // we know they aren't applicable
  static cutFinder *makeCutFinder(BCMip *BCsearcher)
  {return(new PicoCGL<T>(BCsearcher));};

// To let the B&C instance know this cut generator is available
// Be sure to call this for some (dummy) static int
static int advertise()
  {
  if (!available)
     available=new BasicArray<cutMaker>;
  size_type num_finders = available->size();
  available->resize(num_finders + 1);
  (*available)[num_finders] = (cutMaker)(&makeCutFinder);
  return(0);
  }

void generateCuts(const PicoLPInterface &lp,
		  BasicArray<picoRowCut *> &new_cuts,
		  int &num_cuts,
		  DoubleVector &lp_solution,
		  bool &problemInfeasible);

void generateCuts(const OsiSolverInterface&, OsiCuts&) const
  {
  EXCEPTION_MNGR(std::runtime_error, "Shouldn't be calling a PICO-level cut generator with Osi params");
  };

// We can get this through the MIPPtr, but this is more efficient, since we use it often
 double solverInfinity;

  protected:

// Used to invoke the CGL cut generation routines
OsiCuts cutContainer; 

// Eventually used to get current problem. Used
// by pool scanner to get to pools
BCMip *MIPPtr;

// Allows us to go one step up from just compiling in a true/false constant
// answer for global validity.  This is set at construction time for cases where
// the answer depends upon the problem structure.

bool globallyValidAnswer;
};


// Call the CGL routine to generate cuts.  Convert the OSI cuts returned into
// (one-sided) picoRowCuts
template<class T>
void PicoCGL<T>::generateCuts (const PicoLPInterface &lp,
		  BasicArray<picoRowCut *> &new_cuts,
		  int &num_cuts,
		  DoubleVector & /*lp_solution*/,
		  bool &problemInfeasible)
{
  problemInfeasible = false;
  T::generateCuts(lp, cutContainer);
  int num_returned_cuts = cutContainer.sizeRowCuts();
  num_cuts = 0;
  // Make sure there's enough room, even if all the cuts are two-sided
  if (static_cast<size_type>(2*num_returned_cuts) > new_cuts.size())
    new_cuts.resize(2*num_returned_cuts);
  picoRowCut *new_cut;
  OsiRowCut *returned_cut, *duplicate_cut;
  // For now, used only for bad rows returned by a CGL finder
  BasicArray<OsiRowCut *> to_delete(num_returned_cuts);
  size_type num_to_delete = 0;
#ifdef ACRO_VALIDATING
  bool wasTwoSided;
#endif
  for (int i = 0; i < num_returned_cuts; i++)
    {
#ifdef ACRO_VALIDATING
    wasTwoSided = false;
#endif

      // If a cut generator only sometimes generates globally-valid cuts,
      // we may eventually want to create a (finder-specific) test for
      // that and use that instead of the guarantee only.
    returned_cut = cutContainer.rowCutPtr(i);
    if (returned_cut->row().getNumElements() == 0)
      {
      if (returned_cut->ub() < 0.0 || returned_cut->lb() > 0.0)
	{
	  // Infeasible cut to signal the subproblem is infeasible.
	  // We know probing signals infeasibility this way.  For other generators, issue a warning.
	if (cutType() != CGL_P && cutType() != (int)CGL_gom)
	  {
	  ucout << "Warning: CGL finder returned a cut of type " << cutName() << " with no elements (infeasibility).  Check this was intended.\n";
	  // TODO temp
	  ucout << "Cut is: ";
	  returned_cut->print();
	  ucout << "\n";
	  }
	problemInfeasible = true;
	// Don't process the remaining cuts, including this one
	for (int j = num_returned_cuts - 1; j >= i; j--)
	  cutContainer.eraseRowCut(j);
	break;
	}
      // Otherwise, this is a tautology.  Ignore it and give a warning
      ucout << "Warning: CGL finder returned a cut of type " << cutName() << " with no elements (tautology)\n";
      to_delete[num_to_delete++] = returned_cut;
      continue;
      }

    // No consistent notion of infinity among CGL solvers right now.  Try to make
    // more uniform.

    if (returned_cut->lb() == -DBL_MAX)
      returned_cut->setLb(-solverInfinity);
    if (returned_cut->ub() == DBL_MAX)
      returned_cut->setUb(solverInfinity);
    
      // If the cut is double-sided, turn it into two cuts.
      // Can't use the picoRowCut test for two-sidedness since this is only an
      // OsiRowCut.  Duplicate it here.

    if (returned_cut->lb() != -solverInfinity &&
	returned_cut->ub() != solverInfinity) // two-sided
      {
	// should be a copy
	duplicate_cut = new OsiRowCut(*returned_cut);
#ifdef ACRO_VALIDATING
	if (duplicate_cut == NULL)
	  EXCEPTION_MNGR(std::runtime_error, "Failed to allocate a new OsiRowCut\n");
#endif
	// This will have the lower bound
	duplicate_cut->setUb(solverInfinity);
	// This will be converted to upper bounded (negated) during construction
	new_cut = new picoRowCut(MIPptr, duplicate_cut, cutType(), cutName(),
				 globallyValid());
#ifdef ACRO_VALIDATING
	if (new_cut == NULL)
	  EXCEPTION_MNGR(std::runtime_error, "Failed to allocate a new picoRowCut\n");
	new_cut->changes = picoRowCut::DuplicateOfTwoSided;
	wasTwoSided = false;
#endif
	new_cuts[num_cuts++] = new_cut;
	// original cut will have the upper bound
	returned_cut->setLb(-solverInfinity);
      }
    new_cut = new picoRowCut(MIPptr, returned_cut, cutType(), cutName(),
			     globallyValid());
#ifdef ACRO_VALIDATING
    if (new_cut == NULL)
      EXCEPTION_MNGR(std::runtime_error, "Failed to allocate a new picoRowCut\n");
#endif

    new_cuts[num_cuts++] = new_cut;
#ifdef ACRO_VALIDATING
    if (wasTwoSided)
      new_cut->changes = picoRowCut::OriginalOfTwoSided;
#endif
    }

  // Empty the container without deleting the cuts
  // (Some of the CGL routines seem to assume the cut container is empty.
  // Just insert)
  // TODO - verify that this is needed and that the implmentation is as nice
  // as it could be.
  cutContainer.dumpCuts();

  for (size_type i = 0; i < num_to_delete; i++)
    delete(to_delete[i]);
  if (problemInfeasible)
    {
    while (num_cuts > 0)
      delete(new_cuts[--num_cuts]);
    }
}

// TODO make this aware of the preferred block size and only return
// the most violated cuts if there are more than that many (will effect
// readiness).  Also age cuts according to the design document.

class poolScanner: public cutFinder
{
 protected:
char nameBuffer[8];

 public:

poolScanner(BCMip *searcher);

// May want a lower readiness for scans after the first one on a subproblem
// (we don't expect as much success).

double readiness(BCMipNode *current)
{
  // Not ready if pool is empty
if (MIPptr->poolSize() == 0)
  return(0.0);
// Otherwise, default behavior (ready as long as this hasn't run since last resolve).
return(cutFinder::readiness(current));
}    

const char *typeString()
  {return("poolScanner");};

// We enforce this right now.
bool globallyValid()
  {return(true);};

bool LPNeedsResolve()
  {return(false);};

void generateCuts(const PicoLPInterface &,
		  BasicArray<picoRowCut *> &new_cuts,
		  int &num_cuts,
		  DoubleVector &lp_solution,
		  bool &problemInfeasible)
  {
  problemInfeasible = false;
  MIPptr->scanCutPool(new_cuts, num_cuts, lp_solution);
  }

int cutType()
  {return((int)ANYCUT);};

// This cut finder is always there so it'll be made
// explicitly (this is necessary to instantiate a purely
// virtual function.

cutFinder *makeCutFinder(BCMip *)
  {return(NULL);};

  // Finder type (reserved value)
static int type()
  {return(0);}

char* cutName()
  {return(nameBuffer);}

 virtual ~poolScanner();
 protected:
};


//
// This specialization needs to be in the header to make the TFLOPS compiler
// happy.
//
template<>
inline void PicoCGL<picoCglGomory>::globalValidityComputation(BCMip *searcher)
{
  //TODO: Turn this back on when global validity is fixed
  // Globally valid if and only if we have no general integer variables.
  globallyValidAnswer = (searcher->BCMIP->numGeneralIntVars() == 0);
  //globallyValidAnswer = false;
}

} // namespace pico

#endif
