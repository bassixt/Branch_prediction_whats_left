/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SOCKET 256

int main(void)
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[BUFFER_SOCKET];
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
			printf("0x%s\n",buffer);
			bzero(buffer,BUFFER_SOCKET);
		}
		if (n < 0) printf("ERROR reading from socket\n");
	close(newsockfd);
	}
     close(sockfd);
     return 0; 
}
