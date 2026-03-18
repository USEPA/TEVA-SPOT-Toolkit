// CHEMSIM
//
// Multi-species chemical reaction simulator
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
#include "odesolve.h"
#include "newton.h"
//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

//PExprTree * algebraicexpression;
PExprList * algebraicexpression_list;

//-----------------------------------------------------------------------------
// Function Declarations
//-----------------------------------------------------------------------------

// compute time derivative of  each rate specie
void getDcDt(PExprList * differentialexpression, int n, float t, float c[], float dcdt[]);
//-----------------------------------------------------------------------------
// Perform time simulation of chemical reactions
//-----------------------------------------------------------------------------

void differentialSolution(int reactiontype, float * msc, float v, float dtfloat)
/*
**-------------------------------------------------------------------
**   Input:   reactiontype = type of reaction (in pipe or tank)
			  msc = curret WQ in a segment
			  v = segment volume
			  dtfloat = time step 
**   Output:  none                                          
**   Purpose: computer new quality in a segment for differential
              species after reaction occurs
**-------------------------------------------------------------------
*/
{
    int i, n, odeflag;	
	PExprTree * differentialexpression;
	PExprList * differentialexpressionlist;
    float *abstol;

	if (reactiontype == PIPES)
	{
		n = Npiperate;
		differentialexpression = PipeDA;
		differentialexpressionlist = PipeDAList;
        abstol = PSabstol;
	}
	else
	{
		n = Ntankrate;
		differentialexpression = TankDA;
		differentialexpressionlist = TankDAList;
        abstol = TSabstol;
	}
            
	//Call integrator routine

	odeflag = odesolve_integrate(differentialexpressionlist, msc, n, 0, dtfloat, 
        MSreltol, abstol, dtfloat, getDcDt);
	// Don't allow concentration to go negative
//	for (i = 1; i < n+1; i++)
//	{
//		if ( msc[i] < 0.0 ) msc[i] = 0.0;
//	}         
	
	for ( i = 1; i < n+1; i++)
			CurrentConcen[i] =  msc[i];

}    
  
void algebraicSolution(int reactiontype, float * msc)
/*
**-------------------------------------------------------------------
**   Input:   reactiontype = type of reaction (in pipe or tank)
			  msc = curret WQ in pipe segment
**   Output:  none                                          
**   Purpose: computer new quality in a segment for algebraic
              species after reaction occurs
**-------------------------------------------------------------------
*/
{
    int i, n;
	
	CurrentConcen = msc;
	if (reactiontype == PIPES)
	{
		n = Npipedef;
		/*Evaluate definition expressions in pipe dynamics*/
		for ( i = 1; i < n+1; i++)
		{
			msc[i+Npiperate] = mathexprlist_eval(PipeDAList[i+Npiperate]);
		}
		
		/*Equation expressions in pipe dynamics*/
		algebraicexpression_list = &(PipeDAList[Npiperate+Npipedef]);
		
		/*Newton method to solve algebraic equations in pipe dynamics*/
		if (Npipeequ > 0 )
		{
			mnewt(PIPES, 100, algebraicexpression_list, &(msc[Npiperate+Npipedef]), Npipeequ, MSreltol);
			if (Warnflag == 7) return;
		}
		/*Don't allow concentration to go negative*/
		for ( i = Npiperate+1; i < Npiperate+Npipedef+Npipeequ+1; i++)
			if (msc[i] < 0) 
				msc[i] = 0.0; 
	}
	else
	{
		n = Ntankdef;
		/*Evaluate definition expressions in tank dynamics*/
		for ( i = 1; i < n+1; i++)
		{
			msc[i+Ntankrate] = mathexprlist_eval(TankDAList[i+Ntankrate]);
		}

		/*Equation expressions in tank dynamics*/
		algebraicexpression_list = &(TankDAList[Ntankrate+Ntankdef]);	
		/*Newton method to solve algebraic equations in tank dynamics*/
		if (Ntankequ > 0)
		{
			mnewt(TANKS, 100, algebraicexpression_list, &(msc[Ntankrate+Ntankdef]), Ntankequ, MSreltol);
			if (Warnflag == 7) return;
		}
		
		/*Don't allow concentration to go negative*/
		for ( i = Ntankrate+1; i < Ntankrate+Ntankdef+Ntankequ+1; i++)
			if (msc[i] < 0) 
				msc[i] = 0.0; 
	}
}    

//-----------------------------------------------------------------------------
// Find time derivative (reaction rate) of differential species
//-----------------------------------------------------------------------------             

void getDcDt(PExprList * differentialexpression, int n, float t, float c[], float * dydx)
/*
**-----------------------------------------------------------------------------------------
**   Input:   differentialexpression = vector of ordinary differential equation expressions
			  n = vector size
              t = time
			  c = curret WQ for differential species in a segment
**   Output:  dy/dx = reaction rate                                          
**   Purpose: computer reaction rate for differential species
**-----------------------------------------------------------------------------------------
*/
{
    int i;

	/*update rate concentration*/
	for ( i = 1; i < n+1; i++)
		CurrentConcen[i] =  c[i];

	/*update algebraic concentration*/
//	algebraicSolution(reactiontype, c);
	/*Rate expressions evaluation*/

	for (i = 1; i < n+1; i++)
		dydx[i] = mathexprlist_eval(differentialexpression[i]);
} 