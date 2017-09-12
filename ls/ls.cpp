#include <direct.h>
#include <string>
#include <iostream>

#define GetCurrentDir _getcwd

using namespace std;

string getCurrentPath();

int main(int argsc, char* argsv[]) {
    string currentPath = getCurrentPath();
    //TODO PARSE ARGUMENTS

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