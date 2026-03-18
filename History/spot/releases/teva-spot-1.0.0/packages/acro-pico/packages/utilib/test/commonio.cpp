/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// A simple test for CommonIO
//
#include <utilib/utilib_config.h>
#ifdef UTILIB_YES_COMMONIO

#undef WARNINGS
#undef DEBUG
#undef UTILIB_VALIDATING
#include <utilib/CommonIO.h>
#include <utilib/mpiUtil.h>
#ifdef UTILIB_HAVE_MPI
#include <mpi.h>
#endif

using namespace utilib;

int main(int argc,char** argv)
{
#ifdef UTILIB_HAVE_MPI
uMPI::init(&argc,&argv,MPI_COMM_WORLD);
#endif

/********************* Comment out ************
#if 1
#ifdef UTILIB_HAVE_MPI
int rank,size;
MPI_Comm_rank(MPI_COMM_WORLD,&rank);
MPI_Comm_size(MPI_COMM_WORLD,&size);

cout << "Basic Data: " << rank << " " << size << endl;
cout << "uMPI Data:  " << uMPI::rank << " " << uMPI::size << " " << uMPI::running() << endl;
int foo=-1;
cin >> foo;
cout << "FOO=" << foo << endl;
#endif
#endif
******************** End comment out ************/

   CommonIO::begin();
   ucout << "First line of text" << std::endl;
   ucout << Flush;
   CommonIO::end();

   CommonIO::begin();
   CommonIO::begin_tagging();
   ucout << "Second line of text" << std::endl;
   ucout << Flush;
   CommonIO::end_tagging();
   CommonIO::end();

   CommonIO::begin();
   CommonIO::begin_tagging();
   ucout << "Third line of text" << std::endl;
   ucout << Flush;
   double x = 123.4567;
   ucout << x << std::endl;
   for(int i=1; i<=4; i++)
     {
       ucout.precision(i);

       std::cout << utilib::CommonIO::MapCout->precision() << " " << std::cout.precision() << " " << (&ucout)->precision() << std::endl;
       ucout << "UCOUT: " << x << std::endl << Flush;
       ucout << "UCOUT: " << x << std::endl << Flush;
     }
   for(int i=1; i<=4; i++) {
       std::cout.precision(i);
       std::cout <<  "COUT:  " << x << std::endl << std::flush;
     }

   CommonIO::end_tagging();
   CommonIO::end();

#ifdef UTILIB_HAVE_MPI
uMPI::done();
#endif
}

#else
int main() {return 0;}
#endif
