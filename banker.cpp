#include <iostream>	// for printing
#include <vector>	// for vectors
#include <fstream>	// to read from files
#include <string>	// for string manipulation, convert string to int
#include <sstream>
using namespace std;

// INPUT: a string of integers separated by spaces
// OUTPUT: a vector containing the integers
// Based on the algorithm found here: slaystudy.com/c-split-string-by-space-into-vector
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

// INPUT: a vector of ints
// OUTPUT: each int in the vector, separated by a space, ending in a new line.
void printIntVector(vector<int> toPrint) {
	for( int i = 0; i < toPrint.size(); i++){
		cout << toPrint.at(i) << " ";
	}
	cout << "\n";
}

void print2DVector(vector<vector<int>> toPrint){
	for( int i = 0; i < toPrint.size(); i++){
		printIntVector(toPrint[i]);
	}
	cout << "\n";
}

bool compareNeedToAvailable(vector<int> needProcess, vector<int> work){
	for( int i = 0; i < work.size(); i++){
		if( needProcess[i] > work[i] ){
			return false;
		}
	}
	return true;
}

bool isSafeState(int numberOfProcesses, vector<int> available, vector<vector<int>> need, 
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
	while(numberOfProcessesAllocated < numberOfProcesses){
		for( int i = 0; i < numberOfProcesses; i++) {
			cout << "in first for loop, i = " << i << "\n";
			cout << "\n";
			// already allocated
			if( finish[i] == true ){
				continue;
			}
			// move on to next process because there's not enough resources to allocate
			// to this process
			if( finish[i] == false && compareNeedToAvailable(need[i], work) == false ){
				cout << "in if statement, about to break\n";
				continue;
			}
			
		//for(int j = 0; j < numberOfProcesses; j++){
		//	cout << "j loop, working on process " << j << "\n";
			// allocate resources
			for(int k = 0; k < work.size(); k++){ 
				int temp = work[k] + allocation[i][k];
				work[k] = temp;
				cout << "k = " << k << ", work[k] = " << work[k] << "\n";
			}
	//	}	
			finish[i] = true;
			allocatedProcesses.push_back(i);
			cout << "allocating to process " << i << "\n";
			numberOfProcessesAllocated++;
		}
	}

	for( int i = 0; i < finish.size(); i++){
		if( finish[i] == false ){
			return false;
		}
	}
	for(int i = 0; i < allocatedProcesses.size(); i++){
		cout << allocatedProcesses[i] << " ";
	}
	// TODO: instead of returning a bool, return the allocatedProcesses vector
	return true;
}

bool compareRequestToNeedOrAvailable(vector<int> request, vector<int> compare){
	for( int i = 0; i < request.size(); i++){
		if(request[i] > compare[i]){
			return false;
		}
	}
	return true;
}

// maybe allocate resources as it's possible to do so
// and then if resulting vector is less than numberOfProcesses
// we know it's not a safe state because it doesn't include all the processes?

/*vector<int> resourceRequest(vector<vector<int>> request, vector<int> available, 
		vector<vector<int>> allocation){
	vector<vector<int>> = 

}
*/

int main() {
	ifstream input("input2.txt");
	string line;

	vector<int> available;
	vector<vector<int>> allocation;
	vector<vector<int>> max;
	vector<vector<int>> need;

	int numberOfProcesses; // n on the slides
	int numberOfResources; // m on the slides

	if(!input.is_open()){ return 1; }

	
	// special case for first line
	getline(input, line);
	available = convertStringToIntVector(line);
	numberOfResources = available.size(); 
	printIntVector(available);


	getline(input, line);
	while( line != "Max:" ){
		vector<int> temp = convertStringToIntVector(line);
		allocation.push_back(temp);
		getline(input, line);
	}
	numberOfProcesses = allocation.size();
	cout << "numberOfProcesses = " << numberOfProcesses << "\n";
	print2DVector(allocation);

	// allocation.size is the number of processes.  idk how to make it so we just read 
	// cleanly to the end so instead i'm just reading the same number of times as we have
	// processes because that's guarenteed to be the right number of lines.  because if it's
	// not the right number of lines then the input file is messed up, so it should crash anyways.
	// it's a feature, not a bug.
	for(int i = 0; i < numberOfProcesses; i++){
		getline(input, line);
		vector<int> temp = convertStringToIntVector(line);
		max.push_back(temp);
	}

	print2DVector(max);

	// Create the need matrix
	for( int i = 0; i < numberOfProcesses; i++){
		vector<int> temp;
		for( int j = 0; j < numberOfResources; j++){
			int sub = max[i][j] - allocation[i][j];
			temp.push_back(sub);
		}
		need.push_back(temp);
	}

	print2DVector(need);

	bool result = isSafeState(numberOfProcesses, available, need, allocation);

	if( result == true ) { 
		cout << "system is in a safe state\n"; 
	}
	else {
		cout << "system is in an unsafe state\n";
	}

	input.close();
	return 0;
}
