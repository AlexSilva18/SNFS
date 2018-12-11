#include "server.h"
#include <dirent.h>

char curPath[500] = "./serverDir";
char* path = curPath;
char *success = "ok&";

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
  char* tempPath = (char*)malloc(strlen(inputPath)+strlen(path)+1);
  getPath(tempPath, inputPath);

  if(mkdir(tempPath, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1){
    fprintf(stderr, "[ERROR] Unable to Mkdir\n");
    char *failed = "fa&";
    printf("fail len: %zu\n", strlen(failed));
    if(writeToServer(socket_fd, failed, strlen(failed)) == -1){
      fprintf(stderr, "[ERROR] Failed to Write\n");
    }
    free(tempPath);
    return;

  }
  else{
      //success[4] = '&';
      printf("succ len: %zu\n", strlen(success));
      if(writeToServer(socket_fd, success, strlen(success)) == -1)
	fprintf(stderr, "[ERROR] Failed to Write\n");
      free(tempPath);
      return;
    }
}


void getPath(char* destPath, char* inputPath){
	char servDir[500];
	strcpy(servDir, path);
	
	if(inputPath[0] != '/'){
		char tempChar[2] = "/";
		char* temp = tempChar;
		inputPath = strcat(temp, inputPath);
	}
	strcat(servDir, inputPath);
	//strcpy(destPath, servDir);
	memcpy(destPath, servDir, strlen(servDir));
	//strcpy(destPath, strcat(servDir, inputPath));
}
