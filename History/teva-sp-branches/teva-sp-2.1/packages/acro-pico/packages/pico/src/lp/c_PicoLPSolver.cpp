/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file c_PicoLPSolver.cpp
 *
 * Provides a C interface to the PicoLPSolver class.
 **/
//

#include <acro_config.h>
#include <utilib/CommonIO.h>
#include <pico/PicoLPSolver.h>

using pico::PicoLPSolver;
using utilib::CMSparseMatrix;
using utilib::Ereal;
using utilib::BasicArray;

using namespace utilib;
using namespace std;

namespace c_pico {

PicoLPSolver lp;

};

using c_pico::lp;

/* Need to solve min Cx st Ax (<,>,=) b, lb < x < ub,
   rows        rows in a, num of constraints
   cols        cols in a, num of vars
   anz         number of nonzero elements in a
   ar(anz)     row indices for a
   ac(anz)     col indices for a
   a(anz)      coef. values for a
NOTE, A goes row by row, so 
  A =  [  11 12  0
          14  0 16
           0 18 19 ]
  ar =  1  1  2  2  3  3
  ac =  1  2  1  3  2  3
   a = 11 12 14 16 18 19
 anz = 6
          
   zobj(cols)  objective function coeficients, c vals
   zrhs(rows)  rhs of constraints
   zlb(cols)   lowrbounds on z
   zub(cols)   upperbouns on z
   sense(rows) char array, L,  or E ONLY
   x(cols)     returned value for variables...
   lag(rows)   returned lagrange multipliers for constraints + variables
   inform      returned flag for feas / infeas
   NOTE, lagrange currently reported for constraints, not variables.

*/
extern "C"
int c_pico_solve_lp(char* solver,
	          int rows, int cols, int anz, int *ar, int *ac, double *a,
                  double *zobj, double *zrhs, double *zlb, double *zub,
                  char *sense, double *x, double *objv, int *inform, 
		  double *lag)
{
//
// Initialize the LP solver
//
if (strcmp(solver,"cplex-primal") == 0) {
   lp.init(PicoLPSolver::cplex);
   lp()->setLPMethod(pico::PicoLPInterface::primalSimplex);
   }
else if (strcmp(solver,"cplex-dual") == 0) {
   lp.init(PicoLPSolver::cplex);
   lp()->setLPMethod(pico::PicoLPInterface::dualSimplex);
   }
else if (strcmp(solver,"soplex-primal") == 0) {
   lp.init(PicoLPSolver::soplex);
   lp()->setLPMethod(pico::PicoLPInterface::primalSimplex);
   }
else if (strcmp(solver,"soplex-dual") == 0) {
   lp.init(PicoLPSolver::soplex);
   lp()->setLPMethod(pico::PicoLPInterface::dualSimplex);
   }
else if (strcmp(solver,"clp-primal") == 0) {
   lp.init(PicoLPSolver::clp);
   lp()->setLPMethod(pico::PicoLPInterface::primalSimplex);
   }
else if (strcmp(solver,"clp-dual") == 0) {
   lp.init(PicoLPSolver::clp);
   lp()->setLPMethod(pico::PicoLPInterface::dualSimplex);
   }
else {
   lp.init(PicoLPSolver::defaultLP);
   lp()->setLPMethod(pico::PicoLPInterface::primalSimplex);
   }
lp()->initialize();
//
// Setup auxillary arrays
//
BasicArray<Ereal<double> > obj(cols);
BasicArray<Ereal<double> > collb(cols);
BasicArray<Ereal<double> > colub(cols);
BasicArray<Ereal<double> > rowlb(rows);
BasicArray<Ereal<double> > rowub(rows);
for (int i=0; i<rows; i++) {
  if (sense[i] == 'E')
     rowlb[i] = zrhs[i];
  else
     rowlb[i] = Ereal<double>::negative_infinity;
  rowub[i] = zrhs[i];
  obj[i] = zobj[i];
  collb[i] = zlb[i];
  colub[i] = zub[i];
  }
//
// Convert the row-major data into the column-major format required
// by PICO.
// Note: this could probably be done more efficiently
//
CMSparseMatrix<Ereal<double> > matrix( rows, cols, anz );
{
BasicArray<int>& matcnt = matrix.get_matcnt();
BasicArray<int>& matind = matrix.get_matind();
BasicArray<int>& matbeg = matrix.get_matbeg();
BasicArray<Ereal<double> >& matval = matrix.get_matval();
matcnt << 0;

int ctr=0;
BasicArray<int> ndx(anz);
for (int i=0; i<cols; i++) {
  //
  // Collect information for the current column
  //
  int num=0;
  for (int j=0; j<anz; j++) {
    if (ac[j] == (i+1))
       ndx[num++] = j;
    }
  //
  // Add the columns to the CMSparseMatrix
  //
  int prev_min_row = -1;
  for (int j=0; j<num; j++) {
    int cndx = -1;
    for (int k=0; k<num; k++)
      if (prev_min_row == -1) {
	 if ((cndx == -1) || (ar[ndx[k]] < ar[cndx]))
	    cndx = ndx[k];
         }
      else {
	 if ((ar[ndx[k]] > ar[prev_min_row]) && ((cndx == -1) || (ar[ndx[k]] < ar[cndx])))
	    cndx = ndx[k];
         }
    if (cndx == -1)
       EXCEPTION_MNGR(runtime_error, "Problems converting an LP problem.")
    //
    // Add this new point to the CM matrix
    //
    if (matcnt[i] == 0)
       matbeg[i] = ctr;
    matcnt[i]++;
    matind[ctr] = ar[cndx]-1;
    matval[ctr] = a[cndx];
    ctr++;
    prev_min_row = cndx;
    }
  }
}
//
// Load the problem
//
ucout << matrix << endl;
ucout << "collb " << collb << endl;
ucout << "colub " << colub << endl;
ucout << "obj " << obj << endl;
ucout << "rowlb " << rowlb << endl;
ucout << "rowub " << rowub << endl;
lp() -> loadProblem( matrix, collb, colub, obj, rowlb, rowub);


//
// Run the LP
//
lp() -> reset();
lp() -> solve();

//
// Collect information from the solver
//

//
// Need clarification, but something like the following should work
//
if ( lp() -> isProvenDualUnbounded() )
   *inform = 1;
else
   *inform = 0;
//
// Get the value of the solution
//
*objv = lp() -> getObjValue();
//
// Get the solution vector
//
lp() -> getColSolution(x);
//
// Get the row prices
//
lp() -> getRowPrice(lag);

return 0;
}
