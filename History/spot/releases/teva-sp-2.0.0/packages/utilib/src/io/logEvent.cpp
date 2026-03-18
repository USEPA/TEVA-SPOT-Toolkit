/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file logEvent.cpp
 *
 * Routines for doing event logging/tracing of parallel codes using MPE.
 *
 * \author Jonathan Eckstein
 */

#include <utilib/utilib_config.h>
#include <utilib/CommonIO.h>
#include <utilib/logEvent.h>
#include <utilib/mpiUtil.h>

namespace utilib {


#if defined(UTILIB_HAVE_MPI) && defined(UTILIB_VALIDATING) && defined(UTILIB_HAVE_MPE)


int logEvent::numberOfStates = 0;


void logEvent::open()
{
  uMPI::barrier();
  MPE_Start_log();
};


int logEvent::defineState(const char* description,const char* color)
{
  int stateNumber = numberOfStates++;
  if (uMPI::rank == 0)
    {
      char cString[256];
      const char* ptr1 = color;
      char* ptr2 = cString;
      while(*ptr1)
	*(ptr2++) = *(ptr1++);
      *(ptr2++) = ':';
      ptr1 = color;
      while(*ptr1)
	*(ptr2++) = *(ptr1++);
      *(ptr2++) = '\0';
      int startEvent = stateNumber*2;
      int endEvent   = startEvent + 1;
      MPE_Describe_state(startEvent,endEvent,(char*) description,cString);
    }
  return stateNumber;
}


void logEvent::close()
 { 
   if (uMPI::iDoIO)
     ucout << "(Writing event log.)\n";
   MPE_Finish_log((char *) "event"); 
 }

#endif

} // namespace utilib
