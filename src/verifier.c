#include "verifier.h"
#include <string.h>
#include "logger.h"

bool verify_integrity(const char* original, const char* decrypted) {
    if (strcmp(original, decrypted) != 0) {
        log_message(LOG_LEVEL_ERROR, "Integrity verification failed: Original and decrypted messages do not match");
        return false;
    }
    log_message(LOG_LEVEL_INFO, "Integrity verification succeeded");
    return true;
}
