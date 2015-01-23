

Name:		Ravjot Singh

------------------------


Assignment:	[se10] Programming with TCP/IP
----------


Description:	
-----------

My assignment solution consists of the following files:
- server.c
- Server
- serverLog.txt
- makefile
- Readme.txt

Running my solution:
-------------------
	1. Run the command: make
	2. Run the command: ./Server <portnumber>

IDE/Compiler:	
------------
	gcc compiler



Assumptions:
-----------


Conclusions/Remarks:
-------------------
1. 'quit' command exits the client thread on the server.
2. Two locks are used to lock the file and the array list of online clients. 
   The two locks are implemented using ordering to prevent deadlock.
   Since lock calls are not much complicated so, the order is simply
   endsured while actally locking the resources.
   (i.e. first acquire the lock on file then on list)


Errors/Bugs:
-----------
