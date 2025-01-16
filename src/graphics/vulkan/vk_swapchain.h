#pragma once

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "vk_device.h"

// main struct
typedef struct vk_swapchain {
    vk_device_t *device;

    VkSwapchainKHR handle;
    VkFormat format;
    VkExtent2D extent;
    VkImage *images;
    VkImageView *image_views;
    u32 image_count;

    VkRenderPass render_pass;
    VkFramebuffer *framebuffers;

    VkCommandPool command_pool;
    VkCommandBuffer *command_buffers;

    VkSemaphore image_available;
    VkSemaphore render_finished;
    VkFence *in_flight_fences;
} vk_swapchain_t;

// constructor and destructor
vk_swapchain_t *vk_swapchain_create(vk_device_t *device);
void vk_swapchain_destroy(vk_swapchain_t *swapchain);