#!/bin/bash

glslangValidator -V -S vert shaders/vert.glsl -o shaders/vert.spv
glslangValidator -V -S frag shaders/frag.glsl -o shaders/frag.spv

xxd -i shaders/vert.spv > shaders/vert_spv.inc
xxd -i shaders/frag.spv > shaders/frag_spv.inc

g++ main.cpp -lvulkan -lglfw

./a.out