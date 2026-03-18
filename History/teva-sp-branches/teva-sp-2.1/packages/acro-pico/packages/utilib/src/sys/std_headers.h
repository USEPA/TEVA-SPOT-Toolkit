/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file std_headers.h
 *
 * This header file is used to facilitate portability of C and C++ code by
 * encapsulating the includes for C and C++ headers that vary across 
 * different platforms.  For example, this header file will include
 * ANSI-compliant C++ header on platforms that support them.
 *
 * \author William E. Hart
 */

#ifndef utilib_std_headers_h
#define utilib_std_headers_h

#include <utilib/utilib_config.h>
#include <unistd.h>

#if defined(UTILIB_HAVE_STD) && defined(__cplusplus)
/* C++ compiler using new C headers */
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cfloat>
#include <climits>
#include <cmath>
#include <cassert>
#include <csignal>
#include <cerrno>
#include <ctime>
#include <cstddef>

#else
/* C compiler (old style headers are used to avoid std namespace)
   or a C++ compiler which uses old C headers */
#ifdef UTILIB_HAVE_VALUES_H
#include <values.h> /* Obsolete: It defines MAXINT, MAXFLOAT, etc. */
#endif
#ifdef UTILIB_HAVE_FLOAT_H
#include <float.h>
#endif
#ifdef UTILIB_HAVE_LIMITS_H
#include <limits.h>
#endif
#ifdef UTILIB_HAVE_STRINGS_H
#include <strings.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <stddef.h>
#endif

#ifdef __cplusplus
#include <utility>
#include <deque>
//#include <algobase>
#include <functional>
#include <typeinfo>
#include <iterator>
#include <exception>
#include <numeric>

#if defined(UTILIB_HAVE_STD)
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <iomanip>
#include <fstream>
#include <set>
#include <map>
#include <new>
//#include <multimap>
//#include <pair>
#include <queue>
#else
#include <iostream.h>
#include <string.h>
#include <algo.h>
#include <vector.h>
#include <list.h>
#include <iomanip.h>
#include <fstream.h>
#include <set.h>
#include <map.h>
#include <new.h>
#include <queue.h>
//#include <pair.h>
//#include <multimap.h>
#endif

#ifdef UTILIB_HAVE_EXCEPTIONS
#include <stdexcept>
#else
#error "fix file to work in the absence of exceptions"
#endif

#ifndef UTILIB_HAVE_SSTREAM
#ifdef _MSC_VER
#include <strstrea.h>
#elif !defined(UTILIB_HAVE_STD)
#include <strstream.h>
#else
#include <strstream>
#endif
#else   // UTILIB_HAVE_SSTREAM
#include <sstream>
using std::stringstream;
#endif  // UTILIB_HAVE_SSTREAM

#endif

#endif
