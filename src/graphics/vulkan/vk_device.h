#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "core/types.h"
#include "core/logger/logger.h"
#include "core/window/window.h"

// queue family indices
typedef struct queue_family_indices {
    i32 graphics_family;
    i32 present_family;
} queue_family_indices_t;

// allocator
typedef struct vk_allocator {
    VkPhysicalDeviceMemoryProperties mem_properties;
} vk_allocator_t;

// main struct
typedef struct vk_device {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug;
    VkSurfaceKHR surface;
    VkPhysicalDevice physical_device;
    VkDevice device;
    VkQueue graphics_queue;
    VkQueue present_queue;
    queue_family_indices_t indices;
    VkCommandPool command_pool;
    VkDescriptorPool descriptor_pool;

    VkDescriptorSetLayout texture_layout;

    window_t *window;
} vk_device_t;

// constructor and destructor
vk_device_t *vk_device_create(window_t *window);
void vk_device_destroy(vk_device_t *device);