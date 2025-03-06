#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

typedef struct {
    int max_retry_attempts;
    size_t buffer_size;
    int verification_level;
    int thread_pool_size;
} general_config_t;

typedef struct {
    char log_level[10];
    char log_file[256];
} logging_config_t;

typedef struct {
    char encryption_key[33]; // 32 chars + null terminator
} crypto_config_t;

typedef struct {
    general_config_t general;
    logging_config_t logging;
    crypto_config_t crypto;
} app_config_t;

bool load_config(const char* filepath, app_config_t* config);

#endif // CONFIG_H
