/* A simple Internet stream socket server. Our service is to provide
   unique sequence numbers to clients.
*/

// To get definitions of NI_MAXHOST and NI_MAXSERV from <netdb.h>
#define _DEFAULT_SOURCE

#include <netdb.h>
#include "common.h"

#define BACKLOG 50
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)

int initListeningSocket(int lSocket)
{
    int optval;
    struct addrinfo hints;
    struct addrinfo *result, *currResult;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;        /* Allows IPv4 or IPv6 */
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

    /* Get a list of addresses for binding to. The hints argument specifies
    criteria for selecting the values returned in the result list */
    getaddrinfo(NULL, PORT_NUM, &hints, &result);

    /* Walk through returned list until we find an address structure
       that can be used to successfully create and bind a socket */
    optval = 1;
    for (currResult = result; currResult != NULL; currResult = currResult->ai_next) {
        lSocket = socket(
            currResult->ai_family, 
            currResult->ai_socktype, 
            currResult->ai_protocol
        );
        if (lSocket == -1)
            continue;                   /* On error, try next address */

        setsockopt(lSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

        if (bind(lSocket, currResult->ai_addr, currResult->ai_addrlen) == 0)
            break;                      /* Success */

        /* bind() failed: close this socket and try next address */
        close(lSocket);
    }

    listen(lSocket, BACKLOG);

    freeaddrinfo(result);

    return lSocket;
}

int main(int argc, char *argv[])
{
    uint32_t seqNum = 0;
    char reqLenStr[INT_LEN];            /* Length of requested sequence */
    char seqNumStr[INT_LEN];            /* Start of granted sequence */
    struct sockaddr_storage claddr;
    int lSocket, cSocket, reqLen;
    socklen_t addrlen;
    char addrStr[ADDRSTRLEN];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    lSocket = initListeningSocket(lSocket);

    for (;;) {                  /* Handle clients iteratively */

        /* Accept a client connection, obtaining client's address */

        addrlen = sizeof(struct sockaddr_storage);
        cSocket = accept(lSocket, (struct sockaddr *) &claddr, &addrlen);

        if (getnameinfo((struct sockaddr *) &claddr, addrlen,
                    host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
            snprintf(addrStr, ADDRSTRLEN, "(%s, %s)", host, service);                        
        } else
            snprintf(addrStr, ADDRSTRLEN, "(?UNKNOWN?)");
        printf("Connection from %s\n", addrStr);

        /* Read client request, send sequence number back */
        if (readLine(cSocket, reqLenStr, INT_LEN) <= 0) {
            close(cSocket);
            continue;                   /* Failed read; skip request */
        }

        reqLen = atoi(reqLenStr);
        if (reqLen <= 0) {              /* Watch for misbehaving clients */
            close(cSocket);
            continue;                   /* Bad request; skip it */
        }

        snprintf(seqNumStr, INT_LEN, "%d\n", seqNum);
        if (write(cSocket, seqNumStr, strlen(seqNumStr)) !=
                (ssize_t) strlen(seqNumStr))
            fprintf(stderr, "Error on write");

        seqNum += reqLen;               /* Update sequence number */

        close(cSocket);       /* Close connection */            
    }
}