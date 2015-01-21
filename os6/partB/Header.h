#include<iostream>
#include<conio.h>
#include<stdlib.h>
#include<Windows.h>

#define MAX_THREADS 10
using namespace std;
int winnerflag;
int target;
DWORD WINAPI threadWork(LPVOID param);