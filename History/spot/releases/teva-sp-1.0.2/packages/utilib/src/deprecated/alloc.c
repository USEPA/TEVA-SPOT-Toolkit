/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file alloc.c
 *
 * Routines for allocating, freeing and printing vectors and matrices
 * Note:  need to initialize the values here.
 *
 * \author William E. Hart
 */

#if defined(DARWIN)
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#include <stdio.h>
#include "_generic.h"
#include "_alloc.h"
#include "errmsg.h"

#define VOID	char


VOID *generic_make_vector(int nl,int nh, unsigned size)
{
	VOID *v=NULL;

	if ((v=(VOID *)calloc((unsigned) (nh-nl+1), (unsigned) size)) == NULL)
	     return(v);
	return((VOID *) (v - nl*size));		/* assume sizeof(char)==1 */
}


VOID **generic_make_matrix(int nrl,int nrh,int ncl,int nch, unsigned size)
{
	int i;
	VOID **m;
	VOID *temp;
	if ((m=(VOID **) malloc((unsigned) (nrh-nrl+1)*sizeof(VOID *))) == NULL)
	   return(m);
	m -= nrl /* * sizeof(VOID *) */;

	if ((temp = 
	        (VOID *) calloc((unsigned) (nch-ncl+1)*(nrh-nrl+1),
				(unsigned) size))==NULL)
	   return(NULL); 
	for(i=nrl;i<=nrh;i++) {
		m[i] = temp + (i-nrl)*(nch-ncl+1)*size;
		m[i] -= ncl*size;
		}
	return(m);
}


VOID ***generic_make_array(int nrl,int nrh,int ncl,int nch,int nzl,int nzh, unsigned size)
{
int i,j;
VOID ***m;
VOID **n;
VOID *temp;
if ((m=(VOID ***) malloc((unsigned) (nrh-nrl+1)*sizeof(VOID **))) == NULL)
   return(NULL);
m -= nrl /* * sizeof(VOID **) */;

if ((n=(VOID **) malloc((unsigned) (nch-ncl+1)*(nrh-nrl+1)*sizeof(VOID *)))==NULL)
   return(NULL); 
for(i=nrl; i<=nrh; i++) {
	m[i] = n + (i-nrl)*(nch-ncl+1) /* * sizeof(VOID *) */;
	m[i] -= ncl /* * sizeof(VOID *) */;
	}

if ((temp=(VOID *) calloc((unsigned) (nch-ncl+1)*(nrh-nrl+1)*(nzh-nzl+1),
			  (unsigned) size))==NULL)
   return(NULL);
for (i=nrl; i<=nrh; i++)
  for (j=ncl; j<=nch; j++) {
    m[i][j] = temp + (i-nrl)*(nch-ncl+1)*(nzh-nzl+1)*size 
						+ (j-ncl)*(nzh-nzl+1)*size;
    m[i][j] -= nzl*size;
    }

return(m);
}


VOID **generic_transpose_matrix(VOID** matrix, int nrl,int nrh,int ncl,int nch,
					unsigned size)
{
VOID **new_matrix;
int i,j;

new_matrix = generic_make_matrix(ncl, nch, nrl, nrh, size);
for (i=nrl; i<=nrh; i++)
  for (j=ncl; j<=nch; j++)
     new_matrix[j][i] = matrix[i][j];

return(new_matrix);
}



void generic_free_vector(VOID* v,int nl,int nh,unsigned size)
{
	nh = 0;				/* 'need' to use the parameter */
	free((void*) (v + nl*size));
}


void generic_free_matrix(VOID** m,int nrl,int nrh,int ncl,int nch,unsigned size)
{
nrh=0;					/* 'need' to use the parameter */
nch=0;					/* 'need' to use the parameter */
if (m != NULL) {
   free((void*) (m[nrl]+ncl*size));
   free((void*) (m+nrl));
   }
}


void generic_free_array(VOID*** m,int nrl,int nrh,int ncl,int nch,int nzl,unsigned size)
{
nrh=0;					/* 'need' to use the parameter */
nch=0;					/* 'need' to use the parameter */
if (m != NULL) {
   free((void*) (m[0][0] + nzl*size));
   free((void*) (m[0]+ncl));
   free((void*) (m+nrl));
   }
}



/*************************/
/* Routines for printing */
/*************************/

/*  NOT CURRENTLY IN USE

int print_ivector(int* v, int nl, int nh, FILE* fp)
{
int i;

for (i=nl; i<=nh; i++)
    fprintf(fp, "%d ", v[i]);
fprintf(fp, "\n");

return(OK);
}


int print_fvector(float* v, int nl, int nh, FILE* fp)
{
int i;

for (i=nl; i<=nh; i++)
    fprintf(fp, "%f ", v[i]);
fprintf(fp, "\n");

return(OK);
}


int print_dvector(double* v, int nl, int nh, FILE* fp)
{
int i;

for (i=nl; i<=nh; i++)
    fprintf(fp, "%f ", v[i]);
fprintf(fp, "\n");

return(OK);
}



int print_imatrix(int** v, int nrl, int nrh, int ncl, int nch, FILE* fp)
{
int i,j;

for (i=nrl; i<=nrh; i++) {
    for (j=ncl; j<=nch; j++) {
        fprintf(fp, "%d ", v[i][j]);
        }
    fprintf(fp, "\n");
    }

return(OK);
}



int print_fmatrix(float** v, int nrl, int nrh, int ncl, int nch, FILE* fp)
{
int i,j;

for (i=nrl; i<=nrh; i++) {
    for (j=ncl; j<=nch; j++) {
        fprintf(fp, "%f ", v[i][j]);
        }
    fprintf(fp, "\n");
    }

return(OK);
}


int print_dmatrix(double** v, int nrl, int nrh, int ncl, int nch, FILE* fp)
{
int i,j;

for (i=nrl; i<=nrh; i++) {
    for (j=ncl; j<=nch; j++) {
        fprintf(fp, "%f ", v[i][j]);
        }
    fprintf(fp, "\n");
    }

return(OK);
}



int print_farray(float*** v, int nrl, int nrh, int ncl, int nch, int nzl, 
					int nzh, FILE* fp)
{
int i,j,k;

for (i=nrl; i<=nrh; i++) {
    for (j=ncl; j<=nch; j++) {
      for (k=nzl; k<=nzh; k++) {
        fprintf(fp, "%f ", v[i][j][k]);
        }
      fprintf(fp, "\n");
      }
    fprintf(fp, "\n");
    }

return(OK);
}

*/


/************************/
/* Routines for reading */
/************************/


/*  NOT CURRENTLY IN USE

int read_ivector(int* v, int nl, int nh, FILE* fp)
{
int i;

for (i=nl; i<=nh; i++)
    if (fscanf(fp, "%d", &(v[i])) != 1)
	ErrReturn("read_ivector --  fscanf failure");
fscanf(fp,"\n");

return(OK);
}



int read_fvector(float* v, int nl, int nh, FILE* fp)
{
int i;

for (i=nl; i<=nh; i++)
    if (fscanf(fp, "%f", &(v[i])) != 1)
	ErrReturn("read_fvector -- fscanf failure");
fscanf(fp,"\n");

return(OK);
}



int read_dvector(double* v, int nl, int nh, FILE* fp)
{
int i;

for (i=nl; i<=nh; i++)
    if (fscanf(fp, "%lf", &(v[i])) != 1)
	ErrReturn("read_dvector -- fscanf failure");
fscanf(fp,"\n");

return(OK);
}



int read_dmatrix(double** v, int nrl, int nrh, int ncl, int nch, FILE* fp)
{
int i,j;

for (i=nrl; i<=nrh; i++) {
  for (j=ncl; j<=nch; j++) 
    if (fscanf(fp, "%lf", &(v[i][j])) != 1)
	ErrReturn("read_dvector -- fscanf failure");
  fscanf(fp,"\n");
  }

return(OK);
}

*/

/********** Unused routines **********/
/*

float **submatrix(a,oldrl,oldrh,oldcl,oldch,newrl,newcl)
float **a;
int oldrl,oldrh,oldcl,oldch,newrl,newcl;
{
	int i,j;
	float **m;

	m=(float **) malloc((unsigned) (oldrh-oldrl+1)*sizeof(float*));
	if (!m) nrerror("allocation failure in submatrix()");
	m -= newrl;

	for(i=oldrl,j=newrl;i<=oldrh;i++,j++) m[j]=a[i]+oldcl-newcl;

	return m;
}



void free_submatrix(b,nrl,nrh,ncl,nch)
float **b;
int nrl,nrh,ncl,nch;
{
	free((void*) (b+nrl));
}



float **convert_matrix(a,nrl,nrh,ncl,nch)
float *a;
int nrl,nrh,ncl,nch;
{
	int i,j,nrow,ncol;
	float **m;

	nrow=nrh-nrl+1;
	ncol=nch-ncl+1;
	m = (float **) malloc((unsigned) (nrow)*sizeof(float*));
	if (!m) nrerror("allocation failure in convert_matrix()");
	m -= nrl;
	for(i=0,j=nrl;i<=nrow-1;i++,j++) m[j]=a+ncol*i-ncl;
	return m;
}



void free_convert_matrix(b,nrl,nrh,ncl,nch)
float **b;
int nrl,nrh,ncl,nch;
{
	free((void*) (b+nrl));
}
*/

