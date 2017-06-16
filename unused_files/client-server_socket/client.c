#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_SOCKET 256


int main(){
while(1){//to emulate the helper-a64 function

//....socket v1.../
	static int flag=0;
	static int sockfd, portno, n;
   static struct sockaddr_in serv_addr;
   static struct hostent *server;
   static char buffer[BUFFER_SOCKET];
   static uint64_t pc=0;
   pc++;
   
	if (!flag){
    flag = 1;
    portno = 51717;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("ERROR opening socket\n"); fflush(stdout); //exit(0);
        }
    server = gethostbyname("fc"); //TO BE MODIFIED DEPENDING ON YOUR PC NAME
    if (server == NULL) {
        printf("ERROR, no such host\n"); fflush(stdout); //exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){ 
        printf("ERROR connecting\n"); fflush(stdout); flag = 1;//exit(0);
        }
    //printf("Please enter the message: ");
    }
    //bzero(buffer,BUFFER_SOCKET);
    //snprintf(buffer, sizeof(uint64_t), "%"PRIx64"", pc);
    //prima era 64 e stampava a scala
    // sizeof(uint64_t) -> 0xffff000
    // BUFFER_SOCKET -> a scala
		memcpy(buffer, &pc, sizeof(uint64_t));
		printf("%"PRIx64"\n", pc);
    n = write(sockfd,buffer,sizeof(uint64_t));
    if (n < 0){
         printf("ERROR writing to socket\n"); fflush(stdout); 	flag = 1;//exit(0);
         }
    
   //close(sockfd);
	//sleep(1);
//....socket v1...*/

/*/....socket v2.../
  static int clientSocket;
  static char buffer[1024];
  static struct sockaddr_in serverAddr;
  static socklen_t addr_size;

  //---- Create the socket. The three arguments are: ----
  // 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) 
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  //---- Configure settings of the server address struct ----
  // Address family = Internet
  serverAddr.sin_family = AF_INET;
  // Set port number, using htons function to use proper byte order
  serverAddr.sin_port = htons(7891);
  // Set IP address to localhost 
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  // Set all bits of the padding field to 0
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  //---- Connect the socket to the server using the address struct ----
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
  
  //---- Send message to the socket of the incoming connection ----
  strcpy(buffer,"Hello World\n");
  send(clientSocket,buffer,13,0);
//....socket v2...*/
}
  return 0;
}

