#pragma once

#include "core/types.h"
#include "core/window/window.h"
#include "ecs/systems/system_manager.h"
#include "ecs/components/component_manager.h"
#include "ecs/registry.h"
#include "graphics/vulkan/vk_device.h"

// main struct
typedef struct app {
    window_t *window;
    vk_device_t *device;

    registry_t *registry;
    component_manager_t *component_manager;
    system_manager_t *system_manager;

    
} app_t;

// constructor and destructor
app_t *app_create(u32 w, u32 h, const char *title);
void app_destroy(app_t *app);

// action
void app_run(app_t *app);