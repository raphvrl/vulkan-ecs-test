#pragma once

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "vk_device.h"
#include "vk_image.h"

typedef struct swapchain_support_details {
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR *formats;
    VkPresentModeKHR *present_modes;
    u32 format_count;
    u32 present_mode_count;
} swapchain_support_details_t;

#define MAX_FRAMES_IN_FLIGHT 2

// main struct
typedef struct vk_swapchain {
    vk_device_t *device;

    VkSwapchainKHR handle;
    VkFormat format;
    VkExtent2D extent;
    VkImage *images;
    VkImageView *image_views;
    u32 image_count;

    vk_image_t **depth_images;

    VkRenderPass render_pass;
    VkFramebuffer *framebuffers;

    VkCommandBuffer *command_buffers;

    VkSemaphore *image_available;
    VkSemaphore *render_finished;
    VkFence *in_flight_fences;

    u32 current_frame;
    u32 image_index;
} vk_swapchain_t;

// constructor and destructor
vk_swapchain_t *vk_swapchain_create(vk_device_t *device);
void vk_swapchain_destroy(vk_swapchain_t *swapchain);

// actions
bool begin_frame(vk_swapchain_t *swapchain);
void end_frame(vk_swapchain_t *swapchain);

// getters
#define swapchain_get_buffer(swapchain) (swapchain)->command_buffers[(swapchain)->image_index]