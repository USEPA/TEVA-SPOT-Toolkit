//-----------------------------------------------------------------------------
// odesolve.h
//
// Header file for ODE solver contained in odesolve.c
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//   Use this function to open the ODE solver to solve a system
//   of n equations (returns 1 if successful, 0 if not)
//-----------------------------------------------------------------------------
int  odesolve_open(int n);

//-----------------------------------------------------------------------------
//   Use this function to close the ODE solver
//-----------------------------------------------------------------------------
void odesolve_close(void);

//-----------------------------------------------------------------------------
//   Call this function to perform a Runge-Kutta integration with
//   adaptive stepsize control. It integrates the n starting values
//   in ystart[] from x1 to x2 with accuracy eps. h1 is the initial
//   stepsize guess and derivs is a user-supplied function that computes
//   derivatives dy/dx of y. On completion, ystart[] contains the
//   new values of y at the end of the integration interval.
//-----------------------------------------------------------------------------

int  odesolve_integrate(PExprList *, float ystart[], int n, float x1, float x2, float reltol, 
						float *abstol, float h1, void (*derivs)(PExprList*, int, float, float*, float*));

