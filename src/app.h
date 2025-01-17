#pragma once

#include "core/types.h"
#include "core/window/window.h"
#include "ecs/ecs.h"
#include "ecs/components/component_manager.h"
#include "ecs/registry.h"
#include "graphics/vulkan/vk_device.h"
#include "graphics/vulkan/vk_swapchain.h"
#include "graphics/vulkan/vk_pipeline.h"
#include "graphics/pipeline_manager.h"
#include "graphics/mesh.h"

// main struct
typedef struct app {
    window_t *window;
    vk_device_t *device;
    vk_swapchain_t *swapchain;
    mesh_t *mesh;
    pipeline_manager_t *pipeline_manager;


    registry_t *registry;
    component_manager_t *component_manager;
    ecs_t *ecs;
} app_t;

// constructor and destructor
app_t *app_create(u32 w, u32 h, const char *title);
void app_destroy(app_t *app);

// action
void app_run(app_t *app);