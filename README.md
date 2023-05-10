# Multi-threaded_Name_Counter_with_Shared_Data_Structure
This program employs multi-threading in C language to count names from two input files and update a shared data structure. The functionality is the same as Assignment 2, but we have simplified things by assuming that there are exactly two input files of names. If the user provides one or more than two files, the program will print a suitable error message and exit.

# Installation and Usage
To compile the program, run the following command in the terminal:
'''gcc -D_REENTRANT -pthread -o countnames_threaded countnames_threaded.c -Wall -Werror'''

To run the program, provide two input files as command-line arguments:
''' ./countansmes_threaded file1.txt file2.txt'''

# Implementation Details
The program uses two threads, with Thread 1 reading names from one file and Thread 2 reading names from the other file. Each thread updates a shared name counts data structure with the counts of names. The program uses a hash table as the data structure to avoid locking an entire array, which would be slow.

The program protects the allocation and deallocation of THREADDATA object and the updating of log message index and name counts with three critical sections and three mutex locks. Each thread prints a log message to stdout when it opens a new file for reading, with the log index incremented for each message printed to stdout. The log messages have a fixed format of log index, thread index, PID, and date and time.

# Limitations
The program assumes that there are exactly two input files of names, with at most 100 names of length at most 30 characters. If the input files have more than 100 names or longer than 30 characters, the program may not work as expected.

# Future Work
Future work on this program could include:

Adding support for variable numbers of input files
Allowing the user to specify the maximum number of names and the maximum name length
Implementing a more efficient data structure for name counts
Adding support for non-ASCII characters in names
