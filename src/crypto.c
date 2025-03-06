#include "crypto.h"
#include <openssl/evp.h>
#include <string.h>

bool encrypt_message(const char* plaintext, unsigned char* ciphertext, size_t* ciphertext_len, const char* key) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;

    // Initialize encryption operation
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)key, (unsigned char*)key)) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    int len;
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, (unsigned char*)plaintext, strlen(plaintext))) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    *ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    *ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

bool decrypt_message(const unsigned char* ciphertext, size_t ciphertext_len, char* plaintext, size_t plaintext_size, const char* key) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;

    // Initialize decryption operation
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)key, (unsigned char*)key)) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    int len;
    if (1 != EVP_DecryptUpdate(ctx, (unsigned char*)plaintext, &len, ciphertext, ciphertext_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    int plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, (unsigned char*)plaintext + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    plaintext_len += len;
    plaintext[plaintext_len] = '\0';

    EVP_CIPHER_CTX_free(ctx);
    return true;
}
