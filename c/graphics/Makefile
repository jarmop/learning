build-gtk-test:
	gcc gtk-test.c $(shell pkg-config --cflags --libs gtk4) -o bin/gtk-test
run-gtk-test: build-gtk-test
	./bin/gtk-test

build-hello-world:
	gcc hello-world-gtk.c $(shell pkg-config --cflags --libs gtk4) -o bin/hello-world-gtk
run-hello-world: build-hello-world
	./bin/hello-world-gtk

make clean:
	rm -rf bin