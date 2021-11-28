Lauren Egts
November 28, 2021

This code uses the Banker's Algorithm to determine whether a system is in a safe or unsafe state.

Written in Vim on RHEL 8.

Prerequesites:
Git and g++

Installation:
1. Install git and g++ on your system
2. Clone the repository from this URL: https://github.com/laurenegts/OS_Homework2.git

Usage:
1. Compile with g++ banker.cpp -o banker
2. Run with ./banker inputFile.txt

Sample input files and what they should output:
safe.txt: that the system is in a safe state and the safe sequence is < P0, P1, P2 >
unsafe.txt: that the system is in an unsafe state
allocationTooManyRows.txt: an error stating that allocation matrix has too many rows
allocationTooManyCols.txt: an error stating that the max matrix has too many columns
negativeInputAllocation.txt: an error stating that there is a negative number in the allocation matrix
maxTooManyRows.txt: an error stating that the max matrix has too many rows
maxTooManyCols.txt: an error stating that the max matrix has too many columns
negativeInputMax.txt: an error stating that there is negative input in the max matrix
negativeInputAvailable: an error stating that there is negative input in the available matrix

Input file requirements:

File extension is .txt.

Line 1 is the available matrix, the amount of each resource initially available to the system.
	Each entry is separated by a space.  All resources must be specified in this line, even
	if there are no instances of a resource initially available.
Subsequent lines are the allocation matrix, which contains the number of resources allocated
to a process at a current point in time.  
	If allocation[i][j] = k, then process i is currently allocated k instances of resource j.  
	The rows in the allocation matrix must be in increasing order from P0, P1, ..., Pn.  
	Each process has its own row indicating how many instances of what resource it has been allocated.  Values are separated by spaces.
	The columns of the allocation matrix must correspond to the resources in the order that they are placed in the available matrix in line 1.
Following the allocation matrix, there is one line that says "Max:", without the quotes.
Subsequent lines are the max matrix, which defines the maximum resources required by each process.
	If max[i][j] = k, then process i needs at most k instances of resource j.
	The rows of the max matrix must be in increasing order from P0, P1, ..., Pn.  
	Each process has its own row indicating its resource needs.  The values are separated by a space and end in a new line.
	The columns of the max matrix must contain the resources in the same order that they are placed in the available matrix in line 1.

If there is missing or extra data in a matrix, the program will exit with an error.
All numbers in the input file must be non-negative.

Example input file:
The following system has 5 processes, P0 through P4, and 3 types of resources (A, B, and C), and is given as follows:
Available matrix:
    A B C
    3 3 2

Allocation matrix:
    A B C
P0: 0 1 0
P1: 2 0 0
P2: 3 0 2
P3: 2 1 1
P4: 0 0 2

Max matrix:
    A B C
P0: 7 5 3
P1: 3 2 2
P2: 9 0 2
P3: 2 2 2
P4: 4 3 3

The input file for this system would be:
3 3 2
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2
Max:
7 5 3
3 2 2
9 0 2
2 2 2
4 3 3
