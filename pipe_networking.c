#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
void server_request(char *to_subserver,int *from_client){
  mkfifo("request",0644);

  printf("server: waiting for server requests\n");
  *from_client = open("request",O_RDONLY);
  read(*from_client,to_subserver,12);
  remove("request");
}

void server_acknowledge(char * client_data,int *to_client,int *from_client){
  int ack = open(client_data,O_WRONLY);
  *to_client = ack;
  printf("sub-server [%d]: request acknowledged\n",getpid());
  write(ack,"request acknowledged",21);

  char final[38];
  read(*from_client,final,38);
  printf("sub-server [%d]: %s\n",getpid(),final);
}

/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  mkfifo("acknowledge",0644);

  int req = open("request",O_WRONLY);
  *to_server = req;
  printf("client: sending request\n");
  write(req,"acknowledge",12);

  int ack = open("acknowledge",O_RDONLY);
  char acknowledgement[21];
  read(ack,acknowledgement,21);
  printf("client: received acknowledgement\n");
  remove("acknowledge");

  write(req,"received acknowledgement confirmation",38);
  printf("client: sending confirmation\n");

  return ack;
}
