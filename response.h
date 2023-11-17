// Struct to store http request header info
typedef struct RequestQueue {
    char *method;
    char *filepath;
    char *protocol;
    struct RequestQueue *next;
} RequestQueue;

RequestQueue *parse_request(char *request_str);
