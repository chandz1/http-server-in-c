#include "server.h"
#include "queue-ll.h"
#include "threads.h"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char buf[MAXBUFSIZE];
    char path[1024];

    int server_fd = server_setup();

    while (1) {

        int client_fd = accept_connection(server_fd);
        int *client = (int *)malloc(sizeof(client));
        *client = client_fd;
        enqueue(client);

        Request *http_req = handle_request(client_fd);
        memset(buf, 0, MAXBUFSIZE);
        if (http_req->method == GET) {
            printf("GETTTTT\n");
        }
        realpath(http_req->filepath, path);
        printf("%s\n", path);
        int fd = open(http_req->filepath, O_RDONLY);
        size_t file_size = lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);
        snprintf(buf, MAXBUFSIZE,
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html\r\n"
                 "\r\n");
        size_t len = strnlen(buf, MAXBUFSIZE);
        read(fd, buf + len, file_size);
        len = strnlen(buf, MAXBUFSIZE);
        printf("%s\n", buf);
        send(client_fd, buf, len, 0);
        close(client_fd);
    }

    return 0;
}

int server_setup() {
    int server_fd, status;
    struct addrinfo hints, *res, *p;

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

    if (listen(server_fd, BACKLOG) == -1) {
        perror("listen error");
        exit(1);
    }

    return server_fd;
}

int accept_connection(int server_fd) {
    int client_fd;
    struct sockaddr_storage client;
    socklen_t client_len;
    char client_ip[INET_ADDRSTRLEN];

    client_len = (sizeof(client));
    client_fd = accept(server_fd, (struct sockaddr *)&client, &client_len);
    if (client_fd == -1) {
        perror("accept error");
        exit(1);
    }

    inet_ntop(client.ss_family,
              &(((struct sockaddr_in *)((struct sockaddr *)&client))->sin_addr),
              client_ip, sizeof client_ip);

    printf("[*] Connected to client on IP: %s\n", client_ip);
    return client_fd;
}

Request *handle_request(int client_fd) {
    char buf[MAXBUFSIZE];

    if (recv(client_fd, buf, MAXBUFSIZE, 0) == -1) {
        perror("recv error");
        exit(1);
    }

    printf("\n%s", buf);
    Request *http_req = parse_request(buf);

    return http_req;
}

void *thread_run() {
    while (1) {
    }
}
