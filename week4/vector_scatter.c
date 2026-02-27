#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

//declare fucntions
int root_task(int uni_size, int num_args);
void client_task(int num_args, int uni_size, int my_rank);
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

        int *my_array = malloc(num_args * sizeof(int)); //allocate buffer to store array

        //iterate through each index of the array and update each index with its index value plus one
        for (int i = 0; i < num_args; ++i)
        {
                my_array[i] = i + 1;
        }

        int chunk = num_args / (uni_size); //calculate the number of elements assigned to each process
        int leftover = num_args % (uni_size);//calculate the remaining elements when the array has been evenly divided

	int *recvbuf = malloc(chunk * sizeof(int));//MPI_Scatter sends a chunk to root, allocate a buffer for this chunk

        start_time = MPI_Wtime();//start timer before starting the communication

	MPI_Scatter(my_array, chunk, MPI_INT, recvbuf, chunk, MPI_INT, 0, MPI_COMM_WORLD);//Scatter the array into evenly distributed chunks

	//send remaining elements to the last process if the array is not divisible by the number of process
	if (leftover > 0)
	{
		MPI_Send(my_array + (uni_size * chunk), leftover, MPI_INT, (uni_size - 1), 0, MPI_COMM_WORLD);
	}

	//calculate the sum of the root processes chunk
        long long root_sum = 0;
	for (int i = 0; i < chunk; ++i)
	{
		root_sum += recvbuf[i];
	}


	long long total_sum = root_sum;
	long long recv_message = 0;
        MPI_Status status;

        //recieve the sums from each process and accumalte to the total sum
        for (int source = 1; source < uni_size; source++)
        {
                MPI_Recv(&recv_message, 1, MPI_LONG_LONG, source, 0, MPI_COMM_WORLD, &status);
                total_sum += recv_message;
        }

        end_time = MPI_Wtime();//end timer once the final calculation is done

        printf("The sum of the array is %lld\n", total_sum);
        printf("Time it took to sum the array: %f\n", end_time - start_time);

	//free alocated memory
        free(my_array);
	free(recvbuf);

        return total_sum;
}

void client_task(int num_args, int uni_size, int my_rank)
{
	MPI_Status status;
        int chunk = num_args / uni_size;
	int leftover = num_args % uni_size;
        int *my_array = malloc(num_args * sizeof(int));
        long long int sum_of_my_array = 0;

        MPI_Scatter(NULL, chunk, MPI_INT, my_array, chunk, MPI_INT, 0, MPI_COMM_WORLD);//recieve the chunk from root

	//add every value in the chunk
        for (int i = 0; i < chunk; ++i)
        {
                sum_of_my_array += my_array[i];
        }

	//if we are the final procces accept the leftover elements and add them to the total sum
	if (my_rank == uni_size - 1 && leftover > 0)
	{
		int *leftover_values = malloc(leftover *sizeof(int));
		MPI_Recv(leftover_values, leftover, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);


		for (int i = 0; i < leftover; ++i)
		{
			sum_of_my_array += leftover_values[i];
		}
		free(leftover_values);
	}

	//send the total back to root
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
                client_task(num_args, uni_size, my_rank);
        }
}
