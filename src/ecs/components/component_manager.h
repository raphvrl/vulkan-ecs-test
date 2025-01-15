#pragma once

#include <stdlib.h>

#include "core/types.h"
#include "ecs/registry.h"

// component
#include "physics/cmp_transform.h"

// main struct
typedef struct component_manager {
    cmp_transform_t *transforms;
} component_manager_t;

// constructor and destructor
component_manager_t *component_manager_create();
void component_manager_destroy(component_manager_t *manager);