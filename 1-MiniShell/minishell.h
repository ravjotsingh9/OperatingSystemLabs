/****************************************
 * 
 * FileName:	minishell.h
 * Desc	:		Contains all the function
 *				prototypes macros.
 * Author:		Ravjot Singh
 * Student No:	87050134
 *
 */



#ifndef MINISHELL_H
#define MINISHELL_H
/************Macros*******************/
#define FALSE 0
#define TRUE 1 
#define LINE_LEN 80 
#define MAX_ARGS 64
#define MAX_ARG_LEN 64 
#define MAX_PATHS 64 
#define MAX_PATH_LEN 96 
#define WHITESPACE " \t\n" 
#define STD_INPUT 0 
#define STD_OUTPUT 1
#ifndef NULL 
#define NULL 0 
#endif 

/***********Structure definition*******/
struct command_t { 
	char *name; 
	int argc; 
	char *argv[MAX_ARGS]; 
}; 
//char promptString[];

/**********Function prototypes**********/
char *lookupPath(char **, char **);
int parseCommand(char *, struct command_t *);
int parsePath(char **);
void printPrompt();
void readCommand(char *);
#endif // MINISHELL_H

