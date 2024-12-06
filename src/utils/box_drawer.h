#pragma once

#ifdef _WIN32
    #include <windows.h>
#endif

#include <stdio.h>
#include <string.h>

#include "core/type.h"

typedef struct s_box_style {
    char top_left;
    char top_right;
    char bottom_left;
    char bottom_right;
    char horizontal;
    char vertical;
    char left_T;
    char right_T;
} box_style_t;

void draw_box(const char *content, usize width);

void draw_box_2(const char *top, const char *bottom, usize width);