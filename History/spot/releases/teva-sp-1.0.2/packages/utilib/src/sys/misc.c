/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file misc.c
 * \author William E. Hart
 */

#include <stdio.h>


int calc_filesize(char* fname);


int calc_filesize(char* fname)
{
FILE* fptr;
int ctr;
char c,prev_c;

fptr = fopen(fname,"r");
if (fptr == NULL)
   return 0;

prev_c = '\n';
ctr=0;
c = (char) fgetc(fptr);
while (!feof(fptr)) {
  if (c == '\n') ctr++;
  prev_c = c;
  c = (char) fgetc(fptr);
  }
if (prev_c != '\n') ctr++;

return ctr;
}
