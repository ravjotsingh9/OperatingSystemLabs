#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<semaphore.h>
#include<sys/stat.h>

#define buf_size 4
#define item_size 1000

FILE * infile;
struct Data
{
	char buf[buf_size][item_size];
	int islast;
	int count;
};
struct Data d;

int main(int argc, char **argv)
{
	//char *buf;
	struct Data * buff;
	char buffer[item_size];
	int shm_handle;
	if(argc != 2)
	{
		printf("Input filename is required.\n");
		return 0;
	}
	//printf("----p====");
	key_t key = ftok("./",528);
	//char *shm_ptr;
	//create a shared memory
	if((shm_handle=shmget(key,sizeof(struct Data), IPC_CREAT|0666))<0)
	{
		perror("shmget");
		exit(1);
	}
	//attach the shared memory created
	if((buff =(struct Data *) shmat(shm_handle, 0, 0))==(char *)-1)
	{
		perror("shmat");
		exit(1);
	}
	
	//printf("----p====");
	int i=0;
	d.islast = 0;
	d.count =0;
	buff->islast =0;
	buff->count =0;
	//open the file	
	infile= fopen(argv[1],"rt");
	sem_t *empty =sem_open("/sema_empty3",O_CREAT,S_IRUSR | S_IWUSR,buf_size);
	sem_t *full = sem_open("/sema_full3", O_CREAT, S_IRUSR | S_IWUSR, 0);
	while(1)
	{
		//printf("----p====");
		//fflush(stdout);
		int bytes_read = fread(buffer,1, item_size, infile);
		sem_wait(empty);
		if (bytes_read < item_size)
		{
			buff->islast = 1;
			buff->count = bytes_read;	
		}
		memcpy(buff +(i*item_size),buffer, item_size);
		sem_post(full);
		//sleep(1);
		i++;
		//printf("----p=============");
		//fflush(stdout);
		i=i%buf_size;
		if(bytes_read< item_size)
		{
			break;
		}
	};
	close(infile);
	return 0;
}

