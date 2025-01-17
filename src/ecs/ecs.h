#pragma once

#include "core/types.h"
#include "ecs/registry.h"
#include "ecs/components/component_manager.h"
#include "core/window/window.h"
#include "graphics/pipeline_manager.h"

// systems
#include "ecs/systems/physics/sys_physics.h"
#include "ecs/systems/render/sys_render.h"

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
} ecs_t;

// constructor and destructor
ecs_t *ecs_create(ecs_info_t *info);

void ecs_destroy(ecs_t *ecs);

// actions
void ecs_update(ecs_t *ecs, f32 dt);