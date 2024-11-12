#include "server.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// Define the maximum number of threads in the pool
#define MAX_THREADS 10

// Thread pool structure
typedef struct {
    pthread_t threads[MAX_THREADS];  // Array of worker threads
    int client_sockets[MAX_THREADS]; // Array of client sockets
    int thread_count;                // Number of active threads
    pthread_mutex_t lock;           // Mutex to protect access to the pool
    pthread_cond_t cond;            // Condition variable to signal worker threads
    int shutdown;                   // Flag to indicate if the pool should be shut down
} thread_pool_t;

// Global thread pool instance
static thread_pool_t thread_pool;

// Worker thread function
void *worker_thread(void *arg) {
    while (1) {
        pthread_mutex_lock(&thread_pool.lock);

        // Wait for a client request to handle, or for shutdown signal
        while (thread_pool.thread_count == 0 && !thread_pool.shutdown) {
            pthread_cond_wait(&thread_pool.cond, &thread_pool.lock);
        }

        if (thread_pool.shutdown) {
            pthread_mutex_unlock(&thread_pool.lock);
            break;
        }

        // Get the client socket from the pool
        int client_socket = thread_pool.client_sockets[--thread_pool.thread_count];

        pthread_mutex_unlock(&thread_pool.lock);

        // Handle the request for this client socket
        handle_request((void *)(intptr_t)client_socket);
    }

    return NULL;
}

// Initialize the thread pool
void init_thread_pool() {
    thread_pool.thread_count = 0;
    thread_pool.shutdown = 0;
    pthread_mutex_init(&thread_pool.lock, NULL);
    pthread_cond_init(&thread_pool.cond, NULL);

    // Create worker threads
    for (int i = 0; i < MAX_THREADS; ++i) {
        if (pthread_create(&thread_pool.threads[i], NULL, worker_thread, NULL) != 0) {
            perror("Failed to create thread");
            exit(1);
        }
    }
}

// Add a client socket to the thread pool for processing
void add_to_thread_pool(int client_socket) {
    pthread_mutex_lock(&thread_pool.lock);

    // If the thread pool is full, wait for a thread to be available
    while (thread_pool.thread_count == MAX_THREADS) {
        pthread_cond_wait(&thread_pool.cond, &thread_pool.lock);
    }

    // Add the client socket to the pool
    thread_pool.client_sockets[thread_pool.thread_count++] = client_socket;

    // Signal one worker thread that a task is available
    pthread_cond_signal(&thread_pool.cond);

    pthread_mutex_unlock(&thread_pool.lock);
}

// Shutdown the thread pool and clean up resources
void shutdown_thread_pool() {
    pthread_mutex_lock(&thread_pool.lock);
    thread_pool.shutdown = 1;
    pthread_cond_broadcast(&thread_pool.cond);  // Wake up all threads to terminate
    pthread_mutex_unlock(&thread_pool.lock);

    // Join all worker threads
    for (int i = 0; i < MAX_THREADS; ++i) {
        pthread_join(thread_pool.threads[i], NULL);
    }

    pthread_mutex_destroy(&thread_pool.lock);
    pthread_cond_destroy(&thread_pool.cond);
}
