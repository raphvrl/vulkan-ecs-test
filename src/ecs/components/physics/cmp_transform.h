#pragma once

#include "core/types.h"

typedef struct cmp_transform {
    v3 position;
    v3 rotation;
    v3 scale;
} cmp_transform_t;

#define COMPONENT_TRANSFORM (1 << 0)