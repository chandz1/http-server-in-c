#if !defined(_THREADS_H)
#define _THREADS_H

#include <pthread.h>

#define POOL_SIZE 8
extern pthread_mutex_t mutex;
extern pthread_cond_t cond;

void *thread_run();
void thread_pool_create();
void thread_pool_join();

#endif // _THREADS_H
