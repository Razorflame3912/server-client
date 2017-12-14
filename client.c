#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  printf("%d\n",to_server);
  printf("%d\n",from_server);
  while(1){
    char input[BUFFER_SIZE];
    printf("Input: ");
    scanf("%s",input);
    write(to_server,input,BUFFER_SIZE);
    char output[BUFFER_SIZE];
    read(from_server,output,BUFFER_SIZE);
    printf("Output from server: %s\n",output);
    }
  
}
