/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file ilp_read.h
 *
 * Read in an MPS file into datatypes that can be used to initialize
 * a generic LP object.
 */

#ifndef pico_readmps_h
#define pico_readmps_h

#include <acro_config.h>
#if defined(__cplusplus)

#include <utilib/SparseMatrix.h>
#include <utilib/CharString.h>
#include <utilib/BasicArray.h>

namespace pico {

using utilib::Ereal;
using utilib::CMSparseMatrix;
using utilib::CharString;
using utilib::BasicArray;

enum ilp_format {mps_format=0, lp_format=1, verbose_format=2, unknown_format=3, cplex_mps_format=4, glpk_ampl_format=5, glpk_lp_format=6, glpk_mps_format=7, ampl_nl_format=8, missing_file=9};

/*
 * free column MPS reader that returns a ranged LP representation:
 *	l <= Ax <= u
 */
void read_mps(const char* filename, short verbose,
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
			BasicArray<int>& listOfIntegers);

/*
 * LP reader that returns a ranged LP representation:
 *	l <= Ax <= u
 */
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
			BasicArray<int>& listOfIntegers);

/*
 * GLPK reader for AMPL model/data and LP format inputs; returns a ranged 
 * LP representation:
 *	l <= Ax <= u
 */
void read_glpk(const char* filename1, const char* filename2,
			short verbose,
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
			BasicArray<int>& listOfIntegers,
			ilp_format);

/*
 * Reader for AMPL model/data in NL format; returns a ranged 
 * LP representation:
 *	l <= Ax <= u
 */
void read_ampl(const char* filename1,
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
                        BasicArray<Ereal<double> >& constraintLHS,
                        BasicArray<Ereal<double> >& constraintRHS,
                        BasicArray<CharString>& constraintNames,
			BasicArray<int>& listOfIntegers,
			ilp_format);

/*
 * ILP reader that returns a one-sided LP representation:
 *	Ax (R) h
 */
void ilp_read(const CharString& filename1, 
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
			ilp_format format);

/*
 * ILP reader that returns a two-sided LP representation:
 *	lhs (R) Ax (R) rhs
 */
void ilp_read(const CharString& filename1, 
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
			ilp_format format);

/*
 * ILP reader that returns a one-sided LP representation:
 *	Ax (R) h
 */
inline void ilp_read(const CharString& filename1, 
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
ilp_read(filename1,filename1,verbose,A,probname,objsen,objCoefs,
	lowerBounds,upperBounds,variableNames,objConst,constraintTypes,constraintRHS,
	constraintNames,listOfIntegers,format);
}

/*
 * ILP reader that returns a two-sided LP representation:
 *	lhs (R) Ax (R) rhs
 */
inline void ilp_read(const CharString& filename1, 
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
ilp_read(filename1,filename1,verbose,A,probname,objsen,objCoefs,lowerBounds,
	upperBounds,variableNames,objConst,constraintLHS,
	constraintRHS,constraintNames,listOfIntegers,format);
}

/*
 * Processes the data setup with the ILPread_* functions
 */
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
                        BasicArray<int>& listOfIntegers);

} // namespace pico

#endif

/**
 * Functions used to process lp input
 */
#ifdef __cplusplus
extern "C" {
#endif
void ILPread_reset();
void ILPread_set_sense(const int);
void ILPread_add_coef(const char *row, const char* col, const double value);
void ILPread_set_constr_type(const char* row, const char rel_op);
void ILPread_set_constr_bound(const char* row, double value, char rel_op);
void ILPread_set_int_var(const char* col);
void ILPread_set_bounds(const char* col, const char* type, double value);
void ILPread_bound_constraint(const char* row, const char* col);
void ILPread_var_append(char*);
char* ILPread_get_last_var();
#ifdef __cplusplus
};
#endif

#endif
