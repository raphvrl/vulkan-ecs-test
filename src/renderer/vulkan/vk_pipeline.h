#pragma once

#include <vulkan/vulkan.h>

#include "core/type.h"

typedef struct s_vulkan_ctx vulkan_ctx_t;

typedef struct s_vulkan_pipeline {
    VkPipeline handle;
    VkPipelineLayout layout;
} vk_pipeline_t;

vk_pipeline_t *vulkan_pipeline_create(
    vulkan_ctx_t *ctx,
    const char *vert_path,
    const char *frag_path
);

void vulkan_pipeline_destroy(
    vulkan_ctx_t *ctx,
    vk_pipeline_t *pipeline
);