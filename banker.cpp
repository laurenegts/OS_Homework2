#include <iostream>	// for printing
#include <vector>	// for vectors
#include <fstream>	// to read from files
#include <string>	// for string manipulation, convert string to int
#include <sstream>
using namespace std;

int main() {
	ifstream input("input.txt");
	string line;

	if(!input.is_open()){ return 1; }

	vector<int> available;
	
	// special case for first line
	getline(input, line);
	
	
	cout << line << "\n";

	int currentSpace = 0;
	int nextSpace = 0;

	string temp = "";
	for( int i = 0; i < line.length() + 1; i++ ){
		cout << "i = " << i << "\n";

		if(line[i] == ' ' || i == line.length()){
			int intVal = stoi(temp);
			available.push_back(intVal);
			cout << "in if, line[i] = " << line[i] << "\n";
			temp = "";
		}
		else{
			cout << "in else, line[i] = " << line[i] << "\n";
			temp.push_back(line[i]);
		}
	}


	for( int i = 0; i < available.size(); i++){
		std::cout << available.at(i) << " ";
	}

	input.close();
	return 0;
}
