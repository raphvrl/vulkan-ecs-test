#pragma once

#include "core/types.h"

typedef struct c_transform {
    v3 pos;
    v3 rotate;
    v3 scale;
} c_transform_t;

#define DEFAULT_TRANSFORM (c_transform_t) {.pos = {0.0f, 0.0f, 0.0f}, .rotate = {0.0f, 0.0f, 0.0f}, .scale = {1.0f, 1.0f, 1.0f}}