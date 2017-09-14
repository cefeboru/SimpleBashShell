#include <Windows.h>
#include <WinBase.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstddef>
#include <direct.h>

#define GetCurrentDir _getcwd

using namespace std;

string getCurrentPath();

int main(int argsc, char* argsv[]) {
    string currentPath = getCurrentPath();
    string newDirectory(argsv[1]);
	/*size_t found = newDirectory.find_first_of(" ");
	if(found!=string::npos){
		newDirectory = newDirectory.substr(found + 1, newDirectory.length());
	}*/
    string outputFolder = currentPath + "\\" + newDirectory;
    CreateDirectory(outputFolder, NULL);
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