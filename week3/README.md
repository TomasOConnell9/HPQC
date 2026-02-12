# Week 3 - Introduction to MPI

##Part 1: MPI Hello World

I created my own version of "hello_mpi.c" called "my_version_hello_mpi.c". This implemented a basic MPI hello world program and introduced me to the basics of MPI programming.
This file was compiled in the root directory using "mpicc" and was saved to the bin directory, as my_version_hello_mpi"
I then exectured the file with "mpirun -np 4 bin/my_version_hello_mpi" which displayed the 4 different ranks.
The last step in this part was to use the time command to evaluate the run times for different number of processes. Each different value for num of proccess verified that parallel processing is going on since user + sys > real.

##Part 2: MPI exercise breakdown

#Documenting the code:

main() - This function initalises the error handeling variables that are required for a MPI script. It also initialises and finalises the MPI function, again without this the script wouldn't work. It also runs multiple checks to extract and validate the numerical inputs of the system.

root_taks() - This fucntion recieves the values from all the client processors and adds all the recieved values together. It also outputs the final result. This function is only ran in the root processor.

client_task() - The calculation for each processes is performed here. The process's rank is multiplied by the input numerical value, and the result is sent to the root processor.

check_args() - This function ensures that the numerical command line that was passed has the correct number of arguments. It also converts it from a string to an integer. If the number of arguments is invalid the program is terminated and recommended arguments are displayed to the user.

check_uni_size() - This function makes sure that the amount of process's is at least 1. If less than 1 the program terminates. (I have a feeling that this should be > 1 not >= 1. If 1 is selected than we have no clients as the 1 is our root proccess and we need to have at least one client)

check_task() - The check_task() function checks if the process is the root or a client. If we are the root case we perform the root_task(), and if we are a client we perform client_task().

#Mathermatical operation:

N(N+1)/2

##Part 3: MPI Vector Addition

#Modifying the serial code

The final task required first the given script "vector_serial.c" to be modified such that the script would run with non-zero values.
I altered the script so that the vector created was a vector increasing from 1 to n where n was the user selected size of the vector.
For example, after compiling the code, running "./bin/my_serial_vector 5" returns the sum of all didgets from 1 to 5.

#Creating the MPI version

Lastly, based on the logic of the serial_vector.c and the given pseudocode in the lecture notes a parallel version of the code was designed and tested.
The parallel code was designed using the same implementation of MPI as was in the proof.c program. The bulk of the changes come in the client_task() function.
client_task() takes in an extra argument, uni_size, to ensure that all the parallel processes are dividing the work evenly. 
The logic within the function is also altered to represent the fucntion we are trying to create. Within the function I designed it to calculate the average size of each chunk by dividing the size of the vector by the amount of processes we have. Next I calculate a "leftover" amount which represents the amount of remaining numbers when the vector size is not evenly divisible by the amount of processes.
Similar to the pseudocode given in the lectures, I set the start point of the chunk as the end point of the previous chunks work. To avoid more complicated code, I simply add the leftover numbers in the vector to the final process. 
The function loops through each number of its given vector, updating "sum_of_this_chunk" as it goes. Once the loop reaches its stop point. The function sends the message to the root.
I have it set so the root does no summing of vector chunks but instead only recieves and sums the chunks from each process.

The last change made was to both the serial and MPI version. They were updated to use long long int for the sums of the vectors as when the input got very large it couldn't handle it. 

#Benchmarking

Using "time" on both the serial and the MPI versions of the program we can test to find where the parallel version completes faster in real time than the serial one.
Without updating the datatype to long long int, the parallel version failed to compelte faster than the serial one without encountering overflow errors.
Once the datatype change was implemented I tested multiple values for n across both programs to find the point in which parallel outperformed the serial.
Up until 50 million serial outperforms parallel marginally. Once the input reaches 60 million the two complete at roughly the same time. However, from here the parallel version takes off and starts performing much better.
At 100 million the serial program takes roughly 0.7s whereas the parallel program takes 0.5s. 
The power of the parallel system can be seen as we increase n even further. At n = 1 billion it takes the serial program 7.5s and only takes the parallel system 1.3s. 


***Running the files***
Compile the "my_version_hello_mpi.c" with "mpicc my_version_hello_mpi.c -o  my_hello_mpi". Then to run it "mpirun -np n my_hello_mpi" where n is the number of processes you want to run.
Benchmark the program using the time command.

For the serial script "my_vector_serial.c" use "gcc my_vector_serial.c -o my_vector_serial" and run it using "./my_vector_serial N" where N is the size of the vector.
For the parallel script use "mpicc vector_MPI.c -o  vector_MPI" and run it using "mpirun -np n vector_MPI N" again where n is the number of processes and N is the size of the vector.

