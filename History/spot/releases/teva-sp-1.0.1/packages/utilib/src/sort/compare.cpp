/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/* compare.c
 *
 * Some standard comparison functions for sorting.
 */

#include <stdio.h>
#include <utilib/utilib_dll.h>
#include <utilib/c_sort.h>
#include <utilib/Ereal.h>

using utilib::Ereal;


extern "C" int compare_int(COMP_VOID* x, COMP_VOID* y) 
{
int X,Y;
X = *(int*)x;
Y = *(int*)y;

if (X<Y) return -1;
if (X>Y) return 1;
return 0;
}




extern "C" int compare_double(COMP_VOID* x, COMP_VOID* y)
{ 
double X,Y;
X = *(double*)x;
Y = *(double*)y;

if (X<Y) return -1;
if (X>Y) return 1;
return 0;
}


extern "C" int compare_ereal_double(COMP_VOID* x, COMP_VOID* y)
{ 
Ereal<double> X,Y;
X = *(Ereal<double>*)x;
Y = *(Ereal<double>*)y;

if (X<Y) return -1;
if (X>Y) return 1;
return 0;
}



extern "C" int compare_float(COMP_VOID* x, COMP_VOID* y) 
{
float X,Y;
X = *(float*)x;
Y = *(float*)y;

if (X<Y) return -1;
if (X>Y) return 1;
return 0;
}
