#!/bin/bash

PORT=2223
USER="jarmovm"
PASSFILE="passw"
SRCDIR="src"
WORKDIR="/home/jarmovm/wayland-client-book"
BIN="./wayland-client"
COMPILE="gcc ${WORKDIR}/${SRCDIR}/*.c -o ${WORKDIR}/${BIN} $(pkg-config --cflags --libs wayland-client)"
RUN="${WORKDIR}/${BIN}"
SCRIPT="${COMPILE};${RUN}"

sshpass -f ${PASSFILE} scp -P ${PORT} -r ${SRCDIR} ${USER}@localhost:${WORKDIR}

# sshpass -f ${PASSFILE} ssh -p ${PORT} ${USER}@localhost ${SCRIPT}
sshpass -f ${PASSFILE} ssh -p ${PORT} ${USER}@localhost ${COMPILE}
