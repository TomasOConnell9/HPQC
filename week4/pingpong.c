#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int check_args(int argc, char **argv);
void check_uni_size(int uni_size);
void root_task(int num_pings);
void client_task(int num_pings);
void perform_task(int my_rank, int num_pings);

int main(int argc, char **argv)
{
	int ierror = 0;

	int num_pings = check_args(argc, argv);

	ierror = MPI_Init(&argc, &argv);

	int my_rank, uni_size;
	my_rank = uni_size = 0;

	ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);

	check_uni_size(uni_size);

	perform_task(my_rank, num_pings);

	ierror = MPI_Finalize();
	return 0;
}

//function to ensure correct command line argument count and extract the user defined number of pingpongs
int check_args(int argc, char **argv)
{
	int num_arg = 0; //create buffer for the number of arguments

	if (argc == 2) //if two arguments are inputted by the user...
	{
		num_arg = atoi(argv[1]); //convert the second number to an integer and store in the buffer
	}
	else
	{
		fprintf(stderr, "ERRPR: You have the enter a numerical argument.\n");
		fprintf(stderr, "Correct use: mpirun -np 2 pingpong [number of ping and pongs]\n");
		exit(-1); //else, aport the program
	}
	return num_arg; //return the number in the argument so it can be passed into other function in main
}

//ensure program has exactly 2 process, root and a single client.
void check_uni_size(int uni_size)
{
	if (uni_size == 2)
	{
		return;
	}
	else
	{
		fprintf(stderr, "ERROR: The program only works with a root process and a single client.\n");
		fprintf(stderr, "Correct use: mpirun -n 2 pingpong [number of ping and pongs]\n");
		exit(-1);
	}
}


//function to perform the root task
void root_task(int num_pings)
{
	int counter = 0;
	int count = 1;
	int source = 1;
	int dest = 1;
	int tag = 0;
	MPI_Status status;

	double start_time = MPI_Wtime();

	while (counter < num_pings) //while counter is less than the amount of ping pongs
	{
		MPI_Send(&counter, count, MPI_INT, dest, tag, MPI_COMM_WORLD); //Send out ping

		MPI_Recv(&counter, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status); // Recieve pong
	}

	MPI_Send(&counter, count, MPI_INT, dest, tag, MPI_COMM_WORLD); //final singal so client exists its infinite loop

	double end_time = MPI_Wtime();
	double time_running = end_time - start_time;
	double average_time = time_running / num_pings;

	//show results
	printf("Runtime: %f seconds\n", time_running);
	printf("Average time per pingpong: %f seconds\n", average_time);
	printf("Final counter: %d\n", counter);
}

//fuction to perform task for the non root process
void client_task(int num_pings)

{
	int counter;
	int count = 1;
	int source = 0;
	int dest = 0;
	int tag = 0;
	MPI_Status status;

	while(1)//always true, infinite loop
	{
		MPI_Recv(&counter, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);//recieve the ping
		if (counter >= num_pings) //if counter is greater than inputted pingpings...
		{
			break; //end
		}

		++counter; //for each pingpomg increase the counter by 1

		MPI_Send(&counter, count, MPI_INT, dest, tag, MPI_COMM_WORLD); //send the pong back
	}
}

//decide which task to perform for each process
void perform_task(int my_rank, int num_pings)
{
	if (my_rank == 0)
	{
		root_task(num_pings);
	}
	else
	{
		client_task(num_pings);
	}
}





