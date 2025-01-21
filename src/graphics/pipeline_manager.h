#pragma once

#include "core/types.h"
#include "vulkan/vk_swapchain.h"
#include "vulkan/vk_pipeline.h"

// pipeline enum
typedef enum pipeline_type {
    MAIN_PIPELINE,
} pipeline_type_e;

// push constant data
typedef struct mvp_pc {
    m4 mvp;
} mvp_pc_t;

// main struct
typedef struct pipeline_manager {
    vk_swapchain_t *swapchain;
    vk_pipeline_t **pipelines;
} pipeline_manager_t;

// constructor and destructor
pipeline_manager_t *pipeline_manager_create(vk_swapchain_t *swapchain);
void pipeline_manager_destroy(pipeline_manager_t *manager);

// actions
void pipeline_manager_bind(pipeline_manager_t *manager, pipeline_type_e type);
void pipeline_manager_push_constant(
    pipeline_manager_t *manager,
    pipeline_type_e type,
    VkShaderStageFlags stage,
    u32 offset,
    u32 size,
    void *data
);