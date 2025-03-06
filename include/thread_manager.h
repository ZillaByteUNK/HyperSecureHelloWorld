#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <pthread.h>

typedef struct {
    pthread_t* threads;
    int thread_count;
} thread_pool_t;

bool initialize_thread_pool(thread_pool_t* pool, int count, void* (*thread_func)(void*));
void shutdown_thread_pool(thread_pool_t* pool);

#endif // THREAD_MANAGER_H
