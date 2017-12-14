#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Perofrms the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int * to_client) {
  unlink("serverf");
  int fif = mkfifo("serverf", 0666);
  printf("%d\n",fif);
  
  printf("waiting for client...\n");
  int wkp = open("serverf",O_RDONLY);
  char fifoname[256];
  read(wkp,fifoname,HANDSHAKE_BUFFER_SIZE);
  close(wkp);
  printf("received private fifo...\n");
  
  to_client = open(fifoname,O_WRONLY);
  char * ack = ACK;
  remove("serverf");
  write(*to_client,ack,HANDSHAKE_BUFFER_SIZE);
  close(*to_client);
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
int client_handshake(int * to_server) {
  
  unlink("privatef");
  int fif = mkfifo("privatef", 0666);
  printf("%d\n",fif);
  
  printf("opening private fifo...\n");  
  int private = open("privatef",O_RDONLY);
  printf( "Finished opening private fifo.\n" );
  
  printf("opening to_server...\n");
  to_server = open("serverf",O_WRONLY);
  
  printf("sending fifoname...\n");
  write(*to_server,"privatef",HANDSHAKE_BUFFER_SIZE);
  close(*to_server);
  
  char ack[256];
  read(private,ack,HANDSHAKE_BUFFER_SIZE);
  close(private);
  if(!strcmp(ack,ACK)){
    remove("privatef");
  }
  else{
    printf("Error: acknowledgement failed.\n");
    exit(0);
  }
  to_server = open("serverf",O_WRONLY);
  write(*to_server,"Connected!",BUFFER_SIZE);
  close(*to_server);
  printf("Connection established\n");
  
  return private;
}
