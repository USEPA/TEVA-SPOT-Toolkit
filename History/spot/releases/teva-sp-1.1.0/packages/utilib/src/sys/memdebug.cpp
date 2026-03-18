/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// memdebug.cpp
//
// Macros that can be used to debug memory allocation
//

//#define USING_MEMDEBUG

#include <utilib/memdebug.h>

#if defined(USING_MEMDEBUG)

#ifndef ANSI_HDRS
#include <stddef.h>
#include <stdlib.h>
#include <new.h>
#else
#include <cstddef>
#include <cstdlib>
#include <new>
#endif
#include <utilib/errmsg.h>
#include <utilib/CommonIO.h>

namespace utilib {

int memdebug::nbytes = 0;
int memdebug::n_news = 0;
int memdebug::n_dels = 0;
int memdebug::num    = 0;

BasicArray<CharString> memdebug::name(10);
IntVector memdebug::num_new(10);
IntVector memdebug::num_del(10);
IntVector memdebug::memory_allocated(10);
IntVector memdebug::memory_deleted(10);
IntVector memdebug::last_total(10);


void memdebug::print_summary(ostream& os)
{
stdcout << "Fail: " << os.fail() << endl;
os << "MEMDEBUG SUMMARY";
stdcout << "Fail: " << os.fail() << endl;
os << endl;
stdcout << "Fail: " << os.fail() << endl;

os << "  Calls to 'new'            : " << memdebug::n_news << endl;
stdcout << "Fail: " << os.fail() << endl;
os << "  Calls to 'delete'         : " << memdebug::n_dels << endl;
stdcout << "Fail: " << os.fail() << endl;
os << "  Total # of bytes allocated: " << memdebug::nbytes << endl;
stdcout << "Fail: " << os.fail() << endl;
os << endl;
stdcout << "Fail: " << os.fail() << endl;
for (int i=0; i<num; i++) {
  os << "CLASS: " << memdebug::name[i] << endl;
  os << "  Number of objects constructed: " << num_new[i] << endl;
  os << "  Total memory allocated:        " << memory_allocated[i] << endl;
  os << "  Avg object size allocated:     " 
			<< (((double)memory_allocated[i])/num_new[i]) << endl;
  //os << "  Number of objects destructed:  " << num_del[i] << endl;
  //os << "  Total memory deleted:          " << memory_deleted[i] << endl;
  //os << "  Avg object size deleted:       " 
			//<< (((double)memory_deleted[i])/num_del[i]) << endl;
  }
}

}


using namespace utilib;

void my_new_handler()
{
ucout << "\n\nERROR:: Out of memory after allocating " << memdebug::nbytes 
							<< "\n";
ucout << Flush;
abort();
}


#ifdef COUGAR
void *operator new(size_t size) throw(__EDG_STD_NAMESPACE::bad_alloc)
#else
void *operator new(size_t size)
#endif
{
void* tmp = malloc(size);
cerr << endl << "MEMDEBUG NEW " << size << " " << tmp << endl;

memdebug::nbytes += size;
memdebug::n_news++;

//return ::new (void*) [size];
return tmp;
}


void operator delete(void* ptr)
{
cerr << endl << "MEMDEBUG DELETE " << ptr << endl;
memdebug::n_dels++;
free(ptr);
}


#endif
