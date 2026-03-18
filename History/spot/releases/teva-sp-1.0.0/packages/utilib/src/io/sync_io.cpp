/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */
#if 0

/**
 * \file sync_io.cpp
 *
 * IO file containing sync information for printing with multiple
 * processors
 *
 * Currently, this software only works if the MPI flag is set
 */

#include <utilib/CommonIO.h>

namespace utilib {


#define PRINT_MARKER_REQUEST    102
#define PRINT_MARKER_RETURN     103



#define HOST_NODE 0
#ifndef NULL
#define NULL 0
#endif

static int sync_print_flag=false;
static int print_marker_status=false;


void set_sync_print(const int flag)
{
sync_print_flag=flag;
}


void clear_print_marker_queue()
{
#ifdef STDLIB_MPI
if ((mpMaxNodes() > 1) && (sync_print_flag==true) && (mpMyID() == HOST_NODE)) {
   while (mpATest(PRINT_MARKER_REQUEST) == true) {
     int tmp;
     mpRecv(PRINT_MARKER_REQUEST,NULL,0);
     tmp = mpLastProcessor();
     if (print_marker_status != HOST_NODE) {
        mpRecv(PRINT_MARKER_RETURN,NULL,0);
        if (mpLastProcessor() != print_marker_status) {
           (*bbums_cerr) << "Node " << mpLastProcessor() << 
				   " returned a Print_Marker it didn't own!\n";
	   (*bbums_cerr).flush();
	   }
        }
     print_marker_status = tmp;
     mpASend(PRINT_MARKER_REQUEST,NULL,0,tmp);
     }
   }
#endif
}


void get_print_marker()
{
#ifdef STDLIB_MPI
if ((mpMaxNodes() > 1) && (sync_print_flag == true)) {
  
  if (mpMyID() == HOST_NODE) {
     //
     // Handle all other requests first
     //
     clear_print_marker_queue();
     // 
     // Get the marker if I don't have it already
     //
     if (print_marker_status == HOST_NODE)
        return;
     mpRecv(PRINT_MARKER_RETURN,NULL,0);
     if (mpLastProcessor() != print_marker_status) {
        (*bbums_cerr) << "Node " << mpLastProcessor() << 
			  " returned a Print_Marker it didn't own!\n";
        (*bbums_cerr).flush();
        }
     print_marker_status = HOST_NODE;
     }
  else {
    if (print_marker_status == false) {
       mpSend(PRINT_MARKER_REQUEST,NULL,0,HOST_NODE);
       mpRecv(PRINT_MARKER_REQUEST,NULL,0);
       print_marker_status = true;
       }
    }
  }
#endif
}

void return_print_marker()
{
#ifdef STDLIB_MPI
if ((mpMaxNodes() > 1) && (sync_print_flag == true))
   if ((mpMyID() != 0) && (print_marker_status == true)){
      mpASend(PRINT_MARKER_RETURN,NULL,0,HOST_NODE);
      print_marker_status = false;
      }
#endif
}

} // namespace utilib

#endif
