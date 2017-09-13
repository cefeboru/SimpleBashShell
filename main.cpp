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
void changeCurrentPath(char newPath[]);
void startProcess(char comando[], STARTUPINFO *si, PROCESS_INFORMATION *pi);

int main() {

    const int N = 64;
    char comando[N];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
    for (;;) {
		string path = getCurrentPath();
		cout << "\n" << path << " sh:> ";
        cin.getline(comando,'n');	

		if (strcmp(comando,"exit") == 0) break;

		//Search if it is a Change Directory Command
		char *output = NULL;
		char cd[] = "cd";
		output = strstr(comando, cd);
		if(output){
			changeCurrentPath(comando);
		} else {
			startProcess(comando, &si, &pi);
		}
		// Cierra los handles de process y thread. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	}
    return 0;
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

void changeCurrentPath(char newPath[]){
	string newPathStr(newPath);
	size_t found = newPathStr.find_first_of(" ");
	if(found!=string::npos){
		newPathStr = newPathStr.substr(found + 1, newPathStr.length());
	}
	int changeDirOutput = chdir(newPathStr.c_str());
	if(changeDirOutput < 0){//It failed to change directory
		printf(" The folder or Path (%s) was not found", newPathStr.c_str());
		return;
	}
}

void startProcess(char comando[], STARTUPINFO *si, PROCESS_INFORMATION *pi){
	if( !CreateProcess( NULL,   // No module name (use command line)
		comando,      // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		si,            // Pointer to STARTUPINFO structure
		pi )           // Pointer to PROCESS_INFORMATION structure
	) 
	{
		printf( "No se reconocio el comando (%s) \n", comando);
	}

	// Espera hasta que el proceso hijo finalize
	WaitForSingleObject( pi->hProcess, INFINITE );
}
 