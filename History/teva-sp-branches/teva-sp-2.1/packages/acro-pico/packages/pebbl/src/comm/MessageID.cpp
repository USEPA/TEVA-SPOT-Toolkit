/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// MessageID.cpp
//

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <utilib/std_headers.h>
#include <mpi.h>
#include <utilib/mpiUtil.h>
#include <pebbl/MessageID.h>

namespace pebbl {

using utilib::uMPI;

int MessageID::next_id=0;

MessageID NullMsg;
MessageID AnyMsg(MPI_ANY_TAG);


int MessageID::check_id()
{
int tmp;
int flag;
MPI_Attr_get(uMPI::comm, MPI_TAG_UB, (void*)&tmp, &flag);
assert(flag == 1);

return (next_id < tmp);
}


void MessageID::reset_ids()
{
//
// Reset the message ID number to the initial value.  Note that we assume that 
// this is 1, since the NullMsg uses the zero id.
//
next_id = 1;
}

} // namespace pebbl

#endif
