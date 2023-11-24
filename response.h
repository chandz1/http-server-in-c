// Struct to store http request header info

typedef enum { GET, POST, PUT } Method;

typedef struct HttpRequest {
    Method method;
    char *filepath;
    char *protocol;
} Request;

Request *parse_request(char *request_str);
