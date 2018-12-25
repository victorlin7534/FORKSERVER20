#include "pipe_networking.h"

static void sighandler(int sig){
	if(sig == SIGINT){
		remove("request");
		exit(0);
	}
}

int main() {
  signal(SIGINT,sighandler);
  int to_client, from_client;
  while(1){
  	char * to_subserver; 
  	server_request(to_subserver,&from_client);
  	if(!fork()){
  		server_acknowledge(to_subserver,&to_client,&from_client);
  		char input[100];
		while(read(from_client,input,100)){
		    printf("sub-server [%d]: received <%s>\n",getpid(),input);
		    char output[strlen(input)];
		    for(int i=0;i<strlen(input);i++)
		    	output[i] = input[strlen(input)-i-1];
		    output[strlen(input)] = '\0';
		    write(to_client,output,strlen(input));
		}
  	}
  }
}
