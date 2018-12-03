/* #include "socket.c" */
/* #include "client_fuse.c" */
#include "client.h"

int main( int argc, char *argv[] ){  
  int fuse_argc = 1;
  char *fuse_argv[60];
  //char **temp = (char**)malloc(sizeof(char*)*sizeof(argv));
  
  if(argc == 7 || argc == 8){
    fuse_argv[0] = argv[0];
    if (argv[7] != NULL && strcmp(argv[7], "-f")){
      fuse_argv[1] = argv[7];
      fuse_argv[2] = argv[6];
      fuse_argc = 3;
     }
     else{
       fuse_argv[1] = argv[6];
       fuse_argc++;
     }
  }
  else{
    fprintf(stderr, "[ERROR]: Invalid number of arguments.\n");
    return 0;
  }
  //char *pathname = argv[4];
  
  if(socketInit(argv[4], argv[2]) < 0){
    fprintf(stderr, "[ERROR]: Unable to initialize connection\n");
    return 0;
  }
  printf("Socket Init Completed\n");

  int socket_fd;
  if((socket_fd = connectToServer()) == -1)
    return -1;

  global_socket = socket_fd;
  
  /* int fd = socketOpen(pathname); */
  /* if(fd == -1){ */
  /*   fprintf(stderr, "[ERROR]: File Descriptor not found\n"); */
  /*   return 0; */
  /* } */
  /* char buff[501]; */
  /* int bytes_read; */
  /* bytes_read = socketRead(fd, buff, 50); */
  /* printf("Read: %d bytes, %s\n", bytes_read, buff); */
  /* int bytes_written; */
  /* bytes_written = socketWrite(fd, buff, bytes_read); */
  /* printf("Written: %d bytes, %s\n", bytes_written, buff); */
  /* int bytes_read; */
  /* bytes_read = socketRead(fd, buff, 50); */
  /* printf("Read: %d bytes, %s\n", bytes_read, buff); */
  /* int bytes_written; */
  /* bytes_written = socketWrite(fd, buff, bytes_read); */
  /* printf("Written: %d bytes, %s\n", bytes_written, buff); */
  /* if(socketClose(fd) == -1){ */
  /*   fprintf(stderr, "[ERROR]: Unable to close file\n"); */
  /*   return 0; */
  /* } */
  printf("SUCCESS\n");
  //struct fuse_args args = FUSE_ARGS_INIT(fuse_argc, fuse_argv);
  
  return fuse_main(fuse_argc, fuse_argv, &fops, NULL);
}

int getFlag(char* inputFlags[], inputStream *stream){
  int i = 0;
  
  // check each flag and make copy it to execution stream struct
  if (strcmp(inputFlags[1], "-port") == 0){
    ++i;
    const char *s = inputFlags[2];
    while (*s){
      if(isdigit(*s++) != 0)
	continue;
      else
        return -1;
    }
    ++i;    
    if (strcmp(inputFlags[3], "-hostname") == 0){
      /* stream->hostname = inputFlags[4]; */
      if (strcmp(inputFlags[4], "localhost") == 0){
	++i;
	if (strcmp(inputFlags[5], "-mount") == 0){
	  /* stream->mountPath = inputFlags[6]; */
	  if (strcmp(inputFlags[6], "/tmp/hello") == 0)
	    ++i;
	}
      }       
    }
  }  
  if (i == 6 || i == 7)
    return 0;
  return -1;
}
