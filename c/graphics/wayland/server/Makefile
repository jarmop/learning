default: run

build:
	@gcc src/client.c src/xdg-shell-protocol.c $(shell pkg-config --cflags --libs wayland-client) -o client

run: build
	@./client