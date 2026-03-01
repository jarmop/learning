#!/bin/bash

PORT=2223
USER="jarmovm"
WORKDIR="/home/jarmovm/drm-demo"
PASSFILE="passw-ubuntu"

SRC="src/*"
BIN="drm-demo"
COMPILE="gcc ${WORKDIR}/*.c -o ${WORKDIR}/${BIN} $(pkg-config --cflags --libs libdrm) -std=gnu11"
RUN="${WORKDIR}/${BIN}"
SCRIPT="${COMPILE};${RUN}"

sshpass -f ${PASSFILE} scp -P ${PORT} -r ${SRC} ${USER}@localhost:${WORKDIR}

sshpass -f ${PASSFILE} ssh -p ${PORT} ${USER}@localhost "${COMPILE}"

# sshpass -f ${PASSFILE} ssh -p ${PORT} ${USER}@localhost "${SCRIPT}"
