#pragma once

#include <vulkan/vulkan.h>
#include <stdbool.h>

#include "core/type.h"

// forward declaration
typedef struct s_vulkan_ctx vulkan_ctx_t;

// Vulkan validation layer configuration
typedef struct s_queue_family_indices {
    u32 graphics_family;
    u32 present_family;
    bool graphics_family_found;
    bool present_family_found;
} queue_family_indices_t;

void vulkan_physical_device_pick(vulkan_ctx_t *ctx);