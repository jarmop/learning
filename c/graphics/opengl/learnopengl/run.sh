#!/bin/bash

# gcc -Wall -Wextra hello_triangle.c glad/glad.c lib/stb_image.c -lglfw -lm -lcglm
# gcc -Wall -Wextra hello_triangle_indexed.c glad/glad.c lib/stb_image.c -lglfw -lm -lcglm
# gcc -Wall -Wextra shaders/shaders.c glad/glad.c lib/stb_image.c -lglfw -lm -lcglm
# gcc -Wall -Wextra textures/textures.c glad/glad.c lib/stb_image.c -lglfw -lm -lcglm
# gcc -Wall -Wextra transformations/transformations.c glad/glad.c lib/stb_image.c -lglfw -lm -lcglm
# gcc -Wall -Wextra coordinate_systems/coordinate_systems.c glad/glad.c lib/stb_image.c -lglfw -lm -lcglm
# gcc -Wall -Wextra camera/camera.c glad/glad.c lib/stb_image.c -lglfw -lm -lcglm
# gcc -Wall -Wextra camera/camera2.c glad/glad.c lib/stb_image.c -lglfw -lm -lcglm
gcc -Wall -Wextra lighting/directional/directional.c glad/glad.c lib/stb_image.c -lglfw -lm -lcglm

./a.out