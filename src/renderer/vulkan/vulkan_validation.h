#pragma once

#include <vulkan/vulkan.h>
#include <stdbool.h>

#include "core/type.h"

// forward declaration
typedef struct s_vulkan_ctx vulkan_ctx_t;

// Vulkan validation layer configuration
typedef struct s_vulkan_validation {
    const char **layers;
    u32 count;
    bool enable;
} vulkan_validation_t;

void vulkan_validation_create(vulkan_ctx_t *ctx);
void vulkan_validation_destroy(vulkan_ctx_t *ctx);
VkResult create_debug_messenger(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *create_info,
    const VkAllocationCallbacks *allocator,
    VkDebugUtilsMessengerEXT *messenger);

void populate_debug_messenger_create_info(
    VkDebugUtilsMessengerCreateInfoEXT *create_info
);

void destroy_debug_messenger(
    VkInstance instance,
    VkDebugUtilsMessengerEXT messenger,
    const VkAllocationCallbacks *allocator
);