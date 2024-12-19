#include "vk_command_pool.h"
#include "vk_context.h"
#include "core/log.h"

void vulkan_command_pool_create(vulkan_ctx_t *ctx)
{
    VkCommandPoolCreateInfo pool_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .queueFamilyIndex = ctx->device.queue_families.graphics_family,
        .flags = 0
    };

    VkResult res = vkCreateCommandPool(
        ctx->device.device,
        &pool_info,
        NULL,
        &ctx->command_pool
    );

    if (res != VK_SUCCESS) {
        LERROR("Failed to create command pool: %d", res);
    }

    LOG("Command pool created");
}

void vulkan_command_pool_destroy(vulkan_ctx_t *ctx)
{
    if (!ctx->command_pool) {
        return;
    }

    vkDestroyCommandPool(ctx->device.device, ctx->command_pool, NULL);
    LOG("Command pool destroyed");
}

VkCommandBuffer vulkan_command_buffer_create(
    vulkan_ctx_t *ctx
)
{
    VkCommandBufferAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = ctx->command_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    VkCommandBuffer command_buffer;
    VkResult res = vkAllocateCommandBuffers(
        ctx->device.device,
        &alloc_info,
        &command_buffer
    );

    if (res != VK_SUCCESS) {
        LERROR("Failed to allocate command buffer: %d", res);
        return VK_NULL_HANDLE;
    }

    return command_buffer;
}

void vulkan_command_buffer_destroy(
    vulkan_ctx_t *ctx,
    VkCommandBuffer command_buffer
)
{
    if (!command_buffer) {
        return;
    }

    vkFreeCommandBuffers(
        ctx->device.device,
        ctx->command_pool,
        1,
        &command_buffer
    );
}