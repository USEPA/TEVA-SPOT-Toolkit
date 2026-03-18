// --- mathexpr.c
//
//   Evaluates symbolic mathematical expression consisting
//   of numbers, variable names, math functions & arithmetic
//   operators.
//
//   Date:     6/16/03
//             11/22/03
//			   04/13/05	
//   Author:   L. Rossman
//			   F. Shang
//			   J. Uber	
//-----------------------------------------------------------------------------

#include <malloc.h>
#include <string.h>
#include <stdio.h>
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
#define M_PI_2      1.57079632679489661923      
/* --- Function Names */
char *MathFunc[] = {"COS", "SIN", "TAN", "COT", "ABS", "SGN",
                    "SQRT", "LOG", "EXP", "ASIN", "ACOS", "ATAN",
                    "ACOT", "SINH", "COSH", "TANH", "COTH", "LOG10", NULL};

/* --- Local Variables */
static int    Err;
static int    Bc;
static int    PrevLex, CurLex;
static int    Len, Pos;
static char   *S;
static char   Token[255];
static int    Ivar;
double *     Exprst;
static double Fvalue;

/* --- Function declarations */
int     isDigit(char);
int     isLetter(char);
void    getToken(void);
int     getMathFunc(void);
int     getVariable(void);
int     getOperand(void);
int     getLex(void);
double  getNumber(void);

ExprTree*  newNode(void);
ExprTree*  getSingleOp(int *);
ExprTree*  getOp(int *);
ExprTree*  getTree(void);

/* --- Callback functions */
static int    (*getSpecieIndex) (char *);    // return index of named variable

int isDigit(char c)
{
    if (c >= '1' && c <= '9') return 1;
    if (c == '0') return 1;
    return 0;
}

int isLetter(char c)
{
    if (c >= 'a' && c <= 'z') return 1;
    if (c >= 'A' && c <= 'Z') return 1;
    if (c == '_') return 1;
    return 0;
}

void getToken()
{
    char c[] = " ";
    strcpy(Token, "");
    while ( Pos <= Len &&
        ( isLetter(S[Pos]) || isDigit(S[Pos]) ) )
    {
        c[0] = S[Pos];
        strcat(Token, c);
        Pos++;
    }
    Pos--;
}

int getMathFunc()
{
    int i = 0;
    while (MathFunc[i] != NULL)
    {
        if (strnicmp(MathFunc[i], Token, 6) == 0) return i+11;  //strncmpi case insensitive
        i++;
    }
    return(0);
}

int getVariable()
{
    if ( !getSpecieIndex ) return 0;
    Ivar = getSpecieIndex(Token);
    if (Ivar > 0) return 8;
	Ivar = findparameterID(Token);
	if (Ivar > 0) return 9;
	Ivar = findconstantID(Token);
	if (Ivar > 0) 
	{
		Fvalue = Constant[Ivar].value;
		return 7;
	}
	Ivar = findcoefficientID(Token);
	if (Ivar > 0) return 42;
	if (strcmpi(Token, "Diameter")==0) return 41;
	if (strcmpi(Token, "Velocity")==0) return 43;
    if (strcmpi(Token, "Time")==0) return 44;
	if (strcmpi(Token, "Reynolds")==0) return 45;

    return 0;
}

double getNumber()
{
    char c[] = " ";
    char sNumber[255];
    int  errflag = 0;

    /* --- get whole number portion of number */
    strcpy(sNumber, "");
    while (Pos < Len && isDigit(S[Pos]))
    {
        c[0] = S[Pos];
        strcat(sNumber, c);
        Pos++;
    }

    /* --- get fractional portion of number */
    if (Pos < Len)
    {
        if (S[Pos] == '.')
        {
            strcat(sNumber, ".");
            Pos++;
            while (Pos < Len && isDigit(S[Pos]))
            {
                c[0] = S[Pos];
                strcat(sNumber, c);  
                Pos++;
            }
        }

        /* --- get exponent */
        if (Pos < Len && (S[Pos] == 'e' || S[Pos] == 'E'))
        {
            strcat(sNumber, "E");  
            Pos++;
            if (Pos >= Len) errflag = 1;
            else
            {
                if (S[Pos] == '-' || S[Pos] == '+')
                {
                    c[0] = S[Pos];
                    strcat(sNumber, c);  
                    Pos++;
                }
                if (Pos >= Len || !isDigit(S[Pos])) errflag = 1;
                else while ( Pos < Len && isDigit(S[Pos]))
                {
                    c[0] = S[Pos];
                    strcat(sNumber, c);  
                    Pos++;
                }
            }
        }
    }
    Pos--;
    if (errflag) return 0;
    else return atof(sNumber);
}

int getOperand()
{
    int code;
    switch(S[Pos])
    {
      case '(': code = 1;  break;
      case ')': code = 2;  break;
      case '+': code = 3;  break;
      case '-': code = 4;
                if (Pos < Len-1 &&
                    isDigit(S[Pos+1]) &&
                        (CurLex == 0 || CurLex == 1))
                {
                    Pos++;
                    Fvalue = -getNumber();
                    code = 7;
                }
                break;
      case '*': code = 5;  break;
      case '/': code = 6;  break;
      case '^': code = 31; break;
      default:  code = 0;
    }
    return code;
}

int getLex()
{
    int n;

    /* --- skip spaces */
    while ( Pos < Len && S[Pos] == ' ' ) Pos++;
    if ( Pos >= Len ) return 0;

    /* --- check for operand */
    n = getOperand();

    /* --- check for function/variable/number */
    if ( n == 0 )
    {
        if ( isLetter(S[Pos]) )
        {
            getToken();
            n = getMathFunc();
            if ( n == 0 ) n = getVariable();
        }
        else if ( isDigit(S[Pos]) )
        {
            n = 7;
            Fvalue = getNumber();
        }
    }
    Pos++;
    PrevLex = CurLex;
    CurLex = n;
    return n;
}

ExprTree* newNode()
{
    ExprTree* node;
    node = (ExprTree *) malloc(sizeof(ExprTree));
    if (!node) Err = 2;
    else
    {
        node->opcode = 0;
        node->ivar   = -1;
        node->fvalue = 0.;
        node->left   = NULL;
        node->right  = NULL;
    }
    return node;
}

ExprTree* getSingleOp(int *lex)
{
    int bracket;
    int opcode;
    ExprTree *left;
    ExprTree *right;
    ExprTree *node;

    /* --- open parenthesis, so continue to grow the tree */
    if ( *lex == 1 )
    {
        Bc++;
        left = getTree();
    }

    else
    {
        /* --- Error if not a singleton operand */
        if ( *lex < 7 || *lex == 10 || *lex == 31)
        {
            Err = 1;
            return NULL;
        }

        opcode = *lex;

        /* --- simple number or variable name */
        if ( *lex == 7 || *lex == 8 || *lex == 9 ||*lex > 40)
        {
            left = newNode();
            left->opcode = opcode;
            if ( *lex == 7 ) left->fvalue = Fvalue;
            if ( *lex == 8 || *lex == 9 || *lex == 42) left->ivar = Ivar;
        }

        /* --- function which must have a '(' after it */
        else
        {
            *lex = getLex();
            if ( *lex != 1 )
            {
               Err = 1;
               return NULL;
            }
            Bc++;
            left = newNode();
            left->left = getTree();
            left->opcode = opcode;
        }
    }   
    *lex = getLex();

    /* --- exponentiation */
    while ( *lex == 31 )
    {
        *lex = getLex();
        bracket = 0;
        if ( *lex == 1 )
        {
            bracket = 1;
            *lex = getLex();
        }
        if ( *lex != 7 )
        {
            Err = 1;
            return NULL;
        }
        right = newNode();
        right->opcode = *lex;
        right->fvalue = Fvalue;
        node = newNode();
        node->left = left;
        node->right = right;
        node->opcode = 31;
        left = node;
        if (bracket)
        {
            *lex = getLex();
            if ( *lex != 2 )
            {
                Err = 1;
                return NULL;
            }
        }
        *lex = getLex();
    }
    return left;
}

ExprTree* getOp(int *lex)
{
    int opcode;
    ExprTree* left;
    ExprTree* right;
    ExprTree* node;
    int neg = 0;

    *lex = getLex();
    if (PrevLex == 0 || PrevLex == 1)
    {
        if ( *lex == 4 )
        {
            neg = 1;
            *lex = getLex();
        }
        else if ( *lex == 3) *lex = getLex();
    }
    left = getSingleOp(lex);
    while ( *lex == 5 || *lex == 6 )
    {
        opcode = *lex;
        *lex = getLex();
        right = getSingleOp(lex);
        node = newNode();
        if (Err) return NULL;
        node->left = left;
        node->right = right;
        node->opcode = opcode;
        left = node;
    }
    if ( neg )
    {
        node = newNode();
        if (Err) return NULL;
        node->left = left;
        node->right = NULL;
        node->opcode = 10;
        left = node;
    }
    return left;
}

ExprTree* getTree()
{
    int       lex;
    int       opcode;
    ExprTree* left;
    ExprTree* right;
    ExprTree* node;

    left = getOp(&lex);
    for (;;)
    {
        if ( lex == 0 || lex == 2 )
        {
            if ( lex == 2 ) Bc--;
            break;
        }

        if (lex != 3 && lex != 4 )
        {
            Err = 1;
            break;
        }

        opcode = lex;
        right = getOp(&lex);
        node = newNode();
        if (Err) break;
        node->left = left;
        node->right = right;
        node->opcode = opcode;
        left = node;
    } 
    return left;
}

/*Converts binary tree to linked list (postfix format)*/
void traversaltree(ExprTree* t, ExprList** l)
{
	struct ExprListNode * listnode;
	if ( t == NULL) 
		return;
	else
	{
		traversaltree(t->left,  l);
		traversaltree(t->right, l);
		listnode = (struct ExprListNode *)malloc(sizeof(struct ExprListNode));
		listnode->next = NULL;
		listnode->prev = (*l);
		if (*l != NULL)
			(*l)->next = listnode;
		listnode->fvalue = t->fvalue;
		listnode->opcode = t->opcode;
		listnode->ivar = t->ivar;
		(*l) = listnode;
		return;
	}
}

/*Mathematica expression evaluation using a stack*/
double mathexprlist_eval(ExprList* list)
{
	ExprList * tlist;
	double r1, r2;
	int stackindex = 0;
	tlist = list;
	while(tlist != NULL)
	{
		switch (tlist->opcode)
		{
			case 3:  
				r1 = Exprst[stackindex];
				stackindex--;
				r2 = Exprst[stackindex];
				Exprst[stackindex] = r2+r1;
				break;
			case 4:  
				r1 = Exprst[stackindex];
				stackindex--;
				r2 = Exprst[stackindex];
				Exprst[stackindex] = r2-r1;
				break;
			case 5:  
				r1 = Exprst[stackindex];
				stackindex--;
				r2 = Exprst[stackindex];
				Exprst[stackindex] = r2*r1;
				break;
			case 6:  
				r1 = Exprst[stackindex];
				stackindex--;
				r2 = Exprst[stackindex];
				Exprst[stackindex] = r2/r1;
				break;				
			case 7:  
				stackindex++;
				Exprst[stackindex] = tlist->fvalue;
				break;
			case 8:  
				if (CurrentConcen != NULL)
					r1 = CurrentConcen[tlist->ivar];
				else 
					r1 = 0.0;            
				stackindex++;
				Exprst[stackindex] = r1;
				break;
			case 9:  
				if (CurrentPara != NULL)
					r1 = CurrentPara[tlist->ivar];
				else 
					r1 = 0.0;
				stackindex++;
				Exprst[stackindex] = r1;
				break;
			case 10: 
				Exprst[stackindex] = -Exprst[stackindex];
				break;
			case 11: 
				r1 = Exprst[stackindex];
				r2 = cos(r1);
				Exprst[stackindex] = r2;
				break;
			case 12: 
				r1 = Exprst[stackindex];
				r2 = sin(r1);
				Exprst[stackindex] = r2;
				break;
			case 13: 
				r1 = Exprst[stackindex];
				r2 = tan(r1);
				Exprst[stackindex] = r2;
				break;
			case 14: 
				r1 = Exprst[stackindex];
				r2 = 1.0/tan( r1 );    
				Exprst[stackindex] = r2;
				break;
			case 15: 
				r1 = Exprst[stackindex];
				r2 = fabs( r1 );       
				Exprst[stackindex] = r2;
				break;
			case 16: 
				r1 = Exprst[stackindex];
				if (r1 < 0.0) r2 = -1.0;
				else if (r1 > 0.0) r2 = 1.0;
				else r2 = 0.0;
				Exprst[stackindex] = r2;
				break;
			case 17: 
				r1 = Exprst[stackindex];
				r2 = sqrt( r1 );     
				Exprst[stackindex] = r2;
				break;
			case 18: 
				r1 = Exprst[stackindex];
				r2 = log(r1);
				Exprst[stackindex] = r2;
				break;
			case 28: 
				r1 = Exprst[stackindex];
				r2 = log10( r1 );     
				Exprst[stackindex] = r2;
				break;
			case 19: 
				r1 = Exprst[stackindex];
				r2 = exp(r1);
				Exprst[stackindex] = r2;
				break;
			case 20: 
				r1 = Exprst[stackindex];
				r2 = asin( r1 );
				Exprst[stackindex] = r2;
				break;
			case 21: 
				r1 = Exprst[stackindex];
				r2 = acos( r1 );      
				Exprst[stackindex] = r2;
				break;
			case 22: 
				r1 = Exprst[stackindex];
				r2 = atan( r1 );      
				Exprst[stackindex] = r2;
				break;
			case 23: 
				r1 = Exprst[stackindex];
				r2 = M_PI_2-atan(r1);  
				Exprst[stackindex] = r2;
				break;
			case 24:
				r1 = Exprst[stackindex];
				r2 = (exp(r1)-exp(-r1))/2.0;
				Exprst[stackindex] = r2;
				break;
			case 25: 
				r1 = Exprst[stackindex];
				r2 = (exp(r1)+exp(-r1))/2.0;
				Exprst[stackindex] = r2;
				break;
			case 26: 
				r1 = Exprst[stackindex];
				r2 = (exp(r1)-exp(-r1))/(exp(r1)+exp(-r1));
				Exprst[stackindex] = r2;
				break;
			case 27: 
				r1 = Exprst[stackindex];
				r2 = (exp(r1)+exp(-r1))/(exp(r1)-exp(-r1));
				Exprst[stackindex] = r2;
				break;
			case 31: 
				r1 = Exprst[stackindex];
				r2 = Exprst[stackindex-1];
				r2 = exp(r1*log(r2));
				Exprst[stackindex-1] = r2;
				stackindex--;
				break;
			case 41: 
				stackindex++;
				Exprst[stackindex] = CurrentSegDiam;
				break;
			case 42: 
				stackindex++;
				Exprst[stackindex] = CurrentMasstracoeff[tlist->ivar];
				break;
			case 43: 
				stackindex++;
				Exprst[stackindex] = CurrentSegVelocity;
				break;
			case 44: 
				stackindex++;
				Exprst[stackindex] = CurrentTime;
				break;
			case 45: 
				stackindex++;
				Exprst[stackindex] = CurrentReynolds;
				break;
		}
		tlist = tlist->next;
	}
		r1 = Exprst[stackindex];
		return r1;
}

void mathexpr_delete(ExprTree* t)
{
    if (t)
    {
        if (t->left)  mathexpr_delete(t->left);
        if (t->right) mathexpr_delete(t->right);
        free(t);
    }
}

void mathexprlist_delete(ExprList* l)
{
	ExprList* next;
    if (l != NULL)
    {
		next = l->next;
        free(l);
		l = next;
    }
}

/*Create binary tree to store mathematical expression*/
ExprTree* mathexpr_create(char *formula, int (*getVar) (char *))
{
    ExprTree* t;
    getSpecieIndex = getVar;
    Err = 0;
    PrevLex = 0;
    CurLex = 0;
    S = formula;
    Len = strlen(S);
    Pos = 0;
    Bc = 0;
    t = getTree();
    if (Bc != 0 || Err > 0)
    {
        mathexpr_delete(t);
        return NULL;
    }
    return t;
}

void buildstack(n)
{
	Exprst = (double *)malloc((n+1)*sizeof(double));;
}

