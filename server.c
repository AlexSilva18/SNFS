#include "server.h"
#include "socket.c"
#include "server_ops.c"
#include <pthread.h>

int main(int argc, char* argv[]){
  int socket_fd;
  struct addrinfo hints, *res, *r;
  struct sockaddr_storage client_addr;
  struct threadinput *input;
  socklen_t client_addr_len;
  int rc, err, connection_id;
  char buff[50];
  pthread_t tid;
  
  //char buff[1024];
  
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
    /* printf("Connection has been made\n"); */
    /* read(connection_id, buff, 50); */
    /* printf("nbytes: %zu, %s\n", strlen(buff), buff); */
    /* char string[30]; */
    /* char nullb = '&'; */
    /* sprintf(string, "confirmed message: %s%c",buff, nullb);  */
    /* if(writeToServer(connection_id, string, strlen(string)) == -1) */
    /*   return -1; */
    input = malloc(sizeof(struct threadinput));
    input->clientaddr = client_addr;
    input->socket_fd = connection_id;

    //testing(connection_id);
    printf("New Thread Created\n");
    pthread_create(&tid, NULL, threadInit, input);
    printf("Detach\n");
    pthread_detach(tid);
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

void * threadInit(void * args){
  int socket_fd = ((struct threadinput*) args)->socket_fd;
  printf("socket_fd: %d\n", socket_fd);
  printf("Child Thread\n\n");
  char buff[1024];
  //char *buff = (char*)malloc(sizeof(char)*1024);
  while(1){
    printf("Awaiting new instructions\n");
    ssize_t nbytes;
    bzero(buff, 1024);
    if((nbytes = read(socket_fd, buff, 1024)) == 0){
      fprintf(stderr, "[Error], Unable to read\n");
      //return;
    }
  printf("nbytes: %zu, %s\n", strlen(buff), buff);

  char *temp = NULL;
  char operation = buff[0];
  printf("operation: %c\n", operation);
  char path[1024];
  bzero(path, 1024);
  int i = 0;
  
  for(temp = buff; *temp != '&'; temp += 1){
    if (i ==0){
      i++;
      continue;
    }
    //printf("temp: %c\n", *temp);
    path[i-1] = *temp;
    //printf("path: %c\n", *path);
    i++;
  }


  printf("path: %s\n\n", path);
  
  switch(operation){
  case 'g':
  	printf("do_getattr() called\n");
    if (do_getattr(path, socket_fd) == -1)
      fprintf(stderr, "[ERROR] Unable to get_attr");
    memset(path, 0, sizeof(path));
    break;
    
  case 'e':
  	printf("do_readdir() called\n");
    do_readdir(path, socket_fd);
    break;
    
  case 'r':
  	printf("do_read() called\n");
		// CODE TO FINISH READING BUFFER FROM SOCKET  	
  	// empty statement needed
  	char* curPos = buff;
  	
  	while(*curPos != '&')
  		curPos++;
  		
  	curPos++;
  	char strSize[10];
  	int i = 0;
  	
  	while(*curPos != '%'){
  		strSize[i] = *curPos;
  		i++;
  		curPos++;
  	}
  	
  	int size;
  	sscanf(strSize, "%d", &size);
  	curPos++;
  	
  	char strOffset[10];
  	i = 0;
  	while(*curPos != '%'){
  		strOffset[i] = *curPos;
  		i++;
  		curPos++;
  	}
  	
  	int offset;
  	sscanf(strOffset, "%d", &offset);
  	
  	printf("PATH: %s\n", path);
  	printf("SIZE: %s\n", strSize);
  	printf("OFFSET: %s\n", strOffset);
  	
  	char readBuffer[5000];
  	
    do_read(path, readBuffer, size, offset, socket_fd); //5 should be size
    break;
    
  case 'c':
    // do_create
    break;
  case 'o':
    // do_open
    do_open(path);
    break;
  case 'w':
    // do_write
    break;
  case 'f':
    // do_flush
    break;
  case 'm':
    do_mkdir(path, socket_fd);
    break;
  case 't':
    // do_trunkate
    break;
  case 'p':
    // do_opendir
    break;
  case 'l':
    // do_releasedir
    break;
  default:
    fprintf(stderr, "[ERROR]: Invalid Command.\n");
  }

  printf("Resetting\n");
  memset(buff, 0, strlen(buff));
  /* strcpy(path, ""); */
  /* strcpy(buff, ""); */
  }
  return NULL;
  /* char string[30]; */
  /* char nullb = '&'; */
  /* sprintf(string, "conf: %s%c",buff, nullb); */
  /* if(writeToServer(connection_id, string, strlen(string)) == -1) */
  /*   return -1; */
}
