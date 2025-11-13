/* is_seqnum.h

   Header file for is_seqnum_sv.c and is_seqnum_cl.c.
*/
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include "lib/read_line.h"          /* Declaration of readLine() */
#include "lib/my_tlpi_hdr.h"

#define PORT_NUM "50000"        /* Port number for server */

#define INT_LEN 30              /* Size of string able to hold largest
                                   integer (including terminating '\n') */