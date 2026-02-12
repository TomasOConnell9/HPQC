#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// declare my functions
int root_task(int uni_size);
void client_task(int my_rank, int num_arg, int uni_size);
int check_args(int argc, char **argv);
void check_uni_size(int uni_size);
void check_task(int uni_size, int my_rank, int num_arg);


int main(int argc, char **argv)
{
	int ierror = 0;

	int num_arg = check_args(argc, argv);

	ierror = MPI_Init(&argc, &argv);

	int my_rank = 0;
	int uni_size = 0;

	ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);

	check_uni_size(uni_size);
	check_task(uni_size, my_rank, num_arg);

	ierror = MPI_Finalize();
	return 0;
}





int root_task(int uni_size)
{

        // creates and initialies transmission variables
	// initialies with long long int to avoid overflow error
        long long int recv_message;
	int count, source, tag;
        recv_message = source = tag = 0;
        count = 1;
        MPI_Status status;

        // creates and intiialises the variable for the final output
        long long int output_sum = 0;

        // iterates through all the other ranks 
        for (source = 1; source < uni_size; source++)
        {
                // receives the messages, use MPI_LONG_LONG to account for the datatype 
                MPI_Recv(&recv_message, count, MPI_LONG_LONG, source, tag, MPI_COMM_WORLD, &status);
                // adds the values to a running tally
                output_sum += recv_message;
        }

        // outputs and returns the result
        printf("The combined result is %lld\n", output_sum);
        return output_sum;
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

void check_task(int uni_size, int my_rank, int num_arg)
{
        // checks which process is running and calls the appropriate task
        if (0 == my_rank)
        {
                root_task(uni_size);
        }
        else
        {
                client_task(my_rank, num_arg, uni_size);
        }
}

void client_task(int my_rank, int num_arg, int uni_size)
{
        // creates and initialies transmission variables
        long long int send_message;
	int count, dest, tag;
        send_message = dest = tag = 0;
        count = 1;

        // sets the destination for the message
        dest = 0; // destination is root

	// create and initialies the chunk variable which decided how large each chunk is
	int chunk = num_arg / (uni_size - 1);
	//create and initialies the leftover values once vector is devided evenly between the clients 
	int leftover = num_arg % (uni_size - 1);
	int start;
	int stop;
	start = (my_rank - 1) * chunk;
	stop = start + chunk;
	long long int sum_of_this_chunk = 0;

	//if final client add the remaining values to the chunk
	if (my_rank == uni_size - 1)
	{
		stop += leftover;
	}
	//sum every element in the chunk
	for (int i =start; i < stop; i++)
	{
		sum_of_this_chunk += i + 1;
	}
        send_message = sum_of_this_chunk;
	//use a new MPI datatype, MPI_LONG_LONG which is used for the sum since its datatype is long long int
        MPI_Send(&send_message, count, MPI_LONG_LONG, dest, tag, MPI_COMM_WORLD);
}
