#pragma once

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <sys/ioctl.h> 
    #include <unistd.h>
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

#include "utils/box_drawer.h"
#include "utils/consol.h"

// Define exit codes
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

// Define log levels
typedef enum log_level {
    LOG_DEBUG,
    LOG_INFO,
    LOG_SUCCESS,
    LOG_WARNING,
    LOG_ERROR
} log_level_e;

#define LOG_BUFFER_SIZE 1024

extern bool g_save_log;

// Log a message
void log_message(log_level_e level, const char *fmt, ...);
#define LOG(fmt, ...) log_message(LOG_INFO, fmt, ##__VA_ARGS__)
#define SUCCESS(fmt, ...) log_message(LOG_SUCCESS, fmt, ##__VA_ARGS__)
#define WARNING(fmt, ...) log_message(LOG_WARNING, fmt, ##__VA_ARGS__)
#define LERROR(fmt, ...) log_message(LOG_ERROR, fmt, ##__VA_ARGS__)
#define DEBUG(fmt, ...) log_message(LOG_DEBUG, fmt, ##__VA_ARGS__)

void set_save_log(bool enable);