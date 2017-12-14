#include "pipe_networking.h"


int main() {

  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );
  
  printf("Server->Client FDS: %d\n",to_client);
  printf("Client->Server FDS: %d\n",from_client);
  
  while(1){
    
    char clienttext[BUFFER_SIZE];
    read(from_client,clienttext,BUFFER_SIZE);
    printf("Received [%s] from client\n",clienttext);

    // Conversion to Uppercase
    int i = 0;
    while(clienttext[i]){
      clienttext[i] = toupper(clienttext[i]);
      i++;
    }
    
    printf("Processed: %s\n",clienttext);

    // Bad file descriptor?
    int w = write(to_client,clienttext,BUFFER_SIZE);
    
    printf("Write errno? = %d\n",w);
    printf("Wrote [%s] to pipe\n",clienttext);
  }
}
