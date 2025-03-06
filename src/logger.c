#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

static FILE* log_fp = NULL;
static log_level_t current_log_level = LOG_LEVEL_DEBUG;
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

bool initialize_logger(const logging_config_t* config) {
    log_fp = fopen(config->log_file, "a");
    if (!log_fp) return false;

    if (strcmp(config->log_level, "DEBUG") == 0) {
        current_log_level = LOG_LEVEL_DEBUG;
    } else if (strcmp(config->log_level, "INFO") == 0) {
        current_log_level = LOG_LEVEL_INFO;
    } else if (strcmp(config->log_level, "WARN") == 0) {
        current_log_level = LOG_LEVEL_WARN;
    } else if (strcmp(config->log_level, "ERROR") == 0) {
        current_log_level = LOG_LEVEL_ERROR;
    } else {
        current_log_level = LOG_LEVEL_DEBUG; // Default
    }

    return true;
}

void log_message(log_level_t level, const char* format, ...) {
    if (level < current_log_level || !log_fp) return;

    const char* level_strings[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    pthread_mutex_lock(&log_mutex);

    // Timestamp
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    fprintf(log_fp, "[%04d-%02d-%02d %02d:%02d:%02d] [%s] ",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec,
            level_strings[level]);

    // Log message
    va_list args;
    va_start(args, format);
    vfprintf(log_fp, format, args);
    va_end(args);

    fprintf(log_fp, "\n");
    fflush(log_fp);
    pthread_mutex_unlock(&log_mutex);
}

void shutdown_logger() {
    if (log_fp) {
        fclose(log_fp);
        log_fp = NULL;
    }
}
