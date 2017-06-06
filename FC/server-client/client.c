//....shared mem & sem.../
// this program create a shared memory between client and server. This shm is partitioned in 2, such that both C and S can work concurrently. Semaphores (stored in files) are used to synchronise both running apps.
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h> // shared mem
#include <fcntl.h> // shared mem O_RDWR
#include <unistd.h> // for close
#include <inttypes.h> // print uint64
#include <semaphore.h>
#include <sys/stat.h>

typedef struct shm_structure_type{
uint64_t pc;//program cnt
uint64_t tAddr;//target address
uint64_t t_nt;//taken - not taken
}shmStr;

#define NDATA					0x80000//data per block
#define SHM_NAME 				"/shm-serverClient"
//since program is badly closed, changing name for those files is needed
#define SEM_CLIENT_WROTE	"/sem-clientWrote-x6"
#define SEM_SERVER_READ		 "/sem-serverRead-x6"
#define SEM_STATUS_MUTEX	"/sem-statusMutex-x6"

typedef struct sharedMemory{
shmStr shm_s0[NDATA];//sector 1
shmStr shm_s1[NDATA];
int status;//0=empty; 1= only s0 full; 2= only s1 full; 3= full
int nextToRead;//by the server: 0 for s0
int nextToWrite;//by the client
}shMemory;
//....shared mem & sem...*/

//....shared mem & sem.../
int main(){	
while(1){//here to emulate my helper-a64.c branch prediction prj
//printf("-----0\n");
	static sem_t *clientWrote_sem, *serverRead_sem, *statusMutex_sem;//semaphores used with mutual exclusion
	static int firstTime=1, cnt = 0, round=0;
	static shMemory *shmPtr;
	shmStr *shm_sector;//where to work
	int shm_fd, currStatus, clientWrote_sem_value;
	static uint64_t tmp=0;
	
	if (!firstTime){
//printf("01\n");
		sem_wait(statusMutex_sem);//decrement sem if > 0; access critical section: status; returned value can be cheked
//printf("011\n");
		currStatus = shmPtr->status;
//printf("02\n");
		sem_post(statusMutex_sem);//increment sem; release shm status access; returned value can be cheked
//printf("03\n");
		sem_getvalue(clientWrote_sem, &clientWrote_sem_value);//read actual value
		
//printf("04\n");
		if(currStatus != 3){// && clientWrote_sem_value < 2shm not full or already written twice
//printf("012\n");
			shm_sector = (shmPtr->nextToWrite == 0) ? shmPtr->shm_s0 : shmPtr->shm_s1;//s to write
//.....code below here...................../
			if (cnt != NDATA-1){//data producer
				(shm_sector + cnt)->pc = cnt;
				(shm_sector + cnt)->tAddr = tmp;
				(shm_sector + cnt)->t_nt = 0xf0f0f0f0;
				cnt ++;
printf("%"PRIx64"\n", tmp);
tmp ++;
			}
			else{
printf("------------------%"PRIx64"\n", tmp);
tmp ++;
				(shm_sector + cnt)->pc = cnt;//last data
				(shm_sector + cnt)->tAddr = tmp;
				(shm_sector + cnt)->t_nt = 0xf0f0f0f0;
				cnt = 0;
//.....code above here...................../
				sem_wait(statusMutex_sem);
				switch (shmPtr->status){
					case 1:
					case 2://only one sector was full
						shmPtr->status = 3;//shm full
						break;
					case 0://shm empty
						if (shmPtr->nextToWrite == 0)//s0 just written
							shmPtr->status = 1;//only s0 full
						else
							shmPtr->status = 2;//only s1 full
						break;
					default://error
						perror("status out of range");
						exit(0);
						break;
				}
				shmPtr->nextToWrite ^= 1;
				sem_post(clientWrote_sem);//tell that client has written
				sem_post(statusMutex_sem);
round ++;
printf("round:%d\n", round);
			}
		}
		else{//shm full
printf("00\n");
sem_getvalue(clientWrote_sem, &clientWrote_sem_value);
printf("client %d\n", clientWrote_sem_value);
sem_getvalue(serverRead_sem, &clientWrote_sem_value);
printf("server %d\n", clientWrote_sem_value);
sem_getvalue(statusMutex_sem, &clientWrote_sem_value);
printf("mutex %d\n", clientWrote_sem_value);
			sem_wait(serverRead_sem);
printf("11\n");
		}
	}
	else{//first time, wait that server create shm
		firstTime = 0;
		if ((serverRead_sem = sem_open (SEM_SERVER_READ, O_CREAT, 0, 0)) == SEM_FAILED){
			perror("sem_open 1 failed");
			exit(0);
		}
printf("5\n");
		if (sem_wait(serverRead_sem) == -1){//decrement sem if > 0; wait that server created the shm
			perror("sem_wait failed while shm creation");
			exit(0);
		}
printf("6\n");
		if ((shm_fd = shm_open(SHM_NAME, O_RDWR, 0)) < 0){//Get shared memory
		 	perror("shm_open failed");
			exit(0);
		}
		if ((shmPtr = (shMemory *)(mmap(0, sizeof(shMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0))) == MAP_FAILED){// Map the memory object
			perror("mmap failed");
			exit(0);
		}
		close(shm_fd);// Remove the link to the memory
		if ((clientWrote_sem = sem_open (SEM_CLIENT_WROTE, O_CREAT, 0, 0)) == SEM_FAILED){
			perror("sem_open 2 failed");
			exit(0);
		}
		if ((statusMutex_sem = sem_open (SEM_STATUS_MUTEX, O_CREAT, 0, 0)) == SEM_FAILED){
			perror("sem_open 3 failed");
			exit(0);
		}
printf("3\n");
	}
}
/*if (munmap (shmPtr, sizeof(shMemory)) == -1){// Detach the shared memory segment.
	perror("munmap");
	exit(0);
}*/
return 0;
}
//....shared mem & sem...*/
//printf("0\n");