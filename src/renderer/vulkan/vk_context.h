#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "vk_validation.h"
#include "vk_device.h"

typedef struct s_vulkan_ctx {
    VkInstance instance;
    VkDebugUtilsMessengerEXT callback;
    VkPhysicalDevice physical_device;
    VkDevice device;
    VkSurfaceKHR surface;

    vulkan_validation_t validation;
    queue_family_indices_t queue_families;
} vulkan_ctx_t;

vulkan_ctx_t *vulkan_ctx_create(GLFWwindow *window);
void vulkan_ctx_destroy(vulkan_ctx_t *ctx);