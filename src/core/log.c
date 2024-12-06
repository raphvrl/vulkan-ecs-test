#include "log.h"

void log_message(log_level_e level, const char *fmt, ...)
{
    int width = consol_get_width() - 2;

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char time_str[32];

    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm);

    va_list args;
    va_start(args, fmt);

    char buffer[LOG_BUFFER_SIZE];
    vsnprintf(buffer, LOG_BUFFER_SIZE, fmt, args);

    va_end(args);

    const char *level_str = NULL;

    color_e color = COLOR_WHITE;

    switch (level) {
        case LOG_DEBUG:
            level_str = "DEBUG";
            color = COLOR_CYAN;
            break;
        
        case LOG_INFO:
            level_str = "INFO";
            color = COLOR_WHITE;
            break;

        case LOG_SUCCESS:   
            level_str = "SUCCESS";
            color = COLOR_GREEN;
            break;

        case LOG_WARNING:
            level_str = "WARNING";
            color = COLOR_YELLOW;
            break;

        case LOG_ERROR:
            level_str = "ERROR";
            color = COLOR_RED;
            break;

        default:
            level_str = "UNKNOWN";
            color = COLOR_MAGENTA;
            break;
    }

    char info[width];
    snprintf(info, width, "%s | %s", time_str, level_str);

    consol_set_color(color, false);

    draw_box_2(info, buffer, width);

    consol_reset_color();

    printf("\n");
}