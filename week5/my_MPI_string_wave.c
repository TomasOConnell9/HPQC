#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

void check_args(int argc, char **argv, int* points, int* cycles, int* sample, char** output_path);//update check args to see user input
void initialise_vector(double vector[], int size, double initial);
void print_vector(double vector[], int size);
int sum_vector(int vector[], int size);
void update_positions(double* positions, int points, double time);
int generate_timestamps(double* time_stamps, int time_steps, double step_size);
double driver(double time);
void print_header(FILE** p_out_file, int points);
void MPI_task(int points, int cycles, int sample, char* output_path, int my_rank, int uni_size); //new function to perform the MPI task

int main(int argc, char **argv)
{
	int points, cycles, sample;
	char* output_path;

	int my_rank, uni_size;

	check_args(argc, argv, &points, &cycles, &sample, &output_path);

	int ierror =0;
	ierror = MPI_Init(&argc, &argv);

	ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);

	MPI_task(points, cycles, sample, output_path, my_rank, uni_size);

	MPI_Finalize();

        return 0;
}


void MPI_task(int points, int cycles, int sample, char* output_path, int my_rank, int uni_size)
{
	int MPI_Status status;
	int time_steps = cycles * sample + 1;
	double step_size = 1.0 / sample;

	int chunk = points / uni_size; //how many points per process
	int leftover = points % uni_size;
	int my_points = chunk; //each rank recieves a chunk

	//allocate the leftover values to root task
	if (my_rank == 0)
	{
		my_points += leftover;
	}

        // creates a vector for the time stamps in the data and generate them
        double* time_stamps = (double*) malloc(time_steps * sizeof(double));
        initialise_vector(time_stamps, time_steps, 0.0);
        generate_timestamps(time_stamps, time_steps, step_size);

	// get reach ranks positions
	double* my_positions = malloc(my_points * sizeof(double));
	initialise_vector(my_positions, my_points, 0.0);


	// iterate through timestep and and update position
	for (int t = 0; t < time_steps; t++)
	{
		// we need to send the receive the last element in the previous array
		if (rank != 0)
		{
			//not sure how to do right now, comeback to this
			MPI_Recv(/*address of where im recving*/, 1, MPI_DOUBLE, my_rank - 1, 0, MPI_COMM_WORLD, &status);
		}

		// update the function using the function
		update_positions(my_positions, my_points, time_stamps[i];

		// send so iteration can recieve
		if (rank != uni_size - 1)
		{
			MPI_Send(local_positions[my_points -1], 1, MPI_DOUBLE, my_rankk + 1, 0, MPI_COMM_WORLD);
		}
		//gather up all the positions
		MPI_Gather();
	}

	


	FILE* out_file;
	if (my_rank ==0)
	{
		out_file = fopen(output_path, "w");
		if (out_file == NULL)
		{
			fprintf(stderr, "ERROR, could not open this file");
			exit(-1);
		}
		print_header(&out_file, points);
	}

	//
}


void check_args(int argc, char **argv, int* points, int* cycles, int* sample, char** output_path)
{
        // declare and initialise the numerical argument
        int num_arg = 0;

        // check the number of arguments is now 5
        if (argc == 5) // program name and numerical argument
        {
                // declare and initialise the numerical argument
                *points = atoi(argv[1]);
                *cycles = atoi(argv[2]);
                *sample = atoi(argv[3]);
                *output_path = argv[4];
        }
        else // the number of arguments is incorrect
        {
                // raise an error
                fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
                fprintf(stderr, "Correct use: %s [POINTS] [CYCLES] [SAMPLES] [OUTPUT_PATH]\n", argv[0]);

                // and exit COMPLETELY
                exit (-1);
        }
}



// prints a header to the file
// double-pointer used to allow this function to move the file pointer
void print_header(FILE** p_out_file, int points)
{
        fprintf(*p_out_file, "#, time");
        for (int j = 0; j < points; j++)
        {
                fprintf(*p_out_file, ", y[%d]", j);
        }
        fprintf(*p_out_file, "\n");
}

// defines a simple harmonic oscillator as the driving force
double driver(double time)
{
        double value = sin(time*2.0*M_PI);
        return(value);
}

// defines a function to update the positions
void update_positions(double* positions, int points, double time)
{
        
}



// defines a set of timestamps
int generate_timestamps(double* timestamps, int time_steps, double step_size)
{
        for (int i = 0; i < time_steps ; i++)
        {
                timestamps[i]=i*step_size;
        }
        return time_steps;
}

// defines a function to sum a vector of ints into another int
int sum_vector(int vector[], int size)
{
        // creates a variable to hold the sum
        int sum = 0;

        // iterates through the vector
        for (int i = 0; i < size; i++)
        {
                // sets the elements of the vector to the initial value
                sum += vector[i];
        }

        // returns the sum
        return sum;
}


// defines a function to initialise all values in a vector to a given inital value
void initialise_vector(double vector[], int size, double initial)
{
        // iterates through the vector
        for (int i = 0; i < size; i++)
        {
                // sets the elements of the vector to the initial value
                vector[i] = initial;
        }
}

// defines a function to print a vector of ints
void print_vector(double vector[], int size)
{
        // iterates through the vector
        for (int i = 0; i < size; i++)
        {
                // prints the elements of the vector to the screen
                printf("%d, %lf\n", i, vector[i]);
        }
}
