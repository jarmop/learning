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

    // The socket file is removed so it can be recreated later 
    remove(SV_SOCK_PATH);

    /* Initialize all the bytes reserved for the socket's address structure as
    0 because that's what the bind function expects all the unused bytes in the
    structure to be */
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un));

    // Mark the sfd socket as a listening socket
    listen(sfd, BACKLOG);

    // Infinite loop
    for (;;) {

        /* Accept a connection. The connection is returned on a new
        socket, 'cfd'; the listening socket ('sfd') remains open
        and can be used to accept further connections. */
        cfd = accept(sfd, NULL, NULL);

        /* Transfer data from connected socket to stdout until EOF, 
        aka there is no data aka numread = 0 */
        while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
            fprintf(stdout,"\nIn while loop because numRead was %ld\n", numRead);
            write(STDOUT_FILENO, buf, numRead);
        }

        fprintf(stdout,"\nExited while loop because numRead was %ld\n", numRead);

        /* Close the connection socket for this client as they have ended the 
        communication. Still listening for other potential clients in the
        listening socket.*/
        close(cfd);
    }
}
