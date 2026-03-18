/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// read_lp.cpp
//

/// WEH - this code is proving problematic for portability and for functionality
/// so I'm disabling it for now...

/*
   ============================================================================
   PURPOSE : translation of lp-problem and storage in sparse matrix

   SHORT : Subroutines for yacc program to store the input in an intermediate
   data-structure. The yacc and lex programs translate the input.  First the
   problemsize is determined and the date is read into an intermediate
   structure, then readinput fills the sparse matrix.

   USAGE : call yyparse(); to start reading the input.  call readinput(); to
   fill the sparse matrix.
   ============================================================================
   Rows : contains the amount of rows + 1. Rows-1 is the amount of constraints
   (no bounds) Rows also contains the rownr 0 which is the objective function

   Columns : contains the amount of columns (different variable names found in
   the constraints)

   Nonnuls : contains the amount of nonnuls = sum of different entries of all
   columns in the constraints and in the objectfunction

   Hash_tab : contains all columnnames on the first level of the structure the
   row information is kept under each column structure in a linked list (also
   the objective funtion is in this structure) Bound information is also
   stored under under the column name

   First_rside : points to a linked list containing all relational operators
   and the righthandside values of the constraints the linked list is in
   reversed order with respect to the rownumbers
   ============================================================================ */

#include <acro_config.h>
#include <pico/ilp_read.h>

#define MAXSTRL 64

//
// extern's for yacc global variables.
//
#if 0
extern FILE* yyin;
extern int yylineno;
extern "C" int yyparse(void);
#endif

namespace pico {


void read_lp(const char* filename, short verbose,
			CMSparseMatrix<Ereal<double> >& A,
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
			BasicArray<int>& listOfIntegers)
{
#if 0
if ((yyin = fopen(filename,"r")) == NULL)
   EXCEPTION_MNGR(runtime_error, "read_lp -- bad filename \"" << filename << "\"");
ILPread_reset();
yyparse();
fclose(yyin);

ILPread_process_input(A, objsen, objCoefs, lowerBounds,
		upperBounds, variableNames, constraintTypes, constraintLHS,
		constraintRHS, constraintNames, listOfIntegers);

ILPread_reset();
probname = filename;
if (strcmp(&(probname[probname.size()-4]),".lp") == 0)
   probname.resize(probname.size()-3);
#endif
}

} // namespace pico
