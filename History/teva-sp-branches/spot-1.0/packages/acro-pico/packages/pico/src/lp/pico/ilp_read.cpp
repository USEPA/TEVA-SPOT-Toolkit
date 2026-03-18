/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// ilp_read.cpp
//

#include <acro_config.h>
#include <utilib/CommonIO.h>
#include <utilib/HashedSet.h>
#include <pebbl/fundamentals.h>
#include <pico/ilp_read.h>
#include <pico/ILPHashItem.h>

using namespace utilib;
using namespace std;
using namespace pico;

char dummy_char='a';

//#define DEBUG_ILPREAD

//
// static global variables that are used to setup the LP.
//
static HashedSet<pico::ILPRowItem>	row_ht;
static HashedSet<pico::ILPColItem>	col_ht;
static pebbl::optimizeSense 		sense;
static CharString			last_var;
static int				var_flag=TRUE;

extern "C" void ILPread_var_append(char* str)
{
if (var_flag)
   last_var = "";
last_var += str;
var_flag=false;
}


/// ACRO_VALIDATING #include <stdio.h>
extern "C" char* ILPread_get_last_var()
{
var_flag=true;
/// ACRO_VALIDATING fprintf(stderr,"VAR: %s\n",last_var.data());
return last_var.data();
}


extern "C" void ILPread_set_sense(const int val)
{
if (val) 	sense = pebbl::maximize;
else 		sense = pebbl::minimize;
}


extern "C" void ILPread_add_coef(const char *row, const char* col, 
							const double value)
{
ILPRowItem rtemp;
HashedSet<ILPRowItem>::iterator curr_row;
ILPColItem ctemp;
HashedSet<ILPColItem>::iterator curr_col;

rtemp.name = row;
if ((curr_row = row_ht.find(rtemp)) == row_ht.end()) {
   ILPRowItem foo(row);
   foo.index = row_ht.size();
   //ucout << "Adding Row(1): " << row << " " << foo.index << endl;
   curr_row = row_ht.insert(foo).first;
   }
ctemp.name = col;
if ((curr_col = col_ht.find(ctemp)) == col_ht.end()) {
   ILPColItem foo(col);
   foo.index = col_ht.size();
   curr_col = col_ht.insert(foo).first;
   }
double val = value;
curr_col->coef.add(val,curr_row->index);


#ifdef DEBUG_ILPREAD
ucout << "Writing out the row_ht hash table now" << endl;
row_ht.write(ucout);
ucout << "Writing out the col_ht hash table now" << endl;
col_ht.write(ucout);
#endif
}


extern "C" void ILPread_set_constr_type(const char* row, const char rel_op)
{
ILPRowItem rtemp(row);
HashedSet<ILPRowItem>::iterator curr_row;
if ((curr_row = row_ht.find(rtemp)) == row_ht.end()) {
   ILPRowItem foo(row);
   foo.index = row_ht.size();
   //ucout << "Adding Row(2): " << row << " " << foo.index << endl;
   curr_row = row_ht.insert(foo).first;
   }

switch(rel_op) {
    
  case '=':
    curr_row->constraintType = 'E'; break;
    
  case '>':
    curr_row->constraintType = 'G'; break;
    
  case '<':
    curr_row->constraintType = 'L'; break;
    
  case 'N':
  case 'R':
  case 'E':
  case 'G':
  case 'L':
    curr_row->constraintType = rel_op; break;

  default:
    fprintf(stderr, "Error: unknown relational operator %c", rel_op);
    exit(EXIT_FAILURE);
    break;
  }

#ifdef DEBUG_ILPREAD
ucout << "Writing out the row_ht hash table now" << endl;
row_ht.write(ucout);
ucout << "Writing out the col_ht hash table now" << endl;
col_ht.write(ucout);
#endif
}


extern "C" void ILPread_set_constr_bound(const char* row, double value,
						char rel_op)
{
ILPRowItem temp(row);
HashedSet<ILPRowItem>::iterator curr_row;
if ((curr_row = row_ht.find(temp)) == row_ht.end())
   EXCEPTION_MNGR(runtime_error, "ILPread_set_constr_bound -- constraint \"" << row << "\" does not exist");

if (rel_op == ' ')	// Use the current value;
   rel_op = curr_row->constraintType;

if ((rel_op == 'G') || (rel_op == '>'))
   curr_row->constraintLHS = value;
else if ((rel_op == 'L') || (rel_op == '<'))
   curr_row->constraintRHS = value;
else if ((rel_op == 'E') || (rel_op == '='))
   curr_row->constraintLHS = curr_row->constraintRHS = value;
else if (rel_op == 'R') {
   char type = curr_row->constraintType;
   if (type == 'G')
      curr_row->constraintRHS = 
				curr_row->constraintLHS + fabs(value);
   else if (type == 'L')
      curr_row->constraintLHS = 
				curr_row->constraintRHS - fabs(value);
   else if ((type == 'E') && (value >= 0))
      curr_row->constraintRHS = 
				curr_row->constraintLHS + fabs(value);
   else if ((type == 'E') && (value < 0))
      curr_row->constraintLHS = 
				curr_row->constraintRHS - fabs(value);
   }
else
   EXCEPTION_MNGR(runtime_error, "ILPread_set_constr_bound -- Bad relational operator: " << rel_op);

if ( (curr_row->constraintLHS != Ereal<double>::negative_infinity) &&
     (curr_row->constraintRHS != Ereal<double>::positive_infinity) &&
     (curr_row->constraintLHS != curr_row->constraintRHS) )
   curr_row->constraintType = 'R';
else if (curr_row->constraintLHS == curr_row->constraintRHS)
   curr_row->constraintType = 'E';
else if (curr_row->constraintLHS != Ereal<double>::negative_infinity)
   curr_row->constraintType = 'G';
else if (curr_row->constraintRHS != Ereal<double>::positive_infinity)
   curr_row->constraintType = 'L';
else
   EXCEPTION_MNGR(runtime_error,"ILPread_set_constr_bound -- not sure how to interpret this constraint...");
}


extern "C" void ILPread_set_int_var(const char* col)
{
ILPColItem temp(col);
HashedSet<ILPColItem>::iterator curr_col;
if ((curr_col = col_ht.find(temp)) == col_ht.end())
   EXCEPTION_MNGR(runtime_error, "ILPread_set_int_var -- variable \"" << col << "\" does not exist");

curr_col->intflag = TRUE;
}


extern "C" void ILPread_set_bounds(const char* col, const char* type,
								double value)
{
ILPColItem temp(col);
HashedSet<ILPColItem>::iterator curr_col;
if ((curr_col = col_ht.find(temp)) == col_ht.end())
   EXCEPTION_MNGR(runtime_error, "ILPread_set_bounds -- variable \"" << col << "\" does not exist");

if (strcmp(type,"UP") == 0)
   curr_col->ubound = value;

else if (strcmp(type,"LO") == 0)
   curr_col->lbound = value;

else if (strcmp(type,"FX") == 0)
   curr_col->ubound = curr_col->lbound = value;

else if (strcmp(type,"PL") == 0) {
   curr_col->lbound = 0.0;
   curr_col->ubound = Ereal<double>::positive_infinity;
   }

else if (strcmp(type,"BV") == 0) {
   curr_col->ubound = 1.0;
   ILPread_set_int_var(col);
   }

else if (strcmp(type,"UI") == 0) {
   curr_col->ubound = value;
   ILPread_set_int_var(col);
   }

else if (strcmp(type,"LI") == 0) {
   curr_col->lbound = value;
   //ILPread_set_int_var(col);				INTEGER?
   }

else if (strcmp(type,"MI") == 0) {
   curr_col->lbound = Ereal<double>::negative_infinity;
   curr_col->ubound = 0.0;
   //ILPread_set_int_var(col);				INTEGER?
   }

else if (strcmp(type,"FR") == 0) {
   curr_col->lbound = Ereal<double>::negative_infinity;
   curr_col->ubound = Ereal<double>::positive_infinity;
   //ILPread_set_int_var(col);				INTEGER?
   }

else {
   EXCEPTION_MNGR(runtime_error,"ILPread_set_bounds -- BOUND type " << type << " is not supported");
   }

/***
  Don't support free or negative variables right now.  This would
  require massaging the matrix some more.

        // hack for free and negative variables. Ugly, and does not
        //   always work. MB 
        else if(strcmp(record.field1, "FR") == 0) { // free variable
          fprintf(stderr, 
                  "Free variable %s is split in a positive part %s and a negative part %s_\n",    
                  record.field3, record.field3, record.field3);
          get_column(Mlp, var, Last_column);
          for (i = 0; i <= Mlp->rows; i++)
            Last_column[i]*=-1;
          add_column(Mlp, Last_column);
          strcat(record.field3, "_");
          set_col_name(col_ht, Mlp->columns, record.field3);
          // should lower and upper bounds of both variables be adjusted? What
          // if lower and upper bound are specified later? MB
        }
        else if(strcmp(record.field1, "MI") == 0) { // negative variable
          fprintf(stderr,
                  "Negative variable %s will be represented by - %s-\n",
                  record.field3, record.field3);
          get_column(Mlp, var, Last_column);
          del_column(Mlp, var);
          for (i = 0; i <= Mlp->rows; i++)
            Last_column[i] *= -1;
          add_column(Mlp, Last_column);
          strcat(record.field3, "-");
          set_row_name(row_ht, var, record.field3);
          // should lower and upper bounds of variable be adjusted? What if
          // lower and upper bound are specified later? (does not work!) MB
        }
***/
}


extern "C" void ILPread_bound_constraint(const char* row, const char* col)
{
ILPRowItem rtemp(row);
HashedSet<ILPRowItem>::iterator curr_row;
if ((curr_row = row_ht.find(rtemp)) == row_ht.end())
   EXCEPTION_MNGR(runtime_error, "ILPread_bound_constraint -- constraint \"" << row << "\" does not exist");

ILPColItem ctemp(col);
HashedSet<ILPColItem>::iterator curr_col;
if ((curr_col = col_ht.find(ctemp)) == col_ht.end())
   EXCEPTION_MNGR(runtime_error, "ILPread_bound_constraint -- variable \"" << col << "\" does not exist");

double bound=0.0;
if (curr_row->constraintType == 'E')
   bound = curr_row->constraintLHS;
else if (curr_row->constraintType == 'L')
   bound = curr_row->constraintRHS;
else if (curr_row->constraintType == 'G')
   bound = curr_row->constraintLHS;

OrderedListItem<double,int>* tmp = 
                 curr_col->coef.find( curr_row->index );
bound /= tmp->data();
   
if (curr_row->constraintType == 'E')
   curr_col->lbound = curr_col->ubound = bound;
else if (curr_row->constraintType == 'L')
   curr_col->ubound = bound;
else if (curr_row->constraintType == 'G')
   curr_col->lbound = bound;

bool status = true;
//
// Remove the col items from the hash table that match the row index
//
curr_col->coef.remove( tmp ); // ADD STATUS FLAG
if (status == false)
   EXCEPTION_MNGR(runtime_error,"ILPread_bound_constraint -- removal from column's coef list failed.");
//
// Remove the row item from the hash table
//
//ucout << "PREVIOUS ROW HT" << endl;
//row_ht.write(ucout);
row_ht.erase(curr_row);
//ucout << "SUBSEQUENT ROW HT" << endl;
//row_ht.write(ucout);
if (status == false)
   EXCEPTION_MNGR(runtime_error,"ILPread_bound_constraint -- removal from row hash table failed.");
}


namespace pico {

void ILPread_process_input(CMSparseMatrix<Ereal<double> >& A,
                        int& objsen,
                        BasicArray<Ereal<double> >& objCoefs,
                        BasicArray<Ereal<double> >& lowerBounds,
                        BasicArray<Ereal<double> >& upperBounds,
                        BasicArray<CharString>&  variableNames,
                        CharString&  constraintTypes,
                        BasicArray<Ereal<double> >& constraintLHS,
                        BasicArray<Ereal<double> >& constraintRHS,
                        BasicArray<CharString>& constraintNames,
			BasicArray<int>& listOfIntegers)
{
int nrows = row_ht.size() - 1; 	// # rows after we take out the objective
int ncols = col_ht.size();
int nnzeros = 0;

objsen = sense;
objCoefs.resize(ncols);
lowerBounds.resize(ncols);
upperBounds.resize(ncols);
variableNames.resize(ncols);

constraintTypes.resize(nrows);
constraintLHS.resize(nrows);
constraintRHS.resize(nrows);
constraintNames.resize(nrows);
int objrow;
int nranged=0;
//
// Find the objective function's row
//
HashedSet<ILPRowItem>::iterator curr_row = row_ht.begin();
HashedSet<ILPRowItem>::iterator end      = row_ht.end();
while (curr_row != end) {
  if (curr_row->constraintType == 'N') {
     objrow = curr_row->index;
     break;
     }
  curr_row++;
  }
//
// Setup the other rows
//
BitArray check(nrows+1);
curr_row = row_ht.begin();
while (curr_row != end) {
  int i = curr_row->index;
  if (check(i))
     EXCEPTION_MNGR(runtime_error, "Row " << i << " registered twice in the row hash table!");
  check.set(i);
  if (i > objrow) i--;
  if (curr_row->constraintType != 'N') {
     constraintTypes[i] = curr_row->constraintType;
     if (constraintTypes[i] == 'R') nranged++;
     constraintLHS[i]   = curr_row->constraintLHS;
     constraintRHS[i]   = curr_row->constraintRHS;
     //
     // Setup default values for non-ranged constraints, assuming that
     // a value of -infinity or infinity implies that the LHS/RHS value
     // was omitted from the input file (e.g. in MPS).
     //
     if ((constraintTypes[i] == 'G') && 
			(constraintLHS[i] == Ereal<double>::negative_infinity))
        constraintLHS[i] = 0.0;
     else if ((constraintTypes[i] == 'L') && 
			(constraintRHS[i] == Ereal<double>::positive_infinity))
        constraintRHS[i] = 0.0;
     else if ((constraintTypes[i] == 'E') && (constraintLHS[i] != constraintRHS[i])) {
        if ((constraintLHS[i] == Ereal<double>::negative_infinity) &&
	    (constraintRHS[i] == Ereal<double>::positive_infinity))
           constraintLHS[i] = constraintRHS[i] = 0;
        else if (constraintLHS[i] == Ereal<double>::negative_infinity)
           constraintLHS[i] = constraintRHS[i];
	else if (constraintRHS[i] == Ereal<double>::positive_infinity)
           constraintRHS[i] = constraintLHS[i];
        else
	   EXCEPTION_MNGR(runtime_error,"ILPread_process_input -- equality constraint with different upper and lower bounds");
        }
     constraintNames[i] = curr_row->name;
     }
  curr_row++;
  }
if (check.nbits() < check.size()) {
     EXCEPTION_MNGR(runtime_error, "Only " << check.nbits() << " bits set out of " << check.size() << " rows!");
   }

int num_ints=0;
HashedSet<ILPColItem>::iterator curr_col = col_ht.begin();
HashedSet<ILPColItem>::iterator col_end  = col_ht.end();
while (curr_col != col_end) {
  int i=curr_col->index;
  lowerBounds[i] = curr_col->lbound;
  upperBounds[i] = curr_col->ubound;
  //
  // Setup default values for the lowerbounds on variables, assuming that
  // a value of -infinity implies that the lower bound was 
  // omitted from the input file (e.g. in MPS or LP format).  By default, a
  // lower bound is 0.0 if the upper bound is > 0.0 and -infty if the 
  // upper bound is <= 0.0
  //
  if ((upperBounds[i] > 0.0) && 
			(lowerBounds[i] == Ereal<double>::negative_infinity))
     lowerBounds[i] = 0.0;
  variableNames[i] = curr_col->name;
  if (curr_col->coef.find(objrow))
     nnzeros += curr_col->coef.size()-1;
  else
     nnzeros += curr_col->coef.size();
  num_ints += (curr_col->intflag == TRUE);
  curr_col++;
  }

listOfIntegers.resize(num_ints);
int intndx=0;
A.initialize(nrows,ncols,nnzeros);
int mvndx=0;
for (int i=0; i<ncols; i++) {
  ILPColItem tmp_item(variableNames[i]);
  curr_col = col_ht.find(tmp_item);

  if (curr_col->intflag == TRUE)
     listOfIntegers[intndx++] = i;

  A.get_matbeg()[i] = mvndx;
  OrderedListItem<double,int>* item = curr_col->coef.head();
  while (item) {
    if (item->key() == objrow) {
       objCoefs[i] = item->data();
       }
    else {
       A.get_matval()[mvndx]   = item->data();
       int ndx = item->key();
       A.get_matind()[mvndx++] = ndx + (ndx >= objrow ? -1 : 0);
       A.get_matcnt()[i]++;
       }
    item = curr_col->coef.next(item);
    }
  }
}



extern "C" void ILPread_reset()
{
//
// Reset the global data
//
row_ht.clear();
col_ht.clear();
sense = pebbl::unknown;
}



ilp_format guess_format(CharString& filename)
{
//
// Guess the file format based on the filename suffix
//
size_type len = filename.size();
if ((len > 4) && (strcmp(&(filename[len-4]),".mps") == 0))
   return mps_format;

if ((len > 4) && (strcmp(&(filename[len-4]),".mod") == 0))
   return glpk_ampl_format;

if ((len > 3) && (strcmp(&(filename[len-3]),".lp") == 0))
   return glpk_lp_format;

if ((len > 3) && (strcmp(&(filename[len-3]),".nl") == 0))
   return ampl_nl_format;
//
// If no recognizable suffix exists and the file exists, then guess 
// the file format based on information in the file
//
{
CharString tmp(256);
ifstream ifstr(filename.data());
ifstr >> tmp;
if (ifstr) {
   if ((tmp[0] == '*') || (tmp == "NAME"))
      return mps_format;
   else
      return unknown_format;
   }
}
//
// If the file does not exist, then guess suffixes and see if those files
// exist
//
// MPS
{
CharString tmp(256);
CharString tfile = filename;
tfile += ".mps";
ifstream ifstr(tfile.data());
ifstr >> tmp;
if (ifstr) { filename = tfile; return mps_format; }
}
// AMPL Model
{
CharString tmp(256);
CharString tfile = filename;
tfile += ".mod";
ifstream ifstr(tfile.data());
ifstr >> tmp;
if (ifstr) { filename = tfile; return glpk_ampl_format; }
}
// LP
{
CharString tmp(256);
CharString tfile = filename;
tfile += ".lp";
ifstream ifstr(tfile.data());
ifstr >> tmp;
if (ifstr) { filename = tfile; return glpk_lp_format; }
}
// AMPL NL
{
CharString tmp(256);
CharString tfile = filename;
tfile += ".nl";
ifstream ifstr(tfile.data());
ifstr >> tmp;
if (ifstr) { filename = tfile; return ampl_nl_format; }
}
//
// Give up
//
return missing_file;
}





void ilp_read(const CharString& filename, 
			const CharString& filename2,
			short verbose,
			CMSparseMatrix<Ereal<double> >& A,
			CharString& probname,
                        int& objsen,
			BasicArray<Ereal<double> >& objCoefs,
                        BasicArray<Ereal<double> >& lowerBounds,
                        BasicArray<Ereal<double> >& upperBounds,
                        BasicArray<CharString>&  variableNames,
			double& objConst, 
                        CharString&  constraintTypes,
                        BasicArray<Ereal<double> >& constraintRHS,
                        BasicArray<CharString>& constraintNames,
			BasicArray<int>& listOfIntegers,
			ilp_format format)
{
CharString filename1 = filename;
if (format == unknown_format)
   format = guess_format(filename1);

BasicArray<Ereal<double> > rhs,lhs;
objConst=0.0;

switch (format) {

  case mps_format:
     read_mps(filename1.data(),verbose, A, probname, objsen, objCoefs, lowerBounds,
			upperBounds, variableNames, constraintTypes,
			lhs, rhs, constraintNames, listOfIntegers);
     break;

  case lp_format:
     read_lp(filename1.data(),verbose, A, probname, objsen, objCoefs, lowerBounds,
			upperBounds, variableNames, constraintTypes,
			lhs, rhs, constraintNames, listOfIntegers);
     break;

  case ampl_nl_format:
     read_ampl(filename1.data(), verbose, A, probname, objsen, objCoefs, lowerBounds,
			upperBounds, variableNames, objConst, constraintTypes,
			lhs, rhs, constraintNames, listOfIntegers,format);
     break;

  case glpk_mps_format:
  case glpk_ampl_format:
  case glpk_lp_format:
     if (filename1 == filename2)
        read_glpk(filename1.data(),0, verbose, A, probname, objsen, objCoefs, lowerBounds,
			upperBounds, variableNames, constraintTypes,
			lhs, rhs, constraintNames, listOfIntegers,format);
     else
        read_glpk(filename1.data(),filename2.data(), verbose, A, probname, objsen, objCoefs, lowerBounds,
			upperBounds, variableNames, constraintTypes,
			lhs, rhs, constraintNames, listOfIntegers,format);
     break;

  case missing_file:
     EXCEPTION_MNGR(runtime_error,"File " << filename.data() << " is missing. Unable to find a valid suffix.")

  default:
     EXCEPTION_MNGR(runtime_error,"Unable to determine the MIP/LP format in filename " << filename.data())
  };

size_type nrows = constraintTypes.size();
int nranged=0;
for (size_type i=0; i<nrows; i++) {
  if (constraintTypes[i] == 'R')
     nranged++;
  }

//
// If there are no 'ranged' rows, then the conversion to a nonranged 
// representation is simple, so let's do it.
//
if (nranged == 0) {
   constraintRHS.resize(rhs.size());
   for (size_type i=0; i<nrows; i++) {
     switch (constraintTypes[i]) {
  	case 'G':
     		constraintRHS[i] = lhs[i];
		break;
  	case 'L':
     		constraintRHS[i] = rhs[i];
		break;
  	case 'E':
     		constraintRHS[i] = rhs[i];
		break;
	default:
		EXCEPTION_MNGR(runtime_error,"Bad constraint type: " << constraintTypes[i]);
	}
     }
   }
//
// There are 'ranged' rows, so count the number of nonzeros, resize the
// matrix, and add copy the ranged rows with their upper bounds.  The resizing
// methods provided by the arrays do the right thing here...
//
else {
   constraintTypes.resize(nrows+nranged);
   constraintRHS.resize(nrows+nranged);
   constraintNames.resize(nrows+nranged);

   RMSparseMatrix<Ereal<double> > B;
   B.convert(A);

   int num_ranged_nzeros=0;
   {for (size_type i=0; i<nrows; i++)
     if (constraintTypes[i] == 'R')
        num_ranged_nzeros += B.get_matcnt()[i];}

   B.initialize(variableNames.size(), nrows+nranged, 
				B.get_nnzero()+num_ranged_nzeros);
   int ctr=nrows;
   for (size_type i=0; i<nrows; i++) {
     switch (constraintTypes[i]) {
  	case 'G':
     		constraintRHS[i] = lhs[i];
		break;
  	case 'L':
     		constraintRHS[i] = rhs[i];
		break;
  	case 'E':
     		constraintRHS[i] = rhs[i];
		break;
        case 'R':
        	B.adjoinRow(B.get_matcnt()[i], &(B.get_matind()[i]), 
							&(B.get_matval()[i]));
		constraintNames[ctr] = constraintNames[i];
                constraintNames[ctr] += "rhs";
		constraintTypes[ctr] = 'L';
		constraintRHS[ctr]   = rhs[i];

		constraintTypes[i]   = 'G';
		constraintRHS[i]     = lhs[i];
		constraintNames[ctr] += "lhs";
		break;
	default:
		EXCEPTION_MNGR(runtime_error,"Bad constraint type: " << constraintTypes[i]);
	}
     }
   A.convert(B);
   }
}


void ilp_read(const CharString& filename, 
			const CharString& filename2,
			short verbose,
                        CMSparseMatrix<Ereal<double> >& A,
                        CharString& probname,
                        int& objsen,
                        BasicArray<Ereal<double> >& objCoefs,
                        BasicArray<Ereal<double> >& lowerBounds,
                        BasicArray<Ereal<double> >& upperBounds,
                        BasicArray<CharString>&  variableNames,
			double& objConst, 
                        BasicArray<Ereal<double> >& constraintLHS,
                        BasicArray<Ereal<double> >& constraintRHS,
                        BasicArray<CharString>& constraintNames,
                        BasicArray<int>& listOfIntegers,
                        ilp_format format)
{
CharString filename1 = filename;
if (format == unknown_format)
   format = guess_format(filename1);
objConst=0.0;

CharString  constraintTypes;
switch (format) {
  case mps_format:
     read_mps(filename1.data(),verbose, A, probname, objsen, objCoefs, lowerBounds,
			upperBounds, variableNames, constraintTypes,
			constraintLHS, constraintRHS, constraintNames, 
			listOfIntegers);
     break;

  case lp_format:
     read_lp(filename1.data(),verbose, A, probname, objsen, objCoefs, lowerBounds,
			upperBounds, variableNames, constraintTypes,
			constraintLHS, constraintRHS, constraintNames, 
			listOfIntegers);
     break;

  case ampl_nl_format:
     read_ampl(filename1.data(), verbose, A, probname, objsen, objCoefs, lowerBounds,
			upperBounds, variableNames, objConst, constraintTypes,
			constraintLHS, constraintRHS, constraintNames, 
			listOfIntegers,format);
     break;

  case glpk_mps_format:
  case glpk_ampl_format:
  case glpk_lp_format:
     if (filename == filename2)
        read_glpk(filename1.data(),0, verbose, A, probname, objsen, objCoefs, lowerBounds,
			upperBounds, variableNames, constraintTypes,
			constraintLHS, constraintRHS, constraintNames, listOfIntegers,format);
     else
        read_glpk(filename1.data(),filename2.data(), verbose, A, probname, objsen, objCoefs, lowerBounds,
			upperBounds, variableNames, constraintTypes,
			constraintLHS, constraintRHS, constraintNames, listOfIntegers,format);
     break;

  case missing_file:
     EXCEPTION_MNGR(runtime_error,"File " << filename.data() << " is missing. Unable to find a valid suffix.")

  default:
     EXCEPTION_MNGR(runtime_error,"Unable to determine the MIP/LP format in filename " << filename.data())
  };

#if 0
if (format == mps_format)
   read_mps(filename1,verbose, A, probname, objsen, objCoefs, lowerBounds,
			upperBounds, variableNames, constraintTypes,
			constraintLHS, constraintRHS, constraintNames, 
			listOfIntegers);
else
   read_lp(filename1,verbose, A, probname, objsen, objCoefs, lowerBounds,
			upperBounds, variableNames, constraintTypes,
			constraintLHS, constraintRHS, constraintNames, 
			listOfIntegers);
#endif

/// WEH - QUESTION: do we need to do anything else??? with constraintTypes?
/*
size_type nrows = constraintTypes.size();
int nranged=0;
for (size_type i=0; i<nrows; i++) {
  if (constraintTypes[i] == 'R')
     nranged++;
  }
*/
}

} // namespace pico
