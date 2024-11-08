#include "vulkan_instance.h"
#include "vulkan_validation.h"
#include "core/type.h"
#include "core/log.h"

static void log_available_extensions()
{
    u32 ext_count = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &ext_count, NULL);

    VkExtensionProperties exts[ext_count];
    vkEnumerateInstanceExtensionProperties(NULL, &ext_count, exts);

    for (u32 i = 0; i < ext_count; i++) {
        LOG("Available extension: %s", exts[i].extensionName);
    }
}

static void set_extensions(vulkan_ctx_t *ctx, VkInstanceCreateInfo *create_info)
{
    u32 glfw_ext_count = 0;
    const char **glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_ext_count);
    if (!glfw_exts) {
        ERROR("Failed to get required GLFW extensions");
        return;
    }

    static const char *exts[64];
    u32 ext_count = glfw_ext_count;

    for (u32 i = 0; i < glfw_ext_count; i++) {
        exts[i] = glfw_exts[i];
    }

    if (ctx->validation.enable) {
        exts[ext_count] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
        ext_count++;
    }

    create_info->enabledExtensionCount = ext_count;
    create_info->ppEnabledExtensionNames = exts;
}

static bool check_validation(vulkan_ctx_t *ctx)
{
    u32 layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, NULL);

    VkLayerProperties layers[layer_count];
    vkEnumerateInstanceLayerProperties(&layer_count, layers);

    for (u32 i = 0; i < ctx->validation.count; i++) {
        bool found = false;
        for (u32 j = 0; j < layer_count; j++) {
            if (strcmp(ctx->validation.layers[i], layers[j].layerName) == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            return false;
        }
    }

    return true;
}

void vulkan_instance_create(vulkan_ctx_t *ctx)
{
    if (ctx->validation.enable && !check_validation(ctx)) {
        WARN("Validation layers requested but not available");
    } else if (ctx->validation.enable) {
        LOG("Validation layers enabled");
    }

    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Vulkan Test",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0
    };

    log_available_extensions();

    VkInstanceCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app_info,
    };

    set_extensions(ctx, &create_info);

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {0};
    if (ctx->validation.enable) {
        create_info.enabledLayerCount = ctx->validation.count;
        create_info.ppEnabledLayerNames = ctx->validation.layers;
        populate_debug_messenger_create_info(&debug_create_info);
        create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debug_create_info;
    } else {
        create_info.enabledLayerCount = 0;
        create_info.pNext = NULL;
    }

    VkResult result = vkCreateInstance(&create_info, NULL, &ctx->instance);
    if (result != VK_SUCCESS) {
        ERROR("Failed to create Vulkan instance (error: %d)", result);
        return;
    }

    LOG("Vulkan instance created");    
}

void vulkan_instance_destroy(vulkan_ctx_t *ctx)
{
    vkDestroyInstance(ctx->instance, NULL);
    LOG("Vulkan instance destroyed");
}
