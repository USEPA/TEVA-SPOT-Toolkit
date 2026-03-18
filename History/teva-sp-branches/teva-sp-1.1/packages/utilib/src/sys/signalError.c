/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file signalError.c
 *
 * \author Jonathan Eckstein
 */

#include <stdio.h>
#include "signalError.h"

#if defined(SOLARIS)
#include <varargs.h>
#else
#include <stdarg.h>
#endif


#if defined(__GNUC__) && defined(SUNOS)
char* vsprintf(char*, char*, va_list);
#endif

/* void signalError(const char *string, ...); */
static char messageBuffer[512];
void cppSignalError(char* woof);

void signalError(const char *string, ...) 
{
  va_list pvar;
#if defined(SOLARIS)
  va_start(pvar);
#else
  va_start(pvar,string);
#endif
  vsprintf(messageBuffer,(char *) string, pvar);
  cppSignalError(messageBuffer);
#if !defined(SOLARIS)
  va_end(pvar);
#endif
}
