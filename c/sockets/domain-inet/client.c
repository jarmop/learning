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
    int cfd;
    ssize_t numRead;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    /* Call getaddrinfo() to obtain a list of addresses that
       we can try connecting to */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC;                /* Allows IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    getaddrinfo(argv[1], PORT_NUM, &hints, &result);

    /* Walk through returned list until we find an address structure
       that can be used to successfully connect a socket */
    for (rp = result; rp != NULL; rp = rp->ai_next) {

        cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (cfd == -1)
            continue;                   /* On error, try next address */

        if (connect(cfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;                              /* Success */

        /* Connect failed: close this socket and try next address */

        close(cfd);
    }

    freeaddrinfo(result);

    /* Send requested sequence length, with terminating newline */
    reqLenStr = (argc > 2) ? argv[2] : "1";
    write(cfd, reqLenStr, strlen(reqLenStr));
    write(cfd, "\n", 1);

    /* Read and display sequence number returned by server */
    numRead = readLine(cfd, seqNumStr, INT_LEN);

    printf("Sequence number: %s", seqNumStr);   /* Includes '\n' */

    exit(EXIT_SUCCESS);                         /* Closes 'cfd' */
}