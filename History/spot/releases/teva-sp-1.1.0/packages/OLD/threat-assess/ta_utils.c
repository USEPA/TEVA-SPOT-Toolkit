#include "ta_utils.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>


/*
**-----------------------------------------------------------
**  ta_error
**  Input:   exit code and varargs (printf-like) format string
**  Output:  None
**  Purpose: display the error message and exit
**-----------------------------------------------------------
*/
void ta_error(int exitCode,const char *errmsg, ...)
{
	va_list ap;
	fprintf(stderr,"\n\n********************************************************************************\n\n");
	va_start(ap,errmsg);
	vfprintf(stderr,errmsg,ap);
	va_end(ap);
	fprintf(stderr,"\n\n********************************************************************************\n\n");

	exit(exitCode);
}

/* not available on linux; case-invariant version of strcmp */

#ifdef __linux__

int _stricmp (char *s, char *t) 
{
  int d = 0;
  do 
    {
      d = toupper(*s) - toupper(*t);
    } 
  while (*s++ && *t++ && !d);
  return (d);
}

#endif

