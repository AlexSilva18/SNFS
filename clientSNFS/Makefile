COMPILER = gcc
FLAGS = -Wall -Werror -g -fsanitize=address -lasan -lpthread
CLIENT_FILES = client.c

default: client

client: $(CLIENT_FILES)
	$(COMPILER) $(FLAGS) $(CLIENT_FILES) -o clientSNFS `pkg-config fuse --cflags --libs`

clean:
	rm -rf *~ *.o .*.swp "#*.c#" serverSNFS clientSNFS

runcli:
	./clientSNFS -port 1462 -hostname localhost -mount /tmp/hello -f
