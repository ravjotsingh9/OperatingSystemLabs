// Threads.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Header.h"


int _tmain(int argc, _TCHAR* argv[])
{
	DWORD threadId;
	winnerflag = 0;

	if (argc != 3)
	{
		printf("Pass two integer arguments.\n");
		return 0;
	}
	int count= atoi((char *)argv[1]);
	target = atoi((char *)argv[2]);
	HANDLE handles[MAX_THREADS];
	if (count > 10)
	{
		printf("Not more than 10 threads allowed.\n");
		return 0;
	}
	int index = 0;
	while (count>index)
	{
		int theArg[] = { rand(), rand() };
		handles[index] = CreateThread(NULL, 0, threadWork, &theArg, 0, &threadId);

		if (handles[index] == NULL){
			printf("Failed to create thread");
			ExitProcess(0);
		}
		else {
			printf("handle = %i \n", handles[index]);
			printf("threadId = %i \n", threadId);
		}
		index++;
	}
	WaitForMultipleObjects(count,handles,true, INFINITE);
	return 0;
}

DWORD WINAPI threadWork(LPVOID param) {

	// cast param to the correct type, in this case an integer, but 
	// depends on the fourth parameter given to the CreateThread function 
	int *arg = ((int*)param);  // do the work 
	int increment = arg[0];
	int sleeptime = arg[1];
	int localcounter = 0;
	while (localcounter < target)
	{
		localcounter = localcounter + increment;
		Sleep(sleeptime);
	};
	if (winnerflag != 1)
	{
		winnerflag = 1;
		printf("%d :I am the Winner!!! :D\n", GetCurrentThreadId());
	}
	else
	{
		printf("%d : Lost Bro :(\n", GetCurrentThreadId());
	}
	return 0; // the thread finishes 
}

