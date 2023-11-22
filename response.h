// Struct to store http request header info
typedef struct RequestQueue {
    char *method;
    char *filepath;
    char *protocol;
    struct RequestQueue *next;
} RequestQueue;

void parse_request(char *request_str);
void initialize(RequestQueue *http_req);
void enqueue(RequestQueue *http_req);
RequestQueue *deque();
