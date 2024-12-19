#include "vk_framebuffer.h"
#include "vk_context.h"
#include "core/log.h"
#include <unistd.h>

void vulkan_framebuffer_create(vulkan_ctx_t *ctx)
{
    u32 image_count = ctx->swapchain.image_count;

    ctx->framebuffers = malloc(sizeof(VkFramebuffer) * image_count);

    for (u32 i = 0; i < image_count; i++) {
        VkImageView attachments[] = {
            ctx->swapchain.image_views[i]
        };

        VkFramebufferCreateInfo framebuffer_info = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = ctx->render_pass,
            .attachmentCount = 1,
            .pAttachments = attachments,
            .width = ctx->swapchain.extent.width,
            .height = ctx->swapchain.extent.height,
            .layers = 1
        };

        VkResult res = vkCreateFramebuffer(
            ctx->device.device,
            &framebuffer_info,
            NULL,
            &ctx->framebuffers[i]
        );

        if (res != VK_SUCCESS) {
            LERROR("Failed to create framebuffer: %d", res);
        }
    }

    LOG("Framebuffers created");
}

void vulkan_framebuffer_destroy(vulkan_ctx_t *ctx)
{
    if (!ctx->framebuffers) {
        return;
    }

    for (u32 i = 0; i < ctx->swapchain.image_count; i++) {
        vkDestroyFramebuffer(ctx->device.device, ctx->framebuffers[i], NULL);
    }

    free(ctx->framebuffers);
    LOG("Framebuffers destroyed");
}