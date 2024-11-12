// server.h - Header file for the web server

#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include <netinet/in.h> // For sockaddr_in
#include <stdio.h>

// Configuration settings
extern int PORT;
extern int MAX_CONNECTIONS;
extern char DOCUMENT_ROOT[256];
extern char LOG_FILE[256];
extern int ENABLE_LOGGING;
extern int THREAD_POOL_SIZE;
extern char AUTH_USERNAME[50];
extern char AUTH_PASSWORD[50];

// Buffer size for client requests
#define BUFFER_SIZE 4096

// HTTP Status Codes
#define HTTP_OK 200
#define HTTP_NOT_FOUND 404
#define HTTP_INTERNAL_ERROR 500
#define HTTP_UNAUTHORIZED 401
#define HTTP_BAD_REQUEST 400

// Function declarations

// Initialization functions
void load_config();  // Loads configurations from server.conf
void start_server(); // Initializes the server socket and starts listening for connections

// Request handling functions
void handle_request(int client_socket); // Parses and responds to HTTP requests

// Thread pool functions
void thread_pool_init();               // Initializes the thread pool
void thread_pool_add_task(int socket); // Adds a client socket to the task queue
void *worker_thread(void *arg);        // Worker function for each thread

// Static file handling
void serve_static_file(int client_socket, const char *file_path); // Sends static files to clients

// HTTP response functions
void send_response(int client_socket, int status_code, const char *content_type, const char *body);
void send_error_response(int client_socket, int status_code, const char *error_message);

// Logging
void log_request(const char *ip, const char *request, int status_code); // Logs incoming requests

// Authentication
int authenticate(const char *username, const char *password); // Basic authentication check

#endif // SERVER_H
