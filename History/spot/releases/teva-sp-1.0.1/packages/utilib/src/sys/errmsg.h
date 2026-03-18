/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file errmsg.h
 *
 * This header defines macros and functions that can be used to notify
 * the user of errors, as well as abort or exit from some code.
 * These error facilites are designed to be compileable into C or C++ code.
 * However, the code that is executed calls a C++-compiled routine that
 * sends its output to \c ucout.  Consequently, these routines can only be
 * used for code that ultimately gets compiled by C++.  This is unfortunate,
 * but necessary because of the incompatability between \c stdout and \c ucout.
 *
 * \note These routines have been largely supplanted by the exception
 * management tools in \c exception_mngr.h.  Exceptions offer a cleaner 
 * mechanism for handling errors.  However, it is unclear how exceptions
 * could be used within C code, so these routines have been kept in UTILIB.
 *
 * \author William E. Hart
 */

#ifndef utilib_errmsg_h
#define utilib_errmsg_h

#include <utilib/utilib_dll.h>
#include <utilib/_generic.h>

/** Write a warning message. */
#define Warning(str)  DoWarning(__FILE__,__LINE__,str);

/** Write a warning message and return with value \c ERR. */
#define ErrReturn(str) {Warning(str); return ERR;}

/** Write a warning message and abort. */
#define ErrAbort(str) DoErrAbort(__FILE__,__LINE__,str);

/** Write a warning message and exit. */
#define ErrExit(str)  DoErrExit(__FILE__,__LINE__,str);


#if defined(__cplusplus)
extern "C" {
#endif

/** Used to compose a message, using the format specified by \a string. */
#if !defined(DOXYGEN)
UTILIB_API
#endif
char* errmsg(const char* string, ...);

/** Call \c errmsg_abort to indicate that all messages cause aborts. */
#if !defined(DOXYGEN)
UTILIB_API
#endif
void errmsg_abort(const int flag);

/** A typedef for functions passed into the \c errmsg_exec_fn function. */
typedef void (*null_fn_type)(void);

/** Call \c errmsg_exit_fn to indicate that exit and abort calls are preceded by
	a call to a user-provided exit function. */
#if !defined(DOXYGEN)
UTILIB_API
#endif
void errmsg_exit_fn(null_fn_type fn);

/** Create a warning about an error in \a file at line \a line, printing string \a str */
#if !defined(DOXYGEN)
UTILIB_API
#endif
void DoWarning(const char* file , const int line , const char* str);

/** Print an error in \a file at line \a line, printing string \a str and then abort. */
#if !defined(DOXYGEN)
UTILIB_API
#endif
void DoErrAbort(const char* file, const int line, const char* str);

/** Print an error in \a file at line \a line, printing string \a str and then exit. */
#if !defined(DOXYGEN)
UTILIB_API
#endif
void DoErrExit(const char* file, const int line, const char* str);

#if defined(__cplusplus)
}
#endif

#endif
