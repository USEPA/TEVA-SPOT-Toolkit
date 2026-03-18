/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// read_glpk.cpp
//


#include <acro_config.h>
#define _GLPLPX_UNLOCK

#if defined(ACRO_USING_GLPK)
extern "C" {
#include "glplpx.h"
};
#endif

#include <utilib/sort.h>
#include <utilib/NumArray.h>
#include <pebbl/fundamentals.h>
#include <pico/ilp_read.h>
#include <pico/PicoLPInterface.h>

using namespace std;

namespace pico {


void read_glpk(const char* filename1, const char* filename2, short verbose,
			CMSparseMatrix<Ereal<double> >& matrix,
			CharString& probname,
                        int& objsen,
                        BasicArray<Ereal<double> >& objCoefs,
                        BasicArray<Ereal<double> >& lowerBounds,
                        BasicArray<Ereal<double> >& upperBounds,
                        BasicArray<CharString>&  variableNames,
                        CharString&  constraintTypes,
                        BasicArray<Ereal<double> >& constraintLHS,
                        BasicArray<Ereal<double> >& constraintRHS,
                        BasicArray<CharString>& constraintNames,
			BasicArray<int>& listOfIntegers,
			ilp_format format)
{
#if defined(ACRO_USING_GLPK)
LPX* lp=0;


if (format == glpk_ampl_format)
   lp = lpx_read_model(const_cast<char*>(filename1), const_cast<char*>(filename2), (char*)0);


else if (format == glpk_lp_format)
   lp = lpx_read_cpxlp(const_cast<char*>(filename1));

else if (format == glpk_mps_format)
   lp = lpx_read_mps(const_cast<char*>(filename1));

if (!lp) 
   EXCEPTION_MNGR(runtime_error,"read_glpk - read failed.")


if (lp->name)
   probname = get_str(lp->str_buf, lp->name);
else
   probname = "unknown";

if (lp->dir == LPX_MAX)
   objsen = -1; // maximize
else
   objsen =  1; // minimize;

objCoefs.resize(lp->n);
lowerBounds.resize(lp->n);
upperBounds.resize(lp->n);
variableNames.resize(lp->n);

int ncols = lpx_get_num_cols(lp);
int nrows = lpx_get_num_rows(lp);
lowerBounds << Ereal<double>::negative_infinity;
upperBounds << Ereal<double>::positive_infinity;
for (int i=1; i<=ncols; i++) {
  variableNames[i-1] = get_str(lp->str_buf,lp->col[i]->name);
  objCoefs[i-1] = lp->col[i]->coef;
  if ( (lp->col[i]->type == LPX_LO) ||
       (lp->col[i]->type == LPX_DB) ||
       (lp->col[i]->type == LPX_FX) )
     lowerBounds[i-1] = lp->col[i]->lb;
  if ( (lp->col[i]->type == LPX_UP) ||
       (lp->col[i]->type == LPX_DB) ||
       (lp->col[i]->type == LPX_FX) )
     upperBounds[i-1] = lp->col[i]->ub;
  }


{
int num_ints=0;
for (int i=1; i<=ncols; i++)
  if (lp->col[i]->kind == LPX_IV)
     num_ints++;
listOfIntegers.resize(num_ints);
int j=0;
for (int i=1; i<=ncols; i++)
  if (lp->col[i]->kind == LPX_IV)
     listOfIntegers[j++] = i-1;
}

constraintTypes.resize(nrows);
constraintLHS.resize(nrows);
constraintRHS.resize(nrows);
constraintNames.resize(nrows);

constraintLHS << 0.0;
constraintRHS << Ereal<double>::positive_infinity;
for (int i=0; i<nrows; i++) {

  if (lp->row[i+1]->type == LPX_FR) {
     constraintLHS[i] = Ereal<double>::negative_infinity;
     constraintRHS[i] = Ereal<double>::positive_infinity;
     constraintTypes[i] = PicoLPInterface::notRestricted;
     }
  else if (lp->row[i+1]->type == LPX_LO) {
     constraintLHS[i] = lp->row[i+1]->lb;
     constraintRHS[i] = Ereal<double>::positive_infinity;
     constraintTypes[i] = PicoLPInterface::greaterOrEqual;
     }
  else if (lp->row[i+1]->type == LPX_UP) {
     constraintLHS[i] = Ereal<double>::negative_infinity;
     constraintRHS[i] = lp->row[i+1]->ub;
     constraintTypes[i] = PicoLPInterface::lessOrEqual;
     }
  else if (lp->row[i+1]->type == LPX_DB) {
     constraintLHS[i] = lp->row[i+1]->lb;
     constraintRHS[i] = lp->row[i+1]->ub;
     constraintTypes[i] = PicoLPInterface::ranged;
     }
  else if (lp->row[i+1]->type == LPX_FX) {
     constraintLHS[i] = lp->row[i+1]->lb;
     constraintRHS[i] = lp->row[i+1]->ub;
     constraintTypes[i] = PicoLPInterface::equal;
     }

  constraintNames[i] = get_str(lp->str_buf, lp->row[i+1]->name);
  }

//matrix.initialize(nrows,ncols,lpx_get_num_nz(lp) - lpx_get_mat_row(lp,1,0,0) );
matrix.initialize(nrows,ncols,lpx_get_num_nz(lp));

 // Adding one here because glpk matrices are 1-based rather than 0-based.
 // glpk won't use 0th entry for returing values.
 IntVector tmpi(nrows + 1);
 tmpi[0] = 0;
BasicArray<double> tmpd(nrows + 1);

// We don't assume the row indices (for the column) are sorted by the
// glpk reader.  This is an auxiliary array used to sort them.
 IntVector ndx(nrows+1);

 int this_num_nonzero;

int matbeg=0;
 bool needs_sorting = false;
 int k;

for (int i=0; i<ncols; i++) {
  matrix.get_matbeg()[i] = matbeg;
  // Unfortunately, the sorting requires the size be exact.  Thus the
  // resize to hold the maximum amount (we don't know the size ahead of time)
  if (needs_sorting) // size changed last time through
    tmpi.resize(nrows + 1);
  this_num_nonzero = lpx_get_mat_col(lp,i+1, tmpi.data(),tmpd.data());
  matrix.get_matcnt()[i] = this_num_nonzero;

  // See if we need to sort.  If not, and this is likely the case, then
  // we can avoid repeated resizing, calling sort, etc.  Might want to
  // make this kind of test part of utilib.

  needs_sorting = false;
  k = 0;
  for (size_type p = 1; p <= (size_type) this_num_nonzero; p++)
    {
      if (tmpi[p] < tmpi[p-1])
	{
	  needs_sorting = true;
	  break;
	}
    }

  // Remember 0th entry is essentially unused by glpk.  Since indices
  // are positive, the 0 entry (made such) will always be first after
  // sorting.
  if (needs_sorting)
    {
      tmpi.resize(this_num_nonzero + 1);
      order(ndx, tmpi);
      for (int j=1; j<= this_num_nonzero; j++) {
	matrix.get_matind()[matbeg+k] = tmpi[ndx[j]]-1;
	matrix.get_matval()[matbeg+k] = tmpd[ndx[j]];
	k++;
      }
    }

  else // no sorting.  Just straight transfer without indirection
    for (int j=1; j<= this_num_nonzero; j++) {
    matrix.get_matind()[matbeg+k] = tmpi[j]-1;
    matrix.get_matval()[matbeg+k] = tmpd[j];
    k++;
    }
  matbeg += this_num_nonzero;
  }

//    cerr << matrix.get_matind() << endl;
//  cerr << matrix.get_matval() << endl;
//  cerr << matrix.get_matbeg() << endl;

//cerr << matrix << endl;
#endif
}


} // namespace pico
