
# Week 2

# This README.md file contains instructions on how the benchmarking test was ran for each script and the summary of my findings for each test.

Part 1: Time to print

The first benchmarking test conducted was testing the print time of numbers to terminal. 
Using the given scripts time_print.py and time_print.c a comparison between run.
First, run "python3 time_print.py {n}" where n in the number of desired numbers, this will print all n numbers and then return the length of time it took for th
Next, ensure the time_print.c file is compiled using "gcc time_print.c -o {filename}.
Once successfully compiled run "./{filename} {n}, where filename is the compile.
Using the results of both scripts we can compare the run times for each language.

For small values of n, python ran marginally outperformed C by microseconds.
At roughly n=1,000 C catches up in speed, and by n=10,000 C is sighly faster.
As n sours C increasingly becomes more dominant, at n=10,000,000 C runs in 4.6 seconds while it takes python 7.3 seconds to perform the same task.

Part 2: Time to write to file

For the second benchmarking test, I took the existing time_print scripts and altered them to store their data in a txt file.
As mentioned as part of the assignment I created a data directory to keep the data files away from my git repository.
The python script saves each data set to the data directory with the user selected n in the name to make it easier for part 3. C however just saves the data with a common name regardless of n.

Similar to part 1, the python script is ran with the same command. This time the data is not printed on screen but instead saved to an external file.
The script still returns a "time" result informing how long it took to perform the task.
The same process occurs for C.

Comparing the results, C is much quicker regardless of the inputted n. Unlike part 1, where python became slower as n increased, python is much slower from the beggining.
As n grows, the difference between run time also grows. When n = 10,000,000 python takes 4.267 seconds whereas in C it only takes 1.3 seconds.
Additionally, it's evident that it takes less time to save the data to a txt file than it does to print the results in terminal. For example, writing 10 million numbers to terminal takes 7.3s but only 4.27 to write to a file.

Part 3: Time to read

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


