#ifndef CRYPTO_H
#define CRYPTO_H

#include <stddef.h>
#include <stdbool.h>

bool encrypt_message(const char* plaintext, unsigned char* ciphertext, size_t* ciphertext_len, const char* key);
bool decrypt_message(const unsigned char* ciphertext, size_t ciphertext_len, char* plaintext, size_t plaintext_size, const char* key);

#endif // CRYPTO_H
