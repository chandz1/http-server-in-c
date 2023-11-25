#include "threads.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>

pthread_t thread_pool[POOL_SIZE];

void thread_pool_create() {
    for (int i = 0; i < POOL_SIZE; i++) {
        pthread_create(&thread_pool[i], NULL, thread_run, NULL);
    }
}

void thread_pool_join() {
    for (int i = 0; i < POOL_SIZE; i++) {
        pthread_join(thread_pool[i], NULL);
    }
}
