/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// CharString.cpp
//

#ifndef ANSI_HDRS
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#else
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#endif

#include <utilib/CharString.h>

namespace utilib {


CharString::CharString(const char* str, const EnumDataOwned o)
  : BasicArray<char>()
{
  if (str)
    construct(strlen(str), (char*)str, o);
}


CharString::CharString(char* str, const EnumDataOwned o)
  : BasicArray<char>()
{
  if (str)
    construct(strlen(str), str, o);
}


// This copies data, overriding the standard copy from ArrayBase.
// It looks for trailing nulls in the input, and also makes sure that
// that there is a trailing null in the output.  If the source is shorter
// than the target, then the target gets filled out with nulls as well.

void CharString::copy_data(char*     target,
			   size_type target_size,
			   char*     source,
			   size_type source_size)
{
  unsigned int i=0;
  for (; i<target_size && i<source_size && source && *source; i++)
    *(target++) = *(source++);
  for (; i<=target_size; i++)
    *(target++) = '\000';
}


void CharString::dump_data(ostream& str, unsigned int max_elements)
{
  char* pointer = Data;
  unsigned int i=0;
  for (; *pointer && i<max_elements; i++)
    str << *(pointer++);
  if (i > 0)
    str << endl;
}
 

CharString& CharString::set_subvec(const size_type start,
				   const size_type len,
				   const CharString& array, 
				   const size_type offset)
{
  for (size_type i=start; i<(len+start); i++)
    Data[i] = array.Data[offset+i-start];
  Data[len+start] = '\000';
  return *this;
}
 

int compare(const CharString& str1, const CharString& str2)
{
  return str1.compare(str2);
}


CharString& CharString::operator+=(char val)
{
  char tmp[2];
  tmp[0] = val;
  tmp[1] = '\000';
  return ( (*this) += tmp );
}


CharString& CharString::operator+=(const double val)
{
  char tmp[256];
  sprintf(tmp,"%f",val);
  return ( (*this) += tmp );
}


CharString& CharString::operator+=(const int val)
{
  char tmp[256];
  sprintf(tmp,"%d",val);
  return ( (*this) += tmp );
}


CharString& CharString::operator+=(const unsigned int val)
{
  char tmp[256];
  sprintf(tmp,"%d",val);
  return ( (*this) += tmp );
}


CharString& CharString::operator+=(const char* str)
{
  if (str) {
    size_type curr = Len;
    size_type next = strlen(str);
 
    resize(curr+next);
 
    for (size_type i=curr; i<(curr+next); i++)
    Data[i] = str[i-curr];
    Data[curr+next] = '\000';
  }
 
  return *this;
}


int CharString::compare(const char* s) const
{
  if (Data)
    {
      if (s) {
	int tmp = strcmp(Data,s);  // Both pointers non-null -- use strcmp
	if (tmp < 0) return -1;
	if (tmp > 0) return 1;
        }
      else
	return *Data != 0;      // if s==NULL, return +1 if we have some data
    }
  else if (s)
    return -(*s != 0);          // Return -1 if s points to some data
  return 0;                     // Both pointers NULL: return 0
}


long int aslong(const CharString& str, int& status)
{
  long int ans=0;
  status = ERR;
  size_type i=0;
  int j=0;
  while ((i < str.size()) && isspace(str[i])) i++;
  while ((i < str.size()) && (isdigit(str[i]) || (str[i] == '.'))) {
    if (isdigit(str[i])) {
       ans = ans*10+ static_cast<long int>(str[i] - '0');
       if (j) j++;
       }
    else
       j++;
    i++;
    }
  if (i == str.size()) {
     if (!j) status = OK;
     return ans;
     }
  if (str[i] != 'e') return ans;	// AN ERROR
  i++;
  if (isalpha(str[i])) return ans;	// AN ERROR
  if (str[i] == '-') return ans;	// AN ERROR
  if (str[i] == '+') i++;
  int expn = atoi( &(str[i]) ) - ((j > 0) ? (j - 1) : 0);
  if (expn < 0) return ans;		// AN ERROR
  for (int k=0; k<expn; k++)
    ans = ans*10;
  status = OK;
  return ans;
}


double asdouble(const CharString& str, int& status)
{
  double ans;
  char* ptr = NULL;
  ans = strtod(str.data(),&ptr);
  if ((ptr == NULL) || (ptr[0] == '\000'))
    status = OK;
  else
    status = ERR;
  return ans;
}

} // namespace utilib
