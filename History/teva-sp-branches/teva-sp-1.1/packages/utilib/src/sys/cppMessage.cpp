/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// cppMessage.cpp
//
// This routine is primarily used to avoid using cout and stdout IO
// at the same time.  Of course, using this implies that you're 
// using C++, even though the errmsg.h routines offer a C interface.
// However, I'm only using C++ these days...
//
#include <utilib/CommonIO.h>

#ifdef NON_ANSI
#include <iostream.h>
#else
#include <iostream>
#endif
#ifndef ANSI_HDRS
#include <stdlib.h>
#else
#include <cstdlib>
#endif

#ifdef USING_MPI
#include <mpi.h>
#endif

#ifdef ANSI_NAMESPACES
using namespace std;
#endif

namespace utilib {


void cppMessage_abort()
{  
CommonIO::end_all(); 
}


extern "C" void cppMessage(char* message,int flush_flag, int CommonIO_end)
{
#ifdef USING_MPI
int mpiActive;
MPI_Initialized(&mpiActive);
if (mpiActive) {
   int rank;
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   ucout << "(PN" << rank << ")  ";
   }
#endif
ucout << message;

if (flush_flag)
   ucout << Flush;

if (CommonIO_end) {
   //
   // Turn off CommonIO before aborting.
   //
   cppMessage_abort();
   }
} 

} // namespace utilib
