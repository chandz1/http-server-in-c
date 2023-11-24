#if !defined(_SERVER_H)
#define _SERVER_H

#include "response.h"

#define PORT "8080"
#define MAXBUFSIZE 8192
#define BACKLOG 20

int main(int argc, char *argv[]);
int server_setup();
int accept_connection(int server_fd);
RequestQueue *handle_request(int client_fd);

#endif // _SERVER_H
