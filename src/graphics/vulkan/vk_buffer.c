#include "vk_buffer.h"

static u32 find_memory_type(
    VkPhysicalDevice physical_device,
    u32 type_filter,
    VkMemoryPropertyFlags properties
) {
    VkPhysicalDeviceMemoryProperties mem_properties;
    vkGetPhysicalDeviceMemoryProperties(physical_device, &mem_properties);

    for (u32 i = 0; i < mem_properties.memoryTypeCount; i++) {
        if (
            (type_filter & (1 << i)) &&
            (mem_properties.memoryTypes[i].propertyFlags & properties) == properties
        ) {
            return i;
        }
    }

    LOG_ERROR("Failed to find suitable memory type!");
    return 0;
}

vk_buffer_t *vk_buffer_create(
    vk_device_t *device,
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties
) {
    vk_buffer_t *buffer = malloc(sizeof(vk_buffer_t));
    if (!buffer) {
        return NULL;
    }

    buffer->size = size;

    VkBufferCreateInfo buffer_info = {0};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult res = vkCreateBuffer(
        device->device,
        &buffer_info,
        NULL,
        &buffer->handle
    );

    if (res != VK_SUCCESS) {
        LOG_ERROR("Failed to create buffer!");
        return NULL;
    }

    VkMemoryRequirements mem_requirements;
    vkGetBufferMemoryRequirements(
        device->device,
        buffer->handle,
        &mem_requirements
    );

    VkMemoryAllocateInfo alloc_info = {0};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_requirements.size;
    alloc_info.memoryTypeIndex = find_memory_type(
        device->physical_device,
        mem_requirements.memoryTypeBits,
        properties
    );

    res = vkAllocateMemory(
        device->device,
        &alloc_info,
        NULL,
        &buffer->memory
    );

    if (res != VK_SUCCESS) {
        LOG_ERROR("Failed to allocate buffer memory!");
        return NULL;
    }

    vkBindBufferMemory(device->device, buffer->handle, buffer->memory, 0);
    buffer->size = size;

    return buffer;
}

void vk_buffer_destroy(vk_device_t *device, vk_buffer_t *buffer)
{
    if (!buffer || !device) { return; }

    vkDestroyBuffer(device->device, buffer->handle, NULL);
    vkFreeMemory(device->device, buffer->memory, NULL);
    free(buffer);
}

void vk_buffer_copy(
    vk_device_t *device,
    VkBuffer src,
    VkBuffer dst,
    VkDeviceSize size
) {
    VkCommandPoolCreateInfo pool_info = {0};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    pool_info.queueFamilyIndex = device->indices.graphics_family;

    VkCommandPool command_pool;
    VkResult res = vkCreateCommandPool(
        device->device,
        &pool_info,
        NULL,
        &command_pool
    );

    if (res != VK_SUCCESS) {
        LOG_ERROR("Failed to create command pool!");
        return;
    }

    VkCommandBufferAllocateInfo alloc_info = {0};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandPool = command_pool;
    alloc_info.commandBufferCount = 1;

    VkCommandBuffer command_buffer;
    res = vkAllocateCommandBuffers(
        device->device,
        &alloc_info,
        &command_buffer
    );

    if (res != VK_SUCCESS) {
        LOG_ERROR("Failed to allocate command buffer!");
        return;
    }

    VkCommandBufferBeginInfo begin_info = {0};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(command_buffer, &begin_info);

    VkBufferCopy copy_region = {0};
    copy_region.size = size;
    vkCmdCopyBuffer(command_buffer, src, dst, 1, &copy_region);

    vkEndCommandBuffer(command_buffer);

    VkSubmitInfo submit_info = {0};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;

    vkQueueSubmit(device->graphics_queue, 1, &submit_info, VK_NULL_HANDLE);
    vkQueueWaitIdle(device->graphics_queue);

    vkFreeCommandBuffers(device->device, command_pool, 1, &command_buffer);
    vkDestroyCommandPool(device->device, command_pool, NULL);
}