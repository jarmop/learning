default: run-hello-world

prepare:
	mkdir -p bin

build-gtk-test: prepare
	gcc gtk-test.c $(shell pkg-config --cflags --libs gtk4) -o bin/gtk-test
run-gtk-test: build-gtk-test
	./bin/gtk-test

build-hello-world: prepare
	gcc hello-world-gtk.c $(shell pkg-config --cflags --libs gtk4) -o bin/hello-world-gtk
run-hello-world: build-hello-world
	./bin/hello-world-gtk

make clean:
	rm -rf bin