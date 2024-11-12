#ifndef SERVER_H
#define SERVER_H

// Define constants and function prototypes here
#define PORT 8080
#define MAX_CONNECTIONS 100
#define BUFFER_SIZE 1024

// Function prototypes
void initialize_server();
void handle_request();
void log_request();

#endif // SERVER_H
