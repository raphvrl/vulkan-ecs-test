#pragma once

#include "core/types.h"

typedef struct c_transform {
    v3 position;
    v3 rotation;
    v3 scale;
} c_transform_t;

#define COMPONENT_TRANSFORM (1 << 0)