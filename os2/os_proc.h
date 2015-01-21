#include<iostream>
#include<fstream>
#include<string>
using namespace std;
class Os_proc
{
private:
	string processorType;
	string kernelVersion;
	long int timeLastboot;
	long int timeUsermode;
/*	long int timeSystemmode;
	long int timeidlemode;
	int NumofIOreq;
	int ctxt;
	long int timeAtLastboot;
	int processCount;
	long int memconfiged;
	long int memavailable;
*/
public:

	string get_processorType();
	string get_kernelVersion();
	long int get_timeLastboot();
	long int get_timeUsermode();
/*	long int get_timeSystemmode();
	long int get_timeidlemode();
	int get_NumofIOreq();
	int get_ctxt();
	long int get_timeAtLastboot();
	int get_processCount();
	long int get_memconfiged();
	long int get_memavailable();
*/
};




