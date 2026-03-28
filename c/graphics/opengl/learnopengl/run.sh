#!/bin/bash

# gcc -Wall -Wextra -O2 shaders.c glad/glad.c -lglfw -lGL -ldl
gcc -Wall -Wextra -O2 shaders.c glad/glad.c -lglfw -lm

./a.out