# Client-server exchanging data by shared memory
## Why shared memory (SHM) technique
Running a benchmark application, produces a lot of jumps and branches in the code, storing current PC, TargetAddress and type of branches information in a file to be analysed later will result in slow process and will fill the hard disk. For that reason, the best option is to let our branch predictor analysing on the fly those data and store only the useful results. To exchange lots of data between applications on the fly the best optimised way is to use shared memory (SHM), since reduce the number of writings and readings from the volatile memory without accessing the HD.  
Also a TCP socket could have been exploited with the advantage to let different machines working concurrently or from different places, however while applying this technique we noticed that after 30 - 60 seconds the running apps where stopped probably by the OS because of the huge data exchange (maybe some firewall configurations were needed).  
## <a name="Named_semaphores"></a>Named semaphores
Since in this case the pids of both app are not known, to synchronise the two processes, named semaphores were chosen which will store the flags in named files in temporary system sub-directories. For that reason, if the server or the client do not destroy the semaphores before the end, the previous name for those semaphores need to be changed in order to run again the apps (it is also indicated in the files). In our case the end of the branch instructions is not detected from the client and so will use those semaphores, thus no one can destroy them, so the name must be changed every time we run those programs.  
## SHM partitioning
For optimisation reason the shared memory is subdivided in two pieces such that the client can fill one while the server can read the other previously written by the client.  
The status flag of the SHM (completely full, half full ...) is stored in the `status` variable of `shMemory` structure and to read it the `statusMutex_sem` semaphore is used for synchronisation between client and server (only one at time should read or modify this variable).  
When the client has filled the first section of the SHM, he will inform the server `sem_post(clientWrote_sem)` that was waiting on that semaphore `sem_wait(clientWrote_sem)`, so the client starts to store data in the second section while the server begins its computations with the data already stored in the first section. If, for example, the client fills the memory faster than the server reads and uses them, the client will wait that at least one section is free `sem_wait(serverRead_sem)`.  
## SHM organisation
As can be seen from *server.c* the shared data are stored in structures such that any modification for future new purposes will be easy to be managed.  
In fact only the `struct shm_cell_type`, that is renamed `shmCell`, need to be modified depending on the data you want to exchange. Thus each partition of our SHM is composed by a vector of cells which are composed by that structure.  
Moreover, reading the 15th data in the vector `shm_s0` means reading the 15th data sent by the client, so both data exchange and order are preserved.  
## How to use it
It is really simple:
1. Modify both the client's (helper-a64.c) and server's `shmStr` structure as you wish. In it there are the data exchanged for each write/read operation.  
Currently the `shmCell` is defined as following:
```c
typedef struct shm_cell_type{
	uint64_t pc;//program cnt
	uint64_t tAddr;//target address
	uint64_t t_nt;//taken - not taken
}shmCell;
```
2. If needed, modify the semaphores' names (read [Named semaphores](#Named_semaphores)) and the SHM partitions' length changing the define `NDATA` value.
3. Modify the server code to do the job you want; put your own code in the indicated point:
```c
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
```
The variable `shm_sector` is a pointer to the first cell of the SHM's partition were the server is working.  
Between those comments you can simply call a function to your own Branch predictor (or whatever).  
In this case we initialised our BP at the beginning, here we use and update the predictor and at the end of the previous `while` we extract the useful information to be printed in a file.
4. Compile the code and launch it passing as parameter the number of instructions you want to analyse, for instance:
```sh
$ gcc -Wall -o server server.c -lrt -lpthread
$ ./server 100000
```
will compile "server.c" and break the `while` after 100000 instructions have been analysed.
## Bugs and improvements
### Lasts instructions
Assuming that we want to analyse all the branches encountered, in this implementation of the SHM a bug is present: the last written data by the client are not read from the server if the memory section is not completely full. It means that if we have SHM sections of 1024 cells and at the end we write only 500 data, the client will not inform the server to read them.  
Solutions can be:  
* using a new flag in the `shMemory` but the client should understand when it has finished (not easy in our case)...
* use SHM sections smaller such that the last lost data are few.
* use sections with only one cell length (no data are lost, but overhead due to synchronisation).  
### Semaphores
To solve the semaphores' name problem and so to finalise the server computations detecting the end of the benchmark, different solutions exists:  
* instruct Qemu to receive a sigkill that stops the current processes.
* detect in the helper function some address that corresponds to the lasts branches.
* add a timeout to the server if no more data arrives.
* fork the server to enter manually a terminating character.  
## Useful links
**For further information on server and client applications, using shared memory and semaphores** those link can be useful:  
https://www.softprayog.in/programming/interprocess-communication-using-posix-shared-memory-in-linux  
http://www.csc.villanova.edu/~mdamian/threads/posixsem.html  
