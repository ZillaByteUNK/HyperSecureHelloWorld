#include "utils.h"
#include <openssl/evp.h>

bool initialize_open_ssl() {
    if (!OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, NULL)) {
        return false;
    }
    return true;
}
