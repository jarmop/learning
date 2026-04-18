#!/bin/bash

glslc shaders/vert.vert -o vert.spv
glslc shaders/frag.frag -o frag.spv

gcc main.c -lvulkan -lglfw

./a.out