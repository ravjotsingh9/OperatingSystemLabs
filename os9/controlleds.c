#include<stdio.h>
#include<string.h>

int main()
{
	FILE * fp;
	int state =0;
	fp = fopen("/dev/leds", "w");
	char statestr[3][2] ={
				{'1','\0'},
				{'2','\0'},
				{'3','\0'} 
				};
	
	while(1)
	{
		int i=0;
		while(i <3)
		{
			fwrite(statestr[i],3,1, fp);
			i++;
			sleep(1);
		};
	};
	return 0;
}

