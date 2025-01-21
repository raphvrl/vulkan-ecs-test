#pragma once

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "vk_device.h"
#include "vk_buffer.h"

// main struct
typedef struct vk_image {
    vk_device_t *device;

    VkImage handle;
    VkDeviceMemory memory;
    VkImageView view;
    u32 width;
    u32 height;
    VkFormat format;
} vk_image_t;

// constructor
vk_image_t *vk_image_create(
    vk_device_t *device,
    u32 width,
    u32 height,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags properties
);

// destructor
void vk_image_destroy(vk_image_t *image);

void vk_image_transition_layout(
    vk_image_t *image,
    VkImageLayout old_layout,
    VkImageLayout new_layout
);

void vk_image_copy_from_buffer(
    vk_image_t *image,
    vk_buffer_t *buffer
);