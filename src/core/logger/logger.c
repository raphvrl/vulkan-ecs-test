#include "logger.h"

bool logger_saved = false;

void log_msg(const char *msg, log_level_e level)
{
    if (!logger_saved) {
        draw_log(msg, level);
    }
}

void draw_log(const char *msg, log_level_e level)
{
    u32 width = console_get_width();

    char buffer[80];
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

    char log_level_str[16];
    char log_msg[256];
    switch (level) {
        case LOG_INFO:
            console_set_color(LIGHTBLUE);
            strcpy(log_level_str, "INFO");
            break;
        case LOG_WARN:
            console_set_color(YELLOW);
            strcpy(log_level_str, "WARN");
            break;
        case LOG_ERROR:
            console_set_color(LIGHTRED);
            strcpy(log_level_str, "ERROR");
            break;
    }

    sprintf(log_msg, "%s | %s", buffer, log_level_str);

    draw_2box(log_msg, msg, width);
    console_reset_color();
}