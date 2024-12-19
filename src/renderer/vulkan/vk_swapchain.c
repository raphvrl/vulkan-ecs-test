#include "vk_swapchain.h"
#include "vk_context.h"
#include "core/log.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static swapchain_support_details_t query_swapchain_support(
    VkPhysicalDevice device,
    VkSurfaceKHR surface
)
{
    swapchain_support_details_t details = {0};

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        device,
        surface,
        &details.capabilities
    );

    u32 format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(
        device,
        surface,
        &format_count,
        NULL
    );

    if (format_count != 0) {
        details.formats = malloc(sizeof(VkSurfaceFormatKHR) * format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            device,
            surface,
            &format_count,
            details.formats
        );
    }

    u32 present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device,
        surface,
        &present_mode_count,
        NULL
    );

    if (present_mode_count != 0) {
        details.present_modes = malloc(sizeof(VkPresentModeKHR) * present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device,
            surface,
            &present_mode_count,
            details.present_modes
        );
    }

    details.format_count = format_count;
    details.present_mode_count = present_mode_count;

    return details;
}

static VkSurfaceFormatKHR choose_swapchain_surface_format(
    swapchain_support_details_t *details
)
{
    for (u32 i = 0; i < details->format_count; i++) {
        VkSurfaceFormatKHR format = details->formats[i];
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
            format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }

    return details->formats[0];
}

static VkPresentModeKHR choose_swapchain_present_mode(
    swapchain_support_details_t *details
)
{
    for (u32 i = 0; i < details->present_mode_count; i++) {
        if (details->present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            return details->present_modes[i];
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

static VkExtent2D choose_swapchain_extent(
    swapchain_support_details_t *details,
    u32 width,
    u32 height
)
{
    if (details->capabilities.currentExtent.width != UINT32_MAX) {
        return details->capabilities.currentExtent;
    }

    VkExtent2D actual_extent = {width, height};

    actual_extent.width = MAX(
        details->capabilities.minImageExtent.width,
        MIN(details->capabilities.maxImageExtent.width, actual_extent.width)
    );

    actual_extent.height = MAX(
        details->capabilities.minImageExtent.height,
        MIN(details->capabilities.maxImageExtent.height, actual_extent.height)
    );

    return actual_extent;
}

void vulkan_swapchain_create(vulkan_ctx_t *ctx)
{
    swapchain_support_details_t details = query_swapchain_support(
        ctx->physical_device,
        ctx->surface
    );

    VkSurfaceFormatKHR surface_format = choose_swapchain_surface_format(
        &details
    );

    VkPresentModeKHR present_mode = choose_swapchain_present_mode(
        &details
    );

    VkExtent2D extent = choose_swapchain_extent(
        &details,
        ctx->width,
        ctx->height
    );

    u32 image_count = details.capabilities.minImageCount + 1;
    if (details.capabilities.maxImageCount > 0 &&
        image_count > details.capabilities.maxImageCount) {
        image_count = details.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = ctx->surface,
        .minImageCount = image_count,
        .imageFormat = surface_format.format,
        .imageColorSpace = surface_format.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = details.capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = present_mode,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };
    
    VkResult res = vkCreateSwapchainKHR(
        ctx->device.device,
        &create_info,
        NULL,
        &ctx->swapchain.handle
    );

    if (res != VK_SUCCESS) {
        LERROR("Failed to create swapchain (error: %d)", res);
        return;
    }

    vkGetSwapchainImagesKHR(
        ctx->device.device,
        ctx->swapchain.handle,
        &ctx->swapchain.image_count,
        NULL
    );

    ctx->swapchain.images = malloc(sizeof(VkImage) * ctx->swapchain.image_count);

    vkGetSwapchainImagesKHR(
        ctx->device.device,
        ctx->swapchain.handle,
        &ctx->swapchain.image_count,
        ctx->swapchain.images
    );

    ctx->swapchain.format = surface_format.format;
    ctx->swapchain.extent = extent;

    LOG("Swapchain created");
}

void vulkan_swapchain_destroy(vulkan_ctx_t *ctx)
{
    if (!ctx) {
        LERROR("Invalid Vulkan context");
        return;
    }

    vkDestroySwapchainKHR(ctx->device.device, ctx->swapchain.handle, NULL);
    free(ctx->swapchain.images);

    LOG("Swapchain destroyed");
}

void vulkan_image_view_create(vulkan_ctx_t *ctx)
{
    ctx->swapchain.image_views = malloc(sizeof(VkImageView) * ctx->swapchain.image_count);

    for (u32 i = 0; i < ctx->swapchain.image_count; i++) {
        VkImageViewCreateInfo create_info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = ctx->swapchain.images[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = ctx->swapchain.format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };

        VkResult res = vkCreateImageView(
            ctx->device.device,
            &create_info,
            NULL,
            &ctx->swapchain.image_views[i]
        );

        if (res != VK_SUCCESS) {
            LERROR("Failed to create image view (error: %d)", res);
            return;
        }
    }

    LOG("Image views created");
}

void vulkan_image_view_destroy(vulkan_ctx_t *ctx)
{
    if (!ctx) {
        LERROR("Invalid Vulkan context");
        return;
    }

    for (u32 i = 0; i < ctx->swapchain.image_count; i++) {
        vkDestroyImageView(ctx->device.device, ctx->swapchain.image_views[i], NULL);
    }

    free(ctx->swapchain.image_views);

    LOG("Image views destroyed");
}