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
    string outputFolder = currentPath + "\\" + newDirectory;
	bool created = CreateDirectory(outputFolder.c_str(), NULL);
	if(!created){
		cout << "Error creating folder: " << outputFolder << endl;
	}
	cout << "" << endl;
	return 0;
}

string getCurrentPath(){
	char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		cout << "ERROR" << endl;
	}
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
	string str(cCurrentPath);
	return str;
}