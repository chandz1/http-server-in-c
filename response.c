#include "response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to parse http RequestQueue
RequestQueue *parse_request(char *request_str) {
    RequestQueue *http_req = (RequestQueue *)malloc(sizeof(RequestQueue));
    char *request_line = NULL;
    while ((request_line = strtok_r(request_str, "\r\n", &request_str)) !=
           NULL) {
        char *save_ptr;
        http_req->method = strtok_r(request_line, " ", &save_ptr);
        http_req->filepath = strtok_r(NULL, " ", &save_ptr);
        http_req->protocol = strtok_r(NULL, " ", &save_ptr);
        break;
    }
    printf("[*] Recieved %s request.\n", http_req->method);
    printf("[*] Recieved request for path: %s\n", http_req->filepath);
    printf("[*] Recieved request for protocol: %s\n", http_req->protocol);
    return http_req;
}
