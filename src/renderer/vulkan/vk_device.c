#include "vk_device.h"
#include "vk_context.h"
#include "core/log.h"

static const char *device_extensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

static bool check_device_extension_support(VkPhysicalDevice device)
{
    u32 extension_count = 0;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, NULL);

    VkExtensionProperties available_extensions[extension_count];
    vkEnumerateDeviceExtensionProperties(
        device,
        NULL,
        &extension_count,
        available_extensions
    );

    for (u32 i = 0; i < ARR_LEN(device_extensions); i++) {
        bool found = false;
        for (u32 j = 0; j < extension_count; j++) {
            if (strcmp(device_extensions[i], available_extensions[j].extensionName) == 0) {
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

static bool is_device_suitable(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(device, &props);

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);

    if (!check_device_extension_support(device)) {
        return false;
    }

    return ((props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ||
             props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) &&
             features.geometryShader);
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

    queue_family_indices_t queue_families = {0};

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

    queue_families = find_queue_families(ctx->physical_device, ctx);
    if (!queue_families.graphics_family_found || !queue_families.present_family_found) {
        LERROR("Failed to find queue families");
        return;
    }

    ctx->device.queue_families = queue_families;

    LOG("Physical device picked");
}

void vulkan_device_create(vulkan_ctx_t *ctx)
{
    queue_family_indices_t indices = ctx->device.queue_families;

    f32 queue_priority = 1.0f;
    VkDeviceQueueCreateInfo queue_create_infos[2] = {0};
    u32 queue_count = 0;

    if (!indices.graphics_family_found) {
        LERROR("Graphics queue family not found");
        return;
    }

    queue_create_infos[queue_count].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_infos[queue_count].queueFamilyIndex = indices.graphics_family;
    queue_create_infos[queue_count].queueCount = 1;
    queue_create_infos[queue_count].pQueuePriorities = &queue_priority;
    queue_count++;

    if (!indices.present_family_found || indices.present_family != indices.graphics_family) {
        queue_create_infos[queue_count].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_infos[queue_count].queueFamilyIndex = indices.present_family;
        queue_create_infos[queue_count].queueCount = 1;
        queue_create_infos[queue_count].pQueuePriorities = &queue_priority;
        queue_count++;
    }

    VkPhysicalDeviceFeatures device_features = {0};

    VkDeviceCreateInfo create_info = {0};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.pQueueCreateInfos = queue_create_infos;
    create_info.queueCreateInfoCount = queue_count;
    create_info.pEnabledFeatures = &device_features;
    create_info.enabledExtensionCount = ARR_LEN(device_extensions);
    create_info.ppEnabledExtensionNames = device_extensions;

    if (vkCreateDevice(ctx->physical_device, &create_info, NULL, &ctx->device.device) != VK_SUCCESS) {
        LERROR("Failed to create logical device");
        return;
    }

    vkGetDeviceQueue(
        ctx->device.device,
        indices.graphics_family,
        0,
        &ctx->device.graphics_queue
    );

    vkGetDeviceQueue(
        ctx->device.device,
        indices.present_family,
        0,
        &ctx->device.present_queue
    );

    LOG("Logical device created");
}

void vulkan_device_destroy(vulkan_ctx_t *ctx)
{
    vkDestroyDevice(ctx->device.device, NULL);
    LOG("Logical device destroyed");
}