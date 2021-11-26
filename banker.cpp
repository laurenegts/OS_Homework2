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

	bool stepTwoTrue = false;
	for( int i = 0; i < numberOfProcesses; i++) {
		// need of process i is <= work
		if( finish[i] == false && compareNeedToAvailable(need[i], work) == true ){
			break;
		}
		for(int j = 0; i < numberOfProcesses; j++){
			for(int k = 0; k < work.size(); k++){ 
				int temp = work[k] + allocation[j][k];
				work[k] = temp;
			}
		}	
		finish[i] = true;
	}

	for( int i = 0; i < finish.size(); i++){
		if( finish[i] == false ){
			return false;
		}
	}
	return true;
}


int main() {
	ifstream input("input.txt");
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



	input.close();
	return 0;
}
