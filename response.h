// Struct to store http request header info

typedef enum { GET, POST, PUT } Method;

typedef struct RequestQueue {
    Method method;
    char *filepath;
    char *protocol;
} RequestQueue;

RequestQueue *parse_request(char *request_str);
