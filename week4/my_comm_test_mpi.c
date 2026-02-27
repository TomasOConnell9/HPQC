#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

void check_uni_size(int uni_size);
void root_task(int uni_size);
void client_task(int my_rank, int uni_size);
void perform_task(int my_rank, int uni_size);


int main(int argc, char **argv)
{
	int ierror = 0;
	int my_rank = 0;
	int uni_size = 0;

	ierror = MPI_Init(&argc, &argv);

	ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);

	check_uni_size(uni_size);//check the program can continue before attempting any processes
	perform_task(my_rank, uni_size);//once clear, perform the tasks


	MPI_Finalize();

	return 0;
}


//ensure the program has at least two processes
void check_uni_size(int uni_size)
{
	if (uni_size >= 2)
	{
		return;//continue performing if there are two or more processes
	}
	else//abort if insufficent processes
	{
		fprintf(stderr, "Unable to communicate with fewer than 1 procesess. Please enter 2 or more procesess");
		exit(-1);
	}
}

//task to perform the root process
void root_task(int uni_size)
{
        int count = 1;
        int source;
        int tag = 0;
	int recv_message = 0;//buffer to store incoming messages 
	MPI_Status status;

	//recieve messages from all other ranks
        for (source = 1; source < uni_size; source++)
        {
                MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

                printf("Hello, I am 0 of %d. Received %d from Rank %d\n", uni_size, recv_message, source);
        }
}

//task for all the non root process
void client_task(int my_rank, int uni_size)
{
	int dest = 0;
	int send_message = my_rank * 10;

	MPI_Send(&send_message, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);

	printf("Hello, I am %d of %d. Sent %d to Rank %d\n", my_rank, uni_size, send_message, dest);
}

//decide which task to perform
void perform_task(int my_rank, int uni_size)
{
	if (my_rank == 0)
	{
		root_task(uni_size);
	}
	else
	{
		client_task(my_rank, uni_size);
	}
}

