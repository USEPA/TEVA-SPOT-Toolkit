/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// read_ampl.cpp
//

//
// Lauren, Elizabeth  -- uncomment the following to enable the proper
//			compilation of this code.
using namespace std;

#include <acro_config.h>
#define TESTING_AMPL


#include <utilib/sort.h>
#include <pebbl/fundamentals.h>
#include <pico/ilp_read.h>
#include <pico/PicoLPInterface.h>

#undef NO
#if defined(ACRO_USING_AMPL ) && defined(TESTING_AMPL)
extern "C" {
#include "asl.h"
};

ASL*  asl = 0;
#endif

namespace pico {
  
  
void asl_init()
{
#if defined(ACRO_USING_AMPL ) && defined(TESTING_AMPL)
if (!asl)
   asl = ASL_alloc(ASL_read_f);
#endif
}



void read_ampl(const char* filename1, short verbose,
		 CMSparseMatrix<Ereal<double> >& matrix,
		 CharString& probname,
		 int& objsen,
		 BasicArray<Ereal<double> >& objCoefs,
		 BasicArray<Ereal<double> >& lowerBounds,
		 BasicArray<Ereal<double> >& upperBounds,
		 BasicArray<CharString>&  variableNames,
		 double& objConst, 
		 CharString&  constraintTypes,
		 BasicArray<Ereal<double> >& constraintLHS,
		 BasicArray<Ereal<double> >& constraintRHS,
		 BasicArray<CharString>& constraintNames,
		 BasicArray<int>& listOfIntegers,
		 ilp_format format)
{
#if defined(ACRO_USING_AMPL ) && defined(TESTING_AMPL)
    
///
/// Read in *.NL file
///
    
if (filename1 == NULL)
   EXCEPTION_MNGR(runtime_error, "error: no stub passed to init_ampl()"); 
    
asl_init();

char* s; //for variable and constraint names
FILE* nl = jac0dim(const_cast<char*>(filename1), (fint) strlen(filename1));
 
BasicArray<real> A_vals_data(nzc);
A_vals = A_vals_data.data();
int status = f_read(nl,0);
if (verbose)
   cout << "Read in AMPL components (status = " << status << ")\n\n";
///  
/// Get problem name
///
probname = obj_name(0);
///
/// Get LP sense
///
if (objtype[0]==1)
   objsen = -1; //maximize
else if (objtype[0]==0)
   objsen = 1;  //minimize
///
/// Get number of rows and columns
///
int ncols = n_var;         //number of variables
int nrows = n_obj + n_con; //number of objectives and constraints
if (verbose)
   cout << "Number of columns is " << ncols << ", number of rows is " << nrows << ", and number of nonzeros is " << nzc << "." << endl;
///
/// Do Some Error Checking
///
if (n_obj != 1){
   EXCEPTION_MNGR(runtime_error,"Problem given has " << n_obj << " objective functions,\n and PICO solves ip or lp with only 1 objective function.");
   }
int num_nonLinVars = max(nlvc, nlvo);
if (num_nonLinVars != 0){
   EXCEPTION_MNGR(runtime_error,"Problem given has " << num_nonLinVars << " nonlinear variables,\n and PICO solves only linear or integer programming problems.\n");
   }
///
/// Resize various arrays
///
objCoefs.resize(ncols);
lowerBounds.resize(ncols);
upperBounds.resize(ncols);
variableNames.resize(ncols);
constraintTypes.resize(nrows-1);
constraintLHS.resize(nrows-1);
constraintRHS.resize(nrows-1);
constraintNames.resize(nrows-1);
///    
/// Get variable and constraint names
///
int rlen = maxrownamelen; //length of longest constraint or objective name
int clen = maxrownamelen; //length of longest variable name
if (rlen == 0 || clen == 0) {
   if (verbose) {
      cout << "To get .row and .col files, in your AMPL session say" << endl;
      s = getenv("solver");
      if (s)
         cout << "\toption " << s << "_auxfiles rc;\n" << s <<
	       "before saying\n\tsolve;\n\twrite ...\n";
      cout << "\toption auxfiles rc;\nbefore saying\n\twrite ...\n";
      }
   if (rlen == 0)
      rlen = strlen(con_name(n_con-1));
   if (clen == 0)
      clen = strlen(var_name(n_var-1));
   }
///
/// Initialize lower and upper bounds on variables
/// Default values are infinity
///
lowerBounds << 0.0;
upperBounds << Ereal<double>::positive_infinity;
for (int i = 0;i < ncols; i++) {
  //
  //var_name is set by AMPL
  //
  variableNames[i] = var_name(i);
  //
  //initialize objective coefficients to 0
  //
  objCoefs[i] = 0;
  //
  // LUv is set by AMPL
  //
  if (LUv[2*i] != negInfinity)
     lowerBounds[i] = LUv[2*i];
  if (LUv[2*i+1] != Infinity)
     upperBounds[i] = LUv[2*i+1];
  }
///
/// Get objective coefficients
///
ograd* og;
for (og = Ograd[0]; og; og = og->next) //fill coefficients in objCoefs
  objCoefs[og->varno] = og->coef; 
objConst = objconst(0);
///
/// Setup the list of integer variables
///
int num_ints = nbv + niv;      //binary and other integer variables
listOfIntegers.resize(num_ints);
int j=0;
for (int i= n_var -nbv -niv; i< n_var; i++) //if nonlinear variables don't exist
  listOfIntegers[j++] = i; 
///    
/// Initialize the constraint lower and upper bounds (per row)
///
constraintLHS << 0.0;
constraintRHS << Ereal<double>::positive_infinity;
for (int i=0; i<nrows-1; i++) {
      
  //cerr << "HERE " << i << " " << LUrhs[2*i] << " " << LUrhs[2*i+1] << endl;

  if (LUrhs[2*i] > negInfinity && LUrhs[2*i+1] == Infinity) {
     //cerr << "GOE" << endl;
     constraintLHS[i] = LUrhs[2*i];                       //LUrhs is set    
     constraintRHS[i] = Ereal<double>::positive_infinity; //by AMPL
     constraintTypes[i] = PicoLPInterface::greaterOrEqual;
  }
  else if (LUrhs[2*i] == negInfinity && LUrhs[2*i+1] < Infinity) {
     //cerr << "LOE" << endl;
     constraintLHS[i] = Ereal<double>::negative_infinity;
     constraintRHS[i] = LUrhs[2*i+1];
     constraintTypes[i] = PicoLPInterface::lessOrEqual;
  }
  else if (LUrhs[2*i] > negInfinity && LUrhs[2*i+1] < Infinity) {
     //cerr << "Range" << endl;
     constraintLHS[i] = LUrhs[2*i];
     constraintRHS[i] = LUrhs[2*i+1];
     constraintTypes[i] = PicoLPInterface::ranged;
  }
  else if (LUrhs[2*i] == LUrhs[2*i+1]) {
     //cerr << "Equal" << endl;
     constraintLHS[i] = LUrhs[2*i];
     constraintRHS[i] = LUrhs[2*i+1];
     constraintTypes[i] = PicoLPInterface::equal;
  }
  else {
     EXCEPTION_MNGR(runtime_error, "Unknown constraint type from AMPL")
     }
      
  constraintNames[i] = con_name(i);   //con_name is set by AMPL
  }
///
/// Setup constraint matrix
///
matrix.initialize(nrows-1, ncols, nzc); //nzc is set by AMPL
for (int i = 0; i<ncols; i++) {
  matrix.get_matbeg()[i] = A_colstarts[i];
  if (verbose)
     cout << "matbeg[" << i << "] = " << matrix.get_matbeg()[i] << endl;    
  matrix.get_matcnt()[i] = A_colstarts[i+1] - A_colstarts[i];
  if (verbose)
     cout << "matcnt[" << i << "] = " << A_colstarts[i+1] - A_colstarts[i]
		<< " ?= " << matrix.get_matcnt()[i] << endl;
  for (int f = A_colstarts[i]; f < A_colstarts[i+1]; f++) {
    matrix.get_matind()[f] = A_rownos[f];
    //cerr << "matind[" << f << "] = " << matrix.get_matind()[f] << endl;
    matrix.get_matval()[f] = A_vals[f];
    //cerr << "matval[" << f << "] = " << matrix.get_matval()[f] << endl;
    }    
      
  //cerr << matrix.get_matind() << endl;
  //cerr << matrix.get_matval() << endl;
  //cerr << matrix.get_matbeg() << endl << endl;
  }
        
  //cerr << matrix << endl;
    
if (verbose)
   cout << "Finished reading in AMPL\n\n";
    
#endif
}

#if 0

/// Resize the constraint matrix
matrix.initialize(nrows-1,ncols,lpx_get_num_nz(lp) - lpx_get_mat_row(lp,1,0,0) );

/// Temporary arrays used to extract data from the GLPK object
BasicArray<int>    tmpi(nrows+1);
BasicArray<int>    ndx(nrows);
BasicArray<double> tmpd(nrows+1);

/// Fill temporary arrays
int matbeg=0;
for (int i=1; i<=ncols; i++) {

  matrix.get_matbeg()[i-1] = matbeg;
  matrix.get_matcnt()[i-1] = lpx_get_mat_col(lp,i, tmpi.data(),tmpd.data());

  orderx(ndx.data(), matrix.get_matcnt()[i-1], &(tmpi[1]), sizeof(int),
        compare_int, qsort);

  bool flag=false;
  int k=0;
  for (int j=0; j<matrix.get_matcnt()[i-1]; j++) {
    if (tmpi[ndx[j]+1] == 1) { flag=true; continue; }
    matrix.get_matind()[matbeg+k] = tmpi[ndx[j]+1]-1;
    matrix.get_matval()[matbeg+k] = tmpd[ndx[j]+1];

    k++;
    }
  if (flag) {
     int k=0;
     for (int j=0; j<matrix.get_matcnt()[i-1]; j++) {
       if (tmpi[ndx[j]+1] == 1) continue;
       matrix.get_matind()[matbeg+k] -= 1;
       k++;
       }
     }

    //cerr << matrix.get_matind() << endl;
    //cerr << matrix.get_matval() << endl;
    //cerr << matrix.get_matbeg() << endl;

  matrix.get_matcnt()[i-1] -= (flag ? 1 : 0);
  matbeg += matrix.get_matcnt()[i-1];
  }

//cerr << matrix << endl;
#endif

} // namespace pico
