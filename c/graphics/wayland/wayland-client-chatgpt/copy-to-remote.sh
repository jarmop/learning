#!/bin/bash

PORT=2223
USER="jarmovm"
PASSFILE="passw"
SRCDIR="src"
BIN="./wayland-client"
COMPILE="gcc ${SRCDIR}/*.c -o ${BIN} $(pkg-config --cflags --libs wayland-client)"
RUN="${BIN}"
SCRIPT="${COMPILE};${RUN}"

sshpass -f ${PASSFILE} scp -P ${PORT} -r ${SRCDIR} ${USER}@localhost:

# sshpass -f ${PASSFILE} ssh -p ${PORT} ${USER}@localhost ${SCRIPT}
sshpass -f ${PASSFILE} ssh -p ${PORT} ${USER}@localhost ${COMPILE}
