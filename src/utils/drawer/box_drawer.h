#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/types.h"

// constants
#define MAX_WORDS 100
#define WORD_BUFFER 256

// actions
void draw_box(const char *str, u32 w);
void draw_2box(const char *top, const char *bottom, u32 w);