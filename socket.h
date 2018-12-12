#ifndef SOCKET_H
#define SOCKET_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>

struct addrinfo hints, *res, *r;
int mode;
struct fuse_data {
  const char *path;
  const mode_t mode;
};

int socketInit(char*, char*);
int socketOpen(const char*);
int socketClose(int);
ssize_t socketRead(int, void*, size_t);
ssize_t socketWrite(int, const void*, size_t);
int connectToServer();
int writeToServer(int, char*, int);
int processError(int, char*, int);
int parseInt(int*, char*, int, int);


#endif
