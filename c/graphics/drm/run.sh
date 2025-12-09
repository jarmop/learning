#!/bin/bash

PORT=2222
PASSFILE="foo"
WORKDIR="/home"
SRC="drm-demo.c"
BIN="drm-demo"
COMPILE="gcc ${WORKDIR}/${SRC} -o ${WORKDIR}/${BIN} $(pkg-config --cflags --libs libdrm) -std=gnu11"
RUN="${WORKDIR}/${BIN}"
SCRIPT="${COMPILE};${RUN}"

sshpass -f ${PASSFILE} scp -P ${PORT} ${SRC} root@localhost:${WORKDIR}

sshpass -f ${PASSFILE} ssh -p ${PORT} root@localhost "${SCRIPT}"
