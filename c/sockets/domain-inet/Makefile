init:
	mkdir -p bin

server: init
	gcc -o bin/server server.c lib/read_line.c

client: init
	gcc -o bin/client client.c lib/read_line.c

my_server: init
	gcc -o bin/my_server my_server.c lib/read_line.c

chatgpt_server: init
	gcc chatgpt_server.c -o bin/chatgpt_server

chatgpt_client: init
	gcc chatgpt_client.c -o bin/chatgpt_client

run_server: server
	./bin/server

run_client: client
	./bin/client localhost

run_my_server: my_server
	./bin/my_server

run_chatgpt_server: chatgpt_server
	./bin/chatgpt_server

run_chatgpt_client: chatgpt_client
	./bin/chatgpt_client

clean:
	rm -rf bin