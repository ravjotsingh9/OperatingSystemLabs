#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<semaphore.h>
#include<sys/stat.h>

#define buf_size 4
#define item_size 1000

FILE * outfile;
struct Data
{
	char buf[buf_size][item_size];
	int islast;
	int count;
};
int main(int argc, char **argv)
{
	//char *buf;
	if(argc != 2)
	{
		printf("Output filename is required.\n");
		return 0;
	}
	struct Data * buff;
	int shm_handle;
	//char buffer[SHM_SIZE- sizeof(fileInfo)];
	key_t key = ftok("./", 528);
	//char *shm_ptr;
	//create a shared memory
	if((shm_handle=shmget(key, sizeof(struct Data), IPC_CREAT|0666))<0)
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
	
	int i=0;
	//open the file	
	outfile= fopen(argv[1],"w");
	sem_t *empty =sem_open("/sema_empty3",O_CREAT,S_IRUSR | S_IWUSR,buf_size);
	sem_t *full = sem_open("/sema_full3", O_CREAT, S_IRUSR | S_IWUSR, 0);
	while(1)
	{
	//	printf("-------c------");
		int bytes_wrote =0;
		sem_wait(full);
		if(buff->islast ==0)
		{
			bytes_wrote = fwrite( buff + i*item_size,1, item_size, outfile);
		}
		else
		{
			bytes_wrote = fwrite( buff + i*item_size,1, buff->count, outfile);
		}
		sem_post(empty);
	
		i--;
		i=i%buf_size;
		if(bytes_wrote< item_size)
		{
			break;
		}
	};
	// detach from the shared memory segment
	shmdt(buff);
	// remove the shared memory segment
	shmctl(shm_handle,IPC_RMID,0);
	close(outfile);
	return 0;
}

