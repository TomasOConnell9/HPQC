import sys
import time

def main(): 
    output = 0
    # checks if there are the right number of arguments
    try:
        # converts the first two arguments to integers
        in_arg = int(sys.argv[1])
    except: # (argc != 2)
        # raises an error
        raise Exception("Incorrect arguments.\nUsage: python time_print.py [NUM]\ne.g.\npython time_print.py 23")

    # gets the start time for the loop
    start_time = time.time()
    # set variable to input number
    n = int(sys.argv[1])
    # open the a file in the data directory and write the numbers to that file
    with open(f"data/python_timetowrite_{n}.txt", "w") as f:
        for i in range(in_arg):
            f.write("{},\n ".format(i))
    # gets the end time for the loop
    end_time = time.time()

    # gets the total time
    run_time = end_time - start_time
    # prints the output
    print("\n\nTime for loop: {} seconds\n".format(run_time))

if __name__ == "__main__":
    main()

