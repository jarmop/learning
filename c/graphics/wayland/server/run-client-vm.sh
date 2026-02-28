#!/bin/bash

PORT=2223
USER="jarmovm"
PASSFILE="passw"
ELF="client.elf"

# Compile
gcc client/client.c client/xdg-shell-protocol.c $(pkg-config --cflags --libs wayland-client) -o client.elf

# Copy to vm
sshpass -f ${PASSFILE} scp -P ${PORT} ${ELF} ${USER}@localhost:

# Run at vm 
# -t flag makes the running program terminate when the ssh connection closes
#    (More specifically, the -t flag makes ssh open up a pseudo-tty which is closed 
#    when the ssh connection closes and that causes the kernel to emit SIGHUP to 
#    the foreground process group effectively terminating each process)
sshpass -f ${PASSFILE} ssh -t -p ${PORT} ${USER}@localhost ./${ELF}