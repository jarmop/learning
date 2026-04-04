#!/bin/bash

rm -f a.out

g++ model_loading.cpp ../glad/glad.c ../lib/stb_image.c -lglfw -lGL -ldl -lassimp -lcglm

./a.out