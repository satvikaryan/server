#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <ctype.h>

extern char DOCUMENT_ROOT[256];
extern char AUTH_USERNAME[50];
extern char AUTH_PASSWORD[50];

// Function to parse HTTP request and extract the requested resource and method
int parse_request(char *request, char *method, char *url, char *http_version) {
    // Parse request line: method URL HTTP version
    return sscanf(request, "%s %s %s", method, url, http_version);
}

// Function to check if the authentication header is correct
int authenticate_request(char *auth_header) {
    if (auth_header == NULL) {
        return 0;
    }

    // Check if the authentication header matches the basic authentication scheme
    if (strncmp(auth_header, "Basic ", 6) != 0) {
        return 0;
    }

    // Decode the base64 encoded credentials
    char *encoded_credentials = auth_header + 6;
    char decoded_credentials[100];
    base64_decode(encoded_credentials, decoded_credentials);

    char *username = strtok(decoded_credentials, ":");
    char *password = strtok(NULL, ":");

    // Validate the username and password
    return (username != NULL && password != NULL && strcmp(username, AUTH_USERNAME) == 0 && strcmp(password, AUTH_PASSWORD) == 0);
}

// Function to send HTTP response
void send_response(int client_socket, int status_code, char *status_message, char *content_type, char *body) {
    char response[BUFFER_SIZE];

    // Create response header
    snprintf(response, sizeof(response),
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        status_code, status_message, content_type, strlen(body));

    // Send the response header
    send(client_socket, response, strlen(response), 0);

    // Send the response body
    send(client_socket, body, strlen(body), 0);
}

// Function to handle GET requests
void handle_get_request(int client_socket, char *url) {
    char file_path[BUFFER_SIZE];
    snprintf(file_path, sizeof(file_path), "%s%s", DOCUMENT_ROOT, url);

    // Open the requested file
    int file = open(file_path, O_RDONLY);
    if (file == -1) {
        // If the file doesn't exist, send a 404 Not Found response
        send_response(client_socket, 404, "Not Found", "text/html", "<h1>404 Not Found</h1>");
        return;
    }

    // Get the file size
    struct stat file_stat;
    fstat(file, &file_stat);
    off_t file_size = file_stat.st_size;

    // Send the HTTP response header
    send_response(client_socket, 200, "OK", "text/html", "");

    // Send the file content
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(file, buffer, sizeof(buffer))) > 0) {
        send(client_socket, buffer, bytes_read, 0);
    }

    close(file);
}

// Function to handle POST requests
void handle_post_request(int client_socket, char *url, char *body) {
    // Example: Handle POST data (could be used to process form data, etc.)
    printf("Received POST request for %s with body: %s\n", url, body);

    // Respond with a generic 200 OK response
    send_response(client_socket, 200, "OK", "text/html", "<h1>POST Request Received</h1>");
}

// Function to handle the HTTP request
void *handle_request(void *client_socket_ptr) {
    int client_socket = (intptr_t)client_socket_ptr;
    char request[BUFFER_SIZE];
    char method[8], url[BUFFER_SIZE], http_version[16];
    char auth_header[BUFFER_SIZE];

    // Receive the request from the client
    ssize_t bytes_received = recv(client_socket, request, sizeof(request) - 1, 0);
    if (bytes_received <= 0) {
        close(client_socket);
        return NULL;
    }

    // Null-terminate the request to make it a valid C string
    request[bytes_received] = '\0';

    // Parse the request to get method, URL, and HTTP version
    if (parse_request(request, method, url, http_version) != 3) {
        send_response(client_socket, 400, "Bad Request", "text/html", "<h1>400 Bad Request</h1>");
        close(client_socket);
        return NULL;
    }

    // Check for Basic Authentication (if required)
    char *auth_header_start = strstr(request, "Authorization: ");
    if (auth_header_start != NULL) {
        sscanf(auth_header_start, "Authorization: %s", auth_header);
        if (!authenticate_request(auth_header)) {
            send_response(client_socket, 401, "Unauthorized", "text/html", "<h1>401 Unauthorized</h1>");
            close(client_socket);
            return NULL;
        }
    }

    // Handle the request based on the HTTP method
    if (strcasecmp(method, "GET") == 0) {
        handle_get_request(client_socket, url);
    } else if (strcasecmp(method, "POST") == 0) {
        // Get the body of the POST request (if any)
        char *body = strstr(request, "\r\n\r\n");
        if (body != NULL) {
            body += 4;  // Skip over the CRLF CRLF separator
        } else {
            body = "";
        }
        handle_post_request(client_socket, url, body);
    } else {
        send_response(client_socket, 405, "Method Not Allowed", "text/html", "<h1>405 Method Not Allowed</h1>");
    }

    // Close the client socket
    close(client_socket);
    return NULL;
}
