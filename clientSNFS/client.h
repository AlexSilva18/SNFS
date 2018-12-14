/**
   Warren Ho | Alex Silva | Kimberly Russo
 **/

#ifndef CLIENT_H
#define CLIENT_H

#include "client_fuse.c"
#include <string.h>
//#include "threads.h"

typedef struct {
  char *port;
  char *hostname;
  char *mountPath;
  
}inputStream;


int getFlag(char**, inputStream*);

#endif
