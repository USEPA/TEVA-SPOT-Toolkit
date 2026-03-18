/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// PicoLPInterface.cpp
//

#include <acro_config.h>
#include <pico/ilp_read.h>
#include <pico/PicoLPInterface.h>
#include <CoinPackedMatrix.hpp>

using namespace std;
using namespace utilib;

#if 0
WEH - I think that this is not needed after Cindys rework of this class.
#if defined(SOLARIS) && !defined(__GNUC__)
namespace utilib {

template<>
int        EnumBitArray<1,pico::basisState>::enum_count    = 4;

template<>
char       EnumBitArray<1,pico::basisState>::enum_labels[] = "NBUL";

template<>
pico::basisState EnumBitArray<1,pico::basisState>::enum_vals[]   = { pico::nonbasic , pico::basic, pico::atUpper, pico::atLower };

}
#endif
#endif

namespace pico {


//
// An initial stab at correlating OSI state method calls to the values of the
// myState variable...
//
void PicoLPInterface::solve(bool knowPrimalBounded)
{
solving = true;
if (solverMethod == primalSimplex)
   initialSolve();
else if (solverMethod == dualSimplex)
   resolve();
else
   EXCEPTION_MNGR(runtime_error,"Non-simplex solvers not currently supported.");

// Turn it off now so we can return whenever we want.  If we ever call for a secondary
// solve, then delay this.

solving = false;

myState = broken;
have_basis=false;

if (isProvenOptimal()) {
   myState = solved;
   have_basis=true;
   }

else if (isIterationLimitReached() || isTimeLimitReached()) {
   myState = suspended;
   have_basis = true;
   
   int curr,tmp;
   getIntParam(OsiMaxNumIterationHotStart,tmp);
   getIntParam(OsiMaxNumIteration,curr);
   setIntParam(OsiMaxNumIteration,curr+tmp);
   }

 else {
   if (isProvenPrimalInfeasible() || isProvenDualUnbounded()) {
      myState = infeasible;
      have_basis = false;
      }
   else if (isProvenPrimalUnbounded()) {
      myState = unbounded;
      have_basis = false;
      }
   else if (isProvenDualInfeasible())
     {
     if (knowPrimalBounded)
       {
	 // Probably temporary
	 ucout << "Warning: no primal status proven, invoking known-primal-bounded flag\n";
       myState = infeasible;
       have_basis = false;
       }
     // TODO Try to find a feasible primal solution by doing a primal stage 1 simplex.
     // If we find any feasible solution, primal is unbounded.  Otherwise it is infeasible.
     // This case is likely to be hit rarely, so add it when it becomes necessary.  For now,
     // this case will be interpreted as broken.
     }
   else if (isDualObjectiveLimitReached()) {
      myState = pastCutoff;
      have_basis = false;
      }
   else if (isPrimalObjectiveLimitReached()) {
      myState = pastCutoff;
      have_basis = false;
      }
   }
}



void PicoLPInterface::loadProblem(CMSparseMatrix<Ereal<double> >& matrix,
                        BasicArray<Ereal<double> >& collb,
                        BasicArray<Ereal<double> >& colub,
                        BasicArray<Ereal<double> >& obj,
                        BasicArray<Ereal<double> >& rowlb,
                        BasicArray<Ereal<double> >& rowub,
                        BasicArray<CharString>& rNames,
                        BasicArray<CharString>& cNames)
{
loadProblem(matrix,collb,colub,obj,rowlb,rowub);
setVariableNames(cNames);
//
// TODO: If this condition occurs, then we have encountered empty rows.
// for now, simply assume that they are at the end of the list of rows, but
// we need a more robust mechanism here...
//
if (rNames.size() > (unsigned int) getNumRows())
   rNames.resize(getNumRows());
setRowNames(rNames);
}


void PicoLPInterface::loadProblem(CMSparseMatrix<Ereal<double> >& _matrix,
                        BasicArray<Ereal<double> >& _collb,
                        BasicArray<Ereal<double> >& _colub,
                        BasicArray<Ereal<double> >& _obj,
                        BasicArray<Ereal<double> >& _rowlb,
                        BasicArray<Ereal<double> >& _rowub)
{
CMSparseMatrix<double> matrix(_matrix);

BasicArray<double> collb(_collb.size());
BasicArray<double> colub(_colub.size());
BasicArray<double> obj(_obj.size());
BasicArray<double> rowlb(_rowlb.size());
BasicArray<double> rowub(_rowub.size());

size_type len;
len = collb.size();
for (size_type i=0; i<len; i++)
  collb[i] = as_double(_collb[i]);

len = colub.size();
for (size_type i=0; i<len; i++)
  colub[i] = as_double(_colub[i]);

len = rowlb.size();
for (size_type i=0; i<len; i++)
  rowlb[i] = as_double(_rowlb[i]);

len = rowub.size();
for (size_type i=0; i<len; i++)
  rowub[i] = as_double(_rowub[i]);

len = obj.size();
for (size_type i=0; i<len; i++)
  obj[i] = as_double(_obj[i]);

CoinPackedMatrix tmp(true, matrix.get_nrows(), matrix.get_ncols(),
			matrix.get_nnzero(), matrix.get_matval().data(),
			matrix.get_matind().data(),
			matrix.get_matbeg().data(),
			matrix.get_matcnt().data());
execLoadProblem(tmp,collb.data(),colub.data(),obj.data(),rowlb.data(),rowub.data());
resetInfo();
num_core_rows = matrix.get_nrows();
for (int i=0; i<matrix.get_ncols(); i++)
  addColInfo(i);
for (int i=0; i<matrix.get_nrows(); i++)
  addRowInfo(i);
}


void PicoLPInterface::loadProblem(const CoinPackedMatrix& matrix,
                             const double* collb, const double* colub,
                             const double* obj,
                             const double* rowlb, const double* rowub,
                             CharString* rNames, CharString* cNames)
{
execLoadProblem(matrix,collb,colub,obj, rowlb,rowub);
resetInfo();
num_core_rows = matrix.getNumRows();
for (int i=0; i<matrix.getNumCols(); i++)
  addColInfo(i);
for (int i=0; i<matrix.getNumRows(); i++)
  addRowInfo(i);

if (cNames) {
   for (int i=0; i<matrix.getNumCols(); i++)
     setVariableName(i,cNames[i]);
   }

if (rNames) {
   for (int i=0; i<matrix.getNumRows(); i++)
     setRowName(i,rNames[i]);
   }
}


void PicoLPInterface::loadProblem(const CoinPackedMatrix& matrix,
                             const double* collb, const double* colub,
                             const double* obj,
                             const char* rowsen, const double* rowrhs,
                             const double* rowrng,
                             CharString* rNames, CharString* cNames)
{
execLoadProblem(matrix,collb,colub,obj, rowsen,rowrhs,rowrng);
resetInfo();
num_core_rows = matrix.getNumRows();
for (int i=0; i<matrix.getNumCols(); i++)
  addColInfo(i);
for (int i=0; i<matrix.getNumRows(); i++)
  addRowInfo(i);

if (cNames) {
   for (int i=0; i<matrix.getNumCols(); i++)
     setVariableName(i,cNames[i]);
   }

if (rNames) {
   for (int i=0; i<matrix.getNumRows(); i++)
     setRowName(i,rNames[i]);
   }
}


void PicoLPInterface::loadProblem(const int numcols, const int numrows,
                             const int* start, const int* index,
                             const double* value,
                             const double* collb, const double* colub,
                             const double* obj,
                             const double* rowlb, const double* rowub)
{
EXCEPTION_MNGR(runtime_error, "PicoLPInterface::loadProblem - unimplemented capability")
}


void PicoLPInterface::loadProblem(const int numcols, const int numrows,
                             const int* start, const int* index,
                             const double* value,
                             const double* collb, const double* colub,
                             const double* obj,
                             const char* rowsen, const double* rowrhs,
                             const double* rowrng)
{
EXCEPTION_MNGR(runtime_error, "PicoLPInterface::loadProblem - unimplemented capability")
}


void PicoLPInterface::loadProblem(const CharString& filename,
                                        BasicArray<int>& listOfIntegersP,
                                        ilp_format format)
{
if (format == cplex_mps_format)
   EXCEPTION_MNGR(runtime_error,"PicoLPInterface::loadProblem -- the CPLEX MPS format is not supported with this LP solver.");

CMSparseMatrix<Ereal<double> > A;
CharString givenName;
int objsen = pebbl::minimize;
BasicArray<Ereal<double> > objCoefs;
BasicArray<Ereal<double> > lowerBounds;
BasicArray<Ereal<double> > upperBounds;
BasicArray<Ereal<double> > constraintLHS;
BasicArray<Ereal<double> > constraintRHS;
BasicArray<CharString>     colNames;
BasicArray<CharString>     rowNames;

ilp_read(filename,(short)(LPdebug),A,givenName,objsen,objCoefs,
                lowerBounds,upperBounds,
                colNames, constraintLHS, constraintRHS,
                rowNames,listOfIntegersP,format);
if (objCoefs.size() == 0)
   return;

loadProblem(A, lowerBounds, upperBounds, objCoefs,
                constraintLHS, constraintRHS, rowNames, colNames);
setName(givenName.data());
setObjSense(objsen);
}



void PicoLPInterface::loadProblem(const CharString& filename1,
				  const CharString& filename2,
                                        BasicArray<int>& listOfIntegersP,
                                        ilp_format format)
{
if (format == cplex_mps_format)
   EXCEPTION_MNGR(runtime_error,"PicoLPInterface::loadProblem -- the CPLEX MPS format is not supported with this LP solver.");

CMSparseMatrix<Ereal<double> > A;
CharString givenName;
int objsen = pebbl::minimize;
BasicArray<Ereal<double> > objCoefs;
BasicArray<Ereal<double> > lowerBounds;
BasicArray<Ereal<double> > upperBounds;
BasicArray<Ereal<double> > constraintLHS;
BasicArray<Ereal<double> > constraintRHS;
BasicArray<CharString>     colNames;
BasicArray<CharString>     rowNames;

ilp_read(filename1,filename2,(short)(LPdebug),A,givenName,objsen,objCoefs,
                lowerBounds,upperBounds,
                colNames, constraintLHS, constraintRHS,
                rowNames,listOfIntegersP,format);
if (objCoefs.size() == 0)
   return;

loadProblem(A, lowerBounds, upperBounds, objCoefs,
                constraintLHS, constraintRHS, rowNames, colNames);
setName(givenName.data());
setObjSense(objsen);
}



//
// TODO: enable specification/requirement of specific file extension
//
int PicoLPInterface::readMps(const char* filename,
                         const char *)
{
BasicArray<int> tmp;
CharString fname = filename;
loadProblem(fname,tmp,mps_format);
return 0; // number of errors
}

void PicoLPInterface::write(char *filename, ilp_format format)
{
  ostream *fileOstream = new ofstream(filename, ios::out);
  write(*fileOstream, format);
}

void PicoLPInterface::write(ostream& os, ilp_format format,
                        const char* extension)
{
//
// writing the LP in MPS format
//
if (format == mps_format) {
   os << "Warning: mps_format not completely implemented!" << endl;
   os << "NAME    " << myName << endl;

   os << "ROWS" << endl;
   int nrows=getNumRows();
   //int ncols=getNumCols();
   os << " N  OBJ" << endl;
   for (int i=0; i<nrows; i++) {
     constraintSense tmp = getRowSense(i);
     switch (tmp) {
        case lessOrEqual: os << " L "; break;
        case greaterOrEqual: os << " G "; break;
        case equal: os << " E "; break;
        case ranged: os << " R "; break;
        case notRestricted: os << " N "; break;
        }
     os << getRowName(i) << endl;
     }

   os << "COLS" << endl;
   }

//
// writing the LP in LP format
//
else if (format == lp_format) {
   os << "Warning: lp_format not implemented!" << endl;
   }

//
// writing verbose diagnostic output
//
else if (format == verbose_format) {
   os << "Status:  " << status << endl;
   os << "State:   " << (int)myState << endl;
   os << "Name:    " << myName << endl;
   os << "Sense:   " << (int)(getObjSense()) << endl;
   os << "NRows:   " << getNumRows() << endl;
   os << "NCols:   " << getNumCols() << endl;
   os << "NNZeros: " << getNumElements() << endl;

   os << Flush;

   BasicArray<Ereal<double> > obj(getNumCols());
   getObjCoefficients(obj);
   os << "Objective:" << endl;
   {for (int i=0; i<getNumCols(); i++) {
#if 1 // WEH -- for debugging
     os << getVariableName(i) << "\t" << obj[i] << endl;
#else
     os << i << "\t" << obj[i] << endl;
#endif
     }}

   os << Flush;

   BasicArray<Ereal<double> > rowupper(getNumRows());
   getRowUpper(rowupper);
   BasicArray<Ereal<double> > rowlower(getNumRows());
   getRowLower(rowlower);
   BasicArray<Ereal<double> > rhs(getNumRows());
   getRightHandSide(rhs);
   os << "Constraints:" << endl;
   {for (int i=0; i<getNumRows(); i++) {
#if 1   // WEH - for debugging
     os << getRowName(i) << "\t(" << getRowSense(i) << "\t" << 
						rhs[i] <<
		 ")\t" << rowlower[i] << "\t" << rowupper[i] << endl;
#else
     os << i << "\t(" << getRowSense(i) << "\t" << 
						rhs[i] <<
		 ")\t" << rowlower[i] << "\t" << rowupper[i] << endl;
#endif
     }}

   os << Flush;

   BasicArray<Ereal<double> > colupper(getNumCols());
   getColUpper(colupper);
   BasicArray<Ereal<double> > collower(getNumCols());
   getColLower(collower);
   os << "Variables:" << endl;
   for (int i=0; i<getNumCols(); i++) {
#if 1 // WEH - for debugging
     os << getVariableName(i) << "\t" << collower[i] << "\t" << 
#else
     os << i << "\t" << collower[i] << "\t" << 
#endif
						colupper[i] << endl;
     }

   os << Flush;

   os << "ConstraintMatrix:" << endl;
   CMSparseMatrix<Ereal<double> > tmp;
   getMatrixByCol(tmp);
   os << tmp << endl;

   os << Flush;
   cerr.flush();
   cout.flush();
   }
}


void PicoLPInterface::write(PackBuffer& outBuffer) const
{
BasicArray<Ereal<double> > obj;
CMSparseMatrix<Ereal<double> > mat;
CharString sense;
BasicArray<Ereal<double> > collb;
BasicArray<Ereal<double> > colub;
BasicArray<Ereal<double> > rowlb;
BasicArray<Ereal<double> > rowub;

getInfo(obj,mat,sense,collb,colub,rowlb,rowub);

double optsense = getObjSense();
outBuffer << optsense << obj 
	<< mat << sense << collb << colub << rowlb << rowub;
}


void PicoLPInterface::loadProblem(UnPackBuffer& inBuffer)
{
double optsense;
BasicArray<Ereal<double> > obj;
CMSparseMatrix<Ereal<double> > mat;
CharString sense;
BasicArray<Ereal<double> > collb;
BasicArray<Ereal<double> > colub;
BasicArray<Ereal<double> > rowlb;
BasicArray<Ereal<double> > rowub;

inBuffer >> optsense >> obj >> mat >> sense >> collb >> colub >> rowlb >> rowub;

loadProblem(mat,collb,colub,obj,rowlb,rowub);

setObjSense(optsense);
}



void PicoLPInterface::setVariableNames(const BasicArray<CharString>& names)
{
size_type nCols = getNumCols();
if (names.size() != nCols)
   EXCEPTION_MNGR(runtime_error,"PicoLPInterface::setVariableNames -- Trying to setup variable names with " << names.size() << " names when there are " << nCols << " columns");

for (size_type i=0; i<nCols; i++)
  setVariableName(i,names[i]);
}


void PicoLPInterface::setRowNames(const BasicArray<CharString>& names)
{
size_type nRows = getNumRows();
if (names.size() != nRows)
   EXCEPTION_MNGR(runtime_error,"PicoLPInterface::setRowNames -- Trying to setup constraint names with " << names.size() << " names when there are " << nRows << " rows");

for (size_type i=0; i<nRows; i++)
  setRowName(i,names[i]);
}


PicoLPInterface::constraintSense PicoLPInterface::getRowSense(int whichConstraint) const
{
char tmp = getRowSense()[whichConstraint];

constraintSense sense = notRestricted;
switch (tmp) {
  case 'L':	sense = lessOrEqual; break;
  case 'G':	sense = greaterOrEqual; break;
  case 'E':	sense = equal; break;
  case 'R':	sense = ranged; break;
  case 'N':	sense = notRestricted; break;
  default:
	EXCEPTION_MNGR(runtime_error,"PicoLPInterface::getRowSense - bad sense type=" << tmp);
  };
return sense;
}


void PicoLPInterface::getMatrixByRow(RMSparseMatrix<Ereal<double> >& mat) const
{
CMSparseMatrix<Ereal<double> > tmp;
getMatrixByCol(tmp);
mat.convert(tmp);
}


void PicoLPInterface::getMatrixByCol(CMSparseMatrix<Ereal<double> >& mat) const
{
const CoinPackedMatrix* tmp = getMatrixByCol();

mat.initialize(tmp->getNumRows(), tmp->getNumCols(), tmp->getNumElements());

const double* matval = tmp->getElements();
const int* matind = tmp->getIndices();
int nElts = tmp->getNumElements();

const int* matbeg = tmp->getVectorStarts();
const int* matcnt = tmp->getVectorLengths();
int nCols = tmp->getNumCols();
// CAP: This form of copy is necessary because CoinPackedMatrix
// objects can have extra space at the beginning or end of rows or
// columns.  The elements for column i start at matbeg[i] and run for
// matcnt[i], but there can then be a gap between this last element of
// column i and the first of column i+1.  So, for example, matval can have
// more than nElts elements.

 // For the matrix we're copying from
 int currColStart;
 // For the matrix we're building
 int elementptr = 0;
for (int j=0; j<nCols; j++)
  {
    mat.get_matbeg()[j] = elementptr;
    currColStart = matbeg[j];
  mat.get_matcnt()[j] = matcnt[j];
  for (int i=0; i<matcnt[j]; i++)
    {
      mat.get_matval()[elementptr] = as_ereal(matval[currColStart + i]);
      mat.get_matind()[elementptr++] = matind[currColStart + i];
    }

  }
}


void PicoLPInterface::getInfo(BasicArray<Ereal<double> >& objCoefs,
                        CMSparseMatrix<Ereal<double> >& mat,
                        CharString& sense,
                        BasicArray<Ereal<double> >& collb, 
			BasicArray<Ereal<double> >& colub,
                        BasicArray<Ereal<double> >& rowlb, 
			BasicArray<Ereal<double> >& rowub) const
{
int nCols=getNumCols();
int nRows=getNumRows();

objCoefs.resize(nCols);
getObjCoefficients(objCoefs);

getMatrixByCol(mat);

sense.resize(nRows);
getRowSense(sense.data());

collb.resize(nCols);
getColLower(collb);

colub.resize(nCols);
getColUpper(colub);

rowlb.resize(nRows);
getRowLower(rowlb);

rowub.resize(nRows);
getRowUpper(rowub);
}


//
// TODO: make this numerically stable
//
double PicoLPInterface::evalVector(BasicArray<double>& testvec)
{
double value = 0;
size_type size = testvec.size();
const double *objectiveCoefs = getObjCoefficients();

for (size_type i = 0; i < size; i++)
  value += objectiveCoefs[i] * testvec[i];

return(value);
}


void PicoLPInterface::setRowSense(int index, constraintSense newSense)
{
constraintSense curr = getRowSense(index);
if (newSense == notRestricted)
   EXCEPTION_MNGR(runtime_error,"PicoLPInterface::setRowSense -- trying to set the sense of a row to unrestricted");
if (newSense == ranged)
   EXCEPTION_MNGR(runtime_error,"PicoLPInterface::setRowSense -- trying to set the sense of a row to ranged");
if (curr == notRestricted)
   EXCEPTION_MNGR(runtime_error,"PicoLPInterface::setRowSense -- trying to set the sense of an unrestricted row=" << index);
if ((curr == ranged) && (newSense == equal))
   EXCEPTION_MNGR(runtime_error,"PicoLPInterface::setRowSense -- ambiguous request: convert a ranged constraint to an equality constraint");

double val=0.0;
switch (curr) {
  case lessOrEqual:
		val = getRowUpper(index); break;
  case greaterOrEqual:
		val = getRowLower(index); break;
  case equal:
		val = getRowLower(index); break;
  case ranged:
		if (newSense == lessOrEqual) 
		   val = getRowUpper(index);
		else
		   val = getRowLower(index);
		break;
  default:
		break;
  };
switch (newSense) {
  case lessOrEqual:
		setRowBounds(index,-getInfinity(),val);
		break;
  case greaterOrEqual:
		setRowBounds(index,val,getInfinity());
		break;
  case equal:
		setRowBounds(index,val,val);
		break;
  default:
		break;
  };
}


void PicoLPInterface::testLP()
{
  BasicArray<Ereal<double> > lowerBounds(getNumCols());
  BasicArray<Ereal<double> > upperBounds(getNumCols());
  getColLower(lowerBounds);
  getColUpper(upperBounds);
  cout << "LP Lower bounds are: " << lowerBounds << "\n";
  cout << "LP Upper bounds are: " << upperBounds << "\n";
}


PicoLPInterface::PicoLPInterface(const PicoLPInterface& lp)
 : OsiSolverInterface(*this)
{
reset();
gutsOfCopy(lp);
}


PicoLPInterface& PicoLPInterface::operator=(const PicoLPInterface& rhs)
{
if (this != &rhs) {
   reset();
   OsiSolverInterface::operator=( rhs );
   gutsOfCopy( rhs );
   }
return *this;
}


void PicoLPInterface::gutsOfCopy(const PicoLPInterface& lp)
{
solverMethod 		= lp.solverMethod;
rowNameCtr 		= lp.rowNameCtr;
colNameCtr		= lp.colNameCtr;
colInfo			= lp.colInfo;
rowInfo			= lp.rowInfo;
myName			= lp.myName;
have_basis		= lp.have_basis;
myState			= lp.myState;
status			= lp.status;
vtype			= lp.vtype;
pureLP                  = lp.pureLP;
}


void PicoLPInterface::updateProblem( BasicArray<Ereal<double> >& collb,
                        BasicArray<Ereal<double> >& colub,
                        BasicArray<Ereal<double> >& rowlb,
                        BasicArray<Ereal<double> >& rowub)
{
double inf = getInfinity();
for (int i=0; i<getNumCols(); i++) {
  if (collb[i] == Ereal<double>::negative_infinity)
     setColLower(i,-inf);
  else
     setColLower(i,collb[i]);
  if (colub[i] == Ereal<double>::positive_infinity)
     setColUpper(i,inf);
  else
     setColUpper(i,colub[i]);
  }
for (int i=0; i<getNumRows(); i++)  {
  double lb = (rowlb[i] == Ereal<double>::negative_infinity ? -inf : (double)rowlb[i]);
  double ub = (rowub[i] == Ereal<double>::positive_infinity ?  inf : (double)rowub[i]);
  setRowBounds(i,lb,ub);
  }
}

} // namespace pico
