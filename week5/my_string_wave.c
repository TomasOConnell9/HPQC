#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void check_args(int argc, char **argv, int* points, int* cycles, int* sample, char** output_path);//update function to check the command line arguments put in by the user and update input values via pointers
void initialise_vector(double vector[], int size, double initial);
void print_vector(double vector[], int size);
int sum_vector(int vector[], int size);
void update_positions(double* positions, int points, double time);
int generate_timestamps(double* time_stamps, int time_steps, double step_size);
double driver(double time);
void print_header(FILE** p_out_file, int points);
double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);

int main(int argc, char **argv)
{
	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;

        // declare and initialise input variables
        int points, cycles, samples;
        char* output_path;

        //pass the address of the variables into check_args
        check_args(argc, argv, &points, &cycles, &samples, &output_path);

        // creates variables for the vibration
        int time_steps = cycles * samples + 1; // total timesteps
        double step_size = 1.0/samples;

        // creates a vector for the time stamps in the data
        double* time_stamps = (double*) malloc(time_steps * sizeof(double));
        initialise_vector(time_stamps, time_steps, 0.0);
        generate_timestamps(time_stamps, time_steps, step_size);

        // creates a vector variable for the current positions
        double* positions = (double*) malloc(points * sizeof(double));
        // and initialises every element to zero
        initialise_vector(positions, points, 0.0);

        // creates a file
        FILE* out_file;
        out_file = fopen(output_path,"w");
	//since user in allocating the out_file is the out_file cannot be opened, send an error.
	if (out_file == NULL)
	{
		fprintf(stderr, "ERROR, could not open this file");
		exit(-1);
	}
        print_header(&out_file, points);

	timespec_get(&start_time, TIME_UTC);

        // iterates through each time step in the collection
        for (int i = 0; i < time_steps; i++)
        {
                // updates the position using a function
                update_positions(positions, points, time_stamps[i]);

                // prints an index and time stamp
                fprintf(out_file, "%d, %lf", i, time_stamps[i]);

                // iterates over all of the points on the line
                for (int j = 0; j < points; j++)
                {
                        // prints each y-position to a file
                        fprintf(out_file, ", %lf", positions[j]);
                }
                // prints a new line
                fprintf(out_file, "\n");
        }

	timespec_get(&end_time, TIME_UTC);
	// calculates the runtime
	time_diff = calculate_runtime(start_time, end_time);
	runtime = to_second_float(time_diff);


	// outputs the runtime
	printf("\n\nRuntime for core loop: %lf seconds.\n\n", runtime);


        // if we use malloc, must free when done!
        free(time_stamps);
        free(positions);

        // closes the file
        fclose(out_file);

        return 0;
}

void check_args(int argc, char **argv, int* points, int* cycles, int* sample, char** output_path)
{
        // declare and initialise the numerical argument
        int num_arg = 0;

        // check the number of arguments is now 5
        if (argc == 5) // program name and numerical argument
        {
                // declare each argument to its corrisponding variable
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
        // creates a temporary vector variable for the new positions
        double* new_positions = (double*) malloc(points * sizeof(double));

        // initialises the index
        int i = 0;
        new_positions[i] = driver(time);
        // creates new positions by setting value of previous element
        for (i = 1; i < points; i++)
        {
                new_positions[i] = positions[i-1];
        }
        // propagates these new positions to the old ones
        for (i = 0; i < points; i++)
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


double to_second_float(struct timespec in_time)
{
	// creates and initialises the variables
	float out_time = 0.0;
	long int seconds, nanoseconds;
	seconds = nanoseconds = 0;

	// extracts the elements from in_time
	seconds = in_time.tv_sec;
	nanoseconds = in_time.tv_nsec;

	// calculates the time in seconds by adding the seconds and the nanoseconds divided by 1e9
	out_time = seconds + nanoseconds/1e9;

	// returns the time as a double
	return out_time;
}


struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time)
{
	// creates and initialises the variables
	struct timespec time_diff;
	long int seconds, nanoseconds;                                                                                                       seconds = nanoseconds = 0;
	double runtime = 0.0;

	// extracts the elements from start_time and end_time
	seconds = end_time.tv_sec - start_time.tv_sec;
	nanoseconds = end_time.tv_nsec - start_time.tv_nsec;

	// if the ns part is negative
	if (nanoseconds < 0)
	{
		// "carry the one!"
		seconds = seconds - 1;
		nanoseconds = ((long int) 1e9) - nanoseconds;
	}

	// creates the runtime
	time_diff.tv_sec = seconds;
	time_diff.tv_nsec = nanoseconds;

	return time_diff;
}
