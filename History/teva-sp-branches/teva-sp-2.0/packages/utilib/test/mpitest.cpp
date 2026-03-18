
//
// A simple test routine for MPI
//
#include <utilib/utilib_config.h>
#include <utilib/mpiUtil.h>
#include <utilib/CharString.h>

using namespace utilib;

int main(int argc, char* argv[])
{
#ifdef UTILIB_HAVE_MPI
	uMPI::init(&argc,&argv);

	int rank = uMPI::rank;
	int size = uMPI::size;
	cout << "My rank is " << rank << " out of " << size << endl;
	if (rank == 0) {
	uMPI::send(&rank, 1, MPI_INT, rank+1, 0);
	}

	int buf;
	MPI_Status status;
	if (rank > 0) {
	   int dest = rank-1;
	   cout << "Processor " << rank << " waiting for " << dest << endl;
	   uMPI::recv((void*)&buf, 1, MPI_INT, dest, 0, &status);
	   }
	else {
	   int dest = size-1;
	   cout << "Processor " << rank << " waiting for " << dest << endl;
	   uMPI::recv((void*)&buf, 1, MPI_INT, dest, 0, &status);
	   }
	cout << "Processor " << rank << " received a message from processor " << buf << endl;
	if (rank == (size-1))
	   uMPI::send(&rank, 1, MPI_INT, 0, 0);
	else
	   uMPI::send(&rank, 1, MPI_INT, rank+1, 0);
	uMPI::done();

#else
	cerr << "MPI is not configured right now!" << endl;
#endif
	return 0;
}
