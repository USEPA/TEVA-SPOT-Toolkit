/* MATHEXPR.H
**
** Header file for Math Expression module MATHEXPR.C
**
** Last Updated: 04/13/05
**
*/

/* Operand codes:
   	1 = (
   	2 = )
   	3 = +
   	4 = - (subtraction)
   	5 = *
   	6 = /
   	7 = number
   	8 = concentration
   	9 = parameter
  	10 = - (negative)
  	11 = cos
  	12 = sin
  	13 = tan
  	14 = cot
  	15 = abs
  	16 = sgn
  	17 = sqrt
  	18 = log
  	19 = exp
  	20 = asin
  	21 = acos
  	22 = atan
  	23 = acot
 	24 = sinh
  	25 = cosh
  	26 = tanh
  	27 = coth
  	28 = log10
  	31 = ^

	41 = diameter
	42 = mass transfer coefficient
	43 = velocity
	44 = time
	45 = Reynolds number
*/

//-----------------------------------------------------------------------------
//   This function creates a tokenized expression tree from a string s
//-----------------------------------------------------------------------------
ExprTree* mathexpr_create(char* s, int (*getVar) (char *));

//-----------------------------------------------------------------------------
//   This function evaluates an expression tree
//-----------------------------------------------------------------------------
double mathexprlist_eval(ExprList* l);

//-----------------------------------------------------------------------------
//   This function deletes a tokenized expression tree
//-----------------------------------------------------------------------------
void  mathexprlist_delete(ExprList* l);

//-----------------------------------------------------------------------------
//   This function converts expression in binary tree format to post fix format
//-----------------------------------------------------------------------------
void traversaltree(ExprTree* t, ExprList** l);

//-----------------------------------------------------------------------------
//   This function creats for mathematical expression evaluation
//-----------------------------------------------------------------------------
void buildstack(int n);