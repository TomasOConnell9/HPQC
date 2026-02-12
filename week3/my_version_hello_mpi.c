#include <stdio.h>
#include <mpi.h>

int main (int argc, char **argv)
{
	int myRank = 0;
	int uniSize = 0;
	int ierror = 0;
	ierror = MPI_Init(&argc, &argv);

	ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD, &uniSize);

	printf("Hello, I am %d of %d\n", myRank, uniSize);

	ierror = MPI_Finalize();

	return 0;
}
