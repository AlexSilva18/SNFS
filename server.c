#include "server.h"
#include "socket.c"
#include <pthread.h>


int main(int argc, char* argv[]){
  int socket_fd;
  struct addrinfo hints, *res, *r;
  struct sockaddr_storage client_addr;
  socklen_t client_addr_len;
  int rc, err, connection_id;
  
  char buff[1024];
  
  if(argc != 5){
    fprintf(stderr, "[ERROR]: Invalid number of arguments.\n");
    return 0;
  }
  
  inputStream *stream = NULL;
  if(getFlag(argv, stream) == -1){
    fprintf(stderr, "[EROR] Invalid Arguments\n");
    return -1;
  }
  
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  
  rc = getaddrinfo(NULL, argv[2], &hints, &res);
  if (rc != 0){
    fprintf(stderr, "Return code %d: %s\n", rc, gai_strerror(rc));
    return 1;
  }
  
  for(r = res; r != NULL; r = r->ai_next){
    socket_fd = socket(r->ai_family, r->ai_socktype, 0);
    
    if(socket_fd == -1){
      fprintf(stderr, "[ERROR]: Socket not found\n");
      return 0;
    }
    printf("Found Socket, fd: %d\n", socket_fd);
    
    if((bind(socket_fd, r->ai_addr, r->ai_addrlen)) < 0){
      fprintf(stderr, "[ERROR]: Could not bind\n");
      close(socket_fd);
      return 0;
    }
    else{
      printf("SUCCESS SERVER\n");
      break;
    }
    close(socket_fd);
  }
  
  if((err = listen(socket_fd, 2) == -1)){
    fprintf(stderr, "[ERROR]: Listen Failed\n");
    return 0;
  }

  while(1){
    printf("Waiting for connection\n");
    client_addr_len = sizeof(struct sockaddr_storage);
    connection_id = accept(socket_fd, (struct sockaddr *) &client_addr, &client_addr_len);

    if (connection_id == -1){
      fprintf(stderr, "[ERROR]: Connection Accept Failed\n");
      return 0;
    }
    else
      memset(buff, 0, sizeof(buff));
    printf("Connection has been made\n");
    read(connection_id, buff, 50);
    printf("nbytes: %zu, %s\n", strlen(buff), buff);
    char string[30];
    char nullb = '&';
    sprintf(string, "confirmed message: %s%c",buff, nullb); 
    if(writeToServer(connection_id, string, strlen(string)) == -1)
      return -1;
  }
  close(socket_fd); 


  
  return 0;
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
    if (strcmp(inputFlags[3], "-mount") == 0){
      ++i;
      //strcpy(stream->dirPath, inputFlags[4]);
      if (strcmp(inputFlags[4], "./serverDir") == 0)
	++i;
    }
  }  
  if (i != 4)
    return -1;
  return 0;
}
