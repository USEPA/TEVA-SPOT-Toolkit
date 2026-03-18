/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file SparseMatrix.h
 *
 * Defines the utilib::SparseMatrix, utilib::RMSparseMatrix and
 * CMSparseMatrix classes
 */

#ifndef utilib_SparseMatrix_h
#define utilib_SparseMatrix_h

#include <utilib/utilib_config.h>
#include <utilib/BitArray.h>
#include <utilib/CharString.h>
#include <utilib/NumArray.h>
#include <utilib/Ereal.h>
 
namespace utilib {

//--- CONSTANTS ---//

//--- TYPES ---//

template <class T>
class CMSparseMatrix; // Forward decl

template <class T>
class RMSparseMatrix; // Forward decl


/**
 * Compute the inner product between a NumArray and RMSparseMatrix.
 *
 * \note This is probably not numerically stable.
 */
template <class T>
inline void product(NumArray<T>& res, const RMSparseMatrix<T>& mat, 
						const NumArray<T>& vec)
{
res.resize(mat.get_nrows());

int nrows=mat.get_nrows();
for (int i=0; i<nrows; i++) {
  res[i] = 0.0;
  const T* val = &(mat.matval[mat.matbeg[i]]);
  const int*    col = &(mat.matind[mat.matbeg[i]]);
  for (int k=0; k<mat.matcnt[i]; k++, val++, col++) {
    res[i] += vec[*col] * (*val);
    }
  }
}


//---------------------------------------------------------------------------

/**
 * A kernel for sparse matrix classes.
 * Adapted from the sparse matrix classes developed by Kalyan S. Perumalla 
 * and Jeff T. Linderoth.
 *
 * Note: this code uses integer indeces.  This limits the total size of the
 * sparse matrix to a smaller size than would be possible if 'long integers'
 * were used for indexing, but this is compatable with CPLEX's use of 
 * sparse arrays.
 */
template <class T>
class SparseMatrix : public PackObject
{
public:

  ///
  void write(std::ostream& ostr) const = 0;

  ///
  void read(std::istream& istr) = 0;

  ///
  void write(PackBuffer& ostr) const;

  ///
  void read(UnPackBuffer& istr);

  /// Returns the number of columns in the matrix.
  int get_ncols( void ) const { return ncols; }

  /// Returns the number of rows in the matrix.
  int get_nrows( void ) const { return nrows; }

  /// Returns the number of non-zeros in the matrix.
  int get_nnzero( void ) const {return nnzeros;}

  /// Returns the \c matbeg array.
  NumArray<int>& get_matbeg( void ) { return matbeg; }

  /// Returns the \c matcnt array.
  NumArray<int>& get_matcnt( void ) { return matcnt; }

  /// Returns the \c matind array.
  NumArray<int>& get_matind( void ) { return matind; }

  /// Returns the \c matval array.
  NumArray<T>& get_matval( void ) { return matval; }

  /// Returns the value of the element at the \a row-th row and \a col-th column
  virtual T& operator()(const int row, const int col) = 0;

  /// Returns the value of the element at the \a row-th row and \a col-th column
  virtual const T& operator()(const int row, const int col) const = 0;

  /// Print out the matrix in a pretty format
  virtual int pretty_print(std::ostream& ostr) const;

  /// Delete the \a i-th row.
  virtual void delete_row(const int i) = 0;

  /// Delete the \a i-th column.
  virtual void delete_col(const int i) = 0;

  /// Delete the \a (row,col)-th element.
  virtual void delete_element(const int row, const int col) = 0;

protected:

  /// A protected constructor, which initializes elements in this class.
  SparseMatrix( void ) :
	ncols(0), nrows(0), nnzeros(0) {}

  /// Dummy destructor
  virtual ~SparseMatrix() {}

  /// Initialize a sparse matrix
  virtual void initialize(int nrow, int ncol, int nnzero) = 0;

  /// Initialize the memory of a sparse matrix
  void setup(int ncol, int nrow, int nnzero, int flag);

  /// The number of columns in the matrix
  int          ncols;

  /// The number of rows in the matrix
  int          nrows;

  /// The number of nonzeros in the matrix
  int	       nnzeros;

  /// Index where the \a i-th row/column begins in matval.
  NumArray<int>    matbeg;

  /// The number of values in the \a i-th row/column
  NumArray<int>    matcnt;

  /// matind[i] is the column/row number for the \a i-th value in row/column
  NumArray<int>    matind;

  /// The vector of values in the matrix
  NumArray<T> matval;

};



//---------------------------------------------------------------------------

/**
 * A column-major sparse matrix class.
 * Adapted from the CMatrix class of Kalyan S. Perumalla and Jeff T. Linderoth.
 */
template <class T>
class 
CMSparseMatrix : public SparseMatrix<T>
{
public:

  typedef SparseMatrix<T> base_t;
  #if !defined(SOLARIS_CC)
  using base_t::nrows;
  using base_t::ncols;
  using base_t::nnzeros;
  using base_t::matind;
  using base_t::matval;
  using base_t::matbeg;
  using base_t::matcnt;
  #endif

#if defined(SOLARIS_CC)
  friend RMSparseMatrix<T>;
#else
  template <class U> friend class RMSparseMatrix;
#endif

  /// Empty constructor.
  CMSparseMatrix( void ) {}

  /// Construct the matrix and resize the internal data.
  CMSparseMatrix( int nrow, int ncol, int nnzero )
		{initialize(nrow,ncol,nnzero);}

  /// Copy constructor
  CMSparseMatrix( CMSparseMatrix<Ereal<T> >& mat);
 
  /// Dummy virtual destructor
  ~CMSparseMatrix() {}

  /// Setup a SparseMatrix that will be filled in later.
  void initialize(int nrow, int ncol, int nnzero)
		{
		this->setup(nrow,ncol,nnzero,1);
		ncols=ncol;
		nrows=nrow;
		nnzeros=nnzero;
		}
  
  /// Returns the value of the element at the \a row-th row and \a col-th column
  T& operator()(const int row, const int col);

  /// Returns the value of the element at the \a row-th row and \a col-th column
  const T& operator()(const int row, const int col) const;

  /// Add a column to the matrix.
  int adjoinColumn(int count, int* rowPosition, T* value);

  ///
  void write(std::ostream& ostr) const;

  ///
  void read(std::istream& istr);

  ///
  void write(PackBuffer& ostr) const
		{ SparseMatrix<T>::write(ostr); }

  ///
  void read(UnPackBuffer& istr)
		{ SparseMatrix<T>::read(istr); }

  /// Convert a row-major matrix to a column-major matrix.
  void convert(const RMSparseMatrix<T>& rowmajor);

  /// Delete the \a i-th row.
  void delete_row(const int i);

  /// Delete the \a i-th column.
  void delete_col(const int i);

  /// Delete the \a (row,col)-th element.
  void delete_element(const int row, const int col);

};



//---------------------------------------------------------------------------

/**
 * A row-major sparse matrix class.
 * Adapted from the RMatrix class of Kalyan S. Perumalla and Jeff T. Linderoth.
 */
template <class T>
class 
RMSparseMatrix : public SparseMatrix<T>
{
public:

  typedef SparseMatrix<T> base_t;
  #if !defined(SOLARIS_CC)
  using base_t::nrows;
  using base_t::ncols;
  using base_t::nnzeros;
  using base_t::matind;
  using base_t::matval;
  using base_t::matbeg;
  using base_t::matcnt;
  #endif

#if defined(SOLARIS_CC)
  friend CMSparseMatrix<T>;
#else
  template <class U> friend class CMSparseMatrix;
#endif

#if !defined(UTILIB_NO_MEMBER_TEMPLATE_FRIENDS)
  friend void utilib::product<T>(NumArray<T>& res, 
				const RMSparseMatrix<T>& mat, 
				const NumArray<T>& vec);
#else
  friend void product<T>(NumArray<T>& res, 
				const RMSparseMatrix<T>& mat, 
				const NumArray<T>& vec);
#endif

  /// Empty constructor.
  RMSparseMatrix( void ) {}

  /// Construct the matrix and resize the internal data.
  RMSparseMatrix( int nrow, int ncol, int nnzero )
		{initialize(nrow,ncol,nnzero);}

  /// Dummy virtual destructor
  virtual ~RMSparseMatrix() {}

  /// Setup a SparseMatrix that will be filled in later.
  void initialize(int nrow, int ncol, int nnzero)
		{
		this->setup(nrow,ncol,nnzero,0);
		ncols=ncol;
		nrows=nrow;
		nnzeros=nnzero;
		}

  /// Resize the \a rowndx-th row to length \a rowlen.
  void resize(const int rowndx, const int rowlen);

  /// Returns the value of the element at the \a row-th row and \a col-th column
  T& operator()(const int row, const int col);

  /// Returns the value of the element at the \a row-th row and \a col-th column
  const T& operator()(const int row, const int col) const;

  /// Add a row to the matrix.
  int adjoinRow(int count, int* colPosition, T* value);

  ///
  void write(std::ostream& ostr) const;

  ///
  void read(std::istream& istr);

  ///
  void write(PackBuffer& ostr) const
		{ SparseMatrix<T>::write(ostr); }

  ///
  void read(UnPackBuffer& istr)
		{ SparseMatrix<T>::read(istr); }

  /// Convert a column-major matrix to a row-major matrix.
  void convert(const CMSparseMatrix<T>& colmajor);

  /// Delete the \a i-th row.
  void delete_row(const int i);

  /// Delete the \a i-th column.
  void delete_col(const int i);

  /// Delete the \a (row,col)-th element.
  void delete_element(const int row, const int col);
};




//
// This just sets up the memory, but does not modify the ncols, nrows or
// nnzeros values
//
template <class T>
void SparseMatrix<T>::setup(int nrow, int ncol, int nnzero, int flag)
{
matind.resize(nnzero);
matval.resize(nnzero);
if (flag) {
   matbeg.resize(ncol);
   matcnt.resize(ncol);
   }
else {
   matbeg.resize(nrow);
   matcnt.resize(nrow);
   }
}


template <class T>
CMSparseMatrix<T>::CMSparseMatrix( CMSparseMatrix<Ereal<T> >& mat)
{
initialize(mat.get_nrows(), mat.get_ncols(), mat.get_nnzero());
matind << mat.get_matind();
matval << mat.get_matval();
matbeg << mat.get_matbeg();
matcnt << mat.get_matcnt();
}


template <class T>
T& CMSparseMatrix<T>::operator()(const int row, const int col)
{
if ((row < 0) || (row >= nrows) || (col < 0) || (col >= ncols))
   EXCEPTION_MNGR(runtime_error, "CMSparseMatrix<T>::operator : iterator out of range. " << row << "x" << col << " not in " << nrows << "x" << ncols)

int pt = matbeg[col];
for (int i=0; i<matcnt[col]; i++)
  if (matind[pt+i] == row)
     return matval[pt+i];

return matval[0];
}


template <class T>
const T& CMSparseMatrix<T>::operator()(const int row, const int col) const
{
if ((row < 0) || (row >= nrows) || (col < 0) || (col >= ncols))
   EXCEPTION_MNGR(runtime_error, "CMSparseMatrix<T>::operator : iterator out of range. " << row << "x" << col << " not in " << nrows << "x" << ncols)

int pt = matbeg[col];
for (int i=0; i<matcnt[col]; i++)
  if (matind[pt+i] == row)
     return matval[pt+i];

return matval[0];
}


template <class T>
T& RMSparseMatrix<T>::operator()(const int row, const int col)
{
if ((row < 0) || (row >= nrows) || (col < 0) || (col >= ncols))
   EXCEPTION_MNGR(runtime_error, "RMSparseMatrix<T>::operator : iterator out of range. " << row << "x" << col << " not in " << nrows << "x" << ncols)

int pt = matbeg[row];
for (int i=0; i<matcnt[row]; i++)
  if (matind[pt+i] == col)
     return matval[pt+i];

return matval[0];
}


template <class T>
const T& RMSparseMatrix<T>::operator()(const int row, const int col) const
{
if ((row < 0) || (row >= nrows) || (col < 0) || (col >= ncols))
   EXCEPTION_MNGR(runtime_error, "RMSparseMatrix<T>::operator : iterator out of range. " << row << "x" << col << " not in " << nrows << "x" << ncols)

int pt = matbeg[row];
for (int i=0; i<matcnt[row]; i++)
  if (matind[pt+i] == col)
     return matval[pt+i];

return matval[0];
}


template <class T>
int SparseMatrix<T>::pretty_print(std::ostream& ostr) const
{
for (int i=0; i<nrows; i++) {
  for (int j=0; j<ncols; j++)
    ostr << operator()(i,j) << " ";
  ostr << std::endl;
  }
return OK;
}


template <class T>
void CMSparseMatrix<T>::write(std::ostream& ostr) const 
{
ostr << nrows << " " << ncols << " " << nnzeros << ":" << std::endl;
int ndx=0;
for (int i=0; i<ncols; i++)
  for (int j=0; j<matcnt[i]; j++) {
    ostr << " (" << matind[ndx] << "," << i << ") " << matval[ndx] << std::endl;
    ndx++;
    }
}


template <class T>
void CMSparseMatrix<T>::read(std::istream& istr)
{
EXCEPTION_MNGR(runtime_error, "CMSparseMatrix<T>::read - not implemented!")
}


template <class T>
void SparseMatrix<T>::write(PackBuffer& ostr) const 
{
ostr << nrows << ncols << nnzeros;
ostr << matbeg << matcnt << matind << matval;
}


template <class T>
void SparseMatrix<T>::read(UnPackBuffer& istr)
{
int ntmp, ctmp, ztmp;
istr >> ntmp >> ctmp >> ztmp;
initialize(ntmp,ctmp,ztmp);
istr >> matbeg >> matcnt >> matind >> matval;
}


template <class T>
void RMSparseMatrix<T>::write(std::ostream& ostr) const 
{
ostr << nrows << " " << ncols << " " << nnzeros << ":" << std::endl;
int ndx=0;
for (int i=0; i<nrows; i++)
  for (int j=0; j<matcnt[i]; j++) {
    ostr << " (" << i << "," << matind[ndx] << ") " << matval[ndx] << std::endl;
    ndx++;
    }
}


template <class T>
void RMSparseMatrix<T>::read(std::istream& istr)
{
EXCEPTION_MNGR(runtime_error, "RMSparseMatrix<T>::read - not implemented!")
}



template <class T>
void RMSparseMatrix<T>::delete_row(const int i)
{
//
// Delete all of the elements in the row
//
while (matcnt[i] > 0)
  delete_element(i,matind[matbeg[i]]);
//
// Delete the row itself
//
for (int j=i; j<(nrows-1); j++) {
  matbeg[j] = matbeg[j+1];
  matcnt[j] = matcnt[j+1];
  }
nrows--;
}


template <class T>
void RMSparseMatrix<T>::delete_col(const int i)
{
//
// Delete all of the elements in the column
//
for (int j=0; j<nrows; j++) {
  //
  // Look for column i in row j
  //
  int k=0;
  while (k < matcnt[j]) {
    if (matind[matbeg[j]+k] == i)
       delete_element(j,i);
    else {
       if (matind[matbeg[j]+k] > i)
          matind[matbeg[j]+k]--;
       k++;
       }
    }
  }
//
// Delete the column itself (the column indeces are decremented above!)
//
ncols--;
}


template <class T>
void RMSparseMatrix<T>::delete_element(const int rowndx, const int colndx)
{
int k=0;
for (; k<matcnt[rowndx]; k++)
  if (matind[matbeg[rowndx]+k] == colndx) {
     break;
     }
if (k == matcnt[rowndx]) return;

for (int i=matbeg[rowndx]+k; i<(nnzeros-1); i++) {
  matind[i] = matind[i+1];
  matval[i] = matval[i+i];
  }
matcnt[rowndx]--;
for (int j=rowndx+1; j<nrows; j++)
  matbeg[j]--;
nnzeros--;
}


template <class T>
void RMSparseMatrix<T>::resize(const int rowndx, const int rowlen)
{
if (rowlen == matcnt[rowndx])
   return;
else if (rowlen > matcnt[rowndx]) {
   // TODO - resize the array up!
   }
else {
   int diff = rowlen-matcnt[rowndx];

   if (diff < 0) {
      for (int i=matbeg[rowndx]+rowlen; i<(nnzeros+diff); i++) {
        matind[i] = matind[i-diff];
        matval[i] = matval[i-diff];
        }
      }
   matcnt[rowndx] += diff;
   for (int j=rowndx+1; j<nrows; j++)
     matbeg[j] += diff;
   nnzeros += diff;
   }
}


template <class T>
void CMSparseMatrix<T>::convert(const RMSparseMatrix<T>& rowmajor)
{
matval.resize(rowmajor.nnzeros);
matind.resize(rowmajor.nnzeros);

ncols = rowmajor.ncols;
nrows = rowmajor.nrows;
nnzeros = rowmajor.nnzeros;
matcnt.resize(ncols);
matcnt << 0;
matbeg.resize(ncols);

NumArray<int> rowndx(nrows);
rowndx << 0;
int prev_cndx=0;

for (int i=0; i<nnzeros; i++) {
  //
  // Set dummy values for cndx and rndx
  //
  int cndx=ncols;
  int rndx=-1;
  //
  // Find the point in the matrix with the next highest column index
  //
  for (int j=0; j<nrows; j++) {
    if ((rowmajor.matcnt[j] > 0) && (rowmajor.matcnt[j] > rowndx[j])) {
       if (rowmajor.matind[rowmajor.matbeg[j]+rowndx[j]] < cndx) {
          cndx = rowmajor.matind[rowmajor.matbeg[j]+rowndx[j]];
          rndx = j;
          }
       }
    if (cndx == prev_cndx)	// Can stop early because we're in the same col
       break;
    }
  //
  // Add this new point to the CM matrix
  //
  if (matcnt[cndx] == 0)
     matbeg[cndx] = i;
  matcnt[cndx]++;
  matind[i] = rndx;
  matval[i] = rowmajor.matval[rowmajor.matbeg[rndx]+rowndx[rndx]];

  rowndx[rndx]++;
  prev_cndx=cndx;
  }
}


template <class T>
void RMSparseMatrix<T>::convert(const CMSparseMatrix<T>& colmajor)
{
matval.resize(colmajor.nnzeros);
matind.resize(colmajor.nnzeros);

ncols = colmajor.ncols;
nrows = colmajor.nrows;
nnzeros = colmajor.nnzeros;
matcnt.resize(nrows);
matcnt << 0;
matbeg.resize(nrows);

NumArray<int> colndx(ncols);
colndx << 0;
int prev_rndx=0;

for (int i=0; i<nnzeros; i++) {
  //
  // Find the point in the matrix with the next highest row index
  //
  int cndx=-1;
  int rndx=nrows;
  for (int j=0; j<ncols; j++) {
    if ((colmajor.matcnt[j] > 0) && (colmajor.matcnt[j] > colndx[j])) {
       if (colmajor.matind[colmajor.matbeg[j]+colndx[j]] < rndx) {
          cndx = j;
          rndx = colmajor.matind[colmajor.matbeg[j]+colndx[j]];
          }
       }
    if (prev_rndx == rndx)	// Can stop because we are on the same row
       break;
    }
  
  //
  // Add this new point to the RM matrix
  //
  if (matcnt[rndx] == 0)
     matbeg[rndx] = i;
  matcnt[rndx]++;
  matind[i] = cndx;
  matval[i] = colmajor.matval[colmajor.matbeg[cndx]+colndx[cndx]];

  colndx[cndx]++;
  prev_rndx=rndx;
  }
}



template <class T>
int CMSparseMatrix<T>::adjoinColumn(int count, int* rowPosition, T* value)
{
if (ncols == (int) matcnt.size()) {
   matcnt.resize(ncols+32);
   matbeg.resize(ncols+32);
   }

if (nnzeros + count > (int) matval.size()) {
   matval.resize(nnzeros+std::max(count,64));
   matind.resize(nnzeros+std::max(count,64));
   }

matbeg[ncols] = nnzeros;
matcnt[ncols++] = count;
for (int i=0; i<count; i++) {
  matind[nnzeros]   = rowPosition[i];
  // If we're building from scratch, number of rows is the highest row
  // number seen so far (+1 since 0 based)
  if (rowPosition[i] >= nrows)
    nrows = rowPosition[i] + 1;
  matval[nnzeros++] = value[i];
  }

return nnzeros;
}


template <class T>
void CMSparseMatrix<T>::delete_col(const int i)
{
//
// Delete all of the elements in the col 
//
while (matcnt[i])
  delete_element(matind[matbeg[i]],i);
//
// Delete the col itself
//
for (int j=i; j<(ncols-1); j++) {
  matbeg[j] = matbeg[j+1];
  matcnt[j] = matcnt[j+1];
  }
ncols--;
}


template <class T>
void CMSparseMatrix<T>::delete_row(const int i)
{
//
// Delete all of the elements in the column
//
for (int j=0; j<ncols; j++) {
  //
  // Look for row i in row j
  //
  int k=0;
  while (k < matcnt[j]) {
    if (matind[matbeg[j]+k] == i)
       delete_element(i,j);
    else {
       if (matind[matbeg[j]+k] > i)
          matind[matbeg[j]+k]--;
       k++;
       }
    }
  }
//
// Delete the row itself (the row indeces are decremented above!)
//
nrows--;
}


template <class T>
void CMSparseMatrix<T>::delete_element(const int rowndx, const int colndx)
{
int k=0;
for (; k<matcnt[colndx]; k++)
  if (matind[matbeg[colndx]+k] == rowndx) {
     break;
     }
if (k == matcnt[colndx]) return;

for (int i=matbeg[colndx]+k; i<(nnzeros-1); i++) {
  matind[i] = matind[i+1];
  matval[i] = matval[i+1];
  }
matcnt[colndx]--;
for (int j=colndx+1; j<ncols; j++)
  matbeg[j]--;
nnzeros--;
}



template <class T>
int RMSparseMatrix<T>::adjoinRow(int count, int* colPosition, T* value)
{
if (nrows == (int) matcnt.size()) {
   matcnt.resize(nrows+32);
   matbeg.resize(nrows+32);
   }

if (nnzeros + count > (int) matval.size()) {
   matval.resize(nnzeros+std::max(count,64));
   matind.resize(nnzeros+std::max(count,64));
   }

matbeg[nrows] = nnzeros;
matcnt[nrows++] = count;
for (int i=0; i<count; i++) {
  matind[nnzeros]   = colPosition[i];
  matval[nnzeros++] = value[i];
  }

return nnzeros;
}

} // namespace utilib

#endif
