/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

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
#include <utilib/mpiUtil.h>
#endif

#ifdef ANSI_NAMESPACES
using namespace std;
#endif

extern "C" void cppSignalError(char* message)
{
  cerr << "**** Error";
#ifdef USING_MPI
  int mpiActive;
  MPI_Initialized(&mpiActive);
  if (mpiActive)
    {
      int rank;
      MPI_Comm_rank(utilib::uMPI::comm,&rank);
      cerr << "(PN" << rank << ')';
    }
#endif
  cerr << " ****  " << message << ".\n";
#ifdef USING_MPI
  if (mpiActive)
    MPI_Abort(MPI_COMM_WORLD,1);	// MPI_Abort always uses MPI_COMM_WORLD
#endif
  exit(1);
}
