#include "response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to parse http request
request *parse_request(char *request_str) {
    request *http_req = (request *)malloc(sizeof(request));
    char *request_line = NULL;
    while ((request_line = strtok_r(request_str, "\r\n", &request_str)) !=
           NULL) {
        char *save_ptr;
        http_req->method = strtok_r(request_line, " ", &save_ptr);
        http_req->filepath = strtok_r(NULL, " ", &save_ptr);
        http_req->protocol = strtok_r(NULL, " ", &save_ptr);
        break;
    }
    return http_req;
}
