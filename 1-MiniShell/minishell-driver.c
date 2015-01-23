
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
			thisChPID= fork();
			if(thisChPID==0)
			{
				//execve(command.name,command.argv,pathv);
				execve(command.name,command.argv,pathv);
				perror("execve");   /* execve() only returns on error */
                exit(0);

				//printf("\nYour command Failed!\n");	
			}
			else
			{
				do 
				{	
					wait(&stat);
				}while(!WIFEXITED(stat)&& !WEXITSTATUS(stat));
				//sleep(1);
				continue;
			}
		}
	}
	return TRUE;
}
