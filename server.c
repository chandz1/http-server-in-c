#include "response.h"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT "8080"
#define MAXBUFSIZE 8192

int main(int argc, char *argv[]) {
    int server_fd, client_fd, status;
    struct addrinfo hints, *res, *p;
    struct sockaddr_storage client;
    socklen_t client_len;
    char buf[MAXBUFSIZE];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, PORT, &hints, &res) != 0)) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(1);
    }

    for (p = res; p != NULL; p = p->ai_next) {

        if ((server_fd =
                 socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("socket error");
            continue;
        }

        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1},
                       sizeof(int)) == -1) {
            close(server_fd);
            perror("setsockopt error");
            continue;
        }

        if (bind(server_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(server_fd);
            perror("bind error");
            continue;
        }
        break;
    }

    freeaddrinfo(res);

    if (p == NULL) {
        fprintf(stderr, "Something went wrong.\n");
        exit(1);
    }

    if (listen(server_fd, 20) == -1) {
        perror("listen error");
        exit(1);
    }

    client_len = (sizeof(client));
    client_fd = accept(server_fd, (struct sockaddr *)&client, &client_len);
    if (client_fd == -1) {
        perror("accept error");
        exit(1);
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(client.ss_family,
              &(((struct sockaddr_in *)((struct sockaddr *)&client))->sin_addr),
              client_ip, sizeof client_ip);

    printf("[*] Connected to client on IP: %s\n", client_ip);

    memset(buf, 0, MAXBUFSIZE);
    recv(client_fd, buf, MAXBUFSIZE - 1, 0);

    buf[MAXBUFSIZE - 1] = '\0';

    request *http_req = parse_request(buf);
    printf("[*] Recieved %s request.\n", http_req->method);
    printf("[*] Recieved request for path: %s\n", http_req->filepath);
    printf("[*] Recieved request for protocol: %s\n", http_req->protocol);

    return 0;
}
