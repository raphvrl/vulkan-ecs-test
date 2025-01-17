#pragma once

#include "core/types.h"

typedef struct c_velocity {
    v3 velocity;
} c_velocity_t;

#define COMPONENT_VELOCITY (1 << 1)