COMPILER = gcc
FLAGS = -Wall -Werror -g -fsanitize=address -lasan -lpthread
CLIENT_FILES = client.c
SERVER_FILES = server.c

default: client server

client: $(CLIENT_FILES)
	$(COMPILER) $(FLAGS) $(CLIENT_FILES) -o clientSNFS `pkg-config fuse --cflags --libs`
	@echo 'To run: ./clientSNFS -f /tmp/hello'
	@echo 'To unmount: fusermount -u /tmp/hello"'
	@echo 'Make sure you create a hello directory in /tmp'

server: $(SERVER_FILES)
	$(COMPILER) $(FLAGS) $(SERVER_FILES) -o serverSNFS `pkg-config fuse --cflags --libs`

clean:
	rm -rf *~ *.o .*.swp "#*.c#" serverSNFS clientSNFS

runserv:
	./serverSNFS -port 55155 -mount ./serverDir

runcli:
	./clientSNFS -port 55155 -hostname localhost -mount /tmp/hello15 -f

unm:
	fusermount -u /tmp/hello

unm1:
	fusermount -u /tmp/hello1
