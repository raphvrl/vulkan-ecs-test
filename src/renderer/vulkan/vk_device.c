#include "vk_device.h"
#include "vk_context.h"
#include "core/log.h"

static bool is_device_suitable(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(device, &props);

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);

    return props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
            VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU &&
            features.geometryShader;
}

static queue_family_indices_t find_queue_families(VkPhysicalDevice device, vulkan_ctx_t *ctx)
{
    queue_family_indices_t indices = {0};

    u32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);

    VkQueueFamilyProperties queue_families[queue_family_count];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);

    for (u32 i = 0; i < queue_family_count; i++) {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics_family = i;
            indices.graphics_family_found = true;
        }

        VkBool32 present_support = false;
        if (vkGetPhysicalDeviceSurfaceSupportKHR(
            device,
            i,
            ctx->surface,
            &present_support
        ) != VK_SUCCESS) {
            continue;
        }

        if (present_support) {
            indices.present_family = i;
            indices.present_family_found = true;
        }

        if (indices.graphics_family_found && indices.present_family_found) {
            break;
        }
    }

    return indices;
}

void vulkan_physical_device_pick(vulkan_ctx_t *ctx)
{
    u32 device_count = 0;
    vkEnumeratePhysicalDevices(ctx->instance, &device_count, NULL);

    if (device_count == 0) {
        LERROR("Failed to find GPUs with Vulkan support");
        return;
    }

    VkPhysicalDevice devices[device_count];
    vkEnumeratePhysicalDevices(ctx->instance, &device_count, devices);

    for (u32 i = 0; i < device_count; i++) {
        if (is_device_suitable(devices[i])) {
            ctx->physical_device = devices[i];
            break;
        }
    }

    if (ctx->physical_device == VK_NULL_HANDLE) {
        LERROR("Failed to find a suitable GPU");
        return;
    }

    ctx->queue_families = find_queue_families(ctx->physical_device, ctx);
    if (!ctx->queue_families.graphics_family_found || !ctx->queue_families.present_family_found) {
        LERROR("Failed to find queue families");
        return;
    }

    LOG("Physical device picked");
}