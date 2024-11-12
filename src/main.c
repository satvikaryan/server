#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

int PORT = 8080;
int MAX_CONNECTIONS = 100;
char DOCUMENT_ROOT[256] = "www";
char LOG_FILE[256] = "logs/access.log";
int ENABLE_LOGGING = 1;
int THREAD_POOL_SIZE = 8;
char AUTH_USERNAME[50] = "admin";
char AUTH_PASSWORD[50] = "password123";

// Function to load the server configuration from a file
void load_config() {
    FILE *config_file = fopen("config/server.conf", "r");
    if (config_file == NULL) {
        perror("Unable to open configuration file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), config_file) != NULL) {
        if (strncmp(line, "port=", 5) == 0) {
            PORT = atoi(line + 5);
        } else if (strncmp(line, "max_connections=", 16) == 0) {
            MAX_CONNECTIONS = atoi(line + 16);
        } else if (strncmp(line, "document_root=", 14) == 0) {
            strcpy(DOCUMENT_ROOT, line + 14);
            DOCUMENT_ROOT[strcspn(DOCUMENT_ROOT, "\n")] = '\0';
        } else if (strncmp(line, "log_file=", 9) == 0) {
            strcpy(LOG_FILE, line + 9);
            LOG_FILE[strcspn(LOG_FILE, "\n")] = '\0';
        } else if (strncmp(line, "enable_logging=", 15) == 0) {
            ENABLE_LOGGING = atoi(line + 15);
        } else if (strncmp(line, "thread_pool_size=", 17) == 0) {
            THREAD_POOL_SIZE = atoi(line + 17);
        } else if (strncmp(line, "username=", 9) == 0) {
            strcpy(AUTH_USERNAME, line + 9);
            AUTH_USERNAME[strcspn(AUTH_USERNAME, "\n")] = '\0';
        } else if (strncmp(line, "password=", 9) == 0) {
            strcpy(AUTH_PASSWORD, line + 9);
            AUTH_PASSWORD[strcspn(AUTH_PASSWORD, "\n")] = '\0';
        }
    }

    fclose(config_file);
}

// Function to initialize the server socket and start listening
void start_server() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started on port %d\n", PORT);
    
    // Start accepting incoming connections
    while (1) {
        int client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

        // Log the incoming request
        char request[BUFFER_SIZE];
        snprintf(request, sizeof(request), "Incoming connection from %s", client_ip);
        log_request(client_ip, request, HTTP_OK);

        // Handle the request in a new thread
        pthread_t thread;
        pthread_create(&thread, NULL, (void *)handle_request, (void *)(intptr_t)client_socket);
        pthread_detach(thread); // Detach the thread so it cleans up after itself
    }
}

// Main function
int main() {
    // Load the server configuration from the conf file
    load_config();

    // Initialize the server (thread pool and other resources)
    thread_pool_init();

    // Start the server and begin accepting client connections
    start_server();

    return 0;
}
