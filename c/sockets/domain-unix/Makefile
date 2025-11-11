all: init stream_client stream_server

init:
	mkdir -p bin

stream_server:
	gcc -o bin/stream_server stream_server.c

stream_client:
	gcc -o bin/stream_client stream_client.c

run_stream_server: init stream_server
	./bin/stream_server

run_stream_client: init stream_client
	./bin/stream_client

clean:
	rm -rf bin