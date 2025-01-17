#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

#include "core/types.h"

typedef enum console_color {
    BLACK = 0,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
} console_color_e;

void console_set_color(console_color_e color);
void console_reset_color();

u32 console_get_width();