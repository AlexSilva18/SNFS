/**
   Warren Ho | Alex Silva | Kimberly Russo
 **/

#include "server.h"
#include <dirent.h>


// GLOBAL VARS
char curPath[500] = "./serverDir";
// path is the directory where the user currently is
char* path = curPath;

int do_getattr(char *path, int socket_fd){
  //char *is_root = "/&";
  char* tempPath = (char*)malloc(500);//malloc(sizeof(inputPath)+sizeof(*path)+1);
  getPath(tempPath, path);
  strcat(tempPath, "&");
  /* char not_root[1024]; */
  /* strcpy(not_root, path); */
  /* strcat(not_root, "&"); */

  
  //printf("sending: %s\n", not_root);
  //if(writeToServer(socket_fd, not_root, strlen(not_root)) == -1)
  if(writeToServer(socket_fd, tempPath, strlen(tempPath)) == -1)
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
	printf("readdir tempPath: %s\n", tempPath);
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
			//printf("dir: %s\n", dir->d_name);
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
	
	struct stat path_stat;
	stat(tempPath, &path_stat);
	
	
	// IF IS FILE
	if(S_ISREG(path_stat.st_mode)){
		int fd = open(tempPath, O_RDWR|O_CREAT, 0666);
	
		bzero(buffer, strlen(buffer));
		pread(fd, buffer, size, offset);
	
		printf("ReadBuffer: %s\n", buffer);
		writeToServer(socket_fd, buffer, strlen(buffer));
		
		close(fd);
	}
	else{
		inputPath += 1;
		writeToServer(socket_fd, inputPath, strlen(inputPath));
	}	
	
	
	
	free(tempPath);
	return;
}



void do_write(char *inputPath, char *buffer, size_t size, off_t offset, int socket_fd){
	char* tempPath = (char*)malloc(500);

	getPath(tempPath, inputPath);
	
	struct stat path_stat;
	stat(tempPath, &path_stat);
	
	// IF IS FILE
	if(S_ISREG(path_stat.st_mode) != 0){
		int fd = open(tempPath, O_RDWR|O_CREAT, 0666);
	
		printf("BUFFER IN WRITE: %s\n", buffer);
		int nbytes = pwrite(fd, buffer, strlen(buffer)-offset, offset);
	
		printf("WriteBuffer: %s\n", buffer);
		char bytesWritten[10];
		sprintf(bytesWritten, "%d", nbytes);
		
		writeToServer(socket_fd, bytesWritten, strlen(bytesWritten));
		close(fd);
	}
	else{
		char errMsg[] = "error";
		writeToServer(socket_fd, errMsg, strlen(errMsg));
	}
	
	free(tempPath);
	return;
}

void do_truncate(char* inputPath, off_t offset){
	
	int fd = open(inputPath, O_TRUNC | O_WRONLY);
	ftruncate(fd, offset);
	close(fd);
	
}




/* void do_mkdir(char* inputPath, mode_t mode, int socket_fd){ */
void do_mkdir(char* inputPath, mode_t mode, int socket_fd){
  char* tempPath = (char*)malloc(500);
  //char *success = "ok&";
  //chdir(path);
  getPath(tempPath, inputPath);
  printf("mkdir tempPath: %s\n", tempPath);
  //printf("%s  %s\n", path,inputPath);
  /* char newPath[50] = "./"; */
  /* strcat(newPath, inputPath); */
  int retval = mkdir(tempPath, mode);
  char strRetval[10];
  sprintf(strRetval, "%d", retval);

  if(retval == -1)
    fprintf(stderr, "[ERROR] Unable to Mkdir\n");
  
  writeToServer(socket_fd, strRetval, strlen(strRetval));
  free(tempPath);
  /*  char *failed = "fa&"; */
  /*   printf("failed: %s len: %zu\n",failed, strlen(failed)); */
  /*   if(writeToServer(socket_fd, failed, strlen(failed)) == -1){ */
  /*     fprintf(stderr, "[ERROR] Failed to Write\n"); */
  /*   } */
  /*   free(tempPath); */
  /*   return; */

  /* } */
  /* else{ */
  /*     //success[4] = '&'; */
  /*   printf("succes: %s len: %zu\n",success, strlen(success)); */
  /*     if(writeToServer(socket_fd, success, strlen(success)) == -1) */
  /* 	fprintf(stderr, "[ERROR] Failed to Write\n"); */
  /*     free(tempPath); */
  /*     return; */
  /*   } */
}

void do_create(char* inputPath, mode_t mode, int socket_fd){
	char* tempPath = (char*)malloc(500);
	getPath(tempPath, inputPath);
	printf("create tempPath: %s\n", tempPath);
	int retval = creat(tempPath, mode);
	if(retval == -1)
	  fprintf(stderr, "[ERROR], File Could not be Created\n");
	
	char strRetval[10];
	sprintf(strRetval, "%d", retval);
	printf("strRetval: %s, RETVAL: %d\n",strRetval, retval);
	writeToServer(socket_fd, strRetval, strlen(strRetval));
	free(tempPath);
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

void checkDirExists(char* inputPath, int socket_fd){
	char* tempPath = (char*)malloc(500);
  getPath(tempPath, inputPath);

  //DIR* dir = opendir(tempPath);
  /* struct dirent*d; */

  /* if (dir){ */
  /*   while((d = readdir(dir))){ */
  /*     if(strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) */
  /* 	continue; */
  /*     printf("dir: %s\n", d->d_name); */
  /*     if(d->d_type == DT_REG){ */
  /* 	printf("FILE\n"); */
  /*     } */
  /*     else if (d->d_type == DT_DIR){ */
  /* 	printf("DIR\n"); */
  /*     } */
  /*   } */
  /* } */
  /* if(ENOENT == errno){ */
  /* 	errno = 0; */
  /* 	char ans[2] = "n"; */
  /* 	writeToServer(socket_fd, ans, strlen(ans)); */
  /* } */
  /* else{ */
	struct stat path_stat;
	stat(tempPath, &path_stat);
	if(S_ISREG(path_stat.st_mode) != 0){
	  char ans[3] = "yf";
	  printf("FILE\n");
	  writeToServer(socket_fd, ans, strlen(ans));
	}
	  
	else if(S_ISDIR(path_stat.st_mode) != 0){
	  char ans[3] = "yd";
	  printf("DIR\n");
	  writeToServer(socket_fd, ans, strlen(ans));
	  }
	  else{
	    char ans[2] = "n";
	    writeToServer(socket_fd, ans, strlen(ans));
	  }
	  
  /* 	closedir(dir); */
  /* } */
  free(tempPath);
  
}




