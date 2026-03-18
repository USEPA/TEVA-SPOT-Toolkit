/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// picoRowCut.cpp
//

//#include <utilib/hash_fn.h>
#include <acro_config.h>
#include <pico/BCMip.h>
#include <pico/picoRowCut.h>

using namespace utilib;
using namespace std;

namespace pico {


  // For now, picoRowCuts are one-sided
double picoRowCut::violationComputation(double value)
{
#ifdef ACRO_VALIDATING
  if (cut->ub() == solverInfinity || cut->lb() != -solverInfinity)
    EXCEPTION_MNGR(runtime_error,"violationComputation: Cut isn't one sided\n");
#endif
  return(value - cut->ub());
}

void picoRowCut::basicInit()
{
Loaded = false;
InPool = false;
ReferenceCount = 0;
age = 0;
IsBinding = true;
changes = None;
}

void picoRowCut::makeSorted()
{
const int* theseIndices = lpform().getIndices();
int num_elements = lpform().getNumElements();
for (int i = 0; i < num_elements-1; i++)
  {
    if (theseIndices[i] > theseIndices[i+1])
      {
      cut->sortIncrIndex();
      break;
      }
  }
}

// Converts cut of the form l <= a^Tx to the form a^Tx <= b.  If
// already of the correct form, check and do nothing.  If there are
// two nontrivial bounds (both upper and lower), throws an exception.

void picoRowCut::convertToUpperBounded()
{
  // Osi's definition of negative infinity.
  if (cut->lb() != -solverInfinity)
    {
      if (cut->ub() != solverInfinity)
	// two-sided, so throw exception
	{
	EXCEPTION_MNGR(runtime_error, "Trying to create a PicoRowCut with a two-sided vector\n");
	}
      // one-sided in the wrong direction, so convert
      cut->setUb(-cut->lb());
      cut->setLb(-solverInfinity);
      (*cut) *= -1.0;
    }
  // Could check here and warn about trivial bounds (-infinity to infinity).  Don't do that for now.
}


  // Used for computing hash values (for comparison).  Two vectors
  // should have the same canonical form if they're parallel within tolerances.
// The input DoubleVector already has the elements of the vector (no need to know
// true indices for this).
// These are one-sided inequalities, so no need to negate (if two vectors have
// canonical forms that are exact negatives, then one gives a lower bound and the
// other an upper bound).

void picoRowCut::computeCanonicalForm(DoubleVector &canonicalForm)
{
  // The largest absolute value of any element
canonicalForm[0] = d_round(canonicalForm[0], canonicalCutDPTol);
double maxAbsVal = fabs(canonicalForm[0]);
// The index of the element with the largest absolute value 
size_type indexWithMax = 0;
size_type i;
size_type numElements = canonicalForm.size();

// Round each element to canonicalCutDPTol # of decimal places after
// the decimal point, and look for the maximum absolute value after rounding.

for (i=1;i < numElements; i++)
  {
    // round to this many digits after the decimal point (in base 10)
  canonicalForm[i] = d_round(canonicalForm[i], canonicalCutDPTol);

  if (fabs(canonicalForm[i]) > maxAbsVal)
    {
    maxAbsVal = fabs(canonicalForm[i]);
    indexWithMax = i;
    }
  }

// Scale so the largest absolute value will be 1.  It's important to round first
// so two vectors that are very close will choose the same element as the scaling
// value.

for (i=0; i< numElements; i++)
    canonicalForm[i] /= maxAbsVal;
}

// Returns true if the two row cuts are parallel (For 2D this would
// mean they have the same slope), and they are pointing in the same
// direction.  Two vectors are parallel if the cosine of the angle
// between them is 1 (within tolerance). Used to detect redundant (or
// competing) cuts.

bool picoRowCut::aligned(const picoRowCut &other)
{
if (HashValue != other.HashValue)
  return(false);
if (cut->row().getElements()[0] * other.cut->row().getElements()[0] < 0)
  return (false); // not pointing in the same direction even if parallel.
double cosine = sortedSparseDotProduct(lpform(), other.lpform())/(normValue * other.normValue);
return(fabs(1.0-cosine) <= alignmentTolerance);
}

// Used by hash table to find an object, so we're looking for precise equality,
// not functional equaliaty.
bool picoRowCut::operator==(const picoRowCut &other)
{
return(cut == other.cut);
}

// Row cuts are of the form l <= ax <= u.  We would like all cuts with the same
// a values (within tolerance) to have the same hash value.  That way, when we check
// for redundancy, we can also check for dominance or merge cuts.

size_type picoRowCut::computeHash()
{

int num_elements = lpform().getNumElements();

// Put a copy of the elements into "canonical form"

const double *elements = lpform().getElements();
DoubleVector canonicalForm(num_elements);
for (int i = 0; i < num_elements; i++)
  canonicalForm[i] = elements[i];
computeCanonicalForm(canonicalForm);

HashValue=0;

// Hash the indices of the sparse array
HashValue = hash_bj(lpform().getIndices(), num_elements, HashValue);

// Hash the coefficients of the sparse array
HashValue = hash_bj(canonicalForm.data(), num_elements, HashValue);

return HashValue;
}


  // compare two aligned cuts (scale and check the bound).  Returns -1 if this
  // cut has a lower upper bound, 0 if the cuts are equal, 1 if other has a
  // lower upper bound (and hence is stronger).

int picoRowCut::compareAligned(const picoRowCut &other) const
{
double scale_factor = getAlignedScaleFactor(other);
double scaled_rhs = cut->ub() * scale_factor;
if (scaled_rhs - other.cut->ub() <= pebbl::pebblBase::integerTolerance)
  return(0);
if (scaled_rhs < other.cut->ub())
  return(-1);
return(1);
}

// Makes this cut the same as other.  For numerical stability (to make
// sure the new rhs is really valid), we do this by copying.  If the
// weaker cut (this one) has nicer coefficients, we may want to try
// just resetting the rhs (using scaling via the commented-out code).

void picoRowCut::convertTo(picoRowCut *other)
{
  delete cut;
  // uses copy constructor
  cut = new OsiRowCut(*other->cut);

#ifdef ACRO_VALIDATING
  if (cut == NULL)
    EXCEPTION_MNGR(runtime_error, "Failed to allocate a new OsiRowCut\n");
  changes = CopyFromStronger;
#endif

  /* old version.  Might have had numerical issues
double scale_factor = getAlignedScaleFactor(other);
double scaled_rhs = cut->ub() * scale_factor;
// Nothing to do if equal.
if (scaled_rhs - other.cut->ub() <= pebblBase::integerTolerance)
  return;
if (scaled_rhs < other.cut->ub())
  {
  other.setUb(scaled_rhs);
  return;
  }
setUb(other.ub()/scale_factor);
  */
}


// Converts the weakest to the strongest.  Assumes cuts are aligned.

void picoRowCut::makeBothEquivalentToStrongest(picoRowCut *other)
{
int compare_result = compareAligned(*other);
if (compare_result == 0)
  return;
if (compare_result < 0)
  other->convertTo(this);
else this->convertTo(other); 
}


  // Assumes this cut and other are aligned.  Scale this
  // cut by the value returned from this call to make the two
  // aligned vectors equal (presumably within tolerance).  This
  // is (max-absolute-value element of this)/(corresponding value in other)

double picoRowCut::getAlignedScaleFactor(const picoRowCut& other) const
{
  // Find the largest element in absolute value (should be the same for both
  // vectors, since they're aligned.)

  // The largest absolute value of any element
const double *row_elements = cut->row().getElements();
double maxAbsVal = fabs(row_elements[0]);
// The index of the element with the largest absolute value 
int indexWithMax = 0;
int numElements = cut->row().getNumElements();
for (int i=1;i < numElements; i++)
  if (fabs(row_elements[i]) > maxAbsVal)
    {
    maxAbsVal = fabs(row_elements[i]);
    indexWithMax = i;
    }
// Vectors are pointing the same way, so the scale factor is positive
double scale_factor = other.cut->row().getElements()[indexWithMax]/row_elements[indexWithMax];
#ifdef ACRO_VALIDATING
// If the vectors are pointing in opposite directions, they aren't really comparable in this sense,
// and they're not redundant.
  if (scale_factor < 0)
    EXCEPTION_MNGR(runtime_error,"Running compareAligned on antiparallel vectors!");
#endif
return(scale_factor);
}


// Used by utilib hash table to find precise cuts (e.g. for removal).
// Just use address of the OsiRowCut pointer.
int picoRowCut::compare(const picoRowCut& other) const
{
if (cut == other.cut) return(0);
if (cut < other.cut) return(-1);
return(1); 
}

// Only for parallel versions
#ifdef ACRO_HAVE_MPI

void picoRowCut::pack(PackBuffer &outBuffer)
{
  // The OsiRowCut object.  This was sorted upon creation, so
  // no need to worry about that now or upon unpacking

  // bounds on the linear combination (doubles).  Only have an upper bound now.

outBuffer << cut->ub();

int num_elements = lpform().getNumElements();
const int *indices = lpform().getIndices();
const double *elements = lpform().getElements();

outBuffer << num_elements;
for (int i = 0; i < num_elements; i++)
  {
  outBuffer << indices[i];
  outBuffer << elements[i];
  }
  
outBuffer << normValue;
// Type will be packed ahead of this vector in a set of cuts and will
// be passed into the unpack method, so no need to pack it here
// Name is stored in static array and can be recovered from type.
outBuffer << HashValue;
outBuffer << GloballyValid;
}

// Upper bound on the size of a particular cut.  Computed incrementally
// for easy update
int picoRowCut::packSize()
{
int packSize = 0;
// For the upper bound on the linear combination
packSize += sizeof(double);
// Tells number of elements in the cut representation
packSize += sizeof(int); 
// The elements of the cut as (index, value) pairs
packSize += lpform().getNumElements() * (sizeof(int) + sizeof(double));
// for norm
packSize += sizeof(double);
// For hash value
packSize += sizeof(size_type);
// For global validity bit
packSize += sizeof(bool);
return(packSize);
}

// Upper bound on the size of a particular cut.  Computed incrementally
// for easy update
int picoRowCut::maxPackSize(BCMipProblem *p)
{
int packSize = 0;
// For the upper bound on the linear combination
packSize += sizeof(double);
// Tells number of elements in the cut representation
packSize += sizeof(int); 
// The elements of the cut as (index, value) pairs
packSize += p->numAllVars() * (sizeof(int) + sizeof(double));
// for norm 
packSize +=  sizeof(double);
// for hash value
packSize += sizeof(size_type);
// For the global validity bit
packSize += sizeof(bool);
return(packSize);
}

void picoRowCut::unpack(UnPackBuffer& inBuffer, int cutType)
{
Type = cutType;
TypeDescription = (*cutTypeStrings)[cutType];

  // The OsiRowCut object.  This was sorted upon creation, so
  // no need to worry about that now

cut = new OsiRowCut();
#ifdef ACRO_VALIDATING
 if (cut == NULL)
   EXCEPTION_MNGR(runtime_error, "Failed to allocate a new OsiRowCut\n");
#endif
  // bounds on the linear combination
double cut_bound;
inBuffer >> cut_bound;
cut->setUb(cut_bound);

 // initialization isn't necessary, but quiets compiler, which doesn't understand
// the next line sets the value of num_elements

int num_elements = 0;
inBuffer >> num_elements;

IntVector indices(num_elements);
DoubleVector elements(num_elements);

for (int i = 0; i < num_elements; i++)
  {
  inBuffer >> indices[i];
  inBuffer >> elements[i];
  }

cut->setRow(num_elements, indices.data(), elements.data());
  
inBuffer >> normValue;
inBuffer >> HashValue;
inBuffer >> GloballyValid;
}
#endif //using MPI (for pack-related methods)

cutID::cutID(BCMip *searcher)
{
#ifdef ACRO_HAVE_MPI
  creatingProcessor = (uMPI::running() ? uMPI::rank : 0);
#else
  creatingProcessor = 0;
#endif
  serial = searcher->getCutSerial();
}

// A shortened output version that can be called from utilib
// methods and can be called directly for debugging.  Just
// modify the output to show the fields most useful at the time.
void picoRowCut::write(ostream &os)
{
  os << "(" << id << ", ";
  //  os << "(" << id << ", " << lp_cut_index;
  //  os << "(" << id << ", " << ReferenceCount;
  if (inPool())
    os << ", P";
  if (loaded())
    os << ", L";
  os << ")";
}


// Information useful for debugging (e.g. for when throwing exceptions).  Edit this
// as needed.

void picoRowCut::print()
{
ucout << "Cut ID: " << id
      << "\nReference count: " << ReferenceCount
      << "\nType: " << TypeDescription
      << "\nHash Value: " << HashValue
      << "\nNorm: " << normValue << "\n"
      << (InPool ? "" : "Not ") << "in pool\n"
      << (Loaded ? "" : "Not ") << "loaded\n"
      << (GloballyValid ? "" : "Not ") << "globally valid\n"
      << (Persistent ? "" : "Not ") << "persistent\n"
      << "Changes indicator: " << changes << "\n";
printCut();
}

picoRowCut::~picoRowCut()
{
  // temporary, for debugging
  //    ucout << "Deleting cut " << id << "\n";
#ifdef ACRO_VALIDATING
if (ReferenceCount > 0)
  EXCEPTION_MNGR(runtime_error, "Deleting Pico row cut with nonzero binding count\n");
#endif
delete(cut);
}

void picoRowCut::cutStorageEffect()
{
      // I think the hash computation (and checking for parallel vectors)
      // won't work if the sparse vectors aren't sorted

  makeSorted();
  convertToUpperBounded();
  normValue = cut->row().twoNorm();
  HashValue = computeHash();
}

picoRowCut::  picoRowCut(BCMip * searcher,
			 OsiRowCut *representation, const int cut_type, 
			 char *cut_string, bool globallyValid): 
  solverInfinity(searcher->lp->getInfinity()),
  cut(representation),
  Type(cut_type), TypeDescription(cut_string),
  GloballyValid(globallyValid),
  Persistent(false),
  id(searcher)
{
  cutStorageEffect();
  basicInit();
}


} // namespace pico

ostream& operator<<(ostream& s, const pico::cutID& cid)
{
  s << '{';
#ifdef ACRO_HAVE_MPI
  if (uMPI::running())
    s << cid.creatingProcessor << ':';
#endif
  return s << cid.serial << '}';
}
