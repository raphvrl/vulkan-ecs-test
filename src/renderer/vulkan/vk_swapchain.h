#pragma once

#include <vulkan/vulkan.h>

#include <unistd.h>

#include "core/type.h"

typedef struct s_vulkan_ctx vulkan_ctx_t;

typedef struct s_swapchain_support_details {
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR *formats;
    VkPresentModeKHR *present_modes;
    u32 format_count;
    u32 present_mode_count;
} swapchain_support_details_t; 

typedef struct s_vk_swapchain {
    VkSwapchainKHR handle;
    VkImage *images;
    VkImageView *image_views;
    u32 image_count;
    VkFormat format;
    VkExtent2D extent;
} vk_swapchain_t;

void vulkan_swapchain_create(vulkan_ctx_t *ctx);
void vulkan_swapchain_destroy(vulkan_ctx_t *ctx);

void vulkan_image_view_create(vulkan_ctx_t *ctx);
void vulkan_image_view_destroy(vulkan_ctx_t *ctx);