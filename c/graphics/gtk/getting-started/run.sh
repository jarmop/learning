#!/bin/bash

n=$1
if [[ -z "$1" ]] then
    n=0
fi

gcc example-$n.c $(pkg-config --cflags --libs gtk4)
./a.out
