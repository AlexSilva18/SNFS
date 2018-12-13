COMPILER = gcc
FLAGS = -Wall -Werror -g -fsanitize=address -lasan -lpthread
CLIENT_FILES = client.c
SERVER_FILES = server.c

default: client server

client: $(CLIENT_FILES)
	$(COMPILER) $(FLAGS) $(CLIENT_FILES) -o clientSNFS `pkg-config fuse --cflags --libs`
	@echo 'To run: ./clientSNFS -port [port_number] -hostname localhost -mount [mount_point] -f'
	@echo 'To unmount: fusermount -u /tmp/hello"'
	@echo 'Make sure you create a hello directory in /tmp'

server: $(SERVER_FILES)
	$(COMPILER) $(FLAGS) $(SERVER_FILES) -o serverSNFS `pkg-config fuse --cflags --libs`

clean:
	rm -rf *~ *.o .*.swp "#*.c#" serverSNFS clientSNFS

runserv:
	./serverSNFS -port 123262 -mount ./serverDir

runcli:
	./clientSNFS -port 123262 -hostname localhost -mount /tmp/hello20 -f

makedir:
	mkdir /tmp/hello

unm:
	fusermount -u /tmp/hello

unm1:
	fusermount -u /tmp/hello1
