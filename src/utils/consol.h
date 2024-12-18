#pragma once

#ifdef _WIN32
    #include <Windows.h>
#endif

#include <stdbool.h>

#include "core/type.h"

typedef enum color {
    COLOR_BLACK = 0,
    COLOR_BLUE,
    COLOR_GREEN,
    COLOR_CYAN,
    COLOR_RED,
    COLOR_YELLOW,
    COLOR_MAGENTA,
    COLOR_WHITE
} color_e;

void consol_set_color(color_e color, bool bright);
void consol_reset_color();

usize consol_get_width();