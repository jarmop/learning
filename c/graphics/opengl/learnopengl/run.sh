#!/bin/bash

gcc -Wall -Wextra transformations/transformations.c glad/glad.c lib/stb_image.c -lglfw -lm -lcglm
# gcc -Wall -Wextra textures/textures.c glad/glad.c lib/stb_image.c -lglfw -lm

./a.out