#pragma once

#include <stdlib.h>

#include "core/types.h"
#include "ecs/registry.h"

// component
#include "physics/cmp_transform.h"
#include "physics/cmp_velocity.h"
#include "render/cmp_model.h"

// main struct
typedef struct component_manager {
    cmp_transform_t *transforms;
    cmp_velocity_t *velocities;
    cmp_model_t *models;
} component_manager_t;

// constructor and destructor
component_manager_t *component_manager_create();
void component_manager_destroy(component_manager_t *manager);