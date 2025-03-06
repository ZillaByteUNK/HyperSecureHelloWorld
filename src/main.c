#include "config.h"
#include "logger.h"
#include "crypto.h"
#include "thread_manager.h"
#include "verifier.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <atomic.h>

// Global state
static volatile sig_atomic_t terminate_flag = 0;
static thread_pool_t thread_pool;

// Signal handler for graceful shutdown
void handle_signal(int signum) {
    log_message(LOG_LEVEL_WARN, "Received signal %d, shutting down...", signum);
    terminate_flag = 1;
}

// Dummy thread function
void* dummy_thread_func(void* arg) {
    while (!terminate_flag) {
        log_message(LOG_LEVEL_DEBUG, "Dummy thread running...");
        sleep(1);
    }
    log_message(LOG_LEVEL_INFO, "Dummy thread terminating...");
    return NULL;
}

int main(void) {
    // Load configuration
    app_config_t config;
    if (!load_config("config/app_config.cfg", &config)) {
        fprintf(stderr, "Failed to load configuration.\n");
        return EXIT_FAILURE;
    }

    // Initialize logger
    if (!initialize_logger(&config.logging)) {
        fprintf(stderr, "Failed to initialize logger.\n");
        return EXIT_FAILURE;
    }
    log_message(LOG_LEVEL_INFO, "Logger initialized.");

    // Initialize OpenSSL
    if (!initialize_open_ssl()) {
        log_message(LOG_LEVEL_ERROR, "Failed to initialize OpenSSL.");
        shutdown_logger();
        return EXIT_FAILURE;
    }
    log_message(LOG_LEVEL_INFO, "OpenSSL initialized.");

    // Set up signal handlers
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    log_message(LOG_LEVEL_INFO, "Signal handlers registered.");

    // Initialize thread pool
    if (!initialize_thread_pool(&thread_pool, config.general.thread_pool_size, dummy_thread_func)) {
        log_message(LOG_LEVEL_ERROR, "Failed to initialize thread pool.");
        shutdown_logger();
        return EXIT_FAILURE;
    }
    log_message(LOG_LEVEL_INFO, "Thread pool initialized with %d threads.", config.general.thread_pool_size);

    // Encrypt "Hello, World!"
    const char* message = "Hello, World!";
    unsigned char ciphertext[256];
    size_t ciphertext_len = 0;
    if (!encrypt_message(message, ciphertext, &ciphertext_len, config.crypto.encryption_key)) {
        log_message(LOG_LEVEL_ERROR, "Encryption failed.");
        terminate_flag = 1;
    }
    log_message(LOG_LEVEL_INFO, "Message encrypted.");

    // Decrypt the message
    char decrypted[256];
    if (!decrypt_message(ciphertext, ciphertext_len, decrypted, sizeof(decrypted), config.crypto.encryption_key)) {
        log_message(LOG_LEVEL_ERROR, "Decryption failed.");
        terminate_flag = 1;
    }
    log_message(LOG_LEVEL_INFO, "Message decrypted: %s", decrypted);

    // Verify integrity
    if (!verify_integrity(message, decrypted)) {
        terminate_flag = 1;
    }

    // Display the message
    printf("%s\n", decrypted);
    log_message(LOG_LEVEL_INFO, "Displayed message to stdout.");

    // Wait for termination signal
    while (!terminate_flag) {
        sleep(1);
    }

    // Shutdown thread pool
    shutdown_thread_pool(&thread_pool);
    log_message(LOG_LEVEL_INFO, "Thread pool shutdown.");

    // Cleanup
    shutdown_logger();
    log_message(LOG_LEVEL_INFO, "Logger shutdown.");
    
    return EXIT_SUCCESS;
}
