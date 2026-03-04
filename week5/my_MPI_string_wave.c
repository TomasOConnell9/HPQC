#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

void check_args(int argc, char **argv, int* points, int* cycles, int* sample, char** output_path);//update check args to see user input
void initialise_vector(double vector[], int size, double initial);
void print_vector(double vector[], int size);
int sum_vector(int vector[], int size);
void update_positions(double* positions, int points, double time, double value_coming_across, int is_root);
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
	double start_time, end_time;
	MPI_Status status;
	MPI_Request recv_req, send_req;
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

	//need somewhere to store all our values in rank 0
	double* all_positions = (double*) malloc(points * sizeof(double));

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

	start_time = MPI_Wtime();

	// iterate through timestep and and update position
	for (int t = 0; t < time_steps; t++)
	{
		//initalise the value that has to come across
		double value_coming_over = 0.0;

		// we need to send the receive the last element in the previous array
		if (my_rank != 0)
		{
			// value is 0.0 currently, when in root we dont add anything
			// this needs to be non-blocking
			MPI_Irecv(&value_coming_over, 1, MPI_DOUBLE, my_rank - 1, 0, MPI_COMM_WORLD, &recv_req);
		}

		// update every value excluding the left most value or index 0
		if (my_rank == 0)
		{
			update_positions(my_positions, my_points, time_stamps[t], value_coming_over, 1);
		}
		else
		{
			// here we are skipping the first element as we dont want to update yet
			update_positions(my_positions + 1, my_points -1, time_stamps[t], value_coming_over, 1);
		}

		//for all non root processes we need to wait for the boundary
		if (my_rank != 0)
		{
			MPI_Wait(&recv_req, &status);
		}

		//update the boundary with driver (rank 0) or the new value coming from previous process
		if (my_rank == 0)
		{
			my_positions[0] = driver(time_stamps[t]);
		}
		else
		{
			my_positions[0] = value_coming_over;
		}

		// send so iteration can recieve
		if (my_rank != uni_size - 1)
		{
			MPI_Isend(&my_positions[chunk - 1], 1, MPI_DOUBLE, my_rank + 1, 0, MPI_COMM_WORLD, &send_req);
		}
		//gather up all the positions
		MPI_Gather(my_positions, chunk, MPI_DOUBLE, all_positions, chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);

		if (my_rank == 0)
		{
			fprintf(out_file, "%d, %lf", t, time_stamps[t]);
			for (int j = 0; j < points; j++)
                	{
                        	// prints each y-position to a file
                        	fprintf(out_file, ", %lf", all_positions[j]);
                	}
                	// prints a new line
                	fprintf(out_file, "\n");
		}
	}

	end_time = MPI_Wtime();

	if (my_rank == 0)
	{
		printf("Time it took for the csv to be created: %f seconds\n", end_time - start_time);
	}

	// free malloc when done
	free(time_stamps);
	free(my_positions);
	free(all_positions);
	if (my_rank == 0)
	{
		fclose(out_file);
	}
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
void update_positions(double* positions, int points, double time, double value_coming_across, int is_root)
{
	// creates a temporary vector variable for the new positions
        double* new_positions = (double*) malloc(points * sizeof(double));

	if (is_root)
	{
		new_positions[0] = driver(time);
	}
	else
	{
		new_positions[0] = value_coming_across;
	}

	// creates new positions by setting value of previous element
	for (int i = 1; i < points; i++)
	{
		new_positions[i] = positions[i-1];
	}
	// propagates these new positions to the old ones
	for (int i = 0; i < points; i++)
        {
                positions[i] = new_positions[i];
        }

	// frees the temporary vector
	free(new_positions);
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
