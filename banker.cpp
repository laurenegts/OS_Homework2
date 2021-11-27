#include <iostream>	// for printing
#include <vector>	// for vectors
#include <fstream>	// to read from files
#include <string>	// for string manipulation, convert string to int
using namespace std;

/* 
 * Utility function to convert a string of ints to a vector of ints.
 * INPUT: a string of integers separated by spaces
 * OUTPUT: a vector containing the integers
 * Based on the algorithm found here: slaystudy.com/c-split-string-by-space-into-vector
*/
vector<int> convertStringToIntVector(string toConvert){
	vector<int> toReturn;
	string temp = "";
        
	for( int i = 0; i < toConvert.length() + 1; i++ ){
		if(toConvert[i] == ' ' || i == toConvert.length()){
			int intVal = stoi(temp);
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
 * INPUT: a vector of ints
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
 * INPUT: a vector of int vectors
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
 * Utility function to compare the need vector to the work vector in isSafeState
 * INPUT: needProcess: the process that is requesting the resources that it needs
 * 	  work: the resources that are currently available to the system
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
 *
 *
*/

vector<int> isSafeState(int numberOfProcesses, vector<int> available, vector<vector<int>> need, 
		vector<vector<int>> allocation){

	vector<int> work (available);

	vector<bool> finish (numberOfProcesses);
	for(int i = 0; i < numberOfProcesses; i++) { finish[i] = false; }

	int numberOfProcessesAllocated = 0;

	vector<int> allocatedProcesses;
	// TODO: if I keep using this idea, add in an additional check to prevent infinite looping
	// Book says that the safety algorithim may require an order of m * n^2 operations to 
	// determine whether a state is safe, so if I stop looping at count = that + 1 then
	// I won't infinitely loop and I'll know for sure that it's not a safe state.
	
	int stop = ( (work.size()) * numberOfProcesses * numberOfProcesses) + 1;
	cout << "made stop\n";
	int count = 0;

	while( (numberOfProcessesAllocated < numberOfProcesses) && 
			count < stop ){
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
		count++;
	}
	
	// Check if finish is true for all processes.  If finish is true for all processes, then
	// the system is in a safe state, otherwise it's in an unsafe state
	for( int i = 0; i < finish.size(); i++){
		if( finish[i] == false ){
			//TODO replace allocatedProcesses with a vector<int> = -1 because that way
			// I'll be able to easily check if it's a safe state or not, outside of this
			// function
			cout << "about to set allocatedProcesses[0] = -1\n";
			vector<int> errorVector(1, -1);
			cout << "set allocatedProcesses[0] = -1\n";
			return errorVector;
		}
	}
	
	// TODO: instead of returning a bool, return the allocatedProcesses vector
	return allocatedProcesses;
}

// TODO: in the readme make sure to mention that the expection is that the processes
// will start at P_0 and increase by 1, i.e. be the order P_0, P_1, ..., P_n, not 
// P_n, ..., P_1, P_0 or P_3, P_0, P_1, P_2 
// also the available matrix is expected to be one line, all of the resources for a process are
// expected to be listed on a single line
int main() {
	ifstream input("safe.txt");
	string line;

	vector<int> available;
	vector<vector<int>> allocation;
	vector<vector<int>> max;
	vector<vector<int>> need;

	int numberOfProcesses; // n on the slides
	int numberOfResources; // m on the slides

	// Exit with error if input fails to open because that will crash the program 
	if(!input.is_open()){ return 1; }

	
	// Special case to get the available matrix in the first line
	getline(input, line);
	available = convertStringToIntVector(line);
	numberOfResources = available.size(); 
	// printIntVector(available);

	// Read in the allocation matrix.  Store each line (aka the allocated resources for an
	// individual process) into its own vector, and put that into the allocation vector.
	getline(input, line);
	while( line != "Max:" ){
		vector<int> temp = convertStringToIntVector(line);
		allocation.push_back(temp);
		getline(input, line);
	}
	numberOfProcesses = allocation.size();
	//cout << "numberOfProcesses = " << numberOfProcesses << "\n";
	//print2DVector(allocation);

	// I'm not sure how to read cleanly to the end of the file, so instead I'm just reading 
	// for as many times as we have processes, because that's guarenteed to be the right 
	// number of lines.  If there's fewer lines remaining than there are processes, then that 
	// means that the max matrix isn't the same size as the allocation matrix, which means the 
	// input file is messed up, so
	// the program is because if it's
	// not the right number of lines then the input file is messed up, so it should crash anyways.
	// it's a feature, not a bug.
	for(int i = 0; i < numberOfProcesses; i++){
		getline(input, line);
		vector<int> temp = convertStringToIntVector(line);
		max.push_back(temp);
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

	input.close();
	return 0;
}
