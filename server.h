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

int getFlag(char**, inputStream*);
void *threadInit(void *);
int do_getattr(char*, int);
void do_readdir(char*, int);
void do_mkdir(char*, int);
/* void do_mkdir(char*, mode_t, int); */

#endif
