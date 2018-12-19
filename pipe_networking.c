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
  char client_data[12];
  read(req,client_data,12);
  remove("request");

  int ack = open(client_data,O_WRONLY);
  *to_client = ack;
  printf("server: request acknowledged\n");
  write(ack,"request acknowledged",21);

  char final[38];
  read(req,final,38);
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
