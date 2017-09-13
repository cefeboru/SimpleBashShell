#include <direct.h>
#include <string>
#include <iostream>
#include <filesystem>

#define GetCurrentDir _getcwd

namespace fs = std::experimental::filesystem;
using namespace std;

string getCurrentPath();

int main(int argsc, char* argsv[]) {
	string currentPath = getCurrentPath();
    //TODO PARSE ARGUMENTS
	for (auto & p : fs::directory_iterator(currentPath)){
		string fileOrFolderName = p.path().string();
		size_t found = fileOrFolderName.find_last_of("\\");
		cout << fileOrFolderName.substr(found  +1) << endl;
	}
		
    //TODO CHANGE OUTPUT DETAILS ACCORDING TO ARGUMENTS
}

string getCurrentPath(){
	char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		cout << "ERROR";
	}
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
	string str(cCurrentPath);
	return str;
}