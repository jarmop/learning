# gcc -Wall -Wextra camera/camera.c glad/glad.c lib/stb_image.c -lglfw -lm -lcglm

rm -rf a.out

g++ -Wall -Wextra terrain1.cpp glad/glad.c lib/stb_image.c -lglfw -lm -lcglm 

#  -lGL -ldl

./a.out
