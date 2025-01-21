#pragma once

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "vk_device.h"

typedef struct vk_buffer {
    vk_device_t *device;

    VkBuffer handle;
    VkDeviceMemory memory;
    VkDeviceSize size;
    void *mapped;
} vk_buffer_t;

// constructor and destructor
vk_buffer_t *vk_buffer_create(
    vk_device_t *device,
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties
);

void vk_buffer_destroy(vk_buffer_t *buffer);

// copy data
void vk_buffer_copy(
    vk_device_t *device,
    VkBuffer src,
    VkBuffer dst,
    VkDeviceSize size
);