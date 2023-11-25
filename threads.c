#include "threads.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>

pthread_t thread_pool[POOL_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

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
