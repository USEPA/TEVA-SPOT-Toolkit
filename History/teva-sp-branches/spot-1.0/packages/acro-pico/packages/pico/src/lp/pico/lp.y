/* ========================================================================= */
/* NAME  : lp.y                                                              */
/* ========================================================================= */


%token ARG VAR CONS SIGN AR_M_OP RE_OP END_C COMMA COLON MINIMISE MAXIMISE LBRACKET RBRACKET INTEGER


%{
#define YYISARRAY
/*#define YYDEBUG 1*/
#include <string.h>
#include <utilib/real.h>
#include <utilib/_generic.h>
#include <utilib/errmsg.h>
#include <pico/ilp_read.h>

/* globals */
#define NAMELEN 128
#define FALSE 0
#define TRUE 1

static char 	Last_var[NAMELEN];
static char 	Last_row[NAMELEN];
static int 	Lin_term_count;
static int      Row;
static double 	f;
static int 	d;
static char     rel_op;
static int 	x;
static int 	Sign;
static int 	isign;
	/* internal_sign variable to make sure nothing goes wrong */
	/* with lookahead */
static int 	make_neg;
	/* is TRUE after the relational operator is seen in order */
	/* to remember if lin_term stands before or after re_op */
static int 	Within_int_decl = FALSE;
	/* TRUE when we are within an int declaration */
static int 	Ignore_decl = FALSE;
	/* TRUE when you can ignore an int declaration. */
int yylineno;


/*
 * errorhandling routine for yyparse()
 */
void yyerror(char *string)
{
  ErrAbort(errmsg("PARSING ERROR: %s on line %d, quiting\n", string, yylineno));}

%}



%start inputfile
%%

inputfile	:
{
Row=0;
isign = 0;
make_neg = 0;
Sign = 0;
sprintf(Last_row,"Objective");
ILPread_set_constr_type(Last_row, 'N');
} 
		  objective_function
		  constraints
                  int_declarations
		;

constraints	: constraint
		| constraints
		  constraint
		;

constraint      : real_constraint
                | var COLON 
{
strcpy(Last_row,ILPread_get_last_var());
}
                  real_constraint
                ;


real_constraint	: x_linear_sum
		  RE_OP
{
  ILPread_set_constr_type(Last_row, (rel_op = yytext[0]));
  make_neg = 1; 
}
		  x_linear_sum
                  END_C
{
  if(Lin_term_count == 0) {
    fprintf(stderr, "WARNING line %d: constraint contains no variables\n",
	    yylineno);
  }
  if (Lin_term_count == 1)         /* convert row to a bound constraint */
     ILPread_bound_constraint(Last_row,ILPread_get_last_var());

  Row++;
  sprintf(Last_row,"Row%d",Row);

  Lin_term_count = 0;
  isign = 0 ;
  make_neg = 0;
}
		;

int_declarations: /* EMPTY */
                | real_int_decls
                ;

real_int_decls  : int_declaration
                | real_int_decls int_declaration
                ;

int_declaration : int_declarator vars END_C
                ;

int_declarator  : VAR
{
if (Within_int_decl) {
   Ignore_decl = FALSE;
   }
else {
   fprintf(stderr, "Unknown declaration specifier on line %d, ignored\n",
            yylineno);
   Ignore_decl = TRUE;
   }
}
                ;

vars            : var
{if(!Ignore_decl)ILPread_set_int_var(ILPread_get_last_var());}
                | vars var
{if(!Ignore_decl)ILPread_set_int_var(ILPread_get_last_var());}
                | vars COMMA var
{if(!Ignore_decl)ILPread_set_int_var(ILPread_get_last_var());}
                ;

var             : VAR
{ ILPread_var_append(Last_var); }
		  LBRACKET
{ ILPread_var_append("["); }
		  tuple
		  RBRACKET
{ ILPread_var_append("]"); }
		| VAR
{ ILPread_var_append(Last_var); }
		;

tuple		: VAR
{ ILPread_var_append(Last_var); }
		| INTEGER
{ sprintf(Last_var,"%d",d); ILPread_var_append(Last_var); }
		| INTEGER
{ sprintf(Last_var,"%d,",d); ILPread_var_append(Last_var); }
		  COMMA
		  tuple
		| ARG
{ ILPread_var_append(Last_var); }
		  tuple
		;

x_linear_sum	: x_linear_term 
		| SIGN
{
  isign = Sign; 
}
		  x_linear_term
		| x_linear_sum
		  SIGN
{
  isign = Sign; 
}
		  x_linear_term
		;

x_linear_term	: linear_term
		| CONS
{
  if (    (isign || !make_neg)
      && !(isign && !make_neg)) /* but not both! */
    f = -f;
  ILPread_set_constr_bound(Last_row,f,rel_op);
  isign = 0;
}
		| INTEGER
{
  if (    (isign || !make_neg)
      && !(isign && !make_neg)) /* but not both! */
    d = -d;
  ILPread_set_constr_bound(Last_row,(double)d,rel_op);
  isign = 0;
}
		;

linear_sum	: linear_term 
		| SIGN
{
  isign = Sign;
}
		  linear_term
		| linear_sum
		  SIGN
{
  isign = Sign;
}
		  linear_term
		;

linear_term	: var
{
  if (    (isign || make_neg)
      && !(isign && make_neg)) /* but not both! */
    ILPread_add_coef(Last_row, ILPread_get_last_var(), (REAL) -1);
  else
    ILPread_add_coef(Last_row, ILPread_get_last_var(), (REAL) 1);
  isign = 0;
  Lin_term_count++;
}
		| CONS 
		  var
{
  if (    (isign || make_neg)
      && !(isign && make_neg)) /* but not both! */
    f = -f;
  ILPread_add_coef(Last_row, ILPread_get_last_var(), f);
  isign = 0;
  Lin_term_count++;
}
		| INTEGER 
		  var
{
  if (    (isign || make_neg)
      && !(isign && make_neg)) /* but not both! */
    d = -d;
  ILPread_add_coef(Last_row, ILPread_get_last_var(), (double)d);
  isign = 0;
  Lin_term_count++;
}
		| CONS 
		  AR_M_OP
		  var
{
  if (    (isign || make_neg)
      && !(isign && make_neg)) /* but not both! */
    f = -f;
  ILPread_add_coef(Last_row, ILPread_get_last_var(), f);
  isign = 0;
  Lin_term_count++;
}
		| INTEGER 
		  AR_M_OP
		  var
{
  if (    (isign || make_neg)
      && !(isign && make_neg)) /* but not both! */
    d = -d;
  ILPread_add_coef(Last_row, ILPread_get_last_var(), (double)d);
  isign = 0;
  Lin_term_count++;
}
		;

objective_function:   MAXIMISE real_of
{
ILPread_set_sense(TRUE);
}
                    | MINIMISE real_of
{
ILPread_set_sense(FALSE);
}
                    | real_of
{
ILPread_set_sense(FALSE);
}
                ;

real_of:            linear_sum
		    END_C
{
  Row++;
  sprintf(Last_row,"Row%d",Row);
  Lin_term_count  =  0;
  isign = 0;
  make_neg = 0;
}
                    | END_C /* allow empty OF */
{
  Row++;
  sprintf(Last_row,"Row%d",Row);
  Lin_term_count  =  0;
  isign = 0;
  make_neg = 0;
}
		;
%%
#include "lex.c"
