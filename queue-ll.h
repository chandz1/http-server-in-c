#if !defined(_QUEUE_LL_H)
#define _QUEUE_LL_H

typedef struct Queue {
    int *data;
    struct Queue *next;
} Queue;

void initialize(Queue *curr);
void enqueue(int *data);
int *dequeue();

#endif // _QUEUE_LL_H
