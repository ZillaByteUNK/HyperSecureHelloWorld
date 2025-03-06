#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool load_config(const char* filepath, app_config_t* config) {
    FILE* file = fopen(filepath, "r");
    if (!file) return false;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Simple parser; in real scenarios, consider using a proper INI parser
        if (strncmp(line, "max_retry_attempts", 18) == 0) {
            sscanf(line, "max_retry_attempts = %d", &config->general.max_retry_attempts);
        } else if (strncmp(line, "buffer_size", 11) == 0) {
            sscanf(line, "buffer_size = %zu", &config->general.buffer_size);
        } else if (strncmp(line, "verification_level", 18) == 0) {
            sscanf(line, "verification_level = %d", &config->general.verification_level);
        } else if (strncmp(line, "thread_pool_size", 16) == 0) {
            sscanf(line, "thread_pool_size = %d", &config->general.thread_pool_size);
        } else if (strncmp(line, "log_level", 9) == 0) {
            sscanf(line, "log_level = %s", config->logging.log_level);
        } else if (strncmp(line, "log_file", 8) == 0) {
            sscanf(line, "log_file = %s", config->logging.log_file);
        } else if (strncmp(line, "encryption_key", 14) == 0) {
            sscanf(line, "encryption_key = %32s", config->crypto.encryption_key);
        }
    }

    fclose(file);
    return true;
}
