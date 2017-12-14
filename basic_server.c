#include "pipe_networking.h"


int main() {

  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );
  printf("%d\n",to_client);
  printf("%d\n",from_client);
  while(1){
    char clienttext[BUFFER_SIZE];
    read(from_client,clienttext,BUFFER_SIZE);
    printf("received [%s] from client\n",clienttext);
    int i = 0;
    while(clienttext[i]){
      clienttext[i] = toupper(clienttext[i]);
      i++;
    }
    printf("processed: %s\n",clienttext);
    int w = write(to_client,clienttext,BUFFER_SIZE);
    printf("%d\n",w);
    printf("wrote[%s] to pipe\n",clienttext);
  }
}
