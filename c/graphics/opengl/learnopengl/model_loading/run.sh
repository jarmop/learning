#!/bin/bash

rm -f a.out

g++ model_loading.cpp ../glad/glad.c ../lib/stb_image.c -lglfw -lGL -ldl -lassimp -lcglm -lm
# gcc -Wall -Wextra test_model_loader.c ../glad/glad.c ../lib/stb_image.c -lassimp -lcglm -lm -lGL -ldl

./a.out