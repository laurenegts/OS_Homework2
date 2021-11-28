#include <iostream>	// for printing
#include <vector>	// for vectors
#include <fstream>	// to read from files
#include <string>	// for string manipulation, convert string to int
using namespace std;

/* 
 * Utility function used when reading in input.  Converts a string of ints into a vector of ints.
 * INPUT: a string of integers separated by spaces.
 * OUTPUT: a vector containing the integers, if all of the integers in the string are non-negative
 * 	   a vector containing -1 if an integer in the string is negative, indicating that the 
 * 	   program should exit with an error because in this algorithm, the system cannot have a
 * 	   negative number of resources available, and a process cannot be allocated or request a 
 * 	   negative number of resources.
 * 	    	   
 * Based on the algorithm found here: slaystudy.com/c-split-string-by-space-into-vector
*/
vector<int> convertStringToIntVector(string toConvert){
	vector<int> toReturn;
	string temp = "";
        
	for( int i = 0; i < toConvert.length() + 1; i++ ){
		if(toConvert[i] == ' ' || i == toConvert.length()){
			int intVal = stoi(temp);
			// Error checking: if the value is negative, return an error so that the program
			// can exit in main
			if( intVal < 0 ){
				vector<int> errorVector (1, -1);
				return errorVector;
			}
			toReturn.push_back(intVal);
			temp = "";
		}
		else{
			temp.push_back(toConvert[i]);
		}
	}
	return toReturn;
}

/* 
 * Utility function to print an int vector to cout.
 * INPUT: a vector of ints.
 * OUTPUT: each int in the vector, separated by a space, ending in a new line.
 */
void printIntVector(vector<int> toPrint) {
	for( int i = 0; i < toPrint.size(); i++){
		cout << toPrint.at(i) << " ";
	}
	cout << "\n";
}

/* 
 * Utility function to print a vector of int vectors to cout.
 * INPUT: a vector of int vectors.
 * OUTPUT: each int vector is printed on its own line, with its contents separated by spaces.  
 * For example, given the input < <1, 2, 3>, <4, 5, 6>, <7, 8, 9> >, the output would be:
 * 1 2 3
 * 4 5 6
 * 7 8 9
*/
void print2DVector(vector<vector<int>> toPrint){
	for( int i = 0; i < toPrint.size(); i++){
		printIntVector(toPrint[i]);
	}
	cout << "\n";
}

/*
 * Utility function to compare the need vector to the work vector in isSafeState.
 * INPUT: needProcess: the process that is requesting the resources that it needs
 * 	  work: the resources that are currently available to the system.
 * OUTPUT: true if, for all of the resources that needProcess is requesting, the available 
 * 		amount of that resource is <= the request.  Otherwise, returns false.
*/  
bool compareNeedToAvailable(vector<int> needProcess, vector<int> work){
	for( int i = 0; i < work.size(); i++){
		if( needProcess[i] > work[i] ){
			return false;
		}
	}
	return true;
}

/* 
 * Determines whether or not a system is in a safe state.
 * INPUT: numberOfProcesses: the number of processes in the system
 * 	  available: the resources that are available to the system
 * 	  need: the resources that each process needs to be able to run
 * 	  allocation: the resources that each process has currently been allocated
 * OUTPUT: if the system is in a safe state: a vector containing the processes in the order
 * 		that they were allocated in.
 * 	   if the system is in an unsafe state: a vector containing -1.
*/

vector<int> isSafeState(int numberOfProcesses, vector<int> available, vector<vector<int>> need, 
		vector<vector<int>> allocation){

	vector<int> work (available);

	vector<bool> finish (numberOfProcesses);
	for(int i = 0; i < numberOfProcesses; i++) { finish[i] = false; }

	int numberOfProcessesAllocated = 0;

	vector<int> allocatedProcesses;
	
	// According to the textbook, the safety algorithm may require an order of 
	// numberofResources * numberofProcesses^2 to determine if a state is safe.
	// So, if I force the program to stop when it has run 
	// numberOfResources * numberOfProcesses^2 + 1 times, that will prevent an infinite
	// loop and ensure that the loop only stops running when the system is guarenteed
	// to be in an unsafe state
	int stop = ( (work.size()) * numberOfProcesses * numberOfProcesses) + 1;
	int loopCount = 0;

	while( (numberOfProcessesAllocated < numberOfProcesses) && 
			loopCount < stop ){
		for( int i = 0; i < numberOfProcesses; i++) {
			// Skip this process because it's already been allocated resources
			if( finish[i] == true ){
				continue;
			}
			// Move on to next process because there's not enough resources to allocate
			// to this process
			if( finish[i] == false && compareNeedToAvailable(need[i], work) == false ){
				continue;
			}
			
			// Allocate the process's requested resources
			for(int k = 0; k < work.size(); k++){ 
				int temp = work[k] + allocation[i][k];
				work[k] = temp;
			}

			// finish[i] = true because the process has been allocated its resources
			// so it can run
			finish[i] = true;

			// Add process i to the list of allocated processes
			allocatedProcesses.push_back(i);
			
			// Increase counter
			numberOfProcessesAllocated++;
		}
		loopCount++;
	}
	
	// Check if finish is true for all processes.  If finish is true for all processes, then
	// the system is in a safe state, otherwise it's in an unsafe state
	for( int i = 0; i < finish.size(); i++){
		if( finish[i] == false ){
			// Return a vector that contains only -1 to indicate an error
			vector<int> errorVector(1, -1);
			return errorVector;
		}
	}
	
	return allocatedProcesses;
}

int main( int argc, char *argv[] ) {
	if( argc != 2){
		cout << "Error: no input file provided.  See readme.txt for usage.\n";
		exit(EXIT_FAILURE);
	}
	ifstream input(argv[1]);
	string line;

	vector<int> available;
	vector<vector<int>> allocation;
	vector<vector<int>> max;
	vector<vector<int>> need;

	int numberOfProcesses; // n on the slides
	int numberOfResources; // m on the slides

	// Error checking: exit with error if input fails to open because that will crash the program 
	if(!input.is_open()){ 
       		cout << "Error: input file failed to open.\n";
		exit(EXIT_FAILURE);
	}

	// Special case to get the available matrix from the first line
	getline(input, line);
	available = convertStringToIntVector(line);
	if( available[0] == -1 ){
		cout << "Error in input file: negative number.  In this algorithm, the system cannot have a negative amount of resources available.\n";
		exit(EXIT_FAILURE);
	}
	numberOfResources = available.size(); 
	// printIntVector(available);

	// Read in the allocation matrix.  Store each line (aka the allocated resources for an
	// individual process) into its own vector, and put that vector into the allocation vector
	getline(input, line);
	while( line != "Max:" ){
		vector<int> temp = convertStringToIntVector(line);
		if( temp[0] == -1 ){
			cout << "Error in input file: negative number.  In this algorithm, a process cannot be allocated a negative amount of resources.\n";
			exit(EXIT_FAILURE);
		}
		if( temp.size() != numberOfResources ){ 
			cout << "Error in input file: a line in the allocation matrix is not the correct length.\n";
			exit(EXIT_FAILURE); 
		}
		allocation.push_back(temp);
		getline(input, line);
	}
	numberOfProcesses = allocation.size();
	//cout << "numberOfProcesses = " << numberOfProcesses << "\n";
	//print2DVector(allocation);

	// Read to end of file to get the max matrix
	while(getline(input, line)){
		if(input.eof()){ break; }
		vector<int> temp = convertStringToIntVector(line);
		if( temp[0] == -1 ) {
			cout << "Error in input file: a line in the max matrix contains a negative number.  In this algorithm, a process cannot request a negative amount of resources\n";
			exit(EXIT_FAILURE);
		}
		if( temp.size() != numberOfResources ) {
		
			cout << "Error in input file: a line in the max matrix is not the correct length.\n";
			exit(EXIT_FAILURE);
		}
		max.push_back(temp);
	}
	
	input.close();

	// Error checking: if the allocation and max  matrices aren't the same size, 
	// then exit with an error because that means the input file is invalid
	if( allocation.size() != max.size() ){ 
		cout << "Error in input file: the allocation matrix and the max matrix are not the same size.\n";
		exit(EXIT_FAILURE);
		return 1; 
	}

	//print2DVector(max);

	// Create the need matrix
	for( int i = 0; i < numberOfProcesses; i++){
		vector<int> temp;
		for( int j = 0; j < numberOfResources; j++){
			int sub = max[i][j] - allocation[i][j];
			temp.push_back(sub);
		}
		need.push_back(temp);
	}

	//print2DVector(need);

	// Call isSafeState and then print the result
	vector<int> result = isSafeState(numberOfProcesses, available, need, allocation);

	if( result[0] == -1 ) { 
		cout << "The system is in a unsafe state.\n"; 
	}
	else {
		cout << "The system is in a safe state.  The safe sequence is < ";
		for( int i = 0; i < result.size(); i++){
			cout << "P" << result[i] << " ";
		}
		cout << ">\n";
	}

	return 0;
}
