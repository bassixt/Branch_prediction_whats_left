//...sh mem.../
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <sched.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>

#define NDATA		1024
#define SHMSZ     NDATA*8 //8 is # byte for uint64

int main()
{
    int shmid;
    key_t key;
    uint64_t *shm;
    /*
     * We need to get the segment named
     * "5678", created by the server.
     */
    key = 6166529;

    /*
     * create the segment.| 0666 or
     */
    //if ((shmid = shmget(key, SHMSZ, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR)) < 0) {
    if ((shmid = shm_open("/mymem", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, 0, 0)) == (uint64_t *) -1) {
        perror("shmat");
        exit(1);
    }
		*shm = 0; //initialisation
    /*
     * Now read what the server put in the memory.
     */
     while(1){
		if(*shm != 0){ //if not jet finished
/*    for (s = shm; *s != NULL; s++)
        putchar(*s);
    putchar('\n');
*/
//numero = (uint64_t *)shm;
					//memcpy(&numero, shm, sizeof(uint64_t));
					printf("PC=0x%" PRIx64 "\n", *(shm+8));

    /*
     * Finally, change the first character of the 
     * segment to '*', indicating we have read 
     * the segment.
     */
    *shm = 0;
		}//else
			//usleep(1);
		}
    return 0;
}
//...sh mem...*/

/*/....socket samu.../
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

#include "mysocket.h"


#define BUFLEN 1024
#define MAX_N 10
#define MAX_TIMES 3
#define BACKLOG 2
#define LIMIT 65535
#define TIMEOUT 40

int performTransfer(SOCKET s, char *name);
int writen(SOCKET s, char *ptr, size_t nbytes);


//server process
int main(void){
				
	char				buf[BUFLEN];
	uint16_t			port_n, port_h;
	SOCKET				s, conn;
	int					result, addrlen, n, flag, len;

	uint64_t numero;
	struct sockaddr_in	serv_addr, cli_addr;
	fd_set 				read_set;
	struct timeval 		tval;


	port_h = atoi("3333");
	if(port_h > LIMIT){
		printf("port number too high\n");
		exit (-1);
	}
	port_n = htons(port_h);

	// create the socket /
	//printf("creating socket\n");
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s == INVALID_SOCKET){
		printf("socket() failed\n");
		perror("");
		exit(-1);
	}
	//printf("done, socket number %u\n",s);


	//prepare client address structure
	//this structure is necessary for binding an unused port to the client
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_port		= port_n; 
	serv_addr.sin_addr.s_addr	= htonl(INADDR_ANY);

	//bind
	//showAddr("Binding to address", &serv_addr);
	result = bind(s, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	if(result == -1){
		printf("bind() failed\n");
		perror("");
		close(s);
		exit(-1);
	}
	//printf("done.\n");


	//listening to sockets
	//printf("start listening...\n");
	result = listen(s, BACKLOG);
	if(result == -1){
		printf("listen() failed\n");
		perror("");
		exit(-1);
	}

	// main server loop 
	for (;;){
		
		//printf("waiting for incoming connections...\n");
		addrlen = sizeof(struct sockaddr_in);
		conn = accept(s, (struct sockaddr *) &cli_addr, &addrlen);
		if(conn == INVALID_SOCKET){
			printf("accept() failed\n");
			perror("");
			continue;
		}
		
		//showAddr("Accepted connection from", &cli_addr);
		//printf("new socket: %d\n",conn);
		flag = 1;

		while(flag){

			FD_ZERO(&read_set);
			FD_SET(conn, &read_set);
			tval.tv_sec = TIMEOUT;
			tval.tv_usec = 0;
			n = select(FD_SETSIZE, &read_set, NULL, NULL, &tval);
			if(n == -1){
				printf("select() failed)\n");
				flag = 0;
				continue;
			}
			
			if(n > 0){
				
				n = recv( conn, buf, BUFLEN, 0);
				if (n < 0){
					printf("Read error\n");
					printf("closing socket %d..\n", conn);
					close(conn);
					printf("Socket closed\n");
					flag = 0;
				}
				else if (n==0){
					//printf("Connection closed by party on socket %d\n",conn);
					//printf("closing socket %d..\n", conn);
					close(conn);
					//printf("Socket closed\n");
					flag = 0;
				}
				else{
					//printf("Received line from socket %d :\n", conn);
					//buf[n-2]='\0'; //eliminating the '\r\n' characters
					len = sizeof(uint64_t);
					memcpy(&numero, buf, len);
					printf("PC=0x%" PRIx64 "\n", numero);
					
				}
			}
			else{
				printf("timeout on socket %d\n", conn);
				printf("closing socket %d..\n", conn);
				close(conn);
				printf("socket closed\n");
				flag = 0;
			}
			printf("closing socket %d..\n", conn);
			close(conn);
			flag = 0;
		}
	}

	close(s);
	return 0; //never reached
}


int performTransfer(SOCKET s, char *buf){

	int 			fd, n, result, file_size, len;
	uint32_t 		n_bytes_n, t_mod_n, n_bytes_h, t_mod_h;
	struct 			stat statbuf;
	char 			tx[BUFLEN];

	len = strlen("GET ");

	//extracting the file name
	strcpy(tx, buf+len);
	strcpy(buf, tx);
	
	printf("name of the requested file: %s\n", buf);


	fd = open(buf, O_RDONLY);
	if(fd == -1){
		printf("open() failed\n");
		perror("");
		return -1;
	}

	result = fstat(fd, &statbuf);
	if(result == -1){
		printf("fstat() failed\n");
		perror("");
		close(fd);
		return -1;
	}

	printf("file size (Bytes): %u\n", (uint32_t) statbuf.st_size);
	printf("timestamp (last modification): %u\n", (uint32_t) statbuf.st_mtime);

	n_bytes_h = statbuf.st_size;
	t_mod_h = statbuf.st_mtime;
	
	n_bytes_n = htonl(n_bytes_h);
	t_mod_n = htonl(t_mod_h);
	
	
	strcpy(tx, "+OK\r\n");
	len = strlen("+OK\r\n");
	n = writen(s, tx, len);
	if(n != len){
		printf("problems on writing_0\n");
		close(fd);
		return -1;
	}

	len = sizeof(uint32_t);
	
	memcpy(tx, &n_bytes_n, len);
	n = writen(s, tx, len);
	if(n != len){
		printf("problems on writing_1\n");
		close(fd);
		return -2;
	}
	
	memcpy(tx, &t_mod_n, len);
	n = writen(s, tx, len);
	if(n != len){
		printf("problems on writing_2\n");
		close(fd);
		return -2;
	}

	
	file_size = (int) statbuf.st_size;
	
	while( file_size > 0 ){
	
		n = read(fd, tx, BUFLEN);
		if( n == -1 ){
			printf("read() failed\n");
			perror("");
			close(fd);
			return -2;
		}
		
		result = writen(s, tx, n);
		if(result != n){
			printf("problems on writing_3\n");
			perror("");
			close(fd);
			return -2;
		}
		
		file_size -= n;
	}
	if(file_size == 0){
		printf("reached EOF\n");
	}
	if(file_size < 0){
		printf("file_size overflow - err\n");
	}
	
	close(fd);
	return file_size;

}



// Writes nbytes from buffer ptr to stream socket s /
int writen(SOCKET s, char *ptr, size_t nbytes)
{
    size_t nleft;
    ssize_t nwritten;

    for (nleft=nbytes; nleft > 0; )
    {
	nwritten = send(s, ptr, nleft, MSG_NOSIGNAL);
	if (nwritten <=0)
	    return (nwritten);
	else
	{
	    nleft -= nwritten;
	    ptr += nwritten;   
	}
    }
    return (nbytes - nleft);
}
//....socket...*/

/*/ A simple server in the internet domain using TCP
   The port number is passed as an argument
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SOCKET 256

int main(void)
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[BUFFER_SOCKET];
     uint64_t numero;
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        printf("ERROR opening socket\n");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = 51717;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              printf("ERROR on binding\n");
     listen(sockfd,1);
     while(1){
     clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) 
			printf("ERROR on accept\n");
		bzero(buffer,BUFFER_SOCKET);
		while((n = read(newsockfd,buffer,BUFFER_SOCKET-1)) > 0){
			//printf("0x%"PRIx64"\n",buffer);
					memcpy(&numero, buffer, sizeof(uint64_t));
					printf("%"PRIx64"\n", numero);
			bzero(buffer,BUFFER_SOCKET);
		}
		if (n < 0) printf("ERROR reading from socket\n");
	close(newsockfd);
	}
     close(sockfd);
     return 0; 
}
//....v1...*/

/*/........SERVER CODE v2......../

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(){
  int welcomeSocket, newSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  //---- Create the socket. The three arguments are: ----
  // 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case)
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  //---- Configure settings of the server address struct ---- 
  // Address family = Internet 
  serverAddr.sin_family = AF_INET;
  // Set port number, using htons function to use proper byte order
  serverAddr.sin_port = htons(7891);
  // Set IP address to localhost
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  // Set all bits of the padding field to 0
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  //---- Bind the address struct to the socket ----
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  //---- Listen on the socket, with 3 max connection requests queued ----
  if(listen(welcomeSocket, 3)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  //---- Accept call creates a new socket for the incoming connection ----
  addr_size = sizeof serverStorage;
  newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

  //---- Read the message from the server into the buffer ----
  recv(newSocket, buffer, 1024, 0);

  //---- Print the received message ----
  printf("Data received: %s",buffer);  

  return 0;
}

//......v2......*/


