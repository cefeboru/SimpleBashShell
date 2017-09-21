#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string ReadFile(char* filename);

int main(int argc, char *argsv[])
{
	if(argsv[1]){
		cout  << ReadFile(argsv[1]);
	} else {
		cout << "Arguments missing, please use: cat <filename>" << endl;
	}	
    return 0;
} 

string ReadFile(char * filename){
	string fileContent = "";
	ifstream myfile (filename);
	if(myfile.is_open()){
		string line;
		while( getline(myfile, line) ){
			fileContent += line + "\n";
		}
		myfile.close();
		return fileContent; 
	} else {
		printf("An unexpected error ocurred when opening the file %s ", filename );
		return "ERROR";
	}
}

