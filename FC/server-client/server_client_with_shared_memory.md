# Client-server exchanging data by shared memory
Since the data about addresses produced while running a benchmark application produces a lot of jumps and branches in the code, storing those ammount of data in a file to be analysed later results in slow process and fills the hard disk, so the best option is to let our branch predictor analysing on the fly those data and store only the useful results. To exchange lots of data between applications on the fly, the best optimised way is to use shared memory (SHM), since reduce the mumber of writings and readings from the volatile memory without accessing the HD.  
Since in this case the pids of both app are not known, to sincronise the processes were chosen named semafores which will store the flags in named files in temporary system sub-directories. For that reason, if the server or the client do not destroy the semafores before the end, the previous name for those semafores need to be changed as it is indicated in the files. In our case the end of the branch instructions is not known so the client cannot destroy them and server are badly terminated and thus the name must be changed every time we run it.  
For optimisation reason the shared memory is subdivided in two pieces such that the client can fill one while the server can read the other previously written by the client.  
The status flag of the SHM (completely full, half full ...) is stored in the `status` variable of `shMemory` stucture and to read it the `statusMutex_sem` semafore is used for synchronisation between client and server (only one at time can read or modify this variable).  
When the client fill the first section of the SHM he inform the server `sem_post(clientWrote_sem);` that was waiting on that semaphore `sem_wait(clientWrote_sem);`, so the client start to store data in the second section and the server do its computations with the data in the first section. If, for example, the client fills the memory faster than the server read and use them, the client will wait that one section will be free `sem_wait(serverRead_sem);`.  
As can be seen from *server.c* the shared data are stored in structures such that any modification for future new purposes will be easy to be managed. In fact only `struct shm_structure_type` need to be modified depending on the data we want to exchange. Thus every cell of our SHM is composed by that structure and for instance reading the 15th data in the vector `shm_s0` means reading the 15th data sent by the client, so both data exchange and time reference is preserved.  
In this implementation of the SHM a bug is present: the last written data by the client are not read from the server if the memory section is not completely full. It means that if we have SHM sections of 1024 cell and at the end we write only 500 data, the client will not inform the server to read them.  
Solutions can be:  
* using a new flag in the `shMemory` but the client should understand when it has finished (not easy in our case)...
* use SHM sections smaller such that the last lost data are few.
* use sections with only one cell lenght (no data are lost, but overhead due to synchronisation).  

To solve the semaphores' name problem and so to finalise the server computations detecting the end of the benchmark, different solutions exists:  
* add a timeout to the server if no more data arrives.
* fork the server to enter manually a terminating character.
* detect in the helper function some address that corresponds to the lasts branches.  

**For further information on server and client applications, using shared memory and semaphores** those link can be useful:  
https://www.softprayog.in/programming/interprocess-communication-using-posix-shared-memory-in-linux  
http://www.csc.villanova.edu/~mdamian/threads/posixsem.html  
