#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "vk_validation.h"
#include "vk_device.h"
#include "vk_swapchain.h"
#include "vk_renderpass.h"
#include "vk_framebuffer.h"
#include "vk_command_pool.h"

typedef struct s_vulkan_ctx {
    VkInstance instance;
    VkDebugUtilsMessengerEXT callback;
    VkPhysicalDevice physical_device;
    vulkan_device_t device;
    VkRenderPass render_pass;
    VkFramebuffer *framebuffers;
    VkCommandPool command_pool;

    u32 width;
    u32 height;
    VkSurfaceKHR surface;
    vk_swapchain_t swapchain;

    vulkan_validation_t validation;
} vulkan_ctx_t;

vulkan_ctx_t *vulkan_ctx_create(GLFWwindow *window);
void vulkan_ctx_destroy(vulkan_ctx_t *ctx);