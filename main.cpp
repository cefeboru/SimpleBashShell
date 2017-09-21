#include <Windows.h>
#include <WinBase.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstddef>
#include <direct.h>
#include <fstream>
#pragma comment(lib, "User32.lib")

#define GetCurrentDir _getcwd

using namespace std;

string getCurrentPath();
void RunCommand(char comando[], string &output);
void changeCurrentPath(char newPath[]);
void startProcess(char comando[], STARTUPINFO *si, PROCESS_INFORMATION *pi);
void DisplayError(char *pszAPI);
void ReadAndHandleOutput(HANDLE hPipeRead, string &output);
int PrepAndLaunchRedirectedChild(char comando[]);
void removeWhitespace(std::string &str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
		{
			str.erase(i, 1);
			i--;
		}
	}
}

HANDLE hChildProcess = NULL;
HANDLE hStdIn = NULL; // Handle to parents std input.
BOOL bRunThread = TRUE;
PROCESS_INFORMATION pi;
STARTUPINFO si;

int main()
{
	const int N = 64;
	char comando[N];

	// Set up the start up info struct.
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;

	for (;;)
	{
		string path = getCurrentPath();
		cout << path << " sh:> ";
		cin.getline(comando, 'n');

		if (strcmp(comando, "exit") == 0)
			break;

		//Search if it is a CD Command
		if (strstr(comando, "cd"))
		{
			changeCurrentPath(comando);
		}
		else if (strstr(comando, ">"))
		{
			string output = "";
			string comandoStr(comando);
			size_t found = comandoStr.find_last_of(">");
			if (found != string::npos)
			{
				string redirectToFile = comandoStr.substr(found + 2);
				comandoStr = comandoStr.substr(0, found - 2);
				strcpy(comando, comandoStr.c_str());
				RunCommand(comando, output);
				removeWhitespace(redirectToFile);
				ofstream out(redirectToFile);
				if (out)
				{
					out << output;
					out.close();
				}
				else
				{
					cout << "Error, uso: <comando> \">\" <filename>" << endl;
				}
			}
		}
		else if (strstr(comando, "|"))
		{
			cout << "Not Implemented!!!!!" << endl;
		}
		else
		{
			string output = "";
			RunCommand(comando, output);
			if (output.length() > 3)
				cout << output << endl;
		}
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return 0;
}

string getCurrentPath()
{
	char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		cout << "ERROR" << endl;
	}
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
	string str(cCurrentPath);
	return str;
}

void changeCurrentPath(char newPath[])
{
	string newPathStr(newPath);
	size_t found = newPathStr.find_first_of(" ");
	if (found != string::npos)
	{
		newPathStr = newPathStr.substr(found + 1, newPathStr.length());
	}
	int changeDirOutput = chdir(newPathStr.c_str());
	if (changeDirOutput < 0)
	{ //It failed to change directory
		printf(" The folder or Path (%s) was not found", newPathStr.c_str());
		return;
	}
}

void RunCommand(char comando[], string &output)
{
	HANDLE hOutputReadTmp, hOutputRead, hOutputWrite;
	HANDLE hInputWriteTmp, hInputRead, hInputWrite;
	HANDLE hErrorWrite;
	HANDLE hThread;
	DWORD ThreadId;
	SECURITY_ATTRIBUTES sa;

	// Set up the security attributes struct.
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	// Create the child output pipe.
	if (!CreatePipe(&hOutputReadTmp, &hOutputWrite, &sa, 0))
		DisplayError("CreatePipe");

	// Create a duplicate of the output write handle for the std error
	// write handle. This is necessary in case the child application
	// closes one of its std output handles.
	if (!DuplicateHandle(GetCurrentProcess(), hOutputWrite,
						 GetCurrentProcess(), &hErrorWrite, 0,
						 TRUE, DUPLICATE_SAME_ACCESS))
		DisplayError("DuplicateHandle");

	// Create the child input pipe.
	if (!CreatePipe(&hInputRead, &hInputWriteTmp, &sa, 0))
		DisplayError("CreatePipe");

	// Create new output read handle and the input write handles. Set
	// the Properties to FALSE. Otherwise, the child inherits the
	// properties and, as a result, non-closeable handles to the pipes
	// are created.
	if (!DuplicateHandle(GetCurrentProcess(), hOutputReadTmp,
						 GetCurrentProcess(),
						 &hOutputRead, // Address of new handle.
						 0, FALSE,	 // Make it uninheritable.
						 DUPLICATE_SAME_ACCESS))
		DisplayError("DupliateHandle");

	if (!DuplicateHandle(GetCurrentProcess(), hInputWriteTmp,
						 GetCurrentProcess(),
						 &hInputWrite, // Address of new handle.
						 0, FALSE,	 // Make it uninheritable.
						 DUPLICATE_SAME_ACCESS))
		DisplayError("DupliateHandle");

	// Close inheritable copies of the handles you do not want to be
	// inherited.
	if (!CloseHandle(hOutputReadTmp))
		DisplayError("CloseHandle");
	if (!CloseHandle(hInputWriteTmp))
		DisplayError("CloseHandle");

	// Get std input handle so you can close it and force the ReadFile to
	// fail when you want the input thread to exit.
	if ((hStdIn = GetStdHandle(STD_INPUT_HANDLE)) ==
		INVALID_HANDLE_VALUE)
		DisplayError("GetStdHandle");

	si.hStdOutput = hOutputWrite;
	si.hStdInput = hInputRead;
	si.hStdError = hErrorWrite;

	if (PrepAndLaunchRedirectedChild(comando) == 0)
	{
		// Close pipe handles (do not continue to modify the parent).
		// You need to make sure that no handles to the write end of the
		// output pipe are maintained in this process or else the pipe will
		// not close when the child process exits and the ReadFile will hang.
		if (!CloseHandle(hOutputWrite))
			DisplayError("CloseHandle");
		if (!CloseHandle(hInputRead))
			DisplayError("CloseHandle");
		if (!CloseHandle(hErrorWrite))
			DisplayError("CloseHandle");

		// Read the child's output.
		ReadAndHandleOutput(hOutputRead, output);
		// Redirection is complete

		if (!CloseHandle(hOutputRead))
			DisplayError("CloseHandle");
		if (!CloseHandle(hInputWrite))
			DisplayError("CloseHandle");
	}
}

int PrepAndLaunchRedirectedChild(char comando[])
{
	if (!CreateProcess(NULL,
					   comando,
					   NULL,
					   NULL,
					   TRUE,
					   0, //CREATE_NEW_CONSOLE,
					   NULL,
					   NULL,
					   &si,
					   &pi))
	{
		cout << "Comando " << comando << " no reconocido!!!!" << endl;
		return 1;
	}

	// Set global child process handle to cause threads to exit.
	hChildProcess = pi.hProcess;

	// Close any unnecessary handles.
	if (!CloseHandle(pi.hThread))
		DisplayError("CloseHandle");

	return 0;
}

void ReadAndHandleOutput(HANDLE hPipeRead, string &output)
{
	CHAR lpBuffer[4096];
	DWORD nBytesRead;
	DWORD nCharsWritten;
	while (TRUE)
	{

		if (!ReadFile(hPipeRead, lpBuffer, sizeof(lpBuffer),
					  &nBytesRead, NULL) ||
			!nBytesRead)
		{
			if (GetLastError() == ERROR_BROKEN_PIPE)
				break; // pipe done - normal exit path.
			else
				DisplayError("ReadFile"); // Something bad happened.
		}
	}
	output = string(lpBuffer);
}

///////////////////////////////////////////////////////////////////////
// DisplayError
// Displays the error number and corresponding message.
///////////////////////////////////////////////////////////////////////
void DisplayError(char *pszAPI)
{
	LPVOID lpvMessageBuffer;
	CHAR szPrintBuffer[512];
	DWORD nCharsWritten;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpvMessageBuffer, 0, NULL);

	wsprintf(szPrintBuffer,
			 "ERROR: API    = %s.\n   error code = %d.\n   message    = %s.\n",
			 pszAPI, GetLastError(), (char *)lpvMessageBuffer);

	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), szPrintBuffer,
				 lstrlen(szPrintBuffer), &nCharsWritten, NULL);

	LocalFree(lpvMessageBuffer);
	ExitProcess(GetLastError());
}
