/*	UTILITY FUNCTIONS FOR IPV4 SOCKET APPLICATIONS
        UNIX BSD4.3 VERSION
*/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "mysocket.h"

/* Error handling function for fatal errors */
void err_fatal(char *mes)
{
	printf("%s, errno=%d\n", mes, errno);
	perror("");
	exit(1);
}

/* Error handling function for nonfatal errors */
void err_continue(char *mes)
{
	printf("%s, errno=%d\n", mes, errno);
	perror("");
}

/* BSD4.3 Sockets Api: Initialization function */
void SockStartup(void)
{
	/* don't need initialization */
        return;
}

/* BSD4.3 Sockets Api: Cleanup function */
void SockCleanup(void)
{
        /* don't need cleanup */
        return;
}

/* Utility function to display a string str
   followed by an internet address a, written
   in decimal notation
*/
void showAddr(char *str, struct sockaddr_in *a)
{
    char *p;
    
    p = inet_ntoa(a->sin_addr);
	printf("%s %s",str,p);
	printf("!%u\n", ntohs(a->sin_port));
}
