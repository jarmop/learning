init:
	mkdir -p bin

server: init
	gcc -o bin/server server.c lib/read_line.c

client: init
	gcc -o bin/client client.c lib/read_line.c

run_server: server
	./bin/server

run_client: client
	./bin/client localhost

clean:
	rm -rf bin