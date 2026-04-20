
# Week 2 - Benchmarking C vs Python

## Overview
This weeks taks involved comparing the performance of Python and C across three computational tasks:
1. Printing numbers to the terminal
2. Writing numbers to a file
3. Reading numbers from a file
The main objective is to evaliate how each language scales with increasing input size n, with the hopes of obtaining a clear answer of which language has better performance.


## Part 1: Time to print
### Method:
The first benchmarking test conducted was testing the print time of numbers to terminal using the given scripts `time_print.py` and `time_print.c` located in the repository. The Python script can be run using `python3 time_print.py {n}` where `n` is the desired number of values printed in the terminal. The program will print all `n` numbers and then output the total execution time for the operation.

Next, compile the `time_print.c` script using `gcc time_print.c -o {filename}`, where `{filename}` is the user defined name for the output executable. Once successfully compiled run `./{filename} {n}`, again where `{filename}` is the selected user defined name for the executable.
Using the results of both scripts we can compare the run times for each language.

### Results:
For small values of `n`, Python marginally outperformed C by microseconds. At approximately `n=1,000` C catches up in speed mirroring Pythons times. As `n` increases to standard sized values, C begins to outperform Python consistently with the performance gap becoming more noticeable as `n` increases to larger and larger values.
By `n=10,000,000` C demonstate much better perfromance, completing the task in approximately 4.6 seconds while it takes python 7.3 seconds to perform the same task.

The results observed in part 1 highlight the improved scalability of C for large-scale iterative tasks.

## Part 2: Time to write to file

### Method:
For the second benchmarking test, the existing `time_print` scripts were altered so that instead of the numbers being printed to terminal they were saved to a `.txt` file instead.
A seperate repositiory was made to store the `.txt` files so they don't crowd the current repository.
The Python script saves each dataset to the data directory, with the user defined value of `n` included in the filename to facilitate use in Part 3. In contrast, the C implementation saves all outputs using a single fixed filename, regardless of the value of n.

Similar to part 1, the python script is ran with the same command. This time the data is not printed on screen but instead saved to an external file.
The script outputs the execution time so both languages can be compared. The same process is then applied for the C file. As before the file should first be compiled and then the executable should be ran.

### Results:
Comparing the results, C is consistently faster than Python across all values of `n`. Unlike Part 1, where Python only becomes slower at larger input sizes, it is slower from the outset in this test.
As `n` grows, the difference between run time also grows. When `n = 10,000,000` Python takes approximately 4.267 seconds to complete, whereas C only takes 1.3 seconds.

Additionally, it is evident that writing data to a file is significantly faster than printing results to the terminal. For example, printing 10 million numbers to the terminal takes approximately 7.3 seconds, whereas writing the same data to a file takes only 4.27 seconds.

## Part 3: Time to read

The final part of week2 was to compare how each language did at reading the saved data to memory. Similar to part two, the supplied C and python scripts could be slightly altered to now read a file instead of write to file.  
As mentioned, the python pragram "time_write.py" saves each dataset with the number ran. For example "python3 time_write 1000" saves a file called "python_timetowrite_1000".
Running "python3 time_read.py 10" will open "python_timetowrite_10.txt" and read through each line, until reaching the end and it'll then print the time taken.
For C the file isnt save with a specific name instead the most recent written dataset is called "c_timetowrite.txt". Similar to the python script this text file is then opened by the c pragram and read and time is recorded.
Since there is only one C txt file, before running the "time_read.c" script run the "time_write.c" script for the desrired tested n.

This task was the quickest of the three. Both programs asa expected start out extremely quick taking fractions of a milliscond to read the data to memory.
As n is increaded however, time to read doesn't grow as exponentially as it it did for time to write and time to print for either language.
In python it took a tenth of of the time it takes to write 10,000,000 numbers to a data set than it does to read them.

Comparing the results of both languages, C still retains its advantage only taking 0.226 seconds to read a million numbers, whereas it took python 0.44 seconds to complete the same task.

Conclusion: Runtime Behaviour of C vs Python

From the three benchmarking tests conducted clear trends emerge in the difference between C and Pythons perrformance across different operations.

1. For all taks the size of n is the dominant factor affecting runtime. At small values of n, the difference between C and Python is negligible. However as seen, when n is increased C scales significantly better than Python.

2. Printing to terminal is the slowest operation for both languages. For small n python slightly outperformed C but as n reached 1000 C caught up with python performance wise and continued to out perform it from there. It seems that the main bottleneck in printing data to a screen is the terminal itself.

3. Writing to file is much faster than printing in the terminal. Unlike printing to terminal Python is slower from the outset and gets worse as n increases. C is clearly more efficient at file I/O than Python.

4. The quickest of the 3 tasks was reading the data to memory. Similar to before C quickly outperforms Python as n is increased. The runtimes in general are much slower than that of printing and writing.

My main take away from the benchmarking test is that despite its simplicity and ease to uses, Python is not ideal for all tasks. C is better suited for performance critical tasks or large scale data processing. As the size of the problem grows C is the optimal choice, esspecially is I/O is involved.


