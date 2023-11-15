// Struct to store http request header info
typedef struct http_request {
    char *method;
    char *filepath;
    char *protocol;
} request;

request *parse_request(char *request_str);
