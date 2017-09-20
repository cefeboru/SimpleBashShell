#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string ReadFile(char* filename);
int WriteFile(char* filename, string text);

int main(int argc, char *argsv[])
{
	
	bool hasRedirection = false;
	string content1;

	if(argsv[1]){
		content1 = ReadFile(argsv[1]);
	} else {
		cout << "Arguments missing, please use: cat <filename>" << endl;
	}

	if(argsv[2]){
		if(!strcmp(argsv[2], "\">\"")){
			printf("El argumento %s no se reconoce", argsv[2]);
		} else {
			if(argsv[3]){
				int result = WriteFile(argsv[3], content1);
				if(result != 0){
					cout << "Error when writing file";
				}
			} else {
				cout << "Arguments missing, please use: cat <source_file> > <target_file>" << endl;
			}
		}
	} else {
		cout << content1 << endl;
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
	} else {
		printf("An unexpected error ocurred when opening the file %s ", filename );
	}
	return fileContent; 
}

int WriteFile(char * filename, string text){
	ofstream targetfile(filename);
	if(targetfile){
		targetfile << text.c_str();
		targetfile.close();
	} else {
		return -1;
	}
	return 0; 
}
