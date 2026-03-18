//-----------------------------------------------------------------------------
//   odesolve.c
//
//   Fifth-order Runge-Kutta integration with adaptive step size control
//   based on code from Numerical Recipes in C (Cambridge University
//   Press, 1992).
//
//   Date:     11/2/01
//   Author:   L. Rossman
//-----------------------------------------------------------------------------
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hash.h"
#include "text.h"
#include "mstext.h"
#include "types.h"
#include "mstypes.h"
#include "funcs.h"
#include "msfuncs.h"

#include "odesolve.h"
#define MAXSTP 10000
#define TINY2  1.0e-30
#define SAFETY 0.9
#define PGROW  -0.2
#define PSHRNK -0.25
#define ERRCON 1.89e-4    // = (5/SAFETY)^(1/PGROW)
#define FLT_MAX 3.402823466e+38F
//-----------------------------------------------------------------------------
//    Local declarations
//-----------------------------------------------------------------------------
int      nmax;      // max. number of equations
float*   y;         // dependent variable
float*   yscal;     // scaling factors
float*   yerr;      // integration errors
float*   ytemp;     // temporary values of y
float*   dydx;      // derivatives of y
float*   ak;        // derivatives at intermediate points


// function that integrates over an error-controlled stepsize
int rkqs(PExprTree * differentialexpression, float* x, int n, float htry, float eps, 
		 float* hdid,float* hnext, void (*derivs)(PExprTree *,int, float, float*, float*));
int rkqs2(PExprList * differentialexpression, float* x, int n, float htry, float eps, 
		 float* hdid,float* hnext, void (*derivs)(PExprList *,int, float, float*, float*));
int rkqs3(PExprList * differentialexpression, float* x, int n, float htry, float reltol, float *abstol,
		 float* hdid,float* hnext, void (*derivs)(PExprList *,int, float, float*, float*));

// function that performs the Runge-Kutta integration step
void rkck(PExprTree * differentialexpression, float x, int n, float h, 
		  void (*derivs)(PExprTree *,int, float, float*, float*));
void rkck2(PExprList * differentialexpression, float x, int n, float h, 
		  void (*derivs)(PExprList *,int, float, float*, float*));


int odesolve_open(int n)
/*
**-----------------------------------------------------------------------
**   Input:   n = number of ordinary differential (rate) equations 
**   Output:  none 
**   Purpose: open the ODE solver to solve system of n equations
	 (return 0 if successful, 101 if not)
**-----------------------------------------------------------------------
*/
{
    nmax  = 0;
    y     = (float *) calloc(n+1, sizeof(float));
    yscal = (float *) calloc(n+1, sizeof(float));
    dydx  = (float *) calloc(n+1, sizeof(float));
    yerr  = (float *) calloc(n+1, sizeof(float));
    ytemp = (float *) calloc(n+1, sizeof(float));
    ak    = (float *) calloc(5*(n+1), sizeof(float));
    if ( !y || !yscal || !dydx || !yerr || !ytemp || !ak ) return 101;
    nmax = n;
    return 0;
}


void odesolve_close()
/*
**-----------------------------------------------------------------------
**   Input:   none
**   Output:  none 
**   Purpose: close the ODE solver
**-----------------------------------------------------------------------
*/

{
    if ( y ) free(y);
    y = NULL;
    if ( yscal ) free(yscal);
    yscal = NULL;
    if ( dydx ) free(dydx);
    dydx = NULL;
    if ( yerr ) free(yerr);
    yerr = NULL;
    if ( ytemp ) free(ytemp);
    ytemp = NULL;
    if ( ak ) free(ak);
    ak = NULL;
    nmax = 0;
}


int odesolve_integrate(PExprList * differentialexpression, float ystart[], int n, float x1, float x2,
      float reltol, float *abstol, float h1, void (*derivs)(PExprList *,int, float, float*, float*))
/*
**--------------------------------------------------------------------------------
**   Input:   differentioalexpression = vector of rate expressions 
			  ystart = vector of starting specie concentrations
			  n = size of the vector
			  x1 = integration start time
			  x2 = integration end time 
			  reltol = relative integration accuracy tolerance
              abstol[] = component-wise absolute accuracy tolerances
			  h1 = initial integration stepsize guess
			  derivs = user-supplied function that computes derivatives dy/dx
**   Output:  ystart = new values of y (at the end of the integration interval) 
**   Purpose: Driver function for Runge-Kutta integration with adaptive 
			  stepsize control
**---------------------------------------------------------------------------------
*/
{
    int   i, errcode, nstp;
    float hdid, hnext;
    float x = x1;
    float h = h1;
    if (nmax < n) return 1;
    for (i=1; i<n+1; i++) y[i] = ystart[i];
    for (nstp=1; nstp<=MAXSTP; nstp++)
    {
        derivs(differentialexpression, n, x, y, dydx); 
        for (i=1; i<n+1; i++)
            yscal[i] = fabs(y[i]) + fabs(dydx[i]*h) + TINY2; //Scaling sed to monitor accuracy.
        if ((x+h-x2)*(x+h-x1) > 0.0) h = x2 - x;	//If stepsize can overshoot,decrease.
        errcode = rkqs3(differentialexpression, &x,n,h,reltol,abstol,&hdid,&hnext,derivs);
        if (errcode) break;
        if ((x-x2)*(x2-x1) >= 0.0)
        {
            for (i=1; i<n+1; i++) 
				ystart[i] = y[i];	//Save final results
            return 0;			//Normal exist
        }
        if (fabs(hnext) <= 0.0) return 2;	//Negative step size
        h = hnext;
    }
    return 3;	// Too many steps 
}

int rkqs(PExprTree * differentialexpression, float* x, int n, float htry, float eps, float* hdid,
         float* hnext, void (*derivs)(PExprTree *, int, float, float*, float*))
/*
**--------------------------------------------------------------------------------
**   Input:   differentioalexpression = vector of rate expressions 
			  x = current time
			  n = size of the vector
			  htry = trial step size
			  eps = integration accuracy
			  derivs = user-supplied function that computes derivatives dy/dx
**   Output:  x = updated
			  hdid = stepsize taken
			  hnext = estimated next stepsize
			  y = updated concentration
**   Purpose: Fifth-order Runge-Kutta integration step with monitoring of local 
			  truncation error to assure accuracy and adjust stepsize. 
**---------------------------------------------------------------------------------
*/
{
    int i;
    float err, errmax, h, htemp, xnew, xold = *x;

    // --- set initial stepsize
    h = htry;
    for (;;)
    {
        // --- take a Runge-Kutta-Cash-Karp step
        rkck(differentialexpression, xold, n, h, derivs);

        // --- compute scaled maximum error
        errmax = 0.0;
        for (i=1; i<n+1; i++)
        {
            err = fabs(yerr[i]/yscal[i]);
            if (err > errmax) errmax = err;
        }
        errmax /= eps;

        // --- error too large; reduce stepsize & repeat
        if (errmax > 1.0)
        {
            htemp = SAFETY*h*pow(errmax,PSHRNK);
            if (h >= 0)
            {
                if (htemp > 0.1*h) h = htemp;
                else h = 0.1*h;
            }
            else
            {
                if (htemp < 0.1*h) h = htemp;
                else h = 0.1*h;
            }
            xnew = xold + h;
            if (xnew == xold) return 2;  //Stepsize underflow
            continue;
        }
        // --- step succeeded; compute size of next step
        else
        {
            if (errmax > ERRCON) *hnext = SAFETY*h*pow(errmax,PGROW);
            else *hnext = 5.0*h;
            *x += (*hdid=h);
            for (i=1; i<n+1; i++) y[i] = ytemp[i];
            return 0;
        }
    }
}
int rkqs2(PExprList * differentialexpression, float* x, int n, float htry, float eps, float* hdid,
         float* hnext, void (*derivs)(PExprList *, int, float, float*, float*))
/*
**--------------------------------------------------------------------------------
**   Input:   differentioalexpression = vector of rate expressions 
			  x = current time
			  n = size of the vector
			  htry = trial step size
			  eps = integration accuracy
			  derivs = user-supplied function that computes derivatives dy/dx
**   Output:  x = updated
			  hdid = stepsize taken
			  hnext = estimated next stepsize
			  y = updated concentration
**   Purpose: Fifth-order Runge-Kutta integration step with monitoring of local 
			  truncation error to assure accuracy and adjust stepsize. 
**---------------------------------------------------------------------------------
*/
{
    int i;
    float err, errmax, h, htemp, xnew, xold = *x;

    // --- set initial stepsize
    h = htry;
    for (;;)
    {
        // --- take a Runge-Kutta-Cash-Karp step
        rkck2(differentialexpression, xold, n, h, derivs);

        // --- compute scaled maximum error
        errmax = 0.0;
        for (i=1; i<n+1; i++)
        {
            err = fabs(yerr[i]/yscal[i]);
            if (err > errmax) errmax = err;
        }
        errmax /= eps;

        // --- error too large; reduce stepsize & repeat
        if (errmax > 1.0)
        {
            htemp = SAFETY*h*pow(errmax,PSHRNK);
            if (h >= 0)
            {
                if (htemp > 0.1*h) h = htemp;
                else h = 0.1*h;
            }
            else
            {
                if (htemp < 0.1*h) h = htemp;
                else h = 0.1*h;
            }
            xnew = xold + h;
            if (xnew == xold) return 2;  //Stepsize underflow
            continue;
        }
        // --- step succeeded; compute size of next step
        else
        {
            if (errmax > ERRCON) *hnext = SAFETY*h*pow(errmax,PGROW);
            else *hnext = 5.0*h;
            *x += (*hdid=h);
            for (i=1; i<n+1; i++) y[i] = ytemp[i];
            return 0;
        }
    }
}

int rkqs3(PExprList * differentialexpression, float* x, int n, float htry, float reltol, float *abstol,
          float* hdid, float* hnext, void (*derivs)(PExprList *, int, float, float*, float*))
/*
**--------------------------------------------------------------------------------
**   Input:   differentioalexpression = vector of rate expressions 
			  x = current time
			  n = size of the vector
			  htry = trial step size
			  reltol = relative integration accuracy tolerance
              abstol[] = element-specific absolute integration accuracy tolerances
			  derivs = user-supplied function that computes derivatives dy/dx
**   Output:  x = updated
			  hdid = stepsize taken
			  hnext = estimated next stepsize
			  y = updated concentration
**   Purpose: Fifth-order Runge-Kutta integration step with monitoring of local 
			  truncation error to assure accuracy and adjust stepsize. 
**---------------------------------------------------------------------------------
*/
{
    int i;
    float avgy, err, errmax, h, htemp, xnew, xold = *x;

    // --- set initial stepsize
    h = htry;
    for (;;)
    {
        // --- take a Runge-Kutta-Cash-Karp step
        rkck2(differentialexpression, xold, n, h, derivs);

        //  Error management based on typical relative error control
        //  modified by component-specific absolute error tolerance.
        //  Based on rksuite STEP subroutine (www.netlib.org)
        //
        //  The error in a solution component is measured relative to a weight
        //  that is the larger of an absolute tolerance and the solution size over
        //  the step.  Using the magnitude of a solution component at both ends
        //  of the step in the definition of "size" increases the robustness of
        //  the test.

        // --- compute component weights, yscal[]
        for (i=1; i<n+1; i++)
        {
            avgy = 0.5*(fabs(y[i]) + fabs(ytemp[i]));
            yscal[i] = avgy>abstol[i] ? avgy : abstol[i];
        }

        // --- compute scaled maximum error

		if (avgy > FLT_MAX)
			errmax = 1000;
		else
		{
			errmax = 0.0;
			for (i=1; i<n+1; i++)
			{
				err = fabs(yerr[i]/yscal[i]);
				if (err > errmax) errmax = err;
			}
			errmax /= reltol;
		}

        // --- error too large; reduce stepsize & repeat
        if (errmax > 1.0)
        {
            htemp = SAFETY*h*pow(errmax,PSHRNK);
            if (h >= 0)
            {
                if (htemp > 0.1*h) h = htemp;
                else h = 0.1*h;
            }
            else
            {
                if (htemp < 0.1*h) h = htemp;
                else h = 0.1*h;
            }
            xnew = xold + h;
            if (xnew == xold) return 2;  //Stepsize underflow
            continue;
        }
        // --- step succeeded; compute size of next step
        else
        {
            if (errmax > ERRCON) *hnext = SAFETY*h*pow(errmax,PGROW);
            else *hnext = 5.0*h;
            *x += (*hdid=h);
            for (i=1; i<n+1; i++) y[i] = ytemp[i];
            return 0;
        }
    }
}

void rkck(PExprTree * differentialexpression, float x, int n, float h, void (*derivs)(PExprTree *, int, float, float*, float*))
//----------------------------------------------------------------------
//   Uses the Runge-Kutta-Cash-Karp method to advance y[] at x
//   over stepsize h.
//----------------------------------------------------------------------
/*
**--------------------------------------------------------------------------------
**   Input:   differentioalexpression = vector of rate expressions 
			  x = current time
			  n = size of the vector
			  h = numerical integration stepsize 
			  derivs = user-supplied function that computes derivatives dy/dx
**   Output:  yerr  = estimated local truncation error
			  ytemp = y advanced after Runge-Kutta-Cash-Karp method	 
**   Purpose: Uses the Runge-Kutta-Cash-Karp method to advance y[] at x 
			  over stepsize h.
**---------------------------------------------------------------------------------
*/
{
    float a2=0.2, a3=0.3, a4=0.6, a5=1.0, a6=0.875,
          b21=0.2, b31=3.0/40.0, b32=9.0/40.0, b41=0.3, b42= -0.9, b43=1.2,
          b51= -11.0/54.0, b52=2.5, b53= -70.0/27.0, b54=35.0/27.0,
          b61=1631.0/55296.0, b62=175.0/512.0, b63=575.0/13824.0,
          b64=44275.0/110592.0, b65=253.0/4096.0, c1=37.0/378.0,
          c3=250.0/621.0, c4=125.0/594.0, c6=512.0/1771.0,
          dc5= -277.0/14336.0;
    float dc1=c1-2825.0/27648.0, dc3=c3-18575.0/48384.0,
          dc4=c4-13525.0/55296.0, dc6=c6-0.25;
    int i;
    float *ak2 = (ak);
    float *ak3 = ((ak)+(n+1));
    float *ak4 = ((ak)+(2*(n+1)));
    float *ak5 = ((ak)+(3*(n+1)));
    float *ak6 = ((ak)+(4*(n+1)));

    for (i=1; i<n+1; i++)				//First step
        ytemp[i] = y[i] + b21*h*dydx[i];

    derivs(differentialexpression, n, x+a2*h,ytemp,ak2);	//Second step
    for (i=1; i<n+1; i++)
        ytemp[i] = y[i] + h*(b31*dydx[i]+b32*ak2[i]);

    derivs(differentialexpression, n, x+a3*h,ytemp,ak3);	//Third step
    for (i=1; i<n+1; i++)
        ytemp[i] = y[i] + h*(b41*dydx[i]+b42*ak2[i] + b43*ak3[i]);

    derivs(differentialexpression, n, x+a4*h,ytemp,ak4);	//Fourth step
    for (i=1; i<n+1; i++)
        ytemp[i] = y[i] + h*(b51*dydx[i]+b52*ak2[i] + b53*ak3[i] + b54*ak4[i]);

    derivs(differentialexpression, n, x+a5*h,ytemp,ak5);	//Fifth step
    for (i=1; i<n+1; i++)
        ytemp[i] = y[i] + h*(b61*dydx[i]+b62*ak2[i] + b63*ak3[i] + b64*ak4[i]
                   + b65*ak5[i]);

    derivs(differentialexpression, n, x+a6*h,ytemp,ak6);	//Sixth step
    for (i=1; i<n+1; i++)			//Accumulate increments with proper weights.	
        ytemp[i] = y[i] + h*(c1*dydx[i] + c3*ak3[i] + c4*ak4[i] + c6*ak6[i]);

	//Estimate error as di .erence between fourth and .fth order methods.
    for (i=1; i<n+1; i++)
        yerr[i] = h*(dc1*dydx[i] +dc3*ak3[i] + dc4*ak4[i] + dc5*ak5[i] + dc6*ak6[i]);
}

void rkck2(PExprList * differentialexpression, float x, int n, float h, void (*derivs)(PExprList *, int, float, float*, float*))
//----------------------------------------------------------------------
//   Uses the Runge-Kutta-Cash-Karp method to advance y[] at x
//   over stepsize h.
//----------------------------------------------------------------------
/*
**--------------------------------------------------------------------------------
**   Input:   differentioalexpression = vector of rate expressions 
			  x = current time
			  n = size of the vector
			  h = numerical integration stepsize 
			  derivs = user-supplied function that computes derivatives dy/dx
**   Output:  yerr  = estimated local truncation error
			  ytemp = y advanced after Runge-Kutta-Cash-Karp method	 
**   Purpose: Uses the Runge-Kutta-Cash-Karp method to advance y[] at x 
			  over stepsize h.
**---------------------------------------------------------------------------------
*/
{
    float a2=0.2, a3=0.3, a4=0.6, a5=1.0, a6=0.875,
          b21=0.2, b31=3.0/40.0, b32=9.0/40.0, b41=0.3, b42= -0.9, b43=1.2,
          b51= -11.0/54.0, b52=2.5, b53= -70.0/27.0, b54=35.0/27.0,
          b61=1631.0/55296.0, b62=175.0/512.0, b63=575.0/13824.0,
          b64=44275.0/110592.0, b65=253.0/4096.0, c1=37.0/378.0,
          c3=250.0/621.0, c4=125.0/594.0, c6=512.0/1771.0,
          dc5= -277.0/14336.0;
    float dc1=c1-2825.0/27648.0, dc3=c3-18575.0/48384.0,
          dc4=c4-13525.0/55296.0, dc6=c6-0.25;
    int i;
    float *ak2 = (ak);
    float *ak3 = ((ak)+(n+1));
    float *ak4 = ((ak)+(2*(n+1)));
    float *ak5 = ((ak)+(3*(n+1)));
    float *ak6 = ((ak)+(4*(n+1)));

    for (i=1; i<n+1; i++)				//First step
        ytemp[i] = y[i] + b21*h*dydx[i];

    derivs(differentialexpression, n, x+a2*h,ytemp,ak2);	//Second step
    for (i=1; i<n+1; i++)
        ytemp[i] = y[i] + h*(b31*dydx[i]+b32*ak2[i]);

    derivs(differentialexpression, n, x+a3*h,ytemp,ak3);	//Third step
    for (i=1; i<n+1; i++)
        ytemp[i] = y[i] + h*(b41*dydx[i]+b42*ak2[i] + b43*ak3[i]);

    derivs(differentialexpression, n, x+a4*h,ytemp,ak4);	//Fourth step
    for (i=1; i<n+1; i++)
        ytemp[i] = y[i] + h*(b51*dydx[i]+b52*ak2[i] + b53*ak3[i] + b54*ak4[i]);

    derivs(differentialexpression, n, x+a5*h,ytemp,ak5);	//Fifth step
    for (i=1; i<n+1; i++)
        ytemp[i] = y[i] + h*(b61*dydx[i]+b62*ak2[i] + b63*ak3[i] + b64*ak4[i]
                   + b65*ak5[i]);

    derivs(differentialexpression, n, x+a6*h,ytemp,ak6);	//Sixth step
    for (i=1; i<n+1; i++)			//Accumulate increments with proper weights.	
        ytemp[i] = y[i] + h*(c1*dydx[i] + c3*ak3[i] + c4*ak4[i] + c6*ak6[i]);

	//Estimate error as di .erence between fourth and .fth order methods.
    for (i=1; i<n+1; i++)
        yerr[i] = h*(dc1*dydx[i] +dc3*ak3[i] + dc4*ak4[i] + dc5*ak5[i] + dc6*ak6[i]);
}