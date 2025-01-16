#pragma once

#include "core/types.h"

typedef struct cmp_velocity {
    v3 velocity;
} cmp_velocity_t;

#define COMPONENT_VELOCITY (1 << 1)