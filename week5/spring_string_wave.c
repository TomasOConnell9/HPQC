#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void check_args(int argc, char **argv, int* points, int* cycles, int* sample, char** output_path);//update function to check the command line arguments put in by the user and update input values via pointers
void initialise_vector(double vector[], int size, double initial);
void print_vector(double vector[], int size);
int sum_vector(int vector[], int size);
void update_positions(double* positions, double* velocities, double* accelerations, int points, double time);
int generate_timestamps(double* time_stamps, int time_steps, double step_size);
double driver(double time);
void print_header(FILE** p_out_file, int points);


int main(int argc, char **argv)
{
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


	double* velocities = (double*) malloc(points * sizeof(double));
	double* accelerations = (double*) malloc(points * sizeof(double));
	initialise_vector(velocities, points, 0.0);
	initialise_vector(accelerations, points, 0.0);



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

        // iterates through each time step in the collection
        for (int i = 0; i < time_steps; i++)
        {
                // updates the position using a function with updated parameters
                update_positions(positions, velocities, accelerations, points, time_stamps[i]);

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

        // if we use malloc, must free when done!
        free(time_stamps);
        free(positions);
	free(velocities);
	free(accelerations);
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
void update_positions(double* positions, double* velocities, double* accelerations, int points, double time)
{
        double k = 0.5;
	double m = 1.0;
	double time_step = 1;

        // initialises the index
        int i = 0;
        positions[i] = driver(time);

        // creates new positions by setting value of previous element
        for (i = 1; i < points - 1; i++)
        {
                double force;
		// calculate force on each point
		force = -k * (positions[i] - positions[i-1]) + -k * (positions[i] - positions[i+1]);
		accelerations[i] = force / m; // f = ma
        }
        // propagates these new positions to the old ones
        for (i = 1; i < points - 1; i++)
        {
                velocities[i] += accelerations[i] * time_step;
		positions[i] += velocities[i] * time_step;
        }

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
