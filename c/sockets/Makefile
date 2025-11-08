all: init stream_client stream_server

init:
	mkdir -p bin

stream_client:
	gcc -o bin/stream_client stream_client.c

stream_server:
	gcc -o bin/stream_server stream_server.c

clean:
	rm -rf bin