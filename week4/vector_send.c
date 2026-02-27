#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

//declare fucntions
int root_task(int uni_size, int num_args);
void client_task(int num_args);
int check_args(int argc, char **argv);
void check_uni_size(int uni_size);
void check_task(int uni_size, int my_rank, int num_args);

int main(int argc, char **argv)
{
	int ierror = 0;

	int num_args = check_args(argc, argv);

	ierror = MPI_Init(&argc, &argv);

	int my_rank , uni_size;
	my_rank = uni_size = 0;

	ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);

	check_uni_size(uni_size);

	check_task(uni_size, my_rank, num_args);

	ierror = MPI_Finalize();

	return 0;
}


int check_args(int argc, char **argv)
{
        // declare and initialise the numerical argument
        int num_arg = 0;

        // check the number of arguments
        if (argc == 2) // program name and numerical argument
        {
                // declare and initialise the numerical argument
                num_arg = atoi(argv[1]);
        }
        else // the number of arguments is incorrect
        {
                // raise an error
                fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
                fprintf(stderr, "Correct use: mpicc -n 4 proof [NUMBER]\n");

                // and exit COMPLETELY
                exit (-1);
        }
        return num_arg;
}



void check_uni_size(int uni_size)
{
        // sets the minimum universe size
        int min_uni_size = 1;
        // checks there are sufficient tasks to communicate with
        if (uni_size >= min_uni_size)
        {
                return;
        } // end if (uni_size >= min_uni_size)
        else // i.e. uni_size < min_uni_size
        {
                // Raise an error
                fprintf(stderr, "Unable to communicate with fewer than %d processes.", min_uni_size);
                fprintf(stderr, "MPI communicator size = %d\n", uni_size);

                // and exit COMPLETELY
                exit(-1);
        }
}


int root_task(int uni_size, int num_args)
{
	double start_time;
	double end_time;

	int *my_array = malloc(num_args * sizeof(int));//initalise pointer to array **

	//iterate through each index of the created array and change the value to its index plus one.
	for (int i = 0; i < num_args; ++i)
	{
		my_array[i] = i + 1;
	}

	int chunk = num_args / (uni_size - 1); //calculate the size of each chunk for each client
	int leftover = num_args % (uni_size - 1);//calculate the remaining values after dividing the array by the amount of clients.

	start_time = MPI_Wtime();

	//for each client, send the required chunk size
	for (int dest = 1; dest < uni_size; dest++)
	{
		int start = (dest - 1) * chunk;
		int send_length = chunk;

		if (dest == uni_size - 1)
		{
			send_length = send_length + leftover;
		}

		MPI_Send(my_array + start, send_length, MPI_INT, dest, 0, MPI_COMM_WORLD);
	}

	long long total_sum = 0;
	long long recv_message = 0;
	MPI_Status status;

	//for each client task sum all their calcualtions to get a final total sum
	for (int source = 1; source < uni_size; source++)
	{
		MPI_Recv(&recv_message, 1, MPI_LONG_LONG, source, 0, MPI_COMM_WORLD, &status);
		total_sum += recv_message;
	}

	end_time = MPI_Wtime();

	printf("The sum of the array is %lld\n", total_sum);
	printf("Time it took to sum the array: %f\n", end_time - start_time);

	free(my_array);

	return total_sum;
}

void client_task(int num_args)
{
	MPI_Status status;
	int chunk_length;
	int *my_array = malloc(num_args * sizeof(int));
	long long int sum_of_my_array = 0;

	MPI_Recv(my_array, num_args, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	MPI_Get_count(&status, MPI_INT, &chunk_length);

	for (int i = 0; i < chunk_length; ++i)
	{
		sum_of_my_array += my_array[i];
	}

	MPI_Send(&sum_of_my_array, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
	free(my_array);
}


void check_task(int uni_size, int my_rank, int num_args)
{
        if (my_rank == 0)
        {
                root_task(uni_size, num_args);
        }
        else
        {
                client_task(num_args);
        }
}
