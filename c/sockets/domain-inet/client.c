/* A simple Internet stream socket client. This client requests a sequence
   number from the server.
*/

// To get definitions of NI_MAXHOST and NI_MAXSERV from <netdb.h>
#define _DEFAULT_SOURCE

#include <netdb.h>
#include "common.h"

int main(int argc, char *argv[])
{
    char *reqLenStr;                    /* Requested length of sequence */
    char seqNumStr[INT_LEN];            /* Start of granted sequence */
    int cSocketFd;
    ssize_t numRead;
    struct addrinfo aiSpec;
    struct addrinfo *firstAI, *currAI;

    /* Call getaddrinfo() to obtain a list of addresses that
       we can try connecting to */
    memset(&aiSpec, 0, sizeof(struct addrinfo));
    aiSpec.ai_canonname = NULL;
    aiSpec.ai_addr = NULL;
    aiSpec.ai_next = NULL;
    aiSpec.ai_family = AF_UNSPEC;                /* Allows IPv4 or IPv6 */
    aiSpec.ai_socktype = SOCK_STREAM;
    aiSpec.ai_flags = AI_NUMERICSERV;

    getaddrinfo(argv[1], PORT_NUM, &aiSpec, &firstAI);

    /* Walk through returned list until we find an address structure
       that can be used to successfully connect a socket */
    for (currAI = firstAI; currAI != NULL; currAI = currAI->ai_next) {
        cSocketFd = socket(
            currAI->ai_family, 
            currAI->ai_socktype, 
            currAI->ai_protocol
        );

        // On error, try next address
        if (cSocketFd == -1)
            continue;                   

        if (connect(cSocketFd, currAI->ai_addr, currAI->ai_addrlen) == 0)
            break;                              /* Success */

        /* Connect failed: close this socket and try next address */

        close(cSocketFd);
    }

    freeaddrinfo(firstAI);

    /* Send requested sequence length, with terminating newline */
    reqLenStr = (argc > 2) ? argv[2] : "1";
    write(cSocketFd, reqLenStr, strlen(reqLenStr));
    write(cSocketFd, "\n", 1);

    /* Read and display sequence number returned by server */
    numRead = readLine(cSocketFd, seqNumStr, INT_LEN);

    printf("Sequence number: %s", seqNumStr);   /* Includes '\n' */

    exit(EXIT_SUCCESS);                         /* Closes 'cfd' */
}