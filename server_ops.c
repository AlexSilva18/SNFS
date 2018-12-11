#include "server.h"
#include <dirent.h>

int do_getattr(char *path, int socket_fd){
  //char *is_root = "/&";
  char not_root[1024];
  strcpy(not_root, path);
  strcat(not_root, "&");
  
  //printf("sending: %s\n", not_root);
  if(writeToServer(socket_fd, not_root, strlen(not_root)) == -1)
    return -1;
  //char *temp = NULL;
  //int i = 0;
  
  /* for(temp = path; *temp != '\0'; temp += 1){ */
  /*   /\* printf("temp: %c\n", *temp); *\/ */
  /*   if (*temp == '/'){ */
  /*     /\* printf("== %d\n", i); *\/ */
  /*     i++; */
  /*   } */
  /* } */
  /* if(strcmp(path, "/") == 0){ */
  /*   if(writeToServer(socket_fd, is_root, strlen(is_root)) == -1){ */
  /*     return -1; */
  /*   } */
  /* } */
  /* else{ */
  /*   if(writeToServer(socket_fd, not_root, strlen(not_root)) == -1){ */
  /*     return -1;     */
  /*   } */
  /* } */
  
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

/* void do_mkdir(char* inputPath, mode_t mode, int socket_fd){ */
void do_mkdir(char* inputPath, int socket_fd){
  /* char *path = "/tmp/hello1"; */
  /* char* tempPath = (char*)malloc(sizeof(*inputPath)+sizeof(*path+1); */
  /* getPath(tempPath, inputPath); */
  /* char *conf = "mkdir&"; */
  /* if(writeToServer(socket_fd, conf, strlen(conf)) == -1) */
  /*     return -1; */
  
  /* struct fuse_data *data; */
  /* recvfrom(socket_fd, data, sizeof(*data), 0, (struct sockaddr*)&sin, sizeof(sin)); */
  /* printf("Reached here\n"); */
  /* int retval = mkdir(tempPath, mode); */
  /* if(retval == -1){ */
  /*   fprintf(stderr, "[ERROR] Unable to Mkdir\n"); */
  /*   if(writeToServer(socket_fd, not_root, strlen(not_root)) == -1) */
  /*     return -1; */
  /*   free(tempPath); */
  /*   return; */
  /* } */
  
  /* if(writeToServer(socket_fd, not_root, strlen(not_root)) == -1) */
  /*   return -1; */
  
  /* free(tempPath); */
}
