/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file _alloc.h
 *
 * Header for generic allocation routines.
 * These routines are derived from similar methods in Numerical Recipies in
 * C.  These routines have largely been supplanted by the \c BasicArray 
 * class and its derived classes.  However, on occassion I find that I _really_
 * need to program with C, and these come in handy.  For that reason, these
 * should probably be better documented, but not right now...
 *
 * \author William E. Hart
 */

#ifndef utilib_alloc_h
#define utilib_alloc_h

#if defined(__cplusplus) && defined(ANSI_HDRS)
#include <cstdio>
#include <cstdlib>
#else
#include <stdio.h>
#include <stdlib.h>
#endif

#include <utilib/_generic.h>



/* vector\matrix allocate statements */

/*
#define vector(nl,nh)			dvector(nl,nh)
#define matrix(nrl,nrh,ncl,nch)		dmatrix(nrl,nrh,ncl,nch)
#define array(nrl,nrh,ncl,nch,nzl,nzh)	darray(nrl,nrh,ncl,nch,nzl,nzh)
*/

#define fvector(nl, nh)	\
		(float *) generic_make_vector(nl,nh,sizeof(float))
#define ivector(nl, nh)	\
		(int *) generic_make_vector(nl,nh,sizeof(int))
#define dvector(nl, nh)	\
		(double *) generic_make_vector(nl,nh,sizeof(double))
#define fmatrix(nrl, nrh, ncl, nch) \
		(float **) generic_make_matrix(nrl,nrh,ncl,nch,sizeof(float))
#define imatrix(nrl, nrh, ncl, nch) \
		(int **) generic_make_matrix(nrl,nrh,ncl,nch,sizeof(int))
#define dmatrix(nrl, nrh, ncl, nch) \
		(double **) generic_make_matrix(nrl,nrh,ncl,nch,sizeof(double))
#define farray(nrl, nrh, ncl, nch, nzl, nzh) \
		(float ***) generic_make_array(nrl,nrh,ncl,nch,nzl,nzh, \
							sizeof(float))
#define iarray(nrl, nrh, ncl, nch, nzl, nzh) \
		(int ***) generic_make_array(nrl,nrh,ncl,nch,nzl,nzh, \
							sizeof(int))
#define darray(nrl, nrh, ncl, nch, nzl, nzh) \
		(double ***) generic_make_array(nrl,nrh,ncl,nch,nzl,nzh, \
							sizeof(double))


/* vector\matrix free statements */

/*
#define free_vector(v,nl,nh)			free_dvector(v,nl,nh)
#define free_matrix(v,nrl,nrh,ncl,nch)		free_dmatrix(v,nrl,nrh,ncl,nch)
#define free_array(v,nrl,nrh,ncl,nch,nzl,nzh)	free_darray(v,nrl,nrh,ncl,nch,nzl,nzh)
*/

#define free_fvector(v, nl, nh) \
		generic_free_vector((VOID *) v,nl,nh,sizeof(float))
#define free_ivector(v, nl, nh) \
		generic_free_vector((VOID *) v,nl,nh,sizeof(int))
#define free_dvector(v, nl, nh) \
		generic_free_vector((VOID *) v,nl,nh,sizeof(double))
#define free_fmatrix(v, nrl, nrh, ncl, nch) \
		generic_free_matrix((VOID **) v,nrl,nrh, ncl, nch,sizeof(float))
#define free_imatrix(v, nrl, nrh, ncl, nch) \
		generic_free_matrix((VOID **) v,nrl,nrh, ncl, nch,sizeof(int))
#define free_dmatrix(v, nrl, nrh, ncl, nch) \
		generic_free_matrix((VOID **) v,nrl,nrh,ncl, nch,sizeof(double))
#define free_farray(v, nrl, nrh, ncl, nch, nzl, nzh) \
		generic_free_array((VOID ***) v,nrl,nrh, ncl, nch,nzl,nzh,\
						sizeof(float))
#define free_iarray(v, nrl, nrh, ncl, nch, nzl, nzh) \
		generic_free_array((VOID ***) v,nrl,nrh, ncl, nch,nzl,nzh,\
						sizeof(int))
#define free_darray(v, nrl, nrh, ncl, nch, nzl, nzh) \
		generic_free_array((VOID ***) v,nrl,nrh, ncl,nch,nzl,nzh,\
						sizeof(double))


/* matrix transpose statements */

#define transpose_fmatrix(m, nrl, nrh, ncl, nch) \
	(float **) generic_transpose_matrix((VOID **) m,nrl,nrh, ncl, nch,\
				sizeof(float))
#define transpose_imatrix(m, nrl, nrh, ncl, nch) \
	(int **) generic_transpose_matrix((VOID **) m,nrl,nrh, ncl, nch,\
				sizeof(int))
#define transpose_dmatrix(m, nrl, nrh, ncl, nch) \
	(double **) generic_transpose_matrix((VOID **)m,nrl,nrh, ncl, nch,\
				sizeof(double))



/* EXTERNS */

#if defined(__cplusplus)
extern "C" {
#endif
#if defined(__STDC__) || defined(__cplusplus)
VOID	*generic_make_vector(int nl, int nh, unsigned size);
VOID	**generic_make_matrix(int nrl, int nrh, int ncl, int nch, 
			unsigned size);
VOID	***generic_make_array(int nrl, int nrh, int ncl, int nch, int nzl, 
			int nzh, unsigned size);
void	generic_free_vector(char* v, int nl, int nh, unsigned size);
void	generic_free_matrix(char** v, int nrl, int nrh, int ncl, int nch, 
			unsigned size);
void	generic_free_array(char*** v, int nrl, int nrh, int ncl, int nch, 
			int nzl, unsigned size);
VOID 	**generic_transpose_matrix(VOID **matrix, int nrl, int nrh, int ncl, 
			int nch, unsigned size);
int print_ivector(int* v, int nl, int nh, FILE* fp);
int print_fvector(float* v, int nl, int nh, FILE* fp);
int print_dvector(double* v, int nl, int nh, FILE* fp);
int print_imatrix(int** v, int nrl, int nrh, int ncl, int nch, FILE* fp);
int print_fmatrix(float** v, int nrl, int nrh, int ncl, int nch, FILE* fp);
int print_dmatrix(double** v, int nrl, int nrh, int ncl, int nch, FILE* fp);
int print_farray(float*** v, int nrl, int nrh, int ncl, int nch, int nzl,
						int nzh, FILE* fp);
int read_ivector(int* v, int nl, int nh, FILE* fp);
int read_dvector(double* v, int nl, int nh, FILE* fp);
int read_fvector(float* v, int nl, int nh, FILE* fp);
int read_dmatrix(double** v, int nrl, int nrh, int ncl, int nch, FILE* fp);
#else
extern VOID 	*generic_make_vector();
extern VOID	**generic_make_matrix();
extern VOID	***generic_make_array();
extern void	generic_free_vector();
extern void	generic_free_matrix();
extern void	generic_free_array();
extern VOID	**generic_transpose_matrix();
extern int	print_ivector();
extern int	print_fvector();
extern int	print_dvector();
extern int	print_imatrix();
extern int	print_fmatrix();
extern int	print_dmatrix();
extern int	print_farray();
extern int 	read_ivector();
extern int 	read_dvector();
extern int 	read_fvector();
extern int	read_dmatrix();
#endif
#ifdef __cplusplus
};
#endif

#ifndef __cplusplus
#define new(type)		(type *) malloc((size_t) sizeof( type ))
#define new_array(type, n) 	(type*) malloc((size_t) sizeof(type)*n)
#endif

#endif
