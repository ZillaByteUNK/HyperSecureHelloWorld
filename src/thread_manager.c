#include "thread_manager.h"
#include "logger.h"
#include <stdlib.h>

bool initialize_thread_pool(thread_pool_t* pool, int count, void* (*thread_func)(void*)) {
    pool->threads = malloc(sizeof(pthread_t) * count);
    if (!pool->threads) return false;

    pool->thread_count = count;
    for (int i = 0; i < count; i++) {
        if (pthread_create(&pool->threads[i], NULL, thread_func, NULL) != 0) {
            log_message(LOG_LEVEL_ERROR, "Failed to create thread %d", i);
            pool->thread_count = i;
            return false;
        }
    }

    return true;
}

void shutdown_thread_pool(thread_pool_t* pool) {
    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }
    free(pool->threads);
    pool->threads = NULL;
    pool->thread_count = 0;
}
