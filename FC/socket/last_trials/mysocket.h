/*	HEADER FOR BSD4.3 UNIX
*/

#ifndef _MYSOCKET_H
#define _MYSOCKET_H

/* Headers necessary for using the socket library */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* The function used for closing a socket */
#define closesocket(x)	close(x)

/* The error code */
#define INVALID_SOCKET	-1

/* The type used for socket identification */
typedef int SOCKET;

/* Utility Function Prototypes */
void err_fatal(char *mes);      /* Error handling function for fatal errors */
void err_continue(char *mes);   /* Error handling function for non-fatal errors */
void SockStartup(void);         /* Library initialization function */
void SockCleanup(void);         /* Library cleanup function */
void showAddr(char * str, struct sockaddr_in * a); /* Utility function to display a string str
                                                    followed by an internet address a, written
                                                    in decimal notation
                                                    */
#endif
