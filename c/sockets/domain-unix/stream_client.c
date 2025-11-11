/* An example UNIX domain stream socket client. This client transmits
contents of stdin to a server socket. */

#include "stream.h"

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);

    /* Initialize all the bytes reserved for the socket's address structure as
    0 because that's what the bind function expects all the unused bytes in the
    structure to be */
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un));

    // Copy stdin to socket
    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        printf("sending: %d\n", *buf);
        write(sfd, buf, numRead);
    }

    // Closes the socket by sending empty data (numRead at server becomes 0)
    exit(EXIT_SUCCESS);         
}
