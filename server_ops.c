#include "server.h"
#include <dirent.h>

int do_getattr(char *path, int socket_fd){
  char *is_root = "/&";
  char *not_root = "&";
  
  char *temp = NULL;
  int i = 0;
  
  for(temp = path; *temp != '\0'; temp += 1){
    /* printf("temp: %c\n", *temp); */
    if (*temp == '/'){
      /* printf("== %d\n", i); */
      i++;
    }
  }
  if(i == 1){
    if(writeToServer(socket_fd, is_root, strlen(is_root)) == -1){
      return -1;
    }
  }
  else{
    if(writeToServer(socket_fd, not_root, strlen(not_root)) == -1){
      return -1;    
    }
  }
  
  return 0;
}

void do_readdir(char *path, int connection_id){
  //char *serverPath = "/tmp/serverDir";

  DIR *d;  
  struct dirent *dir;
  
  d = opendir("./serverDir");
  
  if (d){
    while ((dir = readdir(d)) != NULL){
      if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
	continue;
      printf("%s\n", dir->d_name);
    }
    closedir(d);
  }
  else{	
    fprintf(stderr, "[ERROR] Invalid path\n");
  }
  
  return;
  
}
