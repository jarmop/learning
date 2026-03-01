#!/bin/bash

# alpine
PORT=2222
USER="root"
WORKDIR="/home/drm-demo"
PASSFILE="passw-alpine"

SRCDIR="src"
BIN="drm-demo"
COMPILE="gcc ${WORKDIR}/*.c -o ${WORKDIR}/${BIN} $(pkg-config --cflags --libs libdrm) -std=gnu11"
RUN="${WORKDIR}/${BIN}"
SCRIPT="${COMPILE};${RUN}"

sshpass -f ${PASSFILE} scp -P ${PORT} -r ${SRCDIR} ${USER}@localhost:${WORKDIR}

# sshpass -f ${PASSFILE} ssh -p ${PORT} root@localhost "${SCRIPT}"
sshpass -f ${PASSFILE} ssh -p ${PORT} ${USER}@localhost "${COMPILE}"
