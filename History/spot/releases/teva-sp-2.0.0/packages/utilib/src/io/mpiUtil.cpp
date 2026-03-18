/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file mpiUtil.cpp
 *
 * Utility class to make using MPI a little easier.
 *
 * \author Jonathan Eckstein
 */

#include <utilib/utilib_config.h>
#ifdef UTILIB_HAVE_MPI

#include <unistd.h>

#ifndef UTILIB_HAVE_STD
#include <iostream.h>
#else
#include <iostream>
#include <string>      // is this include file needed here?
#endif

#include <utilib/CommonIO.h>
#include <utilib/mpiUtil.h>

#ifdef UTILIB_HAVE_NAMESPACES
using namespace std;
#endif

namespace utilib {

MPI_Comm uMPI::comm = MPI_COMM_WORLD;
int uMPI::rank   = -1;
int uMPI::size   = 1;
int uMPI::ioProc = 0;
int uMPI::iDoIO  = 1;

int uMPI::errorCode = 0;


void uMPI::init(int* argcP, char*** argvP, MPI_Comm comm_)
{
  if (!running())
    {
      char* prev_dir = getcwd(0,256);
      errorCode = MPI_Init(argcP,argvP);
      if (errorCode)
	 ucerr << "MPI_Init failed, code " << errorCode << endl;
      init(MPI_COMM_WORLD);
      // Work around an mpich problem: force the current working directory
      // after mpi_init to be the same as before we called it.  This is only
      // applied in serial, since otherwise we assume that mpirun has
      // handled this...
      if (size == 1)
	 chdir(prev_dir);
      free(prev_dir);
    }
  else
    init(comm_);
}

void uMPI::init(MPI_Comm comm_)
{
  comm=comm_;
  errorCode = MPI_Comm_rank(comm,&rank);
  if (errorCode) 
     ucerr << "MPI_Comm_rank failed, code " << errorCode << endl;
  errorCode = MPI_Comm_size(comm,&size);
  if (errorCode) 
     ucerr << "MPI_Comm_size failed, code " << errorCode << endl;

  // MSE: modifications required for the case where the incoming comm is not
  // MPI_COMM_WORLD since MPI_IO is not guaranteed to be an attribute of comm.
  // If comm==MPI_COMM_WORLD or if MPI_IO==rank of the calling process, then
  // MPI_IO rank in MPI_COMM_WORLD can be mapped to rank in comm.  Otherwise, 
  // fall back on ioProc=0 default.
  int flag, result;
  int* mpiIOP;
  errorCode = MPI_Attr_get(MPI_COMM_WORLD,MPI_IO,&mpiIOP,&flag);
  if (errorCode || !flag)
     ucerr << "MPI_Attr_get(MPI_IO) failed, code " << errorCode << endl;
  MPI_Comm_compare(comm, MPI_COMM_WORLD, &result);
  if (result==MPI_IDENT || result==MPI_CONGRUENT) // no mapping of MPI_IO reqd.
    ioProc = *mpiIOP;
  else { // MPI_IO can only be mapped to comm in special cases
    int world_rank;
    errorCode = MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    if (errorCode) 
       ucerr << "MPI_Comm_rank failed, code " << errorCode << endl;
    if (*mpiIOP == world_rank) // MPI_IO processor is this processor
      ioProc = rank; // MPI_IO in MPI_COMM_WORLD maps to rank in comm
    else
      ioProc = size; // no mapping of MPI_IO to comm is possible.  Assign size
                     // so that reduce works properly.
  }

  int elected;
  reduce(&ioProc,&elected,1,MPI_INT,MPI_MIN,0);
  ioProc = elected;
  broadcast(&ioProc,1,MPI_INT,0);
  if ((ioProc < 0) || (ioProc >= size))
    ioProc = 0;
  iDoIO = (rank == ioProc);

  if (size > 1)
     CommonIO::begin_tagging();
};


void uMPI::done()
{
  if (size > 1) 
     CommonIO::end_tagging();
  MPI_Finalize();
};


int uMPI::sizeOf(MPI_Datatype t)
{
  MPI_Aint extent;
  errorCode = MPI_Type_extent(t,&extent);
  if (errorCode)
     ucerr << "MPI_Type_extent failed, code " << errorCode << endl;
  return extent;
}


void uMPI::killSendRequest(MPI_Request* request)
{
  if (*request != MPI_REQUEST_NULL)
    {
      MPI_Status trashStatus;
      if (!test(request,&trashStatus))
	{
          ucerr << "uMPI::killSendRequest: incomplete send." << endl;
	  cancel(request);
	  wait(request,&trashStatus);
	}
      *request = MPI_REQUEST_NULL;
    }
}


void uMPI::killRecvRequest(MPI_Request* request)
{
  if (*request != MPI_REQUEST_NULL)
    {
      MPI_Status trashStatus;
      if (!test(request,&trashStatus))
	{
	  cancel(request);
	  wait(request,&trashStatus);
	}
      *request = MPI_REQUEST_NULL;
    }
};

#if defined(UTILIB_HAVE_MPI) && defined(UTILIB_VALIDATING) && defined(UTILIB_HAVE_MPE)

CLASS_PARAMETER_DEF(uMPI,messageLog,0,0,1);


void uMPI::logSend(int dest,int tag,int count,MPI_Datatype t)
{
  int typeSize;
  errorCode = MPI_Type_size(t,&typeSize);
  if (errorCode)
     ucerr << "MPI_Type-size returned error code " << errorCode << endl;
  MPE_Log_send(dest,tag,count*typeSize);
}


void uMPI::logRecv(MPI_Status* status)
{
  MPE_Log_receive(status->MPI_SOURCE,
		  status->MPI_TAG,
		  getCount(status,MPI_PACKED));
}


#endif


} // namespace utilib 

#endif
