#ifndef VERIFIER_H
#define VERIFIER_H

#include "config.h"
#include "crypto.h"

bool verify_integrity(const char* original, const char* decrypted);

#endif // VERIFIER_H
