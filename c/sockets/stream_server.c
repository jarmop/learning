/* An example UNIX stream socket server. Accepts incoming connections
   and copies data sent from clients to stdout. */

#include "stream.h"
#define BACKLOG 5

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sfd, cfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);

    /* Construct server socket address, bind socket to it,
       and make this a listening socket */

    // Me: The file is removed so that it can be recreated? 
    remove(SV_SOCK_PATH);

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un));

    listen(sfd, BACKLOG);

    for (;;) {          /* Handle client connections iteratively */

        /* Accept a connection. The connection is returned on a new
           socket, 'cfd'; the listening socket ('sfd') remains open
           and can be used to accept further connections. */

        cfd = accept(sfd, NULL, NULL);

        /* Transfer data from connected socket to stdout until EOF */

        while ((numRead = read(cfd, buf, BUF_SIZE)) > 0)
            write(STDOUT_FILENO, buf, numRead);

        close(cfd);
    }
}
