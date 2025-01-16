#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "core/types.h"

// main struct
typedef struct vk_device {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug;
    VkPhysicalDevice physical_device;
    VkDevice device;
    VkQueue graphics_queue;
    VkQueue present_queue;
    VkSurfaceKHR surface;
} vk_device_t;

// constructor and destructor
vk_device_t *vk_device_create(GLFWwindow *window);
void vk_device_destroy(vk_device_t *device);