#pragma once

#include <capi.h>

#include "core/types.h"

typedef struct physics_world {
    void *universe;
} physics_world_t;

physics_world_t *physics_world_create();
void physics_world_destroy(physics_world_t *world);

void physics_world_update(physics_world_t *world, float dt);