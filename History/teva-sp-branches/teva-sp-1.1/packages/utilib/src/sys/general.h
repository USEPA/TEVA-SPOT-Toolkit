/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file general.h
 *
 * This header defines macros that provide a simple, C-based method for
 * handling flags. This macro-based approach works very well for C 
 * code, though it can certainly be done better for C++ code.
 *
 * \author Anonymous
*/

#ifndef utilib_general_h
#define utilib_general_h

/** 
 * \def DO_STANDARD_COMMAND_LINE(stmts)
 *
 * Processes argv[1..argc-1] according to the (semicolon-separated) 
 * statements in \a stmts.  Possible statements include 
 *  - \c SIMPLE_FLAG_OPTN
 *  - \c FLAG_OPTN
 *  - \c SIMPLE_CFLAG_OPTN
 *  - \c CFLAG_OPTN
 *  - \c DATA_OPTN
 *  - \c DATA_OPTN2
 *  - \c REQ_ARG
 *  - \c OPT_ARG
 *  - \c NON_SWITCH
 *  - \c FREE_ARGS
 *  - \c FREE_ARGS_CONT
 *  - \c ANY_OPTION
 * In case an error is detected, an error message is constructed from
 * the flag names and descriptive text, and the program exits
 * abnormally after printing the message.  Within one of the statements,
 * the programmer can cause such an error message and termination with
 * the macro \c COMMAND_LINE_ERROR.
 */
/**
 * \def SIMPLE_FLAG_OPTN(s,var)
 *
 * Sets \a var to \c TRUE on encountering the flag -s (s is an unquoted 
 * string of letters and digits).
 */
/**
 * \def FLAG_OPTN(s,stmt)
 *
 * Executes \a stmt on encountering -s.
 */
/**
 * \def SIMPLE_CFLAG_OPTN(s,var) 
 *
 * Like \c SIMPLE_FLAG_OPTN but requires \a s to be a single character and 
 * allows multiple flags to be concatenated (as in ls -AF).
 */
/**
 * \def CFLAG_OPTN(s,stmt)
 *
 * Like \c FLAG_OPTN but requires \a s to be a single character and allows
 * multiple flags to be concatenated (as in ls -AF).
 */
/**
 * \def DATA_OPTN(s,desc,stmt)
 * 
 * Assumes a switch of the form -sdata or -s data, and executes \a stmt 
 * with _OPTION_ set to the start of the data string.  The \a desc is used as 
 * descriptive text in a "Usage" message.
 */
/**
 * \def DATA_OPTN2(s,desc,stmt1,stmt2)
 *
 * Assumes a switch of the form -sdata1 data2 or -s data1 data2, and 
 * executes \a stmt1 with _OPTION_ set to the start of the data string and 
 * \a stmt2 with _OPTION_ set to the start of the NEXT data string.  The 
 * \a desc is used as descriptive text in a "Usage" message.
 */
/**
 * \def REQ_ARG(desc,stmt)
 * 
 * Executes \a stmt for an argument that does not begin with "-".  Here, 
 * \a desc is used as descriptive text in a "Usage" message.  _OPTION_ is 
 * set to the argument.
 */
/**
 * \def OPT_ARG(desc,stmt)
 *
 * Executes \a stmt for an argument that does not begin with "-".  Unlike 
 * \c REQ_ARG, this is an optional argument.
 */
/**
 * \def NON_SWITCH(desc,stmt)
 *
 * Executes \a stmt for the rest of the arguments.  Here, \a desc is used 
 * as descriptive text in a "Usage" message.  _OPTION_ is set to the argument.
 */
/**
 * \def FREE_ARGS_CONT()
 *
 * Ends the argument processing if the given flag is found.  The \a argc and 
 * \a argv arguments are set so \a argv[1] has the first free argument.
 */
/**
 * \def FREE_ARGS()
 * 
 * Ends the arguement processing by setting the \a argc and \a argv arguments.
 * These are set so \a argv[0] has the first free argument.
 */
/**
 * \def ANY_OPTION(desc,stmt)
 *
 * Executes \a stmt for any argument.  The \a desc is as for NON_SWITCH.  
 * _OPTION_ is set to the argument.
 */

#if defined(__cplusplus) && defined(ANSI_HDRS)
#include <cstring>
#include <cstdlib>
#include <cstdio>
#else
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#endif

/** FALSE macro */
#ifndef FALSE
#define FALSE	0
#endif
/** TRUE macro */
#ifndef TRUE
#define TRUE	1
#endif

/**
 * \def PPRINT_DO_COMMAND
 *
 * If this macro is defined, then the '-h' flag prints the 
 * argument list in a nice, multiline format.  Otherwise, the argument list
 * is printed in a compact format on one line.
 */
#if defined(PPRINT_DO_COMMAND)
#define DO_COMMAND_TAB	"\t"
#define DO_COMMAND_EOLN	"\n"
#else
#define DO_COMMAND_TAB	" "
#define DO_COMMAND_EOLN	""
#endif

/*
      ACTION INDEX
	0 - Process flags
	1 - ERR
	2 - Used to make CFLAGS
	3 - Used to print out errors
	4 - Used to print out errors
	5 - Used to print out errors
	6 - Process required arguments
	7 - Exit the processing
 */


#define SIMPLE_FLAG_OPTN(string,var) \
  FLAG_OPTN(string, (var) = TRUE)

#define FLAG_OPTN(string,stmt) \
  if (__ACTION__ == 0) { \
    if (strcmp(_OPTION_, "-" #string) == 0) { \
      {stmt;} \
      continue; \
    } \
  } \
  else if (__ACTION__ == 1 || __ACTION__ == 4) { \
    (void) fputs(DO_COMMAND_TAB,stderr);  \
    (void) fputs("[-" #string "]",stderr); \
    (void) fputs(DO_COMMAND_EOLN, stderr); \
  }

#define CFLAG_OPTN(char,stmt) \
  if (__ACTION__ == 0 && _OPTION_[1] == (#char)[0]) { \
    __ACTION__ = 2; \
    _OPTION_ += 1; \
  }  \
  if (__ACTION__ == 2 && _OPTION_[0] == (#char)[0]) { \
    {stmt;} \
    continue; \
  } else if (__ACTION__ == 1) { \
    (void) fputs(DO_COMMAND_TAB,stderr); \
    (void) fputs("[-" #char , stderr); \
    __ACTION__ = 3; \
  } else if (__ACTION__ == 3) { \
    (void) fputs(#char, stderr); \
  } else if (__ACTION__ == 5) { \
    __ACTION__ = 4; \
  }
  
#define SIMPLE_CFLAG_OPTN(char,var) \
  CFLAG_OPTN(char, (var) = 1)

#define DATA_OPTN(string,desc,stmt) \
  if (__ACTION__ == 0 \
      && strncmp(_OPTION_, "-" #string, sizeof("-" #string) - 1) == 0) { \
    if (_OPTION_[sizeof("-" #string)-1] == '\0') { \
      __i__ += 1; \
      if (__i__ >= argc) __ACTION__ = 1; \
      _OPTION_ = argv[__i__]; \
    } \
    else _OPTION_ += sizeof("-" #string); \
    if (__ACTION__ != 1) {stmt;} \
    continue; \
  } else if (__ACTION__ == 1 || __ACTION__ == 4) { \
    (void) fputs(DO_COMMAND_TAB,stderr); \
    (void) fputs("[-" #string #desc "]",stderr); \
    (void) fputs(DO_COMMAND_EOLN, stderr); \
  }

#define DATA_OPTN2(string,desc,stmt1,stmt2) \
  if (__ACTION__ == 0 \
      && strncmp(_OPTION_, "-" #string, sizeof("-" #string) - 1) == 0) { \
    if (_OPTION_[sizeof("-" #string)-1] == '\0') { \
      __i__ += 1; \
      if (__i__ >= argc) __ACTION__ = 1; \
      _OPTION_ = argv[__i__]; \
    } \
    else _OPTION_ += sizeof("-" #string); \
    if (__ACTION__ != 1) { \
	stmt1; \
	__i__ += 1; \
	if (__i__ >= argc) \
	   __ACTION__ = 1; \
	else { \
	   _OPTION_ = argv[__i__]; \
	   stmt2; \
	   } \
	} \
    continue; \
  } else if (__ACTION__ == 1 || __ACTION__ == 4) { \
    (void) fputs(DO_COMMAND_TAB,stderr); \
    (void) fputs("[-" #string #desc "]",stderr); \
    (void) fputs(DO_COMMAND_EOLN, stderr); \
  }

#define NON_SWITCH(desc,stmt) \
  if (__ACTION__ == 6) { \
     while (__i__ < argc) { \
       _OPTION_ = argv[__i__]; \
       stmt; \
       __i__++; \
       } \
  } \
  else if (__ACTION__ == 1 || __ACTION__ == 4) { \
    (void) fputs(DO_COMMAND_TAB,stderr); \
    (void) fputs("[" #desc "...]",stderr); \
    (void) fputs(DO_COMMAND_EOLN, stderr); \
  }

#define FREE_ARGS_CONT() \
  if (__ACTION__ == 6) { \
     int i; \
     char* tmp = argv[0];\
     for (i=1; i<__i__; i++) \
       argv++; \
     argv[0] = tmp;\
     argc -= __i__-1; \
     __ACTION__ = 7; \
  } \
  else if (__ACTION__ == 1 || __ACTION__ == 4) \
    (void) fputs(DO_COMMAND_EOLN, stderr);

#define FREE_ARGS() \
  if (__ACTION__ == 6) { \
     int i; \
     for (i=1; i<__i__; i++) \
       argv++; \
     argc -= __i__-1; \
     __ACTION__ = 7; \
  } \
  else if (__ACTION__ == 1 || __ACTION__ == 4) { \
    (void) fputs(DO_COMMAND_TAB,stderr); \
    (void) fputs("[argv1 argv2 ...]",stderr); \
    (void) fputs(DO_COMMAND_EOLN, stderr); \
  }

#define REQ_ARG(desc,stmt) \
  if (__ACTION__ == 6) { \
    if (__i__ >= argc) { \
       __ACTION__ = 1; \
       continue; \
    } \
    stmt; \
    __i__ += 1; \
    if (__i__ < argc) \
       _OPTION_ = argv[__i__]; \
  } \
  else if (__ACTION__ == 1 || __ACTION__ == 4) { \
    (void) fputs(DO_COMMAND_TAB, stderr); \
    (void) fputs("<" #desc ">",stderr); \
    (void) fputs(DO_COMMAND_EOLN, stderr); \
  }

#define OPT_ARG(desc,stmt) \
  if ((__ACTION__ == 6) && (__i__ < argc)) { \
    stmt; \
    __i__ += 1; \
    if (__i__ < argc) \
       _OPTION_ = argv[__i__]; \
  } \
  else if (__ACTION__ == 1 || __ACTION__ == 4) { \
    (void) fputs(DO_COMMAND_TAB,stderr); \
    (void) fputs("[<" #desc ">]",stderr); \
    (void) fputs(DO_COMMAND_EOLN, stderr); \
  }

#define ANY_OPTION(desc,stmt) \
  if (__ACTION__ == 0) { \
    stmt; \
    continue; \
  } \
  else if (__ACTION__ == 1 || __ACTION__ == 4) { \
    (void) fputs(DO_COMMAND_TAB,stderr); \
    (void) fputs("" #desc "",stderr); \
    (void) fputs(DO_COMMAND_EOLN, stderr); \
  }

#define COMMAND_LINE_ERROR { __ACTION__ = 1; continue;}

#define DO_STANDARD_COMMAND_LINE(stmts) \
  { \
    int __i__; \
    int __ACTION__; \
    char *_OPTION_ = NULL; \
    __i__ = 0; \
    __ACTION__ = 0; \
    while (1) { \
      if (__ACTION__ == 2) { \
        _OPTION_ += 1; \
        if (_OPTION_[0] == '\0') __ACTION__ = 0; \
      } \
      if ((__ACTION__ == 0) || (__ACTION__ == 6)) { \
        __i__ += 1; \
        if (__i__ >= argc) \
	   __ACTION__ = 6; \
	else \
           _OPTION_ = argv[__i__]; \
      } \
      if (__ACTION__ == 0 && _OPTION_[0] != '-') \
         __ACTION__ = 6; \
      if (__ACTION__ == 1) (void) fprintf(stderr, "Usage: %s%s", argv[0],DO_COMMAND_EOLN); \
      stmts; \
      if (__ACTION__ == 6) {\
        if (__i__ < argc) { \
          __ACTION__ = 1; \
          continue; \
        } \
        else \
          break; \
      } \
      if (__ACTION__ == 7) \
         break; \
      if (__ACTION__ == 1 || __ACTION__ == 4) { \
        fprintf(stderr,"\n"); \
        exit(__ACTION__); \
      } \
      if (__ACTION__ == 3) { \
        (void) fputs("]",stderr); \
        (void) fputs(DO_COMMAND_EOLN, stderr); \
        __ACTION__ = 5; \
      } else { \
         __ACTION__ = 1; \
      } \
    } \
  }

#endif
