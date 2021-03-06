//....shared mem & sem.../
// this program create a shared memory between client and server. This shm is partitioned in 2 blocks, such that both C and S can work concurrently. Semaphores (stored in files) are used to synchronise both running apps.
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h> // shared mem
#include <fcntl.h> // shared mem O_RDWR
#include <unistd.h> // for close
#include <inttypes.h> // print uint64
#include <semaphore.h>
#include <sys/stat.h>
#include "tage_predictor.h"
#include "common_var.h"

typedef struct shm_cell_type{
uint64_t pc;//program cnt
uint64_t tAddr;//target address
uint64_t t_nt;//taken - not taken
}shmCell;

#define NDATA					0x80000//data per block
#define SHM_NAME 				"/shm-serverClient"
//since program is badly closed, changing name for those files is needed
#define SEM_CLIENT_WROTE	"/sem-clientWrote-x1"
#define SEM_SERVER_READ		 "/sem-serverRead-x1"
#define SEM_STATUS_MUTEX	"/sem-statusMutex-x1"

typedef struct sharedMemory{
shmCell shm_s0[NDATA];//sector 1
shmCell shm_s1[NDATA];
int status;//0=empty; 1= only s0 full; 2= only s1 full; 3= full
int nextToRead;//by the server: 0 for s0
int nextToWrite;//by the client
}shMemory;
//....shared mem & sem...*/

//....shared mem & sem.../
int main(int argc, char **argv)
{
//...BP var declaration.../
	double  iCount = 0; 		// counter for total instructions
	double  tage_pred = 0;		// accumulator for total good predictions
	uint64_t tmp_tage_pred;		// temporary variable to be added to accumulator
	struct predictor pred_str; //predictor structure declaration
	
	if (argc != 2){
		perror("missing parameters");
		exit(1);
	}
	int desiredIstr = atoi(argv[1]); 
	initTagePredictor(&pred_str); //initialise the tage
//...BP var declaration...*/

	//semaphores used with mutual exclusion
	sem_t *clientWrote_sem, *serverRead_sem, *statusMutex_sem;
	int shm_fd, i, currStatus;//, serverRead_sem_value;
	shMemory *shmPtr;
	shmCell *shm_sector;//where to work
	
	if ((shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0660)) < 0){//Get shared memory
		perror("shm_open failed");
		exit(0);
	}
	if (ftruncate(shm_fd, sizeof(shMemory)) == -1){// Set the memory object's size 
		perror("ftruncate error");
		exit(0);
	}
	if ((shmPtr = (shMemory *)(mmap(0, sizeof(shMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0))) == MAP_FAILED){// Map the memory object
		perror("mmap failed");
		exit(0);
	}
	close(shm_fd);// Remove the link to the memory
	shmPtr->status = 0;//initialisations
	shmPtr->nextToRead = 0;
	shmPtr->nextToWrite = 0;
	
	if ((statusMutex_sem = sem_open (SEM_STATUS_MUTEX, O_CREAT | O_EXCL, 0660, 1)) == SEM_FAILED){//open sem with initial value 1 and if it exists an error is reported
		//to avoid to forget to change the semaphores files' name the following if can be removed asserting the corresponding error
		perror("statusMutex_sem already exist... Change the sem name...");
		if ((statusMutex_sem = sem_open (SEM_STATUS_MUTEX, 0, 0, 0)) == SEM_FAILED){//if the semaphore already exist but the corresponding file is not opened
			perror("sem_open 1 failed");
			exit(0);
		} else{ //set the initial value
		if (sem_init(statusMutex_sem, 0, 1) != 0){
			perror("sem_init 1 failed");
			exit(0);
	}}}
	if ((clientWrote_sem = sem_open (SEM_CLIENT_WROTE, O_CREAT | O_EXCL, 0660, 0)) == SEM_FAILED){
		perror("clientWrote_sem already exist... Change the sem name...");
		if ((clientWrote_sem = sem_open (SEM_CLIENT_WROTE, 0, 0, 0)) == SEM_FAILED){
			perror("sem_open 2 failed");
			exit(0);
		} else{
		if (sem_init(clientWrote_sem, 0, 0) != 0){
			perror("sem_init 2 failed");
			exit(0);
	}}}
	if ((serverRead_sem = sem_open (SEM_SERVER_READ, O_CREAT | O_EXCL, 0660, 1)) == SEM_FAILED){
		perror("serverRead_sem already exist... Change the sem name...");
		if ((serverRead_sem = sem_open (SEM_SERVER_READ, 0, 0, 0)) == SEM_FAILED){
			perror("sem_open 3 failed");
			exit(0);
		} else{
		if (sem_init(serverRead_sem, 0, 1) != 0){
			perror("sem_init 3 failed");
			exit(0);
	}}}

	while(iCount < desiredIstr){
		sem_wait(statusMutex_sem);//decrement sem if > 0; access critical section: status; returned value can be cheked
		currStatus = shmPtr->status;
		sem_post(statusMutex_sem);//increment sem; release shm status access; returned value can be cheked
		
		if(currStatus == 0){//shm empty or already read twice
			sem_wait(clientWrote_sem);
		}
		else{
			if (shmPtr->nextToRead == 0)//s0 to read
					shm_sector = shmPtr->shm_s0;
			else
					shm_sector = shmPtr->shm_s1;
//.....code below here...................../
			for(i = 0; i < NDATA && iCount < desiredIstr; i++){
				// Performing predicitons
				tmp_tage_pred = get_prediction((shm_sector + i)->pc, &pred_str);
				update_predictor((shm_sector + i)->pc, (shm_sector + i)->t_nt, tmp_tage_pred, (shm_sector + i)->tAddr, &pred_str);
				if(tmp_tage_pred == (shm_sector + i)->t_nt){
					tage_pred += 1;		
				}
				iCount ++;
			}
//.....code above here.....................*/
			sem_wait(statusMutex_sem);
			switch (shmPtr->status){
				case 1:
				case 2://only one sector was full
					shmPtr->status = 0;//shm empty
					break;
				case 3://shm full
					if (shmPtr->nextToRead == 0)//s0 just read
						shmPtr->status = 2;//only s0 full
					else
						shmPtr->status = 1;//only s1 full
					break;
				default://error
					printf("status out of range: %d", shmPtr->status);
					exit(0);
					break;
			}
			shmPtr->nextToRead ^= 1;
			sem_post(serverRead_sem);//tell that server has read
			sem_post(statusMutex_sem);
		}
	}//end while

//...BP end.../	
	endTagePredictor(&iCount, &tage_pred, &pred_str); //prints the useful informations in a file
//...BP end...*/
	
	if (munmap (shmPtr, sizeof(shMemory)) == -1){// Detach the shared memory segment.
		perror("munmap");
		exit(0);
	}
	return 0;
}
//....shared mem & sem...*/
