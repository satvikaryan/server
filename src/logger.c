#include "server.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

// Function to get the current time as a string
void get_current_time(char *buffer) {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 100, "%Y-%m-%d %H:%M:%S", timeinfo);
}

// Function to log the incoming request
void log_request(const char *ip, const char *request, int status_code) {
    if (!ENABLE_LOGGING) return;

    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        perror("Unable to open log file");
        return;
    }

    char time_buffer[100];
    get_current_time(time_buffer);

    // Format log entry
    fprintf(log_file, "%s - %s \"%s\" %d\n", time_buffer, ip, request, status_code);

    fclose(log_file);
}

// Function to log errors
void log_error(const char *error_message) {
    if (!ENABLE_LOGGING) return;

    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        perror("Unable to open log file");
        return;
    }

    char time_buffer[100];
    get_current_time(time_buffer);

    // Log error message with timestamp
    fprintf(log_file, "%s ERROR: %s\n", time_buffer, error_message);

    fclose(log_file);
}
