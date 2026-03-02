#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

	check_args(argc, argv, &points, &cycles, &sample, &output_path);

	int ierror =0;
	ierror = MPI_Init(&argc, &argv);

	ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);

	MPI_task(points, cycles, sample, output_path, my_rank, uni_size);

	MPI_Finalize();

        return 0;
}


int MPI_task(int points, int cycles, int sample, char* output_path, int my_rank, int uni_size)
{
	int time_steps = cycles * sample + 1;
	double step_size;

	int chunk = points / uni_size; //how many points per process
	int leftover = points % uni_size;
	my_points = chunk; //each rank recieves a chunk

	//allocate the leftover values to root task
	if (my_rank == 0)
	{
		my_points += leftover;
	}

        // creates a vector for the time stamps in the data and generate them
        double* time_stamps = (double*) malloc(time_steps * sizeof(double));
        initialise_vector(time_stamps, time_steps, 0.0);
        generate_timestamps(time_stamps, time_steps, step_size);


	//split each process into their chunk

	//for each chunk, update their position for each time_step

	//for each MPI process send last value from its array to the next array since update_position needs i-1

	//then we need to root to gather all the positions and write to the outfile


	FILE* outfile;
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
