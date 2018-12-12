#include "client_fuse.h"
#include "socket.c"
#include <stdlib.h>


/* static struct file_info {
  const char *filename;
  const char *data;
  }finfo;
*/
extern int global_socket;
//static const char *hello_str = "Hello Worlds!\n";
//static const char *hello_path = "/tmp/hello1";

/** gets attributes from file
 * @param const char* path, path of the file which the system asked for its attributes
 * @param struct stat *st, holds the attributes of the file
 * @return 0 on success, -1 on failure
 */
static int client_getattr( const char *path, struct stat *st){
  printf("[getattr] path == %s\n", path);
  //printf("global_socket: %d\n", global_socket);
  if(strncmp(path, "/.Trash", 7) == 0){
      return -1;
  }
  
  char message[1024] = "g";
  strcat(message, path);
  int i = strlen(message);
  message[i] = '&';
  //printf("message %s, length: %zu\n", message, strlen(message));
  if(writeToServer(global_socket, message, strlen(message)) == -1){
    fprintf(stderr, "[ERROR], [getattr] unable to write to server\n");
    return -1;
  }

  char buff[5000];
  //ssize_t nbytes;
  /*nbytes = */read(global_socket, buff, 5000);
  char *temp = NULL;
  int j = 0;
  char server_path[1024];
  for(temp = buff; *temp != '&'; temp += 1){
    //printf("temp: %c\n", *temp);
    server_path[j] = *temp;
    j++;
  }
  
  // 
  int k;
  for(k = j; k < strlen(server_path); k++){
  	server_path[k] = '\0';
  }
  
  
  printf("SERVERPATH: %s\n", server_path);  

  //printf("\nnbytes = %zu\n", nbytes);
  //printf("server_path: %s\n", server_path);
  
  
  /* char buff[1024]; */
  /* int nbytes; */
  /* char *temp; */
  /* char mess[1024]; */
  /* nbytes = read(global_socket, buff, 50); */
  /* int i = 0; */
  /* for( temp = buff; *temp != '&'; temp+=1){ */
  /*   mess[i]= *temp; */
  /*   ++i; */
  /* } */
  /* mess[i+1] = '\0'; */
  /* printf("\nnbytes = %d,  %s\n", nbytes, mess); */
  /* printf("end\n"); */

  /* (void) path; */
  /* if (lstat(hello_path, st) == -1) */
  /*   return -errno; */

  /* if(strcmp(path, "/tmp")){ */
  /*   printf("FOUND TEMP\n"); */
  /* } */
  /* return 0; */
  
  int res = 0;
  
  st->st_uid = getuid(); // user ID of the process invoking the operation
  st->st_gid = getgid(); // group ID of the process invoking the operation
  st->st_atime = time( NULL ); // The last access of the file/directory is right now
  st->st_mtime = time( NULL ); // The last modification of the file/directory is right now
  
  memset(st, 0, sizeof(struct stat));
  
  
  if (strncmp(server_path, "/", strlen(path)) == 0 ){
  //if (server_path == '/'){
    st->st_mode = S_IFDIR | 0755; // sets file system, file type and permission bits
    st->st_nlink = 2;
    //printf("step1\n");
    //return 0;
  }
  else/* if (strcmp(server_path, hello_path) == 0)*/{
    st->st_mode = S_IFREG | 0644;
    st->st_nlink = 1;
    st->st_size = 1024;
    //printf("step2\n");
    //st->st_size = strlen(hello_str);
    return 0;
  }
  
  /*
  //else if (strcmp(path+1, hello_path) == 0){
  else
  	res = 0;
    //res = -ENOENT;
  */

  return res;
  
}

/** shows the files and directories that reside in a specific directory (ls)
 * @param const char*
 * @param void*
 * @param fuse_fill_dir_t
 * @param off_t
 * @param struct fuse_file_info*
 * @return 
 */ 



static int client_readdir( const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi ){
  printf("[readdir] path == %s\n", path);
  
  filler(buffer, ".", NULL, 0); // Current Directory
  filler(buffer, "..", NULL, 0); // Parent Directory
  
  if (strcmp( path, "/" ) == 0 ) {
  
		char message[1024] = "e";
		strcat(message, path);
		int i = strlen(message);
		message[i] = '&';
		if(writeToServer(global_socket, message, strlen(message)) == -1){
		  fprintf(stderr, "[ERROR], [getattr] unable to write to server\n");
		  return -1;
		}
		
		char buff[1024];
		read(global_socket, buff, 1024);
		printf("BUFFER: %s\n", buff);
		
		char* curChar = buff;
		
		while(*curChar != '&'){
			char dirName[100];
			char* dirChar = dirName;
			
			while(*curChar != '%'){
				*dirChar = *curChar;
				curChar++;
				dirChar++;
			}
			curChar++;
			filler(buffer, dirName, NULL, 0);
			bzero(dirName, 100);
		}
		
		
		/* (void) offset; */
		/* (void) fi; */
		/* DIR *dir = opendir(path); */
		/* struct dirent *read; */
		/* if (dir == NULL){ */
		/*   closedir(dir); */
		/*   return -errno; */
		/* } */
		/* while ((read = readdir(dir)) != NULL){ */
		/*   if (strcmp( path, "/" ) == 0 ) { */
		/*     //printf("step1"); */
		/*     if (strcmp(read->d_name, ".") == 0){ */
		/* 	filler(buffer, ".", NULL, 0); // Current Directory */
		/* 	continue; */
		/*     } */
		/*     else if (strcmp(read->d_name, "..") == 0){ */
		/* 	filler(buffer, "..", NULL, 0); // Parent Directory */
		/* 	continue; */
		/*   st.st_ino = read->d_ino; */
		/*   st.st_mode = read->d_type << 12; */
		/*   if(filler(buffer, read->d_name, &st, 0)) */
		/*     break; */
		/* } */
		// If the user is trying to show the files/directories of the root directory show the following
		/* char *message = "message!"; */
		/* if(writeToServer(global_socket, message, strlen(message)) == -1){ */
		/*   fprintf(stderr, "[ERROR], [getattr] unable to write to server"); */
		/*   return -1; */
		/* } */
		(void) offset;
		(void) fi;
  
  

  
  }
    
    //filler(buffer, "hello1", NULL, 0);
    /* filler( buffer, "file1", NULL, 0 ); */
    /* filler( buffer, "file2", NULL, 0 ); */
  //else
  //  return -ENOENT;
  /* closedir(dir); */
  printf("finish readdir\n");
  return 0;
}

/** reads chunk of data from file
 * @param const char*
 * @param char*
 * @param size_t
 * @param off_t
 * @param struct fuse_file_info*
 * @return
 */
 
static int client_read( const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi ){
  printf("[read] path == %s\n", path);
  
  // ADD "r" AND PATH TO MESSAGE
  char message[1024] = "r";
  char* curPos = message;
  strcat(message, path);
  int i = strlen(message);
  message[i] = '&';
  curPos = &message[i];
  curPos += 1;

	// ADD SIZE TO MESSAGE
  char strSize[10];
  sprintf(strSize, "%d", ((int)size));
  strcat(message, strSize);
  curPos += strlen(strSize);
  *curPos = '%';
  curPos += 1;

	// ADD OFFSET TO MESSAGE
  char strOffset[10];
  sprintf(strOffset, "%d", ((int)offset));
  strcat(message, strOffset);
  curPos += strlen(strOffset);
  *curPos = '%';
  curPos += 1;
  *curPos = '&';
  
  // message format for read: "r/path&size%offset%&"
  
  // WRITE MESSAGE TO SOCKET
  //printf("message %s, length: %zu\n", message, strlen(message));
  if(writeToServer(global_socket, message, strlen(message)) == -1){
    fprintf(stderr, "[ERROR], [getattr] unable to write to server\n");
    return -1;
  }
  
  // READ BACK FROM SOCKET
	char buff[1024];
	bzero(buff, 1024);
	read(global_socket, buff, size); 
	printf("BUFFER: %s\n", buff);
  memcpy(buffer, buff, strlen(buff));
  return strlen(buff) - offset;
}

static int client_write(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *fi){
  printf("[write] path == %s\n", path);
  printf("[buffer] = %s\n[size] = %zu\n[offset] = %zu\n", buffer, size, offset);
  
  // ADD "w" AND PATH TO MESSAGE
  char message[1024] = "w";
  char* curPos = message;
  strcat(message, path);
  int i = strlen(message);
  message[i] = '&';
  curPos = &message[i];
  curPos += 1;

	// ADD SIZE TO MESSAGE
  char strSize[10];
  sprintf(strSize, "%d", ((int)size));
  strcat(message, strSize);
  curPos += strlen(strSize);
  *curPos = '%';
  curPos += 1;

	// ADD OFFSET TO MESSAGE
  char strOffset[10];
  sprintf(strOffset, "%d", ((int)offset));
  strcat(message, strOffset);
  curPos += strlen(strOffset);
  *curPos = '%';
  curPos += 1;
  printf("OFFSET IS: %s\n", strOffset);
  
  // ADD STRING TO BE WRITTEN
  strcat(message, buffer);
  curPos += strlen(buffer);
  
  *curPos = '%';
  curPos += 1;
  *curPos = '&';
  
  // message format for write: "w/path&size%offset%stringToWrite%&"
  
  if(writeToServer(global_socket, message, strlen(message)) == -1){
    fprintf(stderr, "[ERROR], [getattr] unable to write to server\n");
    return -1;
  }
  
  // READ BACK FROM SOCKET
	char buff[1024];
	read(global_socket, buff, 1024); 
	printf("RETURN BUFFER bytesWritten: %s\n", buff);
	int bytesWritten;
	sscanf(buff, "%d", &bytesWritten);
  return bytesWritten;
}


static int client_open(const char *path, struct fuse_file_info *fi){
  printf("[open] path == %s\n", path);
  /*
  if (strcmp(path + 1, hello_path) != 0)
    return -ENOENT;
  
  if ((fi->flags & 3) != O_RDONLY)
    return -EACCES;
	*/
  return 0;
}

static int client_release(const char*path, struct fuse_file_info* fi){
	return 0;
}

static int client_truncate(const char* path, off_t offset){
	
	// ADD "t" AND PATH TO MESSAGE
  char message[1024] = "t";
  char* curPos = message;
  strcat(message, path);
  int i = strlen(message);
  message[i] = '&';
  curPos = &message[i];
  curPos += 1;
  
  // ADD OFFSET TO MESSAGE
  char strOffset[10];
  sprintf(strOffset, "%d", ((int)offset));
  strcat(message, strOffset);
  curPos += strlen(strOffset);
  *curPos = '%';
  curPos += 1;
  
  *curPos = '&';
  
  if(writeToServer(global_socket, message, strlen(message)) == -1){
    fprintf(stderr, "[ERROR], [getattr] unable to write to server\n");
    return -1;
  }
  
  return 0;
	
}














static int client_create(const char *path, mode_t mode, struct fuse_file_info *fi){
  printf("[create] path == %s\n", path);
  
  /* int ret; */
  /* if (S_ISREG(mode)){ */
  /*   ret = open(path, O_CREATE | O_EXCL | O_WRONLY, mode); */
  /*   if (ret >= 0) */
  /*     ret = close(ret); */
  /* } */
  /* else if (S_ISFIFO(mode)) */
  /*   ret = mkfifo(path, mode); */
  /* else */
  /*   ret = mknod(path, mode,  */
  
  if((fi->fh = open(path, fi->flags, mode)) == -1){
    printf("File Create Failed!\n");
    return -ENOSYS;
  }    
  
  return 0;
}


static int client_flush(const char *path, struct fuse_file_info *fi){
  printf("[flush] path == %s\n", path);
  return 0;
}
static int client_mkdir(const char *path, mode_t mode){
  printf("[mkdir] path == %s\n", path);
  /* struct fuse_data *data; */
  /* data = malloc(sizeof(struct fuse_data)); */
  /* data->path = path; */
  /* data->mode = mode; */


  /* char message[1024] = "m"; */
  /* strcat(message, path); */
  /* int i = strlen(message); */
  /* message[i] = '&'; */
  /* //printf("message %s, length: %zu\n", message, strlen(message)); */
  /* if(writeToServer(global_socket, message, strlen(message)) == -1){ */
  /*   fprintf(stderr, "[ERROR], [mkdir] unable to write to server\n"); */
  /*   return -1; */
  /* } */
  
  /* char buff[1024]; */
  /* //ssize_t nbytes; */
  /* read(global_socket, buff, 1024); */
  /* if(strncmp(buff, "mkdir&", 6)){ */
    
  /*   if(sendto(global_socket, data, sizeof(*data), 0, (struct sockaddr*) &sin, sizeof(sin))) == -1) */
  /*     fprintf(stderr, "[ERROR], couldn't send struct\n"); */
  /* } */
  /* char *temp = NULL; */
  /* int j = 0; */
  /* char server_path[1024]; */
  /* for(temp = buff; *temp != '&'; temp += 1){ */
  /*   //printf("temp: %c\n", *temp); */
  /*   server_path[j] = *temp; */
  /*   j++; */
  /* } */

  
  if(mkdir(path, mode) == -1){
    printf("MKDIR Failed!\n");
    return -ENOENT;
  }
  
  return 0;
}
/* static int client_truncate(const char*, off_t){ */
/*   return 0; */
/* } */
/* static int client_opendir(const char*, struct fuse_file_info*){ */
/*   return 0; */
/* } */
static int client_releasedir(const char *path, struct fuse_file_info *fi){
  //struct dir_data *dir = (struct dir_data*) (uintptr_t) fi->fh;
  (void) path;
  close(fi->fh);
  //closedir(dir->dp);
  return 0;
}


static struct fuse_operations fops = {
  .getattr = client_getattr,
  .readdir = client_readdir,
  .read = client_read,
  .create = client_create,
  .open = client_open,
  .write = client_write,
  .flush = client_flush,
  .mkdir = client_mkdir,
  /* .truncate = client_truncate, */
  /* .opendir = client_opendir, */
  .releasedir = client_releasedir,
  .release = client_release,
  .truncate = client_truncate,
};
