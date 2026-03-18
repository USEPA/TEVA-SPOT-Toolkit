/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file comments.h
 *
 * Defines stream operators for skipping over white space.
 *
 * \author William E. Hart
 */

#ifndef utilib_comments_h
#define utilib_comments_h

#ifdef NON_ANSI
#include <iostream.h>
#else
#include <iostream>
#endif

#include <utilib/utilib_dll.h>

#ifdef ANSI_NAMESPACES
using namespace std;
#endif

namespace utilib {

/**
 * Move the stream counter forward, passing over whitespace and 
 * comment lines, which begin with a '#'.
 * The argument \a line_counter returns the number of lines that were 
 * skipped.
 */
istream& comment_lines(istream& ins, int& line_counter);

/**
 * Move the stream counter forward, passing over whitespace and 
 * comment lines, which begin with a '#'.
 */
inline istream& comment_lines(istream& ins)
{
int tmp=0;
return (comment_lines(ins,tmp));
}

/**
 * Move the stream counter forward, passing over whitespace.
 * The argument \a line_counter returns the number of newline symbols
 * that were skipped.
 */
istream& whitespace(istream& ins, int& line_counter);

/**
 * Move the stream counter forward, passing over whitespace.
 * This serves the same role as 'ws', but this operator does not work
 * under Solaris 6.0 compilers.
 */
inline istream& whitespace(istream& ins)
{
int tmp=0;
return whitespace(ins,tmp);
}

} // namespace utilib

#endif
