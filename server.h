#ifndef SERVER_H
#define SERVER_H

#include "socket.h"
#include <pthread.h>
//#include "threads.h"

typedef struct {
  char *port;
  char *dirPath;
}inputStream;

struct threadinput{
  struct sockaddr_storage clientaddr;
  int socket_fd;
};

void *threadInit(void *);

int getFlag(char**, inputStream*);

#endif
