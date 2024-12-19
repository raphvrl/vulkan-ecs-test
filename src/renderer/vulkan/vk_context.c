#include "vk_context.h"
#include "vk_instance.h"
#include "core/log.h" 
#include "core/mem.h"
#include "core/type.h"

static const char *validation_layers[] = {
    "VK_LAYER_KHRONOS_validation"
};

#ifndef NDEBUG
    static const bool enable_validation_layers = true;
#else
    static const bool enable_validation_layers = false;
#endif

static void vulkan_surface_create(vulkan_ctx_t *ctx, GLFWwindow *window)
{
    if (!ctx || !window) {
        LERROR("Invalid Vulkan context or window");
        return;
    }

    VkResult result = glfwCreateWindowSurface(
        ctx->instance,
        window,
        NULL,
        &ctx->surface
    );

    if (result != VK_SUCCESS) {
        LERROR("Failed to create window surface (error: %d)", result);
        return;
    }

    LOG("Window surface created");
}

static void vulkan_surface_destroy(vulkan_ctx_t *ctx)
{
    if (!ctx) {
        LERROR("Invalid Vulkan context");
        return;
    }

    vkDestroySurfaceKHR(ctx->instance, ctx->surface, NULL);
    LOG("Window surface destroyed");
}

vulkan_ctx_t *vulkan_ctx_create(GLFWwindow *window)
{
    UNUSED(window);

    vulkan_ctx_t *ctx = malloc(sizeof(vulkan_ctx_t));
    if (!ctx) {
        LERROR("Failed to allocate memory for Vulkan context");
        return NULL;
    }

    vulkan_validation_t validation = {
        .layers = validation_layers,
        .count = ARR_LEN(validation_layers),
        .enable = enable_validation_layers
    };

    ctx->validation = validation;

    vulkan_instance_create(ctx);
    vulkan_validation_create(ctx);
    vulkan_surface_create(ctx, window);
    vulkan_physical_device_pick(ctx);
    vulkan_device_create(ctx);
    vulkan_swapchain_create(ctx);
    vulkan_image_view_create(ctx);
    vulkan_renderpass_create(ctx);
    vulkan_framebuffer_create(ctx);
    vulkan_command_pool_create(ctx);

    return ctx;
}

void vulkan_ctx_destroy(vulkan_ctx_t *ctx)
{
    vulkan_command_pool_destroy(ctx);
    vulkan_framebuffer_destroy(ctx);
    vulkan_renderpass_destroy(ctx);
    vulkan_image_view_destroy(ctx);
    vulkan_swapchain_destroy(ctx);
    vulkan_device_destroy(ctx);
    vulkan_surface_destroy(ctx);
    vulkan_validation_destroy(ctx);
    vulkan_instance_destroy(ctx);
    _free(ctx);
}