/* A simple Internet stream socket server. Our service is to provide
   unique sequence numbers to clients.
*/

// To get definitions of NI_MAXHOST and NI_MAXSERV from <netdb.h>
#define _DEFAULT_SOURCE

#include <netdb.h>
#include "common.h"

#define BACKLOG 50
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)

int initListeningSocket(int lSocketFd)
{
    int optval;
    struct addrinfo aiSpec;
    struct addrinfo *firstAI, *currAI;

    memset(&aiSpec, 0, sizeof(struct addrinfo));
    // options.ai_addr = NULL;
    // options.ai_canonname = NULL;
    // options.ai_next = NULL;
    aiSpec.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
    aiSpec.ai_family = AF_UNSPEC;        /* Allows IPv4 or IPv6 */
    aiSpec.ai_socktype = SOCK_STREAM;

    /** 
     * Get a list of potential IP address structures from the local DNS for
     * binding the listening socket to. The hints argument specifies criteria for
     * selecting the addrinfo structures.    
     * 
     * There are several reasons why the linked list may have more than one 
     * addrinfo structure, including: 
     *   - the network host is multihomed, accessible over multiple protocols 
     *     (for example, both AF_INET and AF_INET6); 
     *   - the same service is available from multiple socket types (one
     *     SOCK_STREAM address and another SOCK_DGRAM address, for example) 
     */
    getaddrinfo(NULL, PORT_NUM, &aiSpec, &firstAI);

    /* Walk through returned list until we find an address structure
       that can be used to successfully create and bind a socket */
    optval = 1;
    for (currAI = firstAI; currAI != NULL; currAI = currAI->ai_next) {
        lSocketFd = socket(
            currAI->ai_family, 
            currAI->ai_socktype, 
            currAI->ai_protocol
        );

        // On error, try next address
        if (lSocketFd == -1) {
            continue;
        }

        /**
         * Set the option SO_REUSEADDR at level SOL_SOCKET to value pointed by 
         * optval for the socket associated with the lSocketFd file descriptor.
         * 
         * The setsockopt() function provides an application program with the
         * means to control socket behavior. An application program can use
         * setsockopt() to allocate buffer space, control timeouts, or permit
         * socket data broadcasts.
         */
        setsockopt(lSocketFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

        if (bind(lSocketFd, currAI->ai_addr, currAI->ai_addrlen) == 0)
            break; // Success

        // bind() failed: close this socket and try next address
        close(lSocketFd);
    }

    listen(lSocketFd, BACKLOG);

    freeaddrinfo(firstAI);

    return lSocketFd;
}

int main(int argc, char *argv[])
{
    uint32_t seqNum = 0;
    char reqLenStr[INT_LEN];            /* Length of requested sequence */
    char seqNumStr[INT_LEN];            /* Start of granted sequence */
    struct sockaddr_storage claddr;
    int lSocketFd, cSocket, reqLen;
    socklen_t addrlen;
    char addrStr[ADDRSTRLEN];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    lSocketFd = initListeningSocket(lSocketFd);

    for (;;) {                  /* Handle clients iteratively */

        /* Accept a client connection, obtaining client's address */

        addrlen = sizeof(struct sockaddr_storage);
        cSocket = accept(lSocketFd, (struct sockaddr *) &claddr, &addrlen);

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