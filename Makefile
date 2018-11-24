CC = gcc
FLAGS = -Wall -Werror -g -fsanitize=address -lasan -lpthread -c -o
COMPILE = $(CC) $(FLAGS)
HEADER = client.h server.h
SERVOBJ = server.o
CLIENTOBJ = client.o

default: client server

server: $(SERVOBJ) Makefile $(HEADER)
	$(COMPILE:-c=) serverSNFS $(SERVOBJ)

client: $(CLIENTOBJ) Makefile $(HEADER)
	$(COMPILE:-c=) clientSNFS $(CLIENTOBJ)

#libnetfiles.o: libnetfiles.c

%.o: %.c $(HEADER)
	$(COMPILE) $@ $<

clean:
	rm -rf *~ *.o .*.swp "#*.c#" serverSNFS clientSNFS

runserv:
	./serverSNFS

runcli:
	./clientSNFS #remainder arguments
