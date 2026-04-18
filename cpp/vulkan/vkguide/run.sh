#!/bin/bash

g++ -Wall -Wextra *.cpp -Ilib -Ithird_party $(pkg-config --cflags --libs sdl2 fmt)

./a.out