#include "vulkan_context.h"
#include "vulkan_instance.h"
#include "core/log.h" 
#include "core/mem.h"
#include "core/type.h"

static const char *validation_layers[] = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    static const bool enable_validation_layers = false;
#else
    static const bool enable_validation_layers = true;
#endif

static void vulkan_surface_create(vulkan_ctx_t *ctx, GLFWwindow *window)
{
    if (!ctx || !window) {
        ERROR("Invalid Vulkan context or window");
        return;
    }

    VkResult result = glfwCreateWindowSurface(
        ctx->instance,
        window,
        NULL,
        &ctx->surface
    );

    if (result != VK_SUCCESS) {
        ERROR("Failed to create window surface (error: %d)", result);
        return;
    }

    LOG("Window surface created");
}

static void vulkan_surface_destroy(vulkan_ctx_t *ctx)
{
    if (!ctx) {
        ERROR("Invalid Vulkan context");
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
        ERROR("Failed to allocate memory for Vulkan context");
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

    return ctx;
}

void vulkan_ctx_destroy(vulkan_ctx_t *ctx)
{
    vulkan_surface_destroy(ctx);
    vulkan_validation_destroy(ctx);
    vulkan_instance_destroy(ctx);
    _free(ctx);
}