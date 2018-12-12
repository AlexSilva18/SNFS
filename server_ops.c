#include "server.h"
#include <dirent.h>


// GLOBAL VARS
char curPath[500] = "./serverDir";
// path is the directory where the user currently is
char* path = curPath;

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

void do_readdir(char *inputPath, int socket_fd){
  //char *serverPath = "/tmp/serverDir";

	char* tempPath = (char*)malloc(500);//malloc(sizeof(inputPath)+sizeof(*path)+1);
	getPath(tempPath, inputPath);
	
	DIR* d;
	struct dirent* dir;
	d = opendir(tempPath);
	
	
	char* listDirs = (char*)malloc(sizeof(char)*500);
	char* curPos = listDirs;
	
	if(d){
		char* curName;
		char* tempPtr;
		int i;
		while((dir = readdir(d)) != NULL){
			char dirName[500];
			strcpy(dirName, dir->d_name);
			curName = dir->d_name;
			tempPtr = curName;
			
			for(i = 0; i < strlen(curName); i++){
				strncpy(curPos, tempPtr, 1);
				curPos++;
				tempPtr++;
			}
			*curPos = '%';
			curPos++; 
		}
		*curPos = '&';
		printf("listDirs: %s\n", listDirs);
		writeToServer(socket_fd, listDirs, strlen(listDirs));
		
		closedir(d);
	}
	
	free(tempPath);
	free(listDirs);
	
}

void do_open(char* inputPath){
	//int fd = open(inputPath, O_RDWR|O_CREAT, 0666); //FLAGS = O_RDWR|O_CREAT
	return;
}


// USE ssize_t pread(int fd, void* buf, size_t size, off_t offset);
void do_read(char *inputPath, char *buffer, size_t size, off_t offset, int socket_fd){
	char* tempPath = (char*)malloc(500);

	getPath(tempPath, inputPath);

	int fd = open(tempPath, O_RDWR|O_CREAT, 0666);
	
	pread(fd, buffer, size, offset);
	
	printf("ReadBuffer: %s\n", buffer);
	writeToServer(socket_fd, buffer, strlen(buffer));
	
	close(fd);
	
	free(tempPath);
	return;
}

/*
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
*/




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

/*
void do_readdir(char* inputPath, int socket_fd){

	char* tempPath = (char*)malloc(sizeof(*inputPath)+sizeof(*path)+1);
	getPath(tempPath, inputPath);
	
	DIR* d;
	struct dirent* dir;
	d = opendir(tempPath);
	
	if(d){
		while((dir = readdir(d)) != NULL){
		
			// write back to socket here
			
			printf("%s\n", dir->d_name);
		}
		closedir(d);
	}
	
	free(tempPath);
}
*/





// CONCATENATES USER INPUT TO CURRENT PATH
// EX) "./serverDir/newFile.txt"
void getPath(char* destPath, char* inputPath){
	char servDir[500];
	strcpy(servDir, path);
	
	if(inputPath[0] != '/'){
		char tempChar[2] = "/";
		char* temp = tempChar;
		inputPath = strcat(temp, inputPath);
	}
	
	strcpy(destPath, strcat(servDir, inputPath));
}
