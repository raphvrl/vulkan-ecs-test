#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

// Define exit codes
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

// Define log levels
typedef enum e_log_level_t {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} log_level_t;

#define LOG_BUFFER_SIZE 1024

// Log a message
void log_message(log_level_t level, const char *file, int line, const char *format, ...);
#define LOG(...) log_message(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define WARN(...) log_message(LOG_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define ERROR(...) log_message(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)