%{
#ifdef yyterminate
#undef yyterminate
#endif
#define yyterminate() yy_delete_buffer(yy_current_buffer); return YY_NULL

/**
<INITIAL>{ARG} {
strncpy(Last_var, (char *)yytext, NAMELEN-1);
**fprintf(stderr,"ARG: \"%s\"\n",Last_var);**
  Last_var[NAMELEN-1] = '\0';
  return(ARG);
}
**/

%}

WS   [ \n\t]+
LT   [A-Za-z]
KR   [A-Za-z0-9_]
DI   [0-9]
NM   {DI}*\.{DI}+([Ee][-+]?{DI}+)?
INT  {DI}+
VR   {LT}{KR}*(<{KR}+>)?
ARG  {KR}*(<{KR}+>)?,
S_OP [-+ \t\n]+
LOG  [<>]?=?

%start COMMENT

%%
<INITIAL>"/*" {
  BEGIN COMMENT;
} /* begin skip comment */

<COMMENT>"*/" {
  BEGIN INITIAL;
} /* end skip comment */

<COMMENT>. {
}

<COMMENT>\n {
}

<INITIAL>{WS} {
}

<INITIAL>"," {
  return(COMMA);
}

<INITIAL>"[" {
  return(LBRACKET);
}

<INITIAL>"]" {
  return(RBRACKET);
}

<INITIAL>[mM][iI][nN]":" {
  return(MINIMISE);
}

<INITIAL>[mM][aA][xX]":" {
  return(MAXIMISE);
}

<INITIAL>{NM} {
  f = atof((char *)yytext);
  return(CONS);
} /* f contains the last float */

<INITIAL>{INT} {
  d = atoi((char *)yytext);
  return(INTEGER);
} /* d contains the last integer */

<INITIAL>{S_OP} {
  Sign = 0;
  for(x = 0; x < yyleng; x++)
    if(yytext[x] == '-' || yytext[x] == '+')
      Sign = (Sign == (yytext[x] == '+'));
  return (SIGN);
  /* Sign is TRUE if the sign-string
     represents a '-'. Otherwise Sign
     is FALSE */
}

<INITIAL>[Ii][Nn][Tt] {
  Within_int_decl = TRUE;
  return(VAR);
}

<INITIAL>{VR} {
  strncpy(Last_var, (char *)yytext, NAMELEN-1);
  /*fprintf(stderr,"VR: \"%s\"\n",Last_var);*/
  Last_var[NAMELEN-1] = '\0';
  return(VAR);
}

<INITIAL>":" {
  return (COLON);
}

<INITIAL>"*" {
  return(AR_M_OP);
}

<INITIAL>{LOG} {
  return(RE_OP);
}

<INITIAL>";" {
  Within_int_decl = FALSE;
  return(END_C);
}

<INITIAL>. {
  fprintf(stderr,"LEX ERROR : %s lineno %d \n" ,yytext,yylineno);
}

%%
