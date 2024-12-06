#pragma once

#ifdef _WIN32
    #include <Windows.h>
#endif

#include <stdbool.h>

#include "core/type.h"

typedef enum color {
    COLOR_BLACK = 0,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE
} color_e;


void consol_set_color(color_e color, bool bright);
void consol_reset_color();

usize consol_get_width();