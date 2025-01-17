#pragma once

#include <stdlib.h>

#include "core/types.h"
#include "ecs/registry.h"

// component
#include "physics/c_transform.h"
#include "physics/c_velocity.h"
#include "render/c_model.h"

// main struct
typedef struct component_manager {
    c_transform_t *transforms;
    c_velocity_t *velocities;
    c_model_t *models;
} component_manager_t;

// constructor and destructor
component_manager_t *component_manager_create();
void component_manager_destroy(component_manager_t *manager);