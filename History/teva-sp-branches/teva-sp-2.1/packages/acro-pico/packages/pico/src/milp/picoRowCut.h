/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file picoRowCut.h
 *
 * Defines the pico::picoRowCut class.
 */

#ifndef pico_picoRowCut_h
#define pico_picoRowCut_h

#include <acro_config.h>
#include <OsiRowCut.hpp>
#include <utilib/hash_fn.h>
#include <utilib/exception_mngr.h>
#include <pico/BCMipBase.h>
#include <utilib/mpiUtil.h>

namespace pico {

using utilib::DoubleVector;
using utilib::PackBuffer;
using utilib::UnPackBuffer;
using utilib::uMPI;

class PicoLPCutMngmnt;
class BCMipProblem;

// Similar to branchSubId so we can track the movement of cuts.  Mostly for debugging.
class cutID
{
public:

  int serial;
  int creatingProcessor;

  // Null constructor

  cutID() :
    serial(-1),
#ifdef ACRO_HAVE_MPI
    creatingProcessor(uMPI::running() ? uMPI::rank : 0)
#else
    creatingProcessor(0)
#endif
  { };

cutID(BCMip *searcher);

void setID(int new_creator, int new_serial)
  {
  creatingProcessor = new_creator;
  serial = new_serial;
  }
};


/// Augments the functionality of OsiRowCut to include methods PICO needs
/// to manage a branch-and-cut search.
/// Osi represents  row cuts of the form ell \le a^Tx \le b,
/// but in PICO, to simplify cut merging, redundancy elimination, etc, we
/// represent cuts as one-sided: a^Tx \le b (and ignore the lower bound
/// after conversion).
class picoRowCut: public BCMipBase
{
friend class PicoLPCutMngmnt;

public:

// Temporary (probably).  For debugging

enum modificationCode
{
None = 0,
OriginalOfTwoSided,
DuplicateOfTwoSided,
CopyFromStronger
};

  modificationCode changes;

   picoRowCut()
    {
    // Important!  This should be set appropriately after construction if one doesn't have
    // access to a BCMip object at construction time.  This default will work with many solvers.
    solverInfinity = DBL_MAX;

    cut = NULL;
    normValue = 0.0;
    GloballyValid = false;
    Persistent = false;
    // This should always be set before reading, but we'll initialize for good form.
    IsDegenerate = false;
    basicInit();
    }

  picoRowCut(BCMip * searcher,
	     OsiRowCut *representation, const int cut_type = -1, 
					char *cut_string = NULL,
	     bool globallyValid = false);

  void cutStorageEffect();

  // Checks to see if the row vector is sorted by index.  If not, sorts it.
  void makeSorted();

  // Converts cut of the form l <= a^Tx to the form a^Tx <= b.  If
  // already of the correct form, check and do nothing.  If there are
  // two nontrivial bounds (both upper and lower), throws an
  // exception.

  void convertToUpperBounded();

  // Returns true of the cut has both upper and lower bounds nontrivial
  bool two_sided()
    {return (cut->lb() != -solverInfinity && cut->ub() != solverInfinity);};

  void basicInit();

  // Used for computing hash values (for comparison).  Two vectors
  // should have the same canonical form if they're parallel within tolerances.
  void computeCanonicalForm(DoubleVector &canonicalForm);

  void putCut(OsiRowCut *new_cut)
    {
    cut = new_cut;
    cutStorageEffect();
    }

  // wrt design document, maxVal() is now ub() through the
  // cut pointer.  Similarly minVal() is now lb()

  // Usage of operator()
  // picoRowCut this_cut;
  // this_cut().ub() now gives the upper bound on the row (b in a^Tx \le b).

  virtual OsiRowCut& operator()() const {return *cut;}

  // Can also access directly, but this may be a more mnemonic name
  // Have to return this as a const since that's what Osi does.
  virtual const CoinPackedVector &lpform() const {return(cut->row());}

  // Has to be set when cut is first stored
  double norm()
	{return(normValue);}

  ///
  double violationComputation(double value);

  /// The next 3 routines have identical behavior, but I think need to
  /// be replicated to allow specification of an LP solution as either
  /// a sparse vector or a dense vector.

  /// returns the worse case violation of either the upper bound or the
  /// lower bound on a^T x (negative means no violation)
  double violation(CoinPackedVector &x)
    {
    EXCEPTION_MNGR(std::runtime_error, "Time to implement a double-sparse dot product in COIN");
    // return(violationComputation(lpform().dotProduct(x))); WEH
    return 0.0;
    }

  ///
  double violation(DoubleVector &x)
        {
	double tmp = lpform().dotProduct(x.data());
	return violationComputation(tmp);
	}

  // Distance from x to the set of x's feasible wrt this cut
  double distance(CoinPackedVector &x)
    {
    return(max(0.0,violation(x))/norm());
    }

  double distance(DoubleVector &x)
    {
    return(max(0.0,violation(x))/norm());
    }

  bool violated(CoinPackedVector &x)
    {
    if (distance(x) > violationTolerance)
      return(true);
    return(false);
    }

  bool violated(DoubleVector &x)
    {
    if (distance(x) > violationTolerance)
      return(true);
    return(false);
    }

  int type()
    {return(Type);}

  // Should only be done once during the construction process
  void setType(int new_type)
    {
    Type = new_type;
    }

  void setTypeString(char *new_string)
    {
    TypeDescription = new_string;
    }

  char* typeString()
    {return(TypeDescription);}


  // How difficult was this cut to generate?
  virtual double difficulty()
    {return(1.0);}

  // This is generally a function of cut type.  Usually true for
  // globally-valid cuts unless cuts are so easy to generate "on the
  // fly" so that scanning a cut pool would be just as expensive.
  // Derived classes should override this virtual function.

  virtual bool poolable()
    {return(globallyValid());}

  bool loaded()
    {return(Loaded);}

  // It may be better to have these accessed only by friends
  void setLoaded()
    {Loaded = true; age=0;}

  void unload()
    {Loaded = false;}

  void setGlobalValidity(bool validity)
    {GloballyValid = validity;}

  bool globallyValid()
    {return(GloballyValid);}

  bool inPool()
    {return(InPool);}

  bool deletable()
    {return refCount()==0 && !loaded() && !inPool();}

  void enterPool()
    {
    InPool = true;
    }

  void leavePool()
    {
    InPool = false;
    }

  bool isPersistent()
    {return(Persistent);};

  // There's no opposite method to make nonPersistent, since we expect
  // persistent cuts will always be persistent till they're deleted.
  void makePersistent()
    {Persistent=true;}

  bool isBinding()
    {return(IsBinding);}

  void nowBinding()
    {IsBinding = true;}

  void nowNotBinding()
    {IsBinding = false;}

  bool isDegenerate()
    {return(IsDegenerate);}

  void nowDegenerate()
    {IsDegenerate = true;}

  void nowNotDegenerate()
    {IsDegenerate = false;}

  //  These shouldn't be public. Only let friends access.  Probably true for
  //  load/unload and inPool bool as well.
  int refCount()
    {return(ReferenceCount);}

  void incrementRefCount()
    {ReferenceCount++;}

  void decrementRefCount()
  {
  ReferenceCount--;
  #ifdef ACRO_VALIDATING
  if (ReferenceCount < 0)
    {
      print();
    EXCEPTION_MNGR(std::runtime_error, "Cut with a negative reference count\n");
    }
  #endif
  }

  // Attempt to get hash table templates to compile
  size_type hash(int table_size) const
    {return(HashValue % table_size);}

  //  double getScaledDual()
  //    {return(scaled_dual);}

  //  void putScaledDual(double new_sd)
  //    {scaled_dual = new_sd;}

  int getAge()
    {return(age);}

  void incAge()
    {age++;}

  void resetAge()
    {age = 0;}

  // These next two comparison operators are used by utilib data structures,
  // specifically the hash table data structures used for the various cut pools.
  // Utilib only uses them to locate a specific cut (e.g. for removal), so equality
  // means exactly the same cut (piece of memory).
  // See the next ones for comparisons useful for detecting redundancy.
  bool operator==(const picoRowCut &other);

  int compare(const picoRowCut& other) const;

  // Assumes this cut and other are aligned.  Scale this
  // cut by the value returned from this call to make the two
  // aligned vectors equal (presumably within tolerance).  This
  // is (max-absolute-value element of this)/(corresponding value in other)

  double getAlignedScaleFactor(const picoRowCut& other) const;

// Makes this cut the same as other.  For numerical stability (to make
// sure the new rhs is really valid), we do this by copying.  If the
// weaker cut (this one) has nicer coefficients, we may want to try
// just resetting the rhs (using scaling via the commented-out code).

  void convertTo(picoRowCut *other);

  // Converts the weakest to the strongest.  Assumes cuts are aligned.

  void makeBothEquivalentToStrongest(picoRowCut *other);

  // Called, e.g. from utilib data structures.  Small amount of information
  // such as ID and pool status for pool debugging.
  void write(std::ostream &os);

  // Returns true if the two row cuts are parallel (For 2D
  // this would mean they have the same slope), and they are pointing
  // in the same direction.  Used to detect redundant (or competing) cuts.

  bool aligned(const picoRowCut& other);

  // compare two aligned cuts (scale as necessary and check the
  // bound).  Returns -1 if this cut has a lower upper bound, 0 if the
  // cuts are equal, 1 if other has a lower upper bound (and hence is
  // stronger).

  int compareAligned(const picoRowCut &other) const;

  // So GenericHashTable code will compile
  void write(std::ostream &) const
    {EXCEPTION_MNGR(std::runtime_error,"write not implemented for picoRowCuts");}

  void read(std::istream &) const
    {EXCEPTION_MNGR(std::runtime_error,"read not implemented for picoRowCuts");}

   virtual ~picoRowCut();

   // These are methods necessary for parallel versions only
#ifdef ACRO_HAVE_MPI
   virtual void pack(PackBuffer &outBuffer);

   // An upper bound on the size of a particular cut instance once packed
   virtual int packSize();

   virtual void unpack(UnPackBuffer& inBuffer, int cutType);
   
   // Maximum packed size for a standard picoRowCut (for this problem).
   // If new derived types have long names, increase the size for the type
   // description [not doing so will only effect maximally dense cuts, which
   // are probably rare].
   static int maxPackSize(BCMipProblem *p);

#endif

   // All derived classes have to define a static counterpart to this
   // method and place a pointer to it into the parBCMipBase::makeNewCut
   // array during static initialization.

   static picoRowCut *makeNew()
      {
	picoRowCut *tmp = new picoRowCut();
#ifdef ACRO_VALIDATING
	if (tmp == NULL)
	  EXCEPTION_MNGR(std::runtime_error, "Failed to allocate a new picRowCut\n");
#endif
	return(tmp);
      }

   //   friend size_type utilib::hash_fn(const pico::picoRowCut& cut, size_type size_type);

  // This will use utilib's char-based hash once it's implemented
  virtual size_type computeHash();

  void printCut()
    {cut->print();}

  // Information useful for debugging (e.g. for when throwing exceptions).  Edit this
  // as needed.
  void print();

  // for debugging.  Temporary

   double ub()
     {return cut->ub();}

   void setUb(double newUB)
     {
     cut->setUb(newUB);
     }
   double lb()
     {return cut->lb();}

   void setLb(double newLB)
     {
     cut->setLb(newLB);
     }

   // This cut will be used in some LP solver.  This is what the solver uses to
   // signal infinity

   double solverInfinity;

protected:

  // We're using a pointer here for easy conversion of the OsiRowCut objects
  // returned from CGL

  // IMPORTANT!  We assume we own the storage for this cut (and can delete it on destruction).
  // We could use an OsiCut.  Right now the only functionality (member data) we use is the
  // global validity flag.

  OsiRowCut *cut;

  double normValue;

  //  We assume the cut generator knows the type of cut and the documentation string
  int Type;

  char *TypeDescription;

  bool Loaded;

  bool InPool;

  // TODO: can we use the field already in OsiRowCut?
  bool GloballyValid;

  bool Persistent;

  // valid for loaded cuts.  Says if the cuts was binding on the last
  // LP solve.  This information is lost after cut aging (immediately
  // after the solve).

  bool IsBinding;

  // Similarly can be lost after cut aging.  Valid for binding loaded cuts.
  // True if the cut is binding but the row slack is still basic.
  bool IsDegenerate;

	/// Used by the PicoLPCutMngmnt class
  int  lp_cut_index;

  // Number of inactive subproblems for which the cut is persistent or
  // binding (binding means that if the cut is removed, the LP
  // solution will (could) violate that cut).  The active subproblem also
  // has an implicit reference to all loaded cuts.

	///
  int ReferenceCount;
	///
  size_type HashValue;

  // Used to determine whether a cut stays active or not (maybe, eventually)
  //  double scaled_dual;

  // Number of consecutive times this cut has been nonbinding (since last addition)

  int age;

  // For debugging.  A unique ID for every cut ever generated.

  cutID id;
};

} // namespace pico

// namespace utilib{
//inline size_type hash_fn(const pico::picoRowCut& cut, size_type size_type)
//{ return cut.HashValue; }
//}

std::ostream& operator<<(std::ostream& s, const pico::cutID& cid);


#endif
