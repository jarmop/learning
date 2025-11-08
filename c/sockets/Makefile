all: init stream_client stream_server

init:
	mkdir -p bin

stream_client: stream_client.c lib/my_error_functions.c
	gcc -o bin/stream_client stream_client.c lib/my_error_functions.c

stream_server: stream_server.c lib/my_error_functions.c
	gcc -o bin/stream_server stream_server.c lib/my_error_functions.c

clean:
	rm -rf bin