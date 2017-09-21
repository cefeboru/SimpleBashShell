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
	string fileList = "";
	for (auto & p : fs::directory_iterator(currentPath)){
		string fileOrFolderName = p.path().string();
		size_t found = fileOrFolderName.find_last_of("\\");
		fileList += "\n" + fileOrFolderName.substr(found  +1);
	}
	cout << fileList << endl;
	return 0;
    //TODO CHANGE OUTPUT DETAILS ACCORDING TO ARGUMENTS
}

string getCurrentPath(){
	char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		cout << "ERROR" << endl;
	}
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
	string str(cCurrentPath);
	return str;
}