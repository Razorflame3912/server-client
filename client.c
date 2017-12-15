#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  
  printf("Client->Server FDS: %d\n",to_server);
  printf("Server->Client FDS:%d\n",from_server);
  
  char input[BUFFER_SIZE];
  while(1){
    
    printf("Input: ");
    scanf(" %[^\t\n]s",input);
    write(to_server,input,BUFFER_SIZE);
    
    int r = read(from_server,input,BUFFER_SIZE);

    char error[8];
    printf("Read errno? = %d\n", errno );
    printf("Output from server: %s\n",input);
    }
  
}
