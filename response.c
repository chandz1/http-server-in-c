#include "response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to parse Request
Request *parse_request(char *request_str) {
    Request *req = (Request *)malloc(sizeof(Request));
    char *request_line = NULL;
    while ((request_line = strtok_r(request_str, "\r\n", &request_str)) !=
           NULL) {
        char *save_ptr;
        strtok_r(request_line, " ", &save_ptr);
        if (strncmp(request_line, "GET", 3) == 0) {
            req->method = GET;
        }
        req->filepath = strtok_r(NULL, " /", &save_ptr);
        req->protocol = strtok_r(NULL, " ", &save_ptr);
        break;
    }
    printf("[*] Recieved %d request.\n", req->method);
    printf("[*] Recieved request for path: %s\n", req->filepath);
    printf("[*] Recieved request for protocol: %s\n", req->protocol);
    return req;
}
