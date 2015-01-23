#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
long unsigned int fibonacci(unsigned int n);
void increment (int signum);
long unsigned int elapsed_usecs(long unsigned int sec, long unsigned int microsec);
static long p_realt_secs = 0, c1_realt_secs = 0, c2_realt_secs = 0;
static long p_virtt_secs = 0, c1_virtt_secs = 0, c2_virtt_secs = 0;
static long p_proft_secs = 0, c1_proft_secs = 0, c2_proft_secs = 0;
static struct itimerval p_realt, c1_realt, c2_realt;
static struct itimerval p_virtt, c1_virtt, c2_virtt;
static struct itimerval p_proft, c1_proft, c2_proft;

int main(int argc, char**argv) {
	long unsigned fib = 0;
 	int pid1, pid2;
 	unsigned int fibarg;
	int status;
 	// Get command line arugment, fibarg
	if (argv[1]!=NULL)
	{
 		fibarg= atoi(argv[1]);
	}
	else
	{
		printf("Give an integer arg!Exiting...\n");
		exit(3);
	}
 	// Initialize parent, cild1, and child 2 timer values
	p_realt_secs = 0;
	c1_realt_secs = 0; 
	c2_realt_secs = 0;
	p_virtt_secs = 0;
	c1_virtt_secs = 0;
	c2_virtt_secs = 0;
	p_proft_secs = 0;
	c1_proft_secs = 0;
	c2_proft_secs = 0;

 	// Enable your signal handlers for the parent
 	signal(SIGALRM,increment);
 	signal(SIGVTALRM, increment);
 	signal(SIGPROF,increment);
 	// Set the parent's itimers
 	p_realt.it_interval.tv_usec=0;
	p_realt.it_interval.tv_sec=1;
	p_realt.it_value.tv_usec=0;
	p_realt.it_value.tv_sec=1;
 	p_virtt.it_interval.tv_usec=0;
	p_virtt.it_interval.tv_sec=1;
	p_virtt.it_value.tv_usec=0;
	p_virtt.it_value.tv_sec=1;
 	p_proft.it_interval.tv_usec=0;
	p_proft.it_interval.tv_sec=1;
	p_proft.it_value.tv_usec=0;
	p_proft.it_value.tv_sec=1;
	setitimer(ITIMER_REAL,&p_realt,NULL);
	setitimer(ITIMER_PROF,&p_proft,NULL);
	setitimer(ITIMER_VIRTUAL,&p_virtt,NULL);

 	pid1 = fork();
 	if(pid1 == 0) {
 		// Enable child 1 signal handlers (disable parent handlers)
 		signal(SIGALRM,increment);
 		signal(SIGVTALRM, increment);
 		signal(SIGPROF, increment);
 		// Set the child 1 itimers
 		c1_realt.it_interval.tv_usec=0;
		c1_realt.it_interval.tv_sec=1;
		c1_realt.it_value.tv_usec=0;
		c1_realt.it_value.tv_sec=1;
 		c1_virtt.it_interval.tv_usec=0;
		c1_virtt.it_interval.tv_sec=1;
		c1_virtt.it_value.tv_usec=0;
		c1_virtt.it_value.tv_sec=1;
	 	c1_proft.it_interval.tv_usec=0;
		c1_proft.it_interval.tv_sec=1;
		c1_proft.it_value.tv_usec=0;
		c1_proft.it_value.tv_sec=1;
		setitimer(ITIMER_REAL,&c1_realt,NULL);
		setitimer(ITIMER_PROF,&c1_proft,NULL);
		setitimer(ITIMER_VIRTUAL,&c1_virtt,NULL);
 		// Start child 1 on the Fibonacci program
 		fib = fibonacci(fibarg);
		// Read the child 1 itimer values, and report them
 		getitimer(ITIMER_REAL,&c1_realt );
 		getitimer(ITIMER_VIRTUAL,&c1_virtt);
 		getitimer(ITIMER_PROF,&c1_proft);
 		printf("\n");
 		printf("Child 1 fib = %ld, real time = %ld sec, %ld msec\n",
 				fib, c1_realt_secs,
 				elapsed_usecs(c1_realt.it_value.tv_sec,
 				c1_realt.it_value.tv_usec) / 1000);
	 	printf("Child 1 fib = %ld, cpu time = %ld sec, %ld msec\n",
 				fib, c1_proft_secs,
 				elapsed_usecs(c1_proft.it_value.tv_sec,
 				c1_proft.it_value.tv_usec) / 1000);
	 	printf("Child 1 fib =%ld, user time = %ld sec, %ld msec\n",
 				fib, c1_virtt_secs,
 				elapsed_usecs(c1_virtt.it_value.tv_sec,
 				c1_virtt.it_value.tv_usec) / 1000);
 		printf("Child 1 ib = %ld, kernel time = %ld sec, %ld msec\n",
				fib, (c1_proft_secs - c1_virtt_secs),
 				(elapsed_usecs(c1_proft.it_value.tv_sec,
 				c1_proft.it_value.tv_usec) / 1000) -
 				(elapsed_usecs(c1_virtt.it_value.tv_sec,
 				c1_virtt.it_value.tv_usec) / 1000));
 		fflush(stdout);
 		exit(0);
 	}
	else {
 		pid2 = fork();
 		if(pid2 == 0) {
 			// Enable the child 2 signal handlers (disable parent handlers)
 			signal(SIGALRM,increment);
 			signal(SIGVTALRM, increment);
 			signal(SIGPROF, increment);
			// Set the child 2 itimers
 			c2_realt.it_interval.tv_usec=0;
			c2_realt.it_interval.tv_sec=1;
			c2_realt.it_value.tv_usec=0;
			c2_realt.it_value.tv_sec=1;
		 	c2_virtt.it_interval.tv_usec=0;
			c2_virtt.it_interval.tv_sec=1;
			c2_virtt.it_value.tv_usec=0;
			c2_virtt.it_value.tv_sec=1;
		 	c2_proft.it_interval.tv_usec=0;
			c2_proft.it_interval.tv_sec=1;
			c2_proft.it_value.tv_usec=0;
			c2_proft.it_value.tv_sec=1;
			setitimer(ITIMER_REAL,&c2_realt,NULL);
			setitimer(ITIMER_PROF,&c2_proft,NULL);
			setitimer(ITIMER_VIRTUAL,&c2_virtt,NULL);
 			// Start the child 2 on the Fibonacci program
 			fib = fibonacci(fibarg);
 			// Read the child 2 itimer values and report them
 			getitimer(ITIMER_REAL,&c2_realt );
 			getitimer(ITIMER_VIRTUAL,&c2_virtt);
 			getitimer(ITIMER_PROF,&c2_proft);
 			printf("\n");
 			printf("Child 2 fib = %ld, real time = %ld sec, %ld msec\n",
 					fib, c2_realt_secs,
 					elapsed_usecs(c2_realt.it_value.tv_sec,
 					c2_realt.it_value.tv_usec) / 1000);
	 		printf("Child 2 fib = %ld, cpu time = %ld sec, %ld msec\n",
 					fib, c2_proft_secs,
 					elapsed_usecs(c2_proft.it_value.tv_sec,
 					c2_proft.it_value.tv_usec) / 1000);
	 		printf("Child 2 fib =%ld, user time = %ld sec, %ld msec\n",
 					fib, c2_virtt_secs,
 					elapsed_usecs(c2_virtt.it_value.tv_sec,
 					c2_virtt.it_value.tv_usec) / 1000);
 			printf("Child 2 ib = %ld, kernel time = %ld sec, %ld msec\n",
 					fib, c2_proft_secs - c2_virtt_secs,
 					(elapsed_usecs(c2_proft.it_value.tv_sec,
 					c2_proft.it_value.tv_usec) / 1000) -
 					(elapsed_usecs(c2_virtt.it_value.tv_sec,
 					c2_virtt.it_value.tv_usec) / 1000));
 			fflush(stdout);
			exit(1);
 		}
		else { /* This is the parent */
 				//Start the parent on the Fibonacci program 
 			fib = fibonacci(fibarg);
			// Wait for the children to terminate
 			waitpid(0, &status, 0);
 			waitpid(0, &status, 0);
			// Read the parent itimer values, and report them
 			getitimer(ITIMER_REAL,&p_realt );
 			getitimer(ITIMER_VIRTUAL,&p_virtt);
 			getitimer(ITIMER_PROF,&p_proft);
 			printf("\n");
 			printf("Parent fib = %ld, real time = %ld sec, %ld msec\n",
 					fib, p_realt_secs,
 					elapsed_usecs(p_realt.it_value.tv_sec,
 					p_realt.it_value.tv_usec) / 1000);
	 		printf("Parent fib = %ld, cpu time = %ld sec, %ld msec\n",
 					fib, p_proft_secs,
 					elapsed_usecs(p_proft.it_value.tv_sec,
 					p_proft.it_value.tv_usec) / 1000);
	 		printf("Parent fib =%ld, user time = %ld sec, %ld msec\n",
 					fib, p_virtt_secs,
 					elapsed_usecs(p_virtt.it_value.tv_sec,
 					p_virtt.it_value.tv_usec) / 1000);
 			printf("Parent fib = %ld, kernel time = %ld sec, %ld msec\n",
 					fib, p_proft_secs - p_virtt_secs,
 					(elapsed_usecs(p_proft.it_value.tv_sec,
 					p_proft.it_value.tv_usec) / 1000) -
 					(elapsed_usecs(p_virtt.it_value.tv_sec,
 					p_virtt.it_value.tv_usec) / 1000));
 			fflush(stdout);
			exit(2);
		}
 		printf("this line should never be printed\n");
	}
}

long unsigned int fibonacci(unsigned int n) 
{
	if(n ==0)
 		return 0;
 	else if (n == 1 || n ==2)
 		return 1;
 	else 
 		return (fibonacci(n-1) + fibonacci(n-2));
}

void increment (int signum)
{
	if(SIGALRM==signum)
	{
		p_realt_secs++;
		c1_realt_secs++; 
		c2_realt_secs++;
	}
	else if(SIGVTALRM==signum)
	{
		p_virtt_secs++;
		c1_virtt_secs++;
		c2_virtt_secs++;
	}
	else if(SIGPROF==signum)
	{
		p_proft_secs++;
		c1_proft_secs++;
		c2_proft_secs++;
	}

}
	
long unsigned int elapsed_usecs(long unsigned int sec, long unsigned int microsec)
{
	return (sec * 1000000)+ microsec;
}
