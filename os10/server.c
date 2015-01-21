#include<stdio.h>
#include <sys/types.h>      
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//
// note: formating and indenting is lost with some long lines of code 
//
#define MAX 50
#define MAX_CONN 10
void callsend(int fd_Index,int new_fd, char *buf, int flag, int *new_fds, int size );
void callclose(int fd_index, int *new_fds,int *threadcount,FILE *fp);
int findIndex(int *fdlist);
void *threadfunction(void * args);
pthread_t thread;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexforfile = PTHREAD_MUTEX_INITIALIZER;

typedef struct passtothread
{
	int sockfd;
	FILE *fp;
	int fd_Index;
	int new_fd;
	int * new_fds;
	int * threadcount;
}argument;

int main(int argc, char *argv[])
{
	if(argc!=2 )
	{
		printf("Syntax: ./Server <portNumber>\n");
		exit(0);
	}	
	FILE *fp;
	// get port number 
	int port = atoi(argv[1]);
	// perform range check on port number 
	if( port <= 1024 ){
		printf("must use a port above 1024, terminate.\n");
		exit(1);
	}
	// fd and socket params 
	int sockfd; 
	struct sockaddr_in my_addr;
	struct sockaddr_in client_addr; 
	socklen_t sin_size;
	int optval = 1;
	// create socket, test if worked 
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if( sockfd < 0 ){
		perror("can't create socket, terminate.\n");
		exit(1);
	}
	// make socket re-use able in case of crashes/restarts
	int set = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if( set == -1 ){
		perror("can't do setsockopt function, terminate.\n");
		exit(1);
	}
	my_addr.sin_family = AF_INET; 
	my_addr.sin_port = htons(port); 
	// automatically fill with my IP
	my_addr.sin_addr.s_addr = INADDR_ANY; 
	memset(my_addr.sin_zero, '\0', sizeof(my_addr.sin_zero));
	// try to bind 
	int bind_result = bind(sockfd,(struct sockaddr *) &my_addr, sizeof(struct sockaddr)); 
	if( bind_result == -1 ){
		perror("can't bind, terminate.\n");
		exit(1);
	}
	// try to listen 
	int listen_result = listen(sockfd, 10);
	if( listen_result == -1 ){
		perror("can't listen, terminate.\n");
		exit(1);
	}
	// debug, show state of socket 
	printf("listening on port [%d] .... \n", port );


	int * new_fds_temp =(int *) malloc(MAX_CONN*(sizeof(int)));
	int * threadcountlocal = (int *)malloc(sizeof(int));
	int i=0;
	while(i< MAX_CONN)
	{
		new_fds_temp[i]= -1;
		i++;
	};
	fp = fopen("serverLog.txt","a");
	if(fp==NULL)
	{
		perror("Cannot open file serverLog.txt\n");
		exit(0);
	}
	//char tmp[]="chjsdbsdmnsdm";
	//fputs(tmp, fp);
	// while forever 
	while(1){ 
		argument *arg = (argument*)malloc(sizeof(argument));

		arg->fp = fp;
		sin_size = sizeof(struct sockaddr_in);
		arg->fd_Index = findIndex(new_fds_temp);
		printf("fd_Index= %d\n", arg->fd_Index);
		if(arg->fd_Index ==-1)
		{
			perror("could not accept request, but non-fatal.. \n");	
		}
		else
		{
			// accept incoming request
			printf("Waiting for new client...\n"); 
			arg->new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size); 
			
			//lock
			pthread_mutex_lock(&mutex1); 
			(*threadcountlocal)++;
			arg->new_fds = new_fds_temp;
			arg->threadcount = threadcountlocal;
			//unlock
			pthread_mutex_unlock(&mutex1); 

			//lock
			pthread_mutex_lock(&mutexforfile); 
			pthread_mutex_lock(&mutex1); 
			fprintf(fp,"New client joined with new_fd = %d\nCurrent Online Clients: %d \n",arg->new_fd,*threadcountlocal);
			//unlock
			pthread_mutex_unlock(&mutex1); 
			pthread_mutex_unlock(&mutexforfile); 

			// valid file descriptor? 
			if( arg->new_fd != -1 )
			{
				//lock
				pthread_mutex_lock(&mutex1); 
				
				arg->new_fds[arg->fd_Index] = arg->new_fd;
				
				//unlock
				pthread_mutex_unlock(&mutex1); 
				
				pthread_create(&thread, NULL, threadfunction, arg);
			} else 
			{
				// non-fatal
				perror("could not accept request, but non-fatal.. \n");
			}
		}
	};// end while
	printf("Bottom of file, should never get here! \n\n");
	//lock
	pthread_mutex_lock(&mutex1); 
	free(new_fds_temp);
	//unlock
	pthread_mutex_unlock(&mutex1); 

	//lock
	pthread_mutex_lock(&mutexforfile);
	fclose(fp); 
	//unlock
	pthread_mutex_unlock(&mutexforfile); 
	return 0;
} // end of main

void * threadfunction(void * args)
{
	argument * arg = (argument *) args;
 	printf("Client Thread: closing sockfd\n");
	char buffer[MAX]; 
	int bytes = -1;
	close(arg->sockfd); 
	// read from client
	while(1) 
	{
		memset(buffer,0, MAX);
 		bytes = recv(arg->new_fd, buffer, MAX,0);
		printf("Bytes read : %s\n", buffer);
 		//...
		buffer[bytes]='\0';
		if(strncmp(buffer,"quit",4)==0)
		{
			char buf[]= "One client has left the chart\n";
			printf("%s",buf);
			callsend(arg->fd_Index,arg->new_fd, buf, MSG_DONTWAIT, arg->new_fds,sizeof(buf)+1);
			break;
		}
		callsend(arg->fd_Index,arg->new_fd, buffer, MSG_DONTWAIT,arg->new_fds,bytes+1);
	};
	callclose(arg->fd_Index,arg->new_fds, arg->threadcount, arg->fp);
 	//close(new_fds[fd_Index]); 
	// kill the child 
	pthread_exit(0);
	
}


void callsend(int fd_Index,int new_fd, char *buf, int flag, int *new_fds, int size )
{
	int count =0;
	while(count< MAX_CONN)
	{
		//lock
		pthread_mutex_lock(&mutex1); 
		int current_fd = new_fds[count];
		//unlock
		pthread_mutex_unlock(&mutex1); 
		if(current_fd>=0)
		{
			printf("Sending from %d to %d \n", fd_Index, count);
			send(current_fd, buf,size, MSG_DONTWAIT);
		}
		count++;
	};
}

void callclose(int fd_index, int *new_fds,int *threadcount,FILE *fp)
{
	//lock
	pthread_mutex_lock(&mutexforfile); 
	pthread_mutex_lock(&mutex1); 
	close(new_fds[fd_index]);
	new_fds[fd_index]= -1;
	(*threadcount)++;
	fprintf(fp,"One client left\nCurrent Online Clients: %d \n",*threadcount);
	//unlock
	pthread_mutex_unlock(&mutex1); 
	pthread_mutex_unlock(&mutexforfile); 
}


int findIndex(int *fdlist)
{
	int i=0;
	while(fdlist[i]!=-1)
	{
		i++;	
	};
	if(i<MAX_CONN)
	{
		return i;
	}
	else
	{
		return -1;
	}
}
