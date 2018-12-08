#include "server.h"

void do_getattr(char *path){
  DIR *d;
  
  struct dirent *dir;
  
  d = opendir("./serverDir");
  
  if (d){
    while ((dir = readdir(d)) != NULL){
      printf("%s\n", dir->d_name);
    }
    closedir(d);
  }
  
  return(0);
}
