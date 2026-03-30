#!/bin/bash

# gcc -Wall -Wextra coordinate_systems/coordinate_systems.c glad/glad.c lib/stb_image.c -lglfw -lm -lcglm
gcc -Wall -Wextra camera/camera.c glad/glad.c lib/stb_image.c -lglfw -lm -lcglm

./a.out