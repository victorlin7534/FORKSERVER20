#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  while(1){
    printf("send info to server: ");
    char input[100];
    fgets(input,100,stdin);
    strtok(input,"\n");
    write(to_server,input,100);
    
    char output[strlen(input)];
    read(from_server,output,strlen(input));
    printf("client: received <%s>\n",output);
  }
}
