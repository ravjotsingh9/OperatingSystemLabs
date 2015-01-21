#include<iostream>
#include<fstream>
#include"os_proc.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
using namespace std;

string Os_proc::get_processorType()
{
	ifstream file1;	
	file1.open("/proc/cpuinfo");
	string line;
	int count=5;
	while(count)
	{
		getline(file1,line);
		count--;
	}
		//string modelname="model name";
		//if(line.find(modelname))
		//{
			this->processorType= line.substr(13,48);
			file1.close();
			return this->processorType;
		//}
		
	//}
	//return NULL;

}
string Os_proc::get_kernelVersion()
{
	ifstream file1;	
	string line;
	file1.open("/proc/version");
	(getline(file1,line));
	this->kernelVersion= line.substr(14,11);
	file1.close();
	return this->kernelVersion;

}

long int Os_proc::get_timeLastboot()
{
	ifstream file1;	
	char line[100];
	file1.open("/proc/uptime");
	file1>>line;
	char result[10];
	strncpy(result,line,10);
	this->timeLastboot= atoi(result);
	file1.close();
	return this->timeLastboot;
}


long int Os_proc::get_timeUsermode()
{
	ifstream file1;	
	char line[100],result[10];
	file1.open("/proc/stat");
	file1>>line;
	memcpy(result,&line[0],6);
	this->timeUsermode=atoi(result);
	file1.close();
	return this->timeUsermode;
	
}
/*
long int Os_proc::get_timeSystemmode()
{
	ifstream file1;	
	string line;
	file1.open("/proc/stat");
	(getline(file1,line));
	this->timeSystemmode= stoi(line.substr(18,24));
	file1.close();
	return this->timeSystemmode;
}
long int Os_proc::get_timeidlemode()
{
	ifstream file1;	
	string line;
	file1.open("/proc/stat");
	(getline(file1,line));
	this->timeidlemode= stoi(line.substr(26,35));
	file1.close();
	return this->timeidlemode;
}
int Os_proc::get_NumofIOreq()
{
	ifstream file1;	
	string line;
	file1.open();
	return 0;
}
int Os_proc::get_ctxt()
{
	ifstream file1;	
	file1.open("/proc/stat");
	string line;
	while(!(getline(file1,line)))
	{
		string ctxt="ctxt";
		if(line.find(ctxt))
		{
			this->ctxt= stoi(line.substr(5,14));
			file1.close();
			return this->ctxt;
		}
		
	}
	return 0;
}
long int Os_proc::get_timeAtLastboot()
{
	ifstream file1;	
	file1.open();
	return 0;

}
int Os_proc::get_processCount()
{
	ifstream file1;	
	file1.open("/proc/stat");
	string line;
	while(!(getline(file1,line)))
	{
		string pcount="processes";
		if(line.find(pcount))
		{
			this->processCount= stoi(line.substr(10,16));
			file1.close();
			return this->processCount;
		}
		
	}
	return 0;

}
long int Os_proc::get_memconfiged()
{
	ifstream file1;	
	string line;
	file1.open("/proc/meminfo");
	(getline(file1,line));
	this->memconfiged=stoi(line.substr(17,24));
	file1.close();
	return this->memconfiged;
}

long int Os_proc::get_memavailable()
{
	ifstream file1;	
	file1.open("/proc/meminfo");
	string line;
	while(!(getline(file1,line)))
	{
		string mem="MemFree";
		if(line.find(mem))
		{
			this->memavailable=std::stoi( line.substr(18,24));
			file1.close();
			return this->memavailable;
		}
		
	}
	return 0;

}*/
