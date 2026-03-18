/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file errmsg.c
 *
 * This is based on the signalError() routine originally developed by
 * Jonathan Eckstein.
 *
 * \author William E. Hart
 */

#if defined(SOLARIS)
#include <varargs.h>
#else
#include <stdarg.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#ifdef USING_MPI
#include "mpi.h"
#endif
#include "errmsg.h"
#include "_generic.h"


static null_fn_type exit_fn = NULL;
static int errmsg_abort_flag=FALSE;
static char messageBuffer[512];
static char MessageBuffer[512];
#ifdef UWIN
extern "C" {
#endif
void cppMessage(char* str, int flush_flag, int CommonIO_end);
#ifdef UWIN
};
#endif


char* errmsg(const char *string, ...)
{
#if defined(SOLARIS)
va_list pvar;
va_start(pvar);
vsprintf(messageBuffer,(char *) string, pvar);
#else
va_list pvar;
va_start(pvar,string);
vsprintf(messageBuffer,(char *) string, pvar);
va_end(pvar);
#endif
return messageBuffer;
}


void errmsg_abort(const int flag)
{ errmsg_abort_flag = flag; }


void errmsg_exit_fn(null_fn_type fn)
{ exit_fn = fn; }


void DoWarning(const char* file, const int line, const char* str)
{
if (errmsg_abort_flag) {
   DoErrAbort(file, line, str);
   return;
   }
sprintf(MessageBuffer,"Warning: file \"%s\", line %d\n\t%s\n", file,
			line, str);
cppMessage(MessageBuffer,TRUE,FALSE);
}


UTILIB_API void DoErrAbort(const char* file, const int line, const char* str)
{
sprintf(MessageBuffer,"Aborting from error: file \"%s\", line %d\n\t%s\n", file,
			line, str);
cppMessage(MessageBuffer,TRUE,TRUE);

if (exit_fn)
   (*exit_fn)();
abort();
}


void DoErrExit(const char* file, const int line, const char* str)
{
#ifdef USING_MPI
int mpiActive;
#endif
if (errmsg_abort_flag) {
   DoErrAbort(file, line, str);
   return;
   }
sprintf(MessageBuffer,"Exiting from error: file \"%s\", line %d\n\t%s\n", file,
			line, str);
cppMessage(MessageBuffer,TRUE,FALSE);

if (exit_fn)
   (*exit_fn)();
#ifdef USING_MPI
MPI_Initialized(&mpiActive);     
if (mpiActive)
   MPI_Abort(MPI_COMM_WORLD,1);
#endif    
exit(ERR);
}
