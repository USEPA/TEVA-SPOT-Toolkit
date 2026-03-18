/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file comments.cpp
 *
 * \author William E. Hart
 */

#include <utilib/utilib_config_bool.h>
#include <utilib/comments.h>

using namespace std;

namespace utilib {


istream& whitespace(istream& ins, int& line_counter)
{
char c='\000';
ins.get(c);

while (ins) {
  if (! ((c == ' ') || (c == '\t') || (c == '\n')) ) {
     break;
     }
  if (c == '\n') line_counter++;
 
  ins.get(c);
  }
if (ins)
   ins.putback(c);
 
return ins;
}


istream& comment_lines(istream& ins, int& line_counter)
{
char c='\000';
bool flag=true;
 
whitespace(ins,line_counter);
 
while ((flag == true) && ins) {
  ins.get(c);
  if (ins && (c != '#')) {
     ins.putback(c);
     flag = false;
     continue;
     }
 
  while (ins && (c != '\n'))
    ins.get(c);
 
  line_counter++;
  whitespace(ins,line_counter);
  }
 
return ins;
}

} // namespace utilib
