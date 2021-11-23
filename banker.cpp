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

int main() {
	ifstream input("input.txt");
	string line;

	if(!input.is_open()){ return 1; }

	
	// special case for first line
	getline(input, line);
	vector<int> available = convertStringToIntVector(line);
	printIntVector(available);
	// now need to iterate through the rest of the input.txt file


	input.close();
	return 0;
}
