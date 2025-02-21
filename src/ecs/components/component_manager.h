#pragma once

#include <stdlib.h>

#include "core/types.h"
#include "ecs/registry.h"

// component
#include "physics/c_transform.h"
#include "physics/c_velocity.h"
#include "physics/c_dynamic.h"
#include "render/c_model.h"
#include "render/c_camera.h"

// const
#define COMPONENT_LIST_NBR 5

// enum
typedef enum component_type {
    C_TRANSFORM,
    C_VELOCITY,
    C_MODEL,
    C_CAMERA,
    C_DYNAMIC,
} component_type_e;

// component list
typedef struct component_list {
    void *data;
    usize size;
    u32 count;
} component_list_t;

// main struct
typedef struct component_manager {
    component_list_t lists[COMPONENT_LIST_NBR];
} component_manager_t;

#define _init_component(manager, type, component) do {                      \
    (manager)->lists[component].data = calloc(MAX_ENTITIES, sizeof(type));  \
    (manager)->lists[component].size = sizeof(type);                        \
    (manager)->lists[component].count = 0;                                  \
} while (0)

// constructor and destructor
component_manager_t *component_manager_create();
void component_manager_destroy(component_manager_t *manager);

// adding components
void component_manager_add(
    component_manager_t *manager,
    u32 id,
    component_type_e component,
    void *data
);

// getting components
void *component_manager_get(
    component_manager_t *manager,
    u32 id,
    component_type_e component
);

