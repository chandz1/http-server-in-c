#include "server.h"
#include "queue-ll.h"
#include "threads.h"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

int main(int argc, char *argv[]) {

    int server_fd = server_setup();
    thread_pool_create();

    while (1) {
        enqueue(accept_connection(server_fd));
        pthread_cond_signal(&cond);
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

int *accept_connection(int server_fd) {
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
    int *client_fd_ptr = (int *)malloc(sizeof(client));
    *client_fd_ptr = client_fd;
    return client_fd_ptr;
}

int handle_request(int *client_fd_ptr) {
    char buf[MAXBUFSIZE];
    int client_fd = *client_fd_ptr;
    free(client_fd_ptr);

    if (recv(client_fd, buf, MAXBUFSIZE, 0) == -1) {
        perror("recv error");
        exit(1);
    }

    // printf("\n%s", buf);
    Request *req = parse_request(buf);
    int status = send_response(req, client_fd);

    return status;
}

int send_response(Request *req, int client_fd) {
    char buf[MAXBUFSIZE];
    char path[1024];

    if (req->method == GET) {
        realpath(req->filepath, path);
        printf("%s\n", path);
        FILE *file = fopen(req->filepath, "r");
        if (file == NULL) {
            perror("fopen error");
            return -1;
        }
        if (strcmp(req->filepath, "index.html") == 0) {
            snprintf(buf, MAXBUFSIZE,
                     "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html\r\n"
                     "\r\n");
        } else {
            snprintf(buf, MAXBUFSIZE,
                     "HTTP/1.1 200 OK\r\n"
                     "Content-Type: image/x-icon\r\n"
                     "\r\n");
        }
        size_t len = strnlen(buf, MAXBUFSIZE);
        send(client_fd, buf, len, 0);

        memset(&buf, 0, sizeof(buf));
        size_t bytesRead;
        while ((bytesRead = fread(buf, 1, sizeof(buf), file)) > 0) {
            send(client_fd, buf, bytesRead, 0);
        }
        printf("%s\n", buf);
        fclose(file);
        close(client_fd);
        return 0;
    }
    return -1;
}

void *thread_run() {
    while (1) {
        int *client_fd;
        pthread_mutex_lock(&mutex);
        if ((client_fd = dequeue()) == NULL) {
            pthread_cond_wait(&cond, &mutex);
            client_fd = dequeue();
        }
        pthread_mutex_unlock(&mutex);

        if (client_fd != NULL) {
            int status = handle_request(client_fd);
            printf("Status Code: %d\n", status);
        }
    }
}
