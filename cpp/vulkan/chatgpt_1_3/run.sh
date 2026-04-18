#!/bin/bash

# glslc shaders/vert.vert -o vert.spv
# glslc shaders/frag.frag -o frag.spv

glslangValidator -V shaders/vert.glsl -o shaders/vert.spv
glslangValidator -V shaders/frag.glsl -o frag.spv

# xxd -i vert.spv > vert_spv.inc
# xxd -i frag.spv > frag_spv.inc

# g++ main.cpp -lvulkan -lglfw

# ./a.out