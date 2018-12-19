#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  mkfifo("request",0644);
  printf("server: waiting for server requests\n");

  int req = open("request",O_RDONLY);
  char *client_data;
  read(req,client_data,100);
  remove("request");

  int ack = open(client_data,O_WRONLY);
  char *ack_data = "request acknowledged";
  printf("server: %s\n",ack_data);
  write(ack,ack_data,strlen(ack_data));
  *to_client = ack;

  char *final;
  read(req,final,100);
  printf("server: %s\n",final);
  return req;
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
  char *my_data = "acknowledge";
  printf("client: sending request\n");
  *to_server = req;
  write(req,my_data,strlen(my_data));

  int ack = open("acknowledge",O_RDONLY);
  char *acknowledgement;
  read(ack,acknowledgement,100);
  printf("client: received acknowledgement\n");
  remove("acknowledge");

  char *confirmation = "received acknowledgement confirmation";
  write(req,confirmation,strlen(confirmation));
  printf("client: sending confirmation\n");
  return ack;
}
