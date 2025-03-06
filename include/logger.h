#ifndef LOGGER_H
#define LOGGER_H

#include "config.h"

typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} log_level_t;

bool initialize_logger(const logging_config_t* config);
void log_message(log_level_t level, const char* format, ...);
void shutdown_logger();

#endif // LOGGER_H
