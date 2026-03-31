#!/bin/bash

# gcc -Wall -Wextra camera/camera2.c glad/glad.c lib/stb_image.c -lglfw -lm -lcglm
gcc -Wall -Wextra camera/camera.c glad/glad.c lib/stb_image.c -lglfw -lm -lcglm

./a.out