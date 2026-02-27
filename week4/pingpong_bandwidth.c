#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int check_args(int argc, char **argv, int *num_pings);
void check_uni_size(int uni_size);
void root_task(int num_pings, int* array, int num_elements);
void client_task(int num_pings, int* array, int num_elements);
void perform_task(int my_rank, int num_pings, int* array, int num_elements);


int main(int argc, char **argv)
{
	int ierror = 0;

	int num_pings = 0;
	int num_elements = check_args(argc, argv, &num_pings);

	ierror = MPI_Init(&argc, &argv);

	int my_rank, uni_size;
	my_rank = uni_size = 0;

	ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);

	check_uni_size(uni_size);

        int* array = malloc(num_elements * sizeof(int)); //allocate array of size of the determined size
        for (int i = 0; i < num_elements; i++)
        {
                array[i] = 0; //initalise the arrary to have 0 at every index
        }


	perform_task(my_rank, num_pings, array, num_elements);

	free(array); //free allocated memory once finished with it
	ierror = MPI_Finalize();
	return 0;
}

//function to ensure correct command line argument count and to extract both number of pingpongs and the number of elements in the array
int check_args(int argc, char **argv, int *num_pings) //need a pointer to store the number of pingpongs since we can only return one value in the fucntion
{
	int array_size = 0;

	if (argc == 3) //if three arguments are inputted by the user...
	{
		*num_pings = atoi(argv[1]); //second argument is the number of pingpongs, store value in the buffer
		array_size = atol(argv[2]); //third argument is the size of the array in bytes
	}
	else
	{
		fprintf(stderr, "ERRPR: You have the enter a numerical argument.\n");
		fprintf(stderr, "Correct use: mpirun -np 3 pingpong_bandwidth [number of pin pongs] [array_size]\n");
		exit(-1); //else, abort the program
	}

	if (array_size < sizeof(int)) //if user has entered and array that is too small, tell them and exit
	{
		fprintf(stderr, "Array size must be at least 4 bytes\n");
		exit(-1);
	}

	int num_elements = array_size / sizeof(int); //convert array size to num of elemetnts

	return num_elements; //return the number of elements to pass into other functions in main
}

//ensure process has two processes
void check_uni_size(int uni_size)
{
	if (uni_size == 2)
	{
		return;
	}
	else
	{
		fprintf(stderr, "ERROR: The program only works with a root process and a single client.\n");
		fprintf(stderr, "Correct use: mpicc -np 2 pingpong [number of ping and pongs] [array_size]\n");
		exit(-1);
	}
}

//function to perform the root task
void root_task(int num_pings, int* array, int num_elements)
{
	array[0] = 0; //use the first element in the array as the counter
	int source = 1;
	int dest = 1;
	int tag = 0;
	MPI_Status status;

	double start_time = MPI_Wtime();

	while (array[0] < num_pings) //while counter is less than the amount of ping pongs
	{
		MPI_Send(array, num_elements, MPI_INT, dest, tag, MPI_COMM_WORLD); //Send out ping

		MPI_Recv(array, num_elements, MPI_INT, source, tag, MPI_COMM_WORLD, &status); // Recieve pong
	}

	MPI_Send(array, num_elements, MPI_INT, dest, tag, MPI_COMM_WORLD); //final signal so client exists its infinite loop

	double end_time = MPI_Wtime();
	double time_running = end_time - start_time;
	double average_time = time_running / num_pings; //average time per pingpong

	//show results
	printf("Runtime: %f seconds\n", time_running);
	printf("Average time per pingpong: %f seconds\n", average_time);
	printf("The array has %d elements\n", num_elements);
	printf("Final counter: %d\n", array[0]);
}

//fuction to perform task for the non root process
void client_task(int num_pings, int* array, int num_elements)
{
	int source = 0;
	int dest = 0;
	int tag = 0;
	MPI_Status status;

	while(1)
	{
		MPI_Recv(array, num_elements, MPI_INT, source, tag, MPI_COMM_WORLD, &status); //recieve ping, along with the array
		if (array[0] >= num_pings) //if counter is greater than inputted pingpings...
		{
			break; //end
		}

		array[0] = array[0] + 1; //for each pingpomg increase the counter by 1 (counter still located at index zero of the array)

		MPI_Send(array, num_elements, MPI_INT, dest, tag, MPI_COMM_WORLD);//send pong, and the array with the updated counter back
	}
}

void perform_task(int my_rank, int num_pings, int* array, int num_elements)
{
	if (my_rank == 0)
	{
		root_task(num_pings, array, num_elements);
	}
	else
	{
		client_task(num_pings, array, num_elements);
	}
}
