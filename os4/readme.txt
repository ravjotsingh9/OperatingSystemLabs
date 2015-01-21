
Name:		Ravjot Singh
Student #:	87050134
------------------------


Assignment:	[os4] minshell
----------


Description:	
-----------
My assignment solution consists of the following files:
- minshell.c
- minshell.h
- minshell-driver.c
- makefile
- readme.txt

For running some of the test cases:
- loop.c
- read.c
- a.out - executable of read.c
- loop - executable of loop.c


Running my solution:
-------------------
You can run my assignment as follows:

	./output

IDE/Compiler:	
------------
	gcc

Running procedure:
-----------------
	make
	./output

Assumptions:
-----------
	Assuming that & is always at the last of the command.

Conclusions/Remarks:
-------------------
	Tested With following commands:
	- ls
	- ./loop &
	- ./a.out < file.txt
	- ls | wc
	- ./a.out > file.txt

	The program is unable to run grep command due to some limited functionality available to minishell.

Errors/Bugs:
----------
	-
