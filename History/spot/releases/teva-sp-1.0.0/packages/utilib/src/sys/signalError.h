/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file signalError.h
 *
 * Simple header file for the signalError function.  
 *
 * \note This code needs to be resolved with the code in \c errmsg.h.
 * Additionally, this functionality is largely supplanted by the
 * functionality in \c exception_mngr.h.
 *
 * \author Jonathan Eckstein
 */

#ifndef utilib_signalError_h
#define utilib_signalError_h

#ifdef __cplusplus
extern "C" {
#endif

/** Write an error signal, using the format specified by \a string. */
void signalError(const char* string, ...);

#ifdef __cplusplus
};
#endif

#endif
