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
  // printf("%d\n",fif);
  
  printf("Waiting for client...\n");
  int wkp = open("serverf",O_RDONLY,0);
  char fifoname[HANDSHAKE_BUFFER_SIZE];
  read(wkp,fifoname,sizeof(fifoname));
  printf("Received private fifo... %s\n",fifoname);

  // POTENTIALLY PROBLEMATIC RDWR - Should work with WRONLY but when set to RDWR, the server actually loops.  With WRONLY, it only loops once.
  *to_client = open(fifoname,O_WRONLY,0);
  char * ack = ACK;
  remove("serverf");
  printf("Sending acknowledgement message...\n");
  write(*to_client,ack,HANDSHAKE_BUFFER_SIZE);
  char est[BUFFER_SIZE];
  read(wkp,est,BUFFER_SIZE);
  printf("Client says: %s\n",est);
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
  // printf("%d\n",fif);
  
  printf("Opening to_server...\n");
  *to_server = open("serverf",O_WRONLY);

  printf("Sending fifoname...\n");
  write(*to_server,"privatef",HANDSHAKE_BUFFER_SIZE);

  printf("Opening private fifo...\n");  
  int private = open("privatef",O_RDONLY);
  printf( "Finished opening private fifo.\n" );
  
  char ack[256];
  read(private,ack,HANDSHAKE_BUFFER_SIZE);
  printf("Acknowledgement message: %s\n",ack);
  if(!strcmp(ack,ACK)){
    remove("privatef");
  }
  else{
    printf("Error: acknowledgement failed.\n");
    exit(0);
  }
  write(*to_server,"Connected!",BUFFER_SIZE);
  printf("Connection established\n");
  
  return private;
}
