/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file string_ops.h
 *
 * Misc operations on CharString objects
 */

#ifndef utilib_string_h
#define utilib_string_h

#include <utilib/utilib_config.h>
#include <string.h>
#include <utilib/CharString.h>

namespace utilib {


/// Split a string into an array of strings
inline BasicArray<CharString> split(const CharString& str, char sep)
{
size_type len = str.size();

int npart=1;
CharString tmp = str;
for (size_type i=0; i<len; i++) {
  if (str[i] == sep) {
     npart++;
     tmp[i] = '\000';
     }
  }

BasicArray<CharString> strs(npart);
size_type ndx=0;
for (size_type i=0; i<len; i++) {
  if (ndx < len)
     strs[i] = &(tmp[ndx]);
  while ((ndx < len) && (tmp[ndx] != '\000'))
    ndx++;
  ndx++;
  }

return strs;
}


/** Join an array of strings into a single string */
inline CharString join(const BasicArray<CharString>& strs, char sep)
{
CharString str;
if (strs.size() == 0) return str;
str += strs[0];

for (size_type i = 1; i<strs.size(); i++) {
  str += sep;
  str += strs[i];
  }

return str;
}

} // namespace utilib

#endif
