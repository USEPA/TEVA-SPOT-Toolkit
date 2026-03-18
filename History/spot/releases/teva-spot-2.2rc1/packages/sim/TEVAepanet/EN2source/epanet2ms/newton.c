// Newton
//
// Newton-Raphson algorithm to solve nonlinear algebraic equations
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hash.h"
#include "text.h"
#include "mstext.h"
#include "types.h"
#include "mstypes.h"
#include "funcs.h"
#include "msfuncs.h"
#include "mathexpr.h"
#define  EXTERN  extern
#include "vars.h"
#include "msvars.h"
#include "newton.h"
//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

#define TINY1 1.0e-20
#define EPS 1.0e-5


//-----------------------------------------------------------------------------
//    Local declarations
//-----------------------------------------------------------------------------
int*     indx;      
double * fvec; 
double * p;  
double * f;   
double ** fjac;
float * vv;


//-----------------------------------------------------------------------------
// Function Declarations
//-----------------------------------------------------------------------------


// function values and Jacobian matrix evaluation
void usrfun(PExprList * algebraicexpression, float *x,int n,double *fvec,double **fjac);

// LU decomposition to solve linear equation set
void lubksb(double **a, int n, int *indx, double b[]);
int  ludcmp(double **a, int n, int *indx, float *d);

// compute function values at x
void vecfunc(int n, PExprList * algebraicexpression_list, float x[], double fvec[]);
// compute Jacobian matrix
void fdjac(int n, PExprList * algebraicexpression, float x[], double fvec[], double **df,void (*vecfunc)(int, PExprList *, float [], double []));

int newton_open(int n)
/*
**-----------------------------------------------------------------------
**   Input:   n = number of algebraic equations 
**   Output:  none 
**   Purpose:  open the algebraic solver to solve system of n equations
	 (return 0 if successful, 101 if not)
**-----------------------------------------------------------------------
*/
{
    int i;

	indx	= (int *) calloc(n+1, sizeof(int));
    vv		= (float *) calloc(n+1, sizeof(float));
    fvec	= (double *) calloc(n+1, sizeof(double));
    p		= (double *) calloc(n+1, sizeof(double));
    f		= (double *) calloc(n+1, sizeof(double));
    fjac    = (double **) calloc((n+1), sizeof(double *));

    if ( !indx || !vv || !fvec || !p || !f || !fjac ) return 101;
	
	for ( i = 1; i < n+1; i++)
	{
		fjac[i] = (double *) calloc((n+1), sizeof(double));
		if (!fjac[i]) return 101;
	}
    return 0;
}

void newton_close (int n)
/*
**-----------------------------------------------------------------------
**   Input:   n = number of algebraic equations 
**   Output:  none 
**   Purpose: close the algebraic solver
**-----------------------------------------------------------------------
*/
{
	int i;
	for ( i = 1; i < n+1; i++)
	{
		if (fjac[i]) free(fjac[i]);
		fjac[i] = NULL;
	}
	if (indx) free(indx);
	indx = NULL;
	if (vv) free(vv);
	vv = NULL;
	if (fvec) free(fvec);
	fvec = NULL;
	if (p) free(p);
	p = NULL;
	if (f) free(f);
	f = NULL;
	if (fjac) free(fjac);
	fjac = NULL;
}


/* Newton-Raphson method to improve the root for a set of nonlinear algebraic equations. 
   Stop if the root converges in summed absolute variable increments tolx. 
   Based on code from Numerical Recipes in C. 
*/
void mnewt(int reactiontype, int ntrial, PExprList * algebraicexpression, float x[], int n, float tolx)
/*
**-------------------------------------------------------------------
**   Input:   ntrial = maximum Newton-Raphson steps
              algebraicexpression = vector algebraic equation expressions
			  x = initial guess of algebraic variables
			  n = number of algebraic variables
			  tolx = tolerance for summed absolute variable increments
			 
**   Output:  x = updated values of algebraic variables                                          
**   Purpose: solve a set of algebraic equations and find the roots
**-------------------------------------------------------------------
*/
{
	int k,i, errcode, index;
	float errx,d, errmax, cscal;

	for (k=1;k<=ntrial;k++) 
	{

		/*User function supplies function values at x in fvec and Jacobian matrix in fjac*/
		usrfun(algebraicexpression, x, n, fvec, fjac);
		/*Check function convergence.*/
		
		for (i=1;i<=n;i++) 
			p[i] = -fvec[i]; /*Right-hand side of linear equations.*/
		
		/*Solve linear equations using LU decomposition.*/		
		errcode = ludcmp(fjac,n,indx,&d); 
		if (errcode == 7) /*Singular matrix*/
		{
			SingularFlag = TRUE;
			Warnflag = 7;
			return;
		}
		lubksb(fjac,n,indx,p);
		
		/*Check root convergence.*/
		
        errmax = 0.0;
        for (i=1; i<n+1; i++)
        {
			cscal = x[i];
			if (reactiontype == PIPES)
			{
				index = i + Npiperate+Npipedef;
				if(cscal < PSabstol[index]) cscal = PSabstol[index];
			}
			else
			{
				index = i + Ntankrate+Ntankdef;
				if(cscal < TSabstol[index]) cscal = PSabstol[index];
			}
			x[i] += p[i];
            errx = fabs(p[i]/cscal);
            if (errx > errmax) errmax = errx;
        }
        errmax /= tolx;

		if (errmax <= 1) return;
	}
	return;
}
/*LU decomposition.*/
int ludcmp(double **a, int n, int *indx, float *d)
/*
**-------------------------------------------------------------------
**   Input:   a = a square matrix
              n = matrix size
**   Output:  a = matrix that contains elements of L and U matrices
			  indx = vector that records the row permutation effected 
					 by the partial pivoting 
			  d = ±1 depending on whether the number of row interchanges 
			      was even or odd, respectively   		 
**   Purpose: LU decomposition of a rowwise permutation of a matrix. 
**-------------------------------------------------------------------
*/
{
	int i,imax,j,k;
	float big,dum,sum,temp;

	*d = 1.0; /*No row interchanges yet.*/

	for (i = 1; i <= n; i++) 
	{ 
		/*Loop over rows to get the implicit scaling information.*/
		big = 0.0;
		for (j = 1;j <= n;j++)
			if ((temp = fabs(a[i][j])) > big) big = temp;
		if (big == 0.0) 
			return 7;  /* Warning for singular matrix*/
		/*No nonzero largest element.*/
		vv[i] = 1.0/big; /*Save the scaling.*/
	}
	for (j = 1;j <= n;j++) /**for each column*/
	{ 
		/*This is the loop over columns of Crout’s method.*/
		for (i = 1;i < j;i++) 
		{ 
			/*Up from the diagonal*/
			sum = a[i][j];
			for (k = 1;k < i;k++) 
				sum -= a[i][k]*a[k][j];
			a[i][j] = sum;
		}
		big = 0.0; /*Initialize for the search for largest pivot element.*/
		for (i = j;i <= n;i++) 
		{ 
			sum = a[i][j];
			for (k = 1;k < j;k++)
				sum -= a[i][k]*a[k][j];
			a[i][j] = sum;
			if ( (dum = vv[i]*fabs(sum)) >= big) 
			{
				big = dum;
				imax = i;
			}
		}
		if (j != imax) 
		{
			/*Do we need to interchange rows?*/
			for (k = 1;k <= n;k++) 
			{ 
				/*Yes,do so...*/
				dum = a[imax][k];
				a[imax][k] = a[j][k];
				a[j][k] = dum;
			}
			*d = -(*d); /* change the parity of d */
			vv[imax] = vv[j]; /* interchange the scale factor.*/
		} 
		indx[j] = imax;
		if (a[j][j] == 0.0) a[j][j] = TINY1;

		if (j != n) /* divide by the pivot element.*/
		{ 
			dum = 1.0/(a[j][j]);
			for (i = j+1;i <= n;i++) a[i][j] *= dum;	
		}
	} 

	return 0;
}

/*Solves the set of n linear equations A · X = B */
void lubksb(double **a, int n, int *indx, double b[])
/*
**-----------------------------------------------------------------------
**   Input:   a = a square matrix, LU decomposition of A that is returned
				  by ludcmp
              n = matrix size
			  indx = permutation vector returned by ludcmp
			  b = right-hand side vector B,
**   Output:  b = solution vector X  		 
**   Purpose: Solve linear equations AX = B after LU decomposition of A 
**------------------------------------------------------------------------
*/
{
	int i,ii=0,ip,j;
	float sum;
	
	/*forward substitution */
	for (i=1;i<=n;i++) 
	{ 
		ip=indx[i];
		sum=b[ip];
		b[ip]=b[i];
		if (ii)
			for (j=ii;j<=i-1;j++) 
				sum -= a[i][j]*b[j];
		else if (sum) ii=i; 
		b[i]=sum;
	}
	
	/* back substitution */
	for (i=n;i>=1;i--) 
	{ 
		sum=b[i];
		for (j=i+1;j<=n;j++) 
			sum -= a[i][j]*b[j];
		b[i]=sum/a[i][i]; 
	}
}


void usrfun(PExprList * algebraicexpression_list, float *x,int n,double *fvec,double **fjac)
/*
**-----------------------------------------------------------------------
**   Input:   algebraicexpression = vector of algebraic equation expressions
			  x = vector of function variables
              n = vector size
**   Output:  fvec = vector of function values
              fjac = Jacobian matrix  		 
**   Purpose: supplies function values at x in fvec and Jacobian matrix 
              in fjac. 
**------------------------------------------------------------------------
*/
{ 
	/* function evaluation */
	vecfunc(n, algebraicexpression_list, x, fvec);

	/* Jacobian matrix */
	fdjac(n, algebraicexpression_list, x, fvec, fjac, vecfunc);
}


void fdjac(int n, PExprList * algebraicexpression_list, float x[], double fvec[], double **df,void (*vecfunc)(int, PExprList *, float [], double []))
/*
**-----------------------------------------------------------------------
**   Input:   n = vector size
			  x = vector of function variables
              algebraicexpression = vector of algebraic equation expressions
			  fvec = vector of function values at x
			  vecfunc = user supplied routine that returns the function
			            values at x
**   Output:  fjac = Jacobian matrix  		 
**   Purpose: computes Jacobian matrix at x 
**------------------------------------------------------------------------
*/
{
	int i,j;
	float temp, h;
	for (j=1;j<=n;j++) 
	{
		temp=x[j];
		h=EPS*fabs(temp);
		if (h == 0.0) h=EPS;
		x[j]=temp+h;
		h=x[j]-temp;
		vecfunc(n,algebraicexpression_list, x,f);
		x[j]=temp;
		/* forward difference formula*/
		for (i=1;i<=n;i++) 
			df[i][j]=(f[i]-fvec[i])/h;  
	}
}


void vecfunc(int n, PExprList * algebraicexpression_list, float x[], double fvec[])
/*
**-----------------------------------------------------------------------
**   Input:   n = vector size
			  algebraicexpression_list = vector of algebraic equation expressions
			  x = vector of function variables
	 Output:  fvec = vector of function values at x
**   Purpose: computes function values at x 
**------------------------------------------------------------------------
*/
{
	int i;
	for ( i = 1; i < n+1; i++)
		fvec[i] = mathexprlist_eval(algebraicexpression_list[i]);
}