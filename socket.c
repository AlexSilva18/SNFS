#include "socket.h"

struct addrinfo *res;
int global_socket = 0;

int socketInit(char *hostname, char *port){
  int rc;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  printf("Parameters have been Set\n");
  rc = getaddrinfo(hostname, port, &hints, &res);

  if(rc != 0){
    fprintf(stderr, "[ERROR]: GAI: Return code %d: %s\n", rc, gai_strerror(rc));
    return -1;
  }
  return 0;
}

int socketOpen(const char *pathname){
  int nbytes;
  //int fd = 0;
  int socket_fd;
  char buff[50];

  if((socket_fd = connectToServer()) == -1)
    return -1;
  
  global_socket = socket_fd;
  printf("global_socket: %d\n", global_socket);
  char *message = "hello!";
  if(writeToServer(socket_fd, message, strlen(message)) == -1)
    return -1;

  char *temp;
  char mess[20];
  nbytes = read(socket_fd, buff, 50);
  int i = 0;
  for( temp = buff; *temp != '&'; temp+=1){
    mess[i]= *temp;
    ++i;
  }
  printf("\nnbytes = %d,  %s\n", nbytes, mess);
  printf("end\n");
  /* char string[24 + strlen(pathname)]; */
  /* sprintf(string, "open %d %d %s %d\n", mode, (int)strlen(pathname), pathname, O_RDWR); */
  /* if(writeToServer(socket_fd, string, strlen(string)) == -1) */
  /*   return -1; */
  
  /* nbytes = read(socket_fd, buff, 50); */
  
  /* if(buff[0] == 'e'){ */
  /*   return processError(socket_fd, buff, nbytes); */
  /* } */

  /* int i; */
  /* int state = 0; */
  /* do{ */
  /*   buff[nbytes] = '\0'; */
  /*   for(i = 0; i < nbytes; i++){ */
  /*     if(state == 0){ */
  /* 	if(isdigit(buff[i])) */
  /* 	  state = 1; */
  /*     } */
  /*     if(state == 1){ */
  /* 	state = parseInt(&fd, buff, i, state); */
  /*     } */
  /*     if(buff[i] == EOF){ */
  /* 	break; */
  /*     } */
  /*   } */
  /*   if(buff[i] == EOF) */
  /*     break; */
  /* } while((nbytes = read(socket_fd, buff, 50))); */
  /* close(socket_fd); */
  /* return -fd; */
  return -socket_fd;
}

int socketClose(int fd){
  int nbytes;
  int socket_fd;
  char buff[50];

  if((socket_fd = connectToServer()) == -1)
    return -1;

  char string[6 + sizeof(fd)];
  sprintf(string, "close %d\n", (-fd));
  if(writeToServer(socket_fd, string, strlen(string)) == -1)
    return -1;
  
  nbytes = read(socket_fd, buff, 50);
  
  if(buff[0] == 'e')
    return processError(socket_fd, buff, nbytes);

  while((nbytes = read(socket_fd, buff, 50)));
  close(socket_fd);
  return 0;
}


int connectToServer(){
  struct addrinfores;
  int return_fd;
  for (r = res; r != NULL; r = r->ai_next){
    return_fd = socket(r->ai_family, r->ai_socktype, 0);
    
    if(return_fd == -1)
      continue;
    
    if(connect(return_fd, r->ai_addr, r->ai_addrlen) == -1){
      close(return_fd);
      continue;
    }
    return return_fd;
  }
  return -1;
}

int writeToServer(int socket_fd, char *str, int len){
  if(write(socket_fd, str, len) == -1){
    fprintf(stderr, "[ERROR]: Could not write to socket\n");
    close(socket_fd);
    return -1;
  }
  //shutdown(socket_fd, SHUT_WR);
  return 0;
}


int processError(int socket_fd, char *buff, int nbytes){
  int i;
  errno = 0;
  buff[nbytes] = '\0';
  do{
    for(i = 0; i < nbytes; i++){
      if(isdigit(buff[i])){
	errno = errno * 10 + (buff[i] - '0');
      }
    }
  }while((nbytes = read(socket_fd, buff, 50)));
  return -1;
}

int parseInt(int *currVal, char *buff, int i, int state){
  if(isdigit(buff[i])){
    *currVal = *currVal * 10 + (buff[i] - '0');
    return state;
  }
  return state += 1;
}
