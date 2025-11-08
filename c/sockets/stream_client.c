/* An example UNIX domain stream socket client. This client transmits
   contents of stdin to a server socket. */

#include "stream.h"

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);      /* Create client socket */

    /* Construct server address, and make the connection */

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un));

    /* Copy stdin to socket */

    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
        write(sfd, buf, numRead);

    exit(EXIT_SUCCESS);         /* Closes our socket; server sees EOF */
}
