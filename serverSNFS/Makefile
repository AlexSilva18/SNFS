COMPILER = gcc
FLAGS = -Wall -Werror -g -fsanitize=address -lasan -lpthread
SERVER_FILES = server.c

default: server

server: $(SERVER_FILES)
	$(COMPILER) $(FLAGS) $(SERVER_FILES) -o serverSNFS `pkg-config fuse --cflags --libs`

clean:
	rm -rf *~ *.o .*.swp "#*.c#" serverSNFS
runserv:
	./serverSNFS -port 1462 -mount ./serverDir
