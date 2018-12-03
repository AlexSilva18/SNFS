#ifndef SERVER_H
#define SERVER_H

#include "socket.h"
#include <pthread.h>
//#include "threads.h"

typedef struct {
  char *port;
  char *dirPath;
}inputStream;


int getFlag(char**, inputStream*);

#endif
