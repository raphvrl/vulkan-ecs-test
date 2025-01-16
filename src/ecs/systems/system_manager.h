#pragma once

#include "core/types.h"
#include "ecs/registry.h"
#include "ecs/components/component_manager.h"
#include "core/window/window.h"
#include "graphics/vulkan/vk_device.h"

// systems
#include "ecs/systems/physics/sys_physics.h"

// main struct
typedef struct system_manager {
    registry_t *registry;
    component_manager_t *component_manager;

    window_t *window;
    vk_device_t *device;
} system_manager_t;

// constructor and destructor
system_manager_t *system_manager_create(
    registry_t *registry,
    component_manager_t *component_manager,
    window_t *window,
    vk_device_t *device
);

void system_manager_destroy(system_manager_t *manager);

// actions
void system_manager_update(system_manager_t *manager, f32 dt);