#!/bin/bash

# Compile

WAYLAND=/home/jarmo/development/git-external/wayland
export PKG_CONFIG_PATH="$WAYLAND/_install/lib/x86_64-linux-gnu/pkgconfig:$WAYLAND/_install/lib/pkgconfig"
gcc -g -O0 \
  $(pkg-config --cflags wayland-server) \
  -o server.elf server/server.c server/xdg-shell-protocol.c \
  $(pkg-config --libs --static wayland-server)

# Copy to vm
PORT=2223
USER="jarmovm"
PASSFILE="passw"
ELF="server.elf"

sshpass -f ${PASSFILE} scp -P ${PORT} ${ELF} ${USER}@localhost:

# Run at vm 
# -t flag makes the running program terminate when the ssh connection closes
#    (More specifically, the -t flag makes ssh open up a pseudo-tty which is closed 
#    when the ssh connection closes and that causes the kernel to emit SIGHUP to 
#    the foreground process group effectively terminating each process)
sshpass -f ${PASSFILE} ssh -t -p ${PORT} ${USER}@localhost ./${ELF}
# sshpass -f ${PASSFILE} ssh -t -p ${PORT} ${USER}@localhost WAYLAND_DEBUG=1 ./${ELF}