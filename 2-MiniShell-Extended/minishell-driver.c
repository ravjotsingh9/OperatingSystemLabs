
/****************************************
 * 
 * FileName:	minishell-driver.c
 * Desc	:		Contains the main controller 
 *				function
 * Author:		Ravjot Singh
 * Student No:	87050134
 *
 */

 /**************Header files***********/
#include <fcntl.h> 
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <string.h>
#include <unistd.h> 
#include "minishell.h"


/*****************************************
 *
 * FunctionName: main()
 * Desc:		 Controller of the program
 * Pre-Cond:	 -
 * Post-Cond:	 -
 *
 */


int main() {
	int flag=0; 
	char forBackground[]={'&','\0'};
	char forRedirectInput[]={'<','\0'};
	char forRedirectOutput[]={'>','\0'};
	char forPipe[]={'|','\0'};
	FILE *file1;
	FILE *file2;
	char *pathv[MAX_PATHS]; 
	int child_pid; // child process pid
	int stat; // used by parent wait 
	pid_t thisChPID;

/* 
	struct command_t command; 
	int i=0;	
	// Command initialization 
	for(i=0; i < MAX_ARGS; i++) 
	{ 
		command.argv[i] = (char *) malloc(MAX_ARG_LEN); 
	}
*/
	// get all directories from PATH env var 
	parsePath(pathv); 
	// Main loop 
	while(TRUE) 
	{ 
		int readInputFlag=0;
		int fd[2];
		flag=0;
		pipe(fd);
		int pipeFlag=0;
		int sendOutputFlag=0;
		char sendOutputto[50];
		char readInputfrom[50];

		// Read the command line 
		printPrompt(); 
		char commandLine[LINE_LEN]; 
		readCommand(commandLine); 
		// Quit the shell ?
		if( (strcmp(commandLine, "exit") == 0) || 
 			(strcmp(commandLine, "quit") == 0) )
		{
		 	break; 
		}
		if(strcmp(commandLine,"")==0)
		{
			continue;
		}
		// Parse the command line
		struct command_t command; 
		int i=0;
		// Command initialization 
		for(i=0; i < MAX_ARGS; i++) 
		{ 
			command.argv[i] = (char *) malloc(MAX_ARG_LEN); 
		}
		/*
		for(i=0; i < MAX_ARGS; i++) 
		{ 
			memset(command.argv[i],0,MAX_ARG_LEN); 
		}
		*/
		parseCommand(commandLine, &command); 
		// Get the full path name
		command.name = lookupPath(command.argv, pathv);
		// report error 
		if( command.name == "" )
		{
			continue; // non-fatal 
		}
		else
		{
			// set flag if & is found at the end of command
			if(strcmp(command.argv[command.argc-1],forBackground)==0)
			{
				flag=1;
				(command.argc)--;
				command.argv[command.argc]=NULL;
			}
			// set flag if < is found at the end of command
			int temp= command.argc-1;
			while(temp && strcmp(command.argv[temp],forRedirectInput)!=0)
			{	temp--; };
			if(0==strcmp(command.argv[temp],forRedirectInput))
			{
				strcpy(readInputfrom,command.argv[temp+1]);
				readInputfrom[strlen(command.argv[temp+1])]='\0';
				readInputFlag=1;
				int x=temp;
				while(x<command.argc-2)
				{
					command.argv[x]= command.argv[x+2];
					x++;
				}
				command.argc= command.argc -2;
				command.argv[command.argc]=NULL;
			}
			
			temp= command.argc-1;
			while(temp && strcmp(command.argv[temp],forRedirectOutput)!=0)
			{	temp--; };
			if(0== strcmp(command.argv[temp],forRedirectOutput))
			{
				strcpy(sendOutputto,command.argv[temp+1]);
				sendOutputto[strlen(command.argv[temp+1])]='\0';
				sendOutputFlag=1;
				int x=temp;
				while(x<command.argc-2)
				{
					command.argv[x]= command.argv[x+2];
					x++;
				}
				command.argc= command.argc -2;
				command.argv[command.argc]=NULL;
			}
			//for pipe

			temp= 0;
			struct command_t command1, command2; 
			// Command initialization 
			for(i=0; i < MAX_ARGS; i++) 
			{ 
				command1.argv[i] = (char *) malloc(MAX_ARG_LEN); 
				command2.argv[i] = (char *) malloc(MAX_ARG_LEN); 
			}
			while(temp< (command.argc-1) && strcmp(command.argv[temp],forPipe)!=0)
			{	temp++; };
			if(0== strcmp(command.argv[temp],forPipe))
			{
				//strcpy(sendOutputto,command.argv[temp+1]);
				//sendOutputto[strlen(command.argv[temp+1])]='\0';
				pipeFlag=1;
				int x=temp-1;
				while(x>=0)
				{
					strcpy(command1.argv[x],command.argv[x]);
					x--;
				}
				command1.argc = temp;
				command1.name = lookupPath(command1.argv,pathv);
				command1.argv[command1.argc]=NULL;
				int y=temp+1;
				while(y<command.argc)
				{
					strcpy(command.argv[y-temp-1],command.argv[y]);
					y++;
				};
				command.argc = command.argc -y+1;
				command.name = lookupPath(command.argv,pathv);
				command.argv[command.argc]= NULL;
			}
			int subproId;
			if(pipeFlag==1)
			{
				subproId= fork();
			}
			if((subproId==0) && pipeFlag==1)
			{
					
					if(1==pipeFlag)
					{
						// TD wrote here to read by other process
						close(1);
						dup(fd[1]);
						close(fd[0]);
						close(fd[1]);
						//close(1);
					//	write(fd[1], string, (strlen(string)+1));	
						//file2=freopen(sendOutputto,"w",fd[1]);
					}
					else if(1==sendOutputFlag)
					{
						file2=freopen(sendOutputto,"w",stdout);
					}
					//execve(command.name,command.argv,pathv);
					execve(command1.name,command1.argv,pathv);
					perror("execve");   /* execve() only returns on error */
					exit(0);
			}
			else
			{
/*				do 
				{	
					wait(&stat);
				}while(!WIFEXITED(stat)&& !WEXITSTATUS(stat));
*/
				thisChPID= fork();
				if(thisChPID==0)
				{
						//file1=freopen("abc","r",stdin);
					if(1==pipeFlag)
					{
						close(0);
						dup(fd[0]);
						close(fd[1]);
						close(fd[0]);
						//file1=freopen(readInputfrom,"r",fd[0]);
					}
					else
					{ 
						if(1==readInputFlag)
						{
							file1=freopen(readInputfrom,"r",stdin);
						}
						if(1==sendOutputFlag)
						{
							file2=freopen(sendOutputto,"w",stdout);
						}
					}
					//execve(command.name,command.argv,pathv);
					execve(command.name,command.argv,pathv);
					perror("execve");   /* execve() only returns on error */
               		 exit(1);

					//printf("\nYour command Failed!\n");	
				}
				else
				{
					if(1==pipeFlag)
					{
					//	close(fd[0]);
						close(fd[1]);
						sleep(1);
					}
					if(0==flag)
					{
						do 
						{	
							wait(&stat);
						}while(!WIFEXITED(stat)&& !WEXITSTATUS(stat));
					}
					else
					{
						sleep(1);
						//continue;
					}
					//continue;
				}
			}
		}
	}
	return TRUE;
}
