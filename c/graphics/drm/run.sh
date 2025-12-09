#!/bin/bash

PORT=2222
PASSFILE="foo"
WORKDIR="/home"
SRCDIR="src"
# SRC="drm-demo.c"
BIN="drm-demo"
COMPILE="gcc ${WORKDIR}/${SRCDIR}/*.c -o ${WORKDIR}/${BIN} $(pkg-config --cflags --libs libdrm) -std=gnu11"
RUN="${WORKDIR}/${BIN}"
SCRIPT="${COMPILE};${RUN}"

sshpass -f ${PASSFILE} scp -P ${PORT} -r ${SRCDIR} root@localhost:${WORKDIR}

sshpass -f ${PASSFILE} ssh -p ${PORT} root@localhost "${SCRIPT}"
