//
// A simple test for CommonIO
//

#ifndef WITHOUT_COMMONIO

#undef WARNINGS
#undef DEBUG
#undef DEBUGGING
#include <utilib/CommonIO.h>
#include <utilib/mpiUtil.h>
#ifdef USING_MPI
#include <mpi.h>
#endif

using namespace utilib;

int main(int argc,char** argv)
{
#ifdef USING_MPI
uMPI::init(&argc,&argv,MPI_COMM_WORLD);
#endif

/********************* Comment out ************
#if 1
#ifdef USING_MPI
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
   ucout << "First line of text" << endl;
   ucout << Flush;
   CommonIO::end();

   CommonIO::begin();
   CommonIO::begin_tagging();
   ucout << "Second line of text" << endl;
   ucout << Flush;
   CommonIO::end_tagging();
   CommonIO::end();

   CommonIO::begin();
   CommonIO::begin_tagging();
   ucout << "Third line of text" << endl;
   ucout << Flush;
   double x = 123.4567;
   ucout << x << endl;
   for(int i=1; i<=4; i++)
     {
       ucout.precision(i);

       cout << utilib::CommonIO::MapCout->precision() << " " << cout.precision() << " " << (&ucout)->precision() << endl;
       ucout << "UCOUT: " << x << endl << Flush;
       ucout << "UCOUT: " << x << endl << Flush;
     }
   for(int i=1; i<=4; i++) {
       cout.precision(i);
       cout <<  "COUT:  " << x << endl << flush;
     }

   CommonIO::end_tagging();
   CommonIO::end();

#ifdef USING_MPI
uMPI::done();
#endif
}

#else
int main() {return 0;}
#endif
