IMGUI_SOURCES = imgui/*.cpp imgui/backends/*.cpp

default: slow

build_with_imgui:
	g++ -Wall -Wextra \
	main.cpp glad/glad.c \
	 lib/stb_image.c \
	${IMGUI_SOURCES} \
	-Iimgui \
	-lglfw -lGL -ldl \

build_without_imgui:
	g++ -Wall -Wextra \
	main.cpp glad/glad.c \
	 lib/stb_image.c \
	-lglfw -lGL -ldl \
	-DIMGUI_DISABLED \

run:
	./a.out

slow: build_with_imgui run

fast: build_without_imgui	run