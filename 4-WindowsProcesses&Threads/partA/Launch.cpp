// Mini-shell.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include"Header.h"


int _tmain(int argc, _TCHAR* argv[])
{
	char   ReadBuffer[BUFFERSIZE] = { 0 };
	
	if (argc != 2)
	{
		printf("Usage Error: Incorrect number of arguments\n\n");
		return 0;
	}
	ifstream file1("input.txt");
	
	while (file1.eof()==false)
	{
		file1.getline(ReadBuffer, BUFFERSIZE);

		//char p[] = "C:\\WINDOWS\\SYSTEM32\\NOTEPAD.EXE";
		LPSTR path = (LPSTR)ReadBuffer;

		STARTUPINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);

		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));

		if (CreateProcess(
			NULL, // Application name 
			path, // Application arguments 
			NULL, // Process handle not inheritable 
			NULL, // Thread handle not inheritable 
			FALSE, // Set handle inheritance 
			CREATE_NEW_CONSOLE, // Creation flags 
			NULL, // Use parent's environment block 
			NULL, // Use parent's starting directory 
			&si, // Pointer to STARTUPINFO structure 
			&pi) // Pointer to PROCESS_INFORMATION structure 
			== FALSE){

			// most common problem is that the program was not found. 
			printf("could not create process with path = (%s)\n", path);

			// send error information to stderr stream 
			fprintf(stderr, "CreateProcess() failed on error %d\n",
				GetLastError());
		}

		// Wait until child process exits. (look this up on MSDN) 
		WaitForSingleObject(pi.hProcess, INFINITE);

		// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	
	return 0;
}

