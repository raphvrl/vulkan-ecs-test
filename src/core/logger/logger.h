#pragma once

#include <time.h>

#include "core/types.h"
#include "utils/console/console.h"
#include "utils/drawer/box_drawer.h"

typedef enum log_level {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} log_level_e;

void log_msg(const char *msg, log_level_e level);
void draw_log(const char *msg, log_level_e level);

#define LOG_INFO(msg) log_msg(msg, LOG_INFO)
#define LOG_WARN(msg) log_msg(msg, LOG_WARN)
#define LOG_ERROR(msg) log_msg(msg, LOG_ERROR)