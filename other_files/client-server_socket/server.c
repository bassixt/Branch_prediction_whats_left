//...v1...some data are lost.../ 
// A simple server in the internet domain using TCP The port number is passed as an argument
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
		while((n = read(newsockfd,buffer,BUFFER_SOCKET-1)) > 0){
			printf("0x%"PRIx64"\n",buffer);
					memcpy(&numero, buffer, sizeof(uint64_t));
					printf("%"PRIx64"\n", numero);
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
