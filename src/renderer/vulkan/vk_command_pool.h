#pragma once

#include <vulkan/vulkan.h>

#include "core/type.h"

typedef struct s_vulkan_ctx vulkan_ctx_t;

void vulkan_command_pool_create(vulkan_ctx_t *ctx);
void vulkan_command_pool_destroy(vulkan_ctx_t *ctx);

VkCommandBuffer vulkan_command_buffer_create(
    vulkan_ctx_t *ctx
);

void vulkan_command_buffer_destroy(
    vulkan_ctx_t *ctx,
    VkCommandBuffer command_buffer
);