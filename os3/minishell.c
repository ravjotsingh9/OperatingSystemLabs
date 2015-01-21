
/****************************************
 * 
 * FileName:	minishell.c
 * Desc	:		Contains the definition 
 *				of all the functions
 *				mentioned in the prototype.
 * Author:		Ravjot Singh
 * Student No:	87050134
 *
 */

/********Header files************/

#include<stdio.h>
#include<stdlib.h>
#include"minishell.h"
#include<string.h>
#include<unistd.h>

char promptString[] = "minishell>";

/*****************************************
 *
 * FunctionName: printPrompt
 * Desc:		 To display the prompt
 * Pre-Cond:	-
 * Post-Cond:	-
 *
 */


void printPrompt()
{
	printf("%s",promptString) ;
}

/*****************************************
 *
 * FunctionName: readCommand()
 * Desc:		 To perform I/O function 
 *				 to read a line
 * Pre-Cond:	 -
 * Post-Cond:	 Buffer shall contain the read
 *				 data
 *
 */
void readCommand(char* buffer)
{
	gets(buffer); 
}

/*****************************************
 *
 * FunctionName: parsePath()
 * Desc:		 To read the PATH variable for
 *				 this environment.
 * Pre-Cond:	 -
 * Post-Cond:	 All found paths shall be saved
 *				 in **dirs
 *
 */
// read the PATH variable for this environment
int parsePath(char* dirs[])
{
	char* pathEnvVar;
	char* thePath;
	int i=0;
	for( i=0 ; i < MAX_ARGS ; i++ )
		dirs[i] = NULL;
	char path[] = "PATH";
	pathEnvVar = (char*) getenv(path); 
	thePath = (char*) malloc(strlen(pathEnvVar) + 1); 
	strcpy(thePath, pathEnvVar);
	// loop through the thePath for ':' delimiter between 
	// each path name
	char * ptr=strtok(thePath,":" );
	int index=0;
	while(NULL!=ptr) 
	{
		dirs[index]= (char *) malloc(strlen(ptr)+1);
		strcpy(dirs[index],ptr);
		dirs[index][strlen(ptr)+1]='\0';
		ptr= strtok(NULL,":");
		index++;
	};
	return TRUE;
}


/*****************************************
 *
 * FunctionName: lookupPath()
 * Desc:		 Searches the direstories identified
 *				 by the dir argument to see if the
 *					argv[0]appears there.
 * Pre-Cond:	 argv should contain all the arguments
 *				 given at the commandline
 * Post-Cond:	 Returns the path where the file is found 
 *				 or the NULL
 *
 */

char* lookupPath(char **argv, char **dir)
{
	char* result;
	char pName[MAX_PATH_LEN];
	int ret=1;
	// check if is already an absolute path name
	if( *argv[0] == '/' )
	{
		ret= access(argv[0],F_OK);
		if(0==ret)
		{
			strcat(pName,argv[0]);
			result= (char *)malloc(strlen(pName));
			strcpy(result,pName);
			return result;
		}
	}
	// look in PATH directories, use access() to see if the 
	// file is in the dir
	int i=0; 
	for( i = 0 ; i < MAX_PATHS ; i++ )
	{
		if(NULL==dir[i])
		{
			break;
		}
		strcpy(pName,dir[i]);
		strcat(pName,"/");
		strcat(pName,argv[0]);
		ret= access(pName,F_OK);
		if(0==ret)
		{
			result= (char *)malloc(strlen(pName));
			strcpy(result,pName);
			return result;
		}
		
	}
	// File name not found in any path variable
	fprintf(stderr, "%s: command not found\n", argv[0]); 
	return NULL; 
}

/*****************************************
 *
 * FunctionName: parseCommand()
 * Desc:		 To parse the given command
 * Pre-Cond:	 CommandLine should contain the
 *				 given command
 * Post-Cond:	 The command structure shall be
 *				 populated
 *
 */
int parseCommand(char * commandLine, struct command_t *command)
{
	char * ptr= strtok(commandLine,WHITESPACE);
	if(NULL!=ptr)
	{
		command->argc=1;
		command->name =(char *)malloc(strlen(ptr)+1);
		strcpy(command->name,ptr);
		(command->name)[strlen(ptr)]='\0';
		(command->argv)[(command->argc)-1]= (char *)malloc(strlen(ptr)+1);
		strcpy((command->argv)[(command->argc)-1],ptr);
		(command->argv)[strlen((command->argv)[(command->argc)-1])]='\0';
		ptr= strtok(NULL,WHITESPACE);
	}
	while(NULL!=ptr && (command->argc)<62)
	{
		(command->argc)++;
		(command->argv)[(command->argc)-1]= (char *)malloc(strlen(ptr)+1);
		strcpy((command->argv)[(command->argc)-1],ptr);
		(command->argv)[strlen((command->argv)[(command->argc)-1])]='\0';
		ptr= strtok(NULL,WHITESPACE);
	}
	(command->argv)[command->argc]=NULL;
}
