/*
**************************************************************************
                                                                   
FUNCS.H -- Function Prototypes for Newton-Raphson Algorithm                       
                                                                   
VERSION:    1.00
DATE:       4/13/05
AUTHOR:                                                              
**************************************************************************
*/

//-----------------------------------------------------------------------------
//   Call this function to perform Mewton-Raphson appraoch to sole
//   a set of nonliner algebraic equations
//-----------------------------------------------------------------------------
void mnewt(int reactiontype, int ntrial, PExprList * algebraicexpression, float x[], int n, float tolx);

//-----------------------------------------------------------------------------
//   Use this function to open the algebraic solver to solve a system
//   of n equations (returns 1 if successful, 0 if not)
//-----------------------------------------------------------------------------
int  newton_open(int n);

//-----------------------------------------------------------------------------
//   Use this function to close the algebraic solver
//-----------------------------------------------------------------------------
void newton_close(int n);
