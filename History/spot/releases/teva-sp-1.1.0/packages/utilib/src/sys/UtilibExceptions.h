/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

#if !defined(_ECC_)

/**
 * \file UtilibExceptions.h
 *
 * Defines the \c utilib::general_runtime_error class, a utilib-specific 
 * exception class that accepts a string as a constructor.
 */

#ifndef utilib_UtilibExceptions_h
#define utilib_UtilibExceptions_h

#include <exception>
#include <stdexcept>
#ifdef ANSI_HDRS
#include <cstdio>
#else
#include <stdio.h>
#endif
#include <stdarg.h>

#ifdef ANSI_NAMESPACES
using namespace std;
#endif

namespace utilib {

/// An exception class that accepts a string that defines the error.
class general_runtime_error : public std::exception {

public:

  /// Constructor
  general_runtime_error(const char* fmt, ...);

  /// Returns the exception description
  virtual const char* what () const throw() { return str; }

protected:

  /// The exception description
  char str[512];
};


inline general_runtime_error::general_runtime_error(const char* fmt, ...)
{
va_list ap;
va_start( ap, fmt );
#if !defined(COUGAR) && !defined(TFLOPS) && !defined(_MSC_VER)
vsnprintf(str, 512, fmt, ap );
#else
vsprintf(str, fmt, ap );
#endif
va_end( ap );
}

}
#endif

#endif
