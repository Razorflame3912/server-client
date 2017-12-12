#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Perofrms the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  int wkp = mkfifo("server", O_RDONLY);
  char fifoname[256];
  read(wkp,fifoname,HANDSHAKE_BUFFER_SIZE);
  int private = open(fifoname,O_WRONLY);
  char * ack = ACK;
  remove("server");
  write(private,ack,HANDSHAKE_BUFFER_SIZE);
  printf("Connection established\n");
  
  return wkp;
}


/*=========================
  client_handshake
  args: int * to_server

  Perofrms the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char * fifoname = "private";
  int private = mkfifo(fifoname, 0666);
  int wkp = open("server",O_WRONLY);
  write(wkp,fifoname,HANDSHAKE_BUFFER_SIZE);
  char ack[256];
  read(private,ack,HANDSHAKE_BUFFER_SIZE);
  if(!strcmp(ack,ACK)){
    remove("private");
  }
  else{
    printf("Error: acknowledgement failed.\n");
    exit(0);
  }
  write(wkp,"Connected!",BUFFER_SIZE);
  printf("Connection established\n");
  return private;
}
