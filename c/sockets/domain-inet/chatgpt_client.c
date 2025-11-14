#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(1);
    }

    write(fd, "Hello from client\n", 19);

    char buf[1024];
    ssize_t n = read(fd, buf, sizeof(buf)-1);
    if (n > 0) {
        buf[n] = '\0';
        printf("Received: %s\n", buf);
    }

    close(fd);
    return 0;
}
