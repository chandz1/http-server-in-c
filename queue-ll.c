#include "queue-ll.h"
#include <stdio.h>
#include <stdlib.h>

Queue *front = NULL;
Queue *rear = NULL;

void initialize(Queue *curr) {
    curr->next = NULL;
    front = rear = curr;
}

void enqueue(int data) {
    Queue *curr = (Queue *)malloc(sizeof(Queue));
    curr->data = data;

    if (front == NULL) {
        initialize(curr);
        return;
    }
    rear->next = curr;
    rear = curr;
}

int deque() {
    if (front == NULL) {
        return -1;
    }
    Queue *curr = (Queue *)malloc(sizeof(Queue));
    curr = front;
    front = front->next;
    curr->next = NULL;
    return curr->data;
}
