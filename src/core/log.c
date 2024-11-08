#include "log.h"

void log_message(
    log_level_t level,
    const char *file,
    int line,
    const char *format,
    ...
)
{
    va_list args;
    va_start(args, format);

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char time_str[26];
    strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("[%s] ", time_str);

    switch (level) {
        case LOG_INFO: printf("INFO: "); break;
        case LOG_WARNING: printf("WARNING: "); break;
        case LOG_ERROR: printf("ERROR: "); break;
    }

    vprintf(format, args);

    if (level == LOG_ERROR) {
        printf(" (%s:%d)", file, line);
    }

    printf("\n");

    va_end(args);
}
