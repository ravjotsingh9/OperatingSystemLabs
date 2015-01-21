
#include<iostream>
#include<fstream>
#include"os_proc.h"

using namespace std;

int main()
{
	Os_proc obj;
	cout<<obj.get_processorType()<<"\n"
	<<obj.get_kernelVersion()<<"\n"

	<<obj.get_timeLastboot()<<"\n"

	<<obj.get_timeUsermode()<<"\n";
/*

	<<obj.get_timeSystemmode()
	<<obj.get_timeidlemode()
	<<obj.get_NumofIOreq()
	<<obj.get_ctxt()
	<<obj.get_timeAtLastboot();
*/
	return 0;
}
