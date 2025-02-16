#pragma once

#include "core/types.h"
#include "ecs/registry.h"
#include "ecs/components/component_manager.h"
#include "core/window/window.h"
#include "graphics/pipeline_manager.h"
#include "graphics/gui.h"

// info struct
typedef struct ecs_info {
    registry_t *registry;
    component_manager_t *component_manager;

    window_t *window;
    pipeline_manager_t *pipeline_manager;
} ecs_info_t;

// main struct
typedef struct ecs {
    registry_t *registry;
    component_manager_t *component_manager;

    window_t *window;
    pipeline_manager_t *pipeline_manager;

    gui_t *gui;
} ecs_t;

// systems
#define DEC_SYSTEM(name) \
    void s_##name##_tick(ecs_t *ecs, f32 dt);

DEC_SYSTEM(physics)
DEC_SYSTEM(render)
DEC_SYSTEM(camera)
DEC_SYSTEM(control)

// constructor and destructor
ecs_t *ecs_create(ecs_info_t *info);
void ecs_destroy(ecs_t *ecs);

// actions
void ecs_update(ecs_t *ecs, f32 dt);

// checks
#define ecs_has(ecs, id, component_type) \
        ((ecs)->registry->entities[id].mask & (1 << component_type))

// get components
#define ecs_get(ecs, id, component) (component_manager_get((ecs)->component_manager, id, component))

// adding components
void ecs_add(ecs_t *ecs, u32 id, component_type_e component, void *data);

// getting component count
#define ecs_get_count(ecs, component) ((ecs)->component_manager->lists[component].count)

// new entity
#define ecs_new(registry) registry_create_entity(registry)

// getting entity active
#define ecs_is_active(ecs, id) ((ecs)->registry->entities[id].active)