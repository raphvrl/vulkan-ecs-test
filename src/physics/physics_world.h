#pragma once

#include <ode/ode.h>

#include "core/types.h"

#define MAX_CONTACTS 10

// main struct
typedef struct physics_world {
    dWorldID world;
    dSpaceID space;
    dJointGroupID contacts;
    int max_contacts;
} physics_world_t;

// constructor and destructor
physics_world_t *physics_world_create();
void physics_world_destroy(physics_world_t *world);

// actions
void physics_world_update(physics_world_t *world, float dt);