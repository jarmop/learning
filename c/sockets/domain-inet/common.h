#include <netinet/in.h>
#include <sys/socket.h>
#include "lib/read_line.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PORT_NUM "50000"

/* Size of string able to hold largest
integer (including terminating '\n') */
#define INT_LEN 30