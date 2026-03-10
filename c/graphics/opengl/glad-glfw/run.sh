#!/bin/bash

gcc hello_gl.c ext/glad.c -lglfw -lGL -ldl

./a.out