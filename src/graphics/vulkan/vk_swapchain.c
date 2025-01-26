#include "vk_swapchain.h"

static void create_sync_objects(vk_swapchain_t *swapchain)
{
    swapchain->image_available = malloc(
        MAX_FRAMES_IN_FLIGHT * sizeof(VkSemaphore)
    );

    swapchain->render_finished = malloc(
        MAX_FRAMES_IN_FLIGHT * sizeof(VkSemaphore)
    );

    swapchain->in_flight_fences = malloc(
        MAX_FRAMES_IN_FLIGHT * sizeof(VkFence)
    );

    VkSemaphoreCreateInfo semaphore_info = {0};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info = {0};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        VkResult res = vkCreateSemaphore(
            swapchain->device->device,
            &semaphore_info,
            NULL,
            &swapchain->image_available[i]
        );

        if (res != VK_SUCCESS) {
            LOG_ERROR("Failed to create semaphores!");
        }

        res = vkCreateSemaphore(
            swapchain->device->device,
            &semaphore_info,
            NULL,
            &swapchain->render_finished[i]
        );

        if (res != VK_SUCCESS) {
            LOG_ERROR("Failed to create semaphores!");
        }

        res = vkCreateFence(
            swapchain->device->device,
            &fence_info,
            NULL,
            &swapchain->in_flight_fences[i]
        );

        if (res != VK_SUCCESS) {
            LOG_ERROR("Failed to create fences!");
        }
    }
}

static void create_command_buffer(vk_swapchain_t *swapchain)
{
    vk_device_t *device = swapchain->device;

    swapchain->command_buffers = malloc(swapchain->image_count * sizeof(VkCommandBuffer));

    VkCommandBufferAllocateInfo alloc_info = {0};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = device->command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = swapchain->image_count;

    VkResult res = vkAllocateCommandBuffers(
        swapchain->device->device,
        &alloc_info,
        swapchain->command_buffers
    );

    if (res != VK_SUCCESS) {
        LOG_ERROR("Failed to allocate command buffers!");
    }
}

static void create_framebuffers(vk_swapchain_t *swapchain)
{
    swapchain->framebuffers = malloc(swapchain->image_count * sizeof(VkFramebuffer));
    for (u32 i = 0; i < swapchain->image_count; i++) {
        VkImageView attachments[] = {
            swapchain->image_views[i],
            swapchain->depth_images[i]->view
        };

        VkFramebufferCreateInfo framebuffer_info = {0};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = swapchain->render_pass;
        framebuffer_info.attachmentCount = 2;
        framebuffer_info.pAttachments = attachments;
        framebuffer_info.width = swapchain->extent.width;
        framebuffer_info.height = swapchain->extent.height;
        framebuffer_info.layers = 1;

        if (vkCreateFramebuffer(swapchain->device->device, &framebuffer_info, NULL, &swapchain->framebuffers[i]) != VK_SUCCESS) {
            LOG_ERROR("Failed to create framebuffer!");
        }
    }
}

static void create_depth_resources(vk_swapchain_t *swapchain)
{
    vk_device_t *device = swapchain->device;

    swapchain->depth_images = malloc(swapchain->image_count * sizeof(vk_image_t *));
    for (u32 i = 0; i < swapchain->image_count; i++) {
        swapchain->depth_images[i] = vk_image_create(
            device,
            swapchain->extent.width,
            swapchain->extent.height,
            VK_FORMAT_D32_SFLOAT,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            VK_IMAGE_ASPECT_DEPTH_BIT
        );
    }
}

static void create_renderpass(vk_swapchain_t *swapchain)
{
    vk_device_t *device = swapchain->device;

    VkAttachmentDescription color_attachment = {0};
    color_attachment.format = swapchain->format;
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_ref = {0};
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depth_attachment = {0};
    depth_attachment.format = VK_FORMAT_D32_SFLOAT;
    depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depth_attachment_ref = {0};
    depth_attachment_ref.attachment = 1;
    depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {0};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;
    subpass.pDepthStencilAttachment = &depth_attachment_ref;

    VkAttachmentDescription attachments[] = {
        color_attachment,
        depth_attachment
    };

    VkRenderPassCreateInfo render_pass_info = {0};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = 2;
    render_pass_info.pAttachments = attachments;
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;

    VkResult res = vkCreateRenderPass(
        device->device,
        &render_pass_info,
        NULL,
        &swapchain->render_pass
    );

    if (res != VK_SUCCESS) {
        LOG_ERROR("Failed to create render pass!");
    }
}

static void create_image_views(vk_swapchain_t *swapchain)
{
    vk_device_t *device = swapchain->device;

    u32 image_count;
    vkGetSwapchainImagesKHR(device->device, swapchain->handle, &image_count, NULL);
    swapchain->images = malloc(image_count * sizeof(VkImage));
    vkGetSwapchainImagesKHR(device->device, swapchain->handle, &image_count, swapchain->images);

    swapchain->image_views = malloc(image_count * sizeof(VkImageView));
    for (u32 i = 0; i < image_count; i++) {
        VkImageViewCreateInfo create_info = {0};
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = swapchain->images[i];
        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = swapchain->format;
        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;
        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;

        VkResult res = vkCreateImageView(
            device->device,
            &create_info,
            NULL,
            &swapchain->image_views[i]
        );

        if (res != VK_SUCCESS) {
            LOG_ERROR("Failed to create image views!");
        }
    }
}

static void create_swapchain(vk_swapchain_t *swapchain)
{
    vk_device_t *device = swapchain->device;

    swapchain_support_details_t details = {0};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        device->physical_device,
        device->surface,
        &details.capabilities
    );

    vkGetPhysicalDeviceSurfaceFormatsKHR(
        device->physical_device,
        device->surface,
        &details.format_count,
        NULL
    );

    details.formats = malloc(
        details.format_count * sizeof(VkSurfaceFormatKHR)
    );

    vkGetPhysicalDeviceSurfaceFormatsKHR(
        device->physical_device,
        device->surface,
        &details.format_count,
        details.formats
    );

    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device->physical_device,
        device->surface,
        &details.present_mode_count,
        NULL
    );

    details.present_modes = malloc(
        details.present_mode_count * sizeof(VkPresentModeKHR)
    );

    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device->physical_device,
        device->surface,
        &details.present_mode_count,
        details.present_modes
    );

    VkSurfaceFormatKHR format = details.formats[0];
    for (u32 i = 0; i < details.format_count; i++) {
        if (
            details.formats[i].format == VK_FORMAT_B8G8R8A8_SRGB &&
            details.formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
        ) {
            format = details.formats[i];
            break;
        }
    }

    VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
    for (u32 i = 0; i < details.present_mode_count; i++) {
        if (details.present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        }
    }

    int width, height;
    glfwGetFramebufferSize(device->window->handle, &width, &height);

    VkExtent2D extent = {0};
    if (details.capabilities.currentExtent.width != UINT32_MAX) {
        extent = details.capabilities.currentExtent;
    } else {
        extent.width = width;
        extent.height = height;
    }

    u32 image_count = details.capabilities.minImageCount + 1;
    if (details.capabilities.maxImageCount > 0 && image_count > details.capabilities.maxImageCount) {
        image_count = details.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info = {0};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = device->surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = format.format;
    create_info.imageColorSpace = format.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    queue_family_indices_t indices = device->indices;
    u32 queue_family_indices[] = {
        indices.graphics_family,
        indices.present_family
    };

    if (indices.graphics_family != indices.present_family) {
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    create_info.preTransform = details.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    VkResult res = vkCreateSwapchainKHR(
        device->device,
        &create_info, NULL,
        &swapchain->handle
    );

    if (res != VK_SUCCESS) {
        LOG_ERROR("Failed to create swapchain!");
    }

    swapchain->format = format.format;
    swapchain->extent = extent;
    swapchain->image_count = image_count;

    free(details.formats);
    free(details.present_modes);
}

vk_swapchain_t *vk_swapchain_create(vk_device_t *device)
{
    vk_swapchain_t *swapchain = malloc(sizeof(vk_swapchain_t));
    if (!swapchain) {
        return NULL;
    }

    swapchain->device = device;

    create_swapchain(swapchain);
    create_image_views(swapchain);
    create_depth_resources(swapchain);
    create_renderpass(swapchain);
    create_framebuffers(swapchain);
    create_command_buffer(swapchain);
    create_sync_objects(swapchain);

    return swapchain;
}

static void cleanup_swapchain(vk_swapchain_t *swapchain)
{
    vk_device_t *device = swapchain->device;

    vkDeviceWaitIdle(device->device);

    for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device->device, swapchain->render_finished[i], NULL);
        vkDestroySemaphore(device->device, swapchain->image_available[i], NULL);
        vkDestroyFence(device->device, swapchain->in_flight_fences[i], NULL);
    }

    free(swapchain->render_finished);
    free(swapchain->image_available);
    free(swapchain->in_flight_fences);

    for (u32 i = 0; i < swapchain->image_count; i++) {
        vkDestroyFramebuffer(
            device->device,
            swapchain->framebuffers[i],
            NULL
        );
    }

    free(swapchain->framebuffers);

    vkFreeCommandBuffers(
        device->device,
        device->command_pool,
        swapchain->image_count,
        swapchain->command_buffers
    );

    free(swapchain->command_buffers);

    vkDestroyRenderPass(
        device->device,
        swapchain->render_pass,
        NULL
    );

    for (u32 i = 0; i < swapchain->image_count; i++) {
        vk_image_destroy(swapchain->depth_images[i]);
    }

    free(swapchain->depth_images);

    for (u32 i = 0; i < swapchain->image_count; i++) {
        vkDestroyImageView(
            device->device,
            swapchain->image_views[i],
            NULL
        );
    }

    free(swapchain->image_views);
    free(swapchain->images);

    vkDestroySwapchainKHR(device->device, swapchain->handle, NULL);
}

void vk_swapchain_destroy(vk_swapchain_t *swapchain)
{
    if (!swapchain) {
        return;
    }

    cleanup_swapchain(swapchain);
    
    free(swapchain);
}

static void recreate_swapchain(vk_swapchain_t *swapchain)
{
    vk_device_t *device = swapchain->device;

    vkDeviceWaitIdle(device->device);

    int width, height;
    glfwGetFramebufferSize(device->window->handle, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(device->window->handle, &width, &height);
        glfwWaitEvents();
    }

    cleanup_swapchain(swapchain);

    create_swapchain(swapchain);
    create_image_views(swapchain);
    create_depth_resources(swapchain);
    create_renderpass(swapchain);
    create_framebuffers(swapchain);
    create_command_buffer(swapchain);
    create_sync_objects(swapchain);

    swapchain->current_frame = 0;
}

bool begin_frame(vk_swapchain_t *swapchain)
{
    vk_device_t *device = swapchain->device;

    vkDeviceWaitIdle(device->device);

    vkWaitForFences(
        device->device,
        1,
        &swapchain->in_flight_fences[swapchain->current_frame],
        VK_TRUE,
        UINT64_MAX
    );

    VkResult res = vkAcquireNextImageKHR(
        device->device,
        swapchain->handle,
        UINT64_MAX,
        swapchain->image_available[swapchain->current_frame],
        VK_NULL_HANDLE,
        &swapchain->image_index
    );

    u32 image_index = swapchain->image_index;

    if (
        res == VK_ERROR_OUT_OF_DATE_KHR ||
        device->window->resized
    ) {
        device->window->resized = false;
        recreate_swapchain(swapchain);
        return false;
    } else if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) {
        LOG_ERROR("Failed to acquire swapchain image!");
    }

    vkResetFences(
        device->device,
        1,
        &swapchain->in_flight_fences[swapchain->current_frame]
    );

    VkCommandBuffer command_buffer = swapchain->command_buffers[image_index];

    vkResetCommandBuffer(command_buffer, 0);

    VkCommandBufferBeginInfo begin_info = {0};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    res = vkBeginCommandBuffer(
        command_buffer,
        &begin_info
    );

    if (res != VK_SUCCESS) {
        LOG_ERROR("Failed to begin recording command buffer!");
    }

    VkClearValue clear_values[2] = {0};

    clear_values[0].color = (VkClearColorValue){{0.0f, 0.0f, 0.0f, 1.0f}};
    clear_values[1].depthStencil = (VkClearDepthStencilValue){1.0f, 0};

    VkRenderPassBeginInfo render_pass_info = {0};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass = swapchain->render_pass;
    render_pass_info.framebuffer = swapchain->framebuffers[image_index];
    render_pass_info.renderArea.offset = (VkOffset2D){0, 0};
    render_pass_info.renderArea.extent = swapchain->extent;
    render_pass_info.clearValueCount = 2;
    render_pass_info.pClearValues = clear_values;
    
    vkCmdBeginRenderPass(
        command_buffer,
        &render_pass_info,
        VK_SUBPASS_CONTENTS_INLINE
    );

    VkViewport viewport = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float)swapchain->extent.width,
        .height = (float)swapchain->extent.height,
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };
    
    VkRect2D scissor = {
        .offset = {0, 0},
        .extent = swapchain->extent
    };

    vkCmdSetViewport(command_buffer, 0, 1, &viewport);
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);

    return true;
}

void end_frame(vk_swapchain_t *swapchain)
{
    vk_device_t *device = swapchain->device;

    vkCmdEndRenderPass(swapchain->command_buffers[swapchain->image_index]);

    VkResult res = vkEndCommandBuffer(
        swapchain->command_buffers[swapchain->image_index]
    );

    if (res != VK_SUCCESS) {
        LOG_ERROR("Failed to end recording command buffer!");
    }

    VkSemaphore wait_semaphores[] = {
        swapchain->image_available[swapchain->current_frame]
    };

    VkSemaphore signal_semaphores[] = {
        swapchain->render_finished[swapchain->current_frame]
    };

    VkPipelineStageFlags wait_stages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    VkSubmitInfo submit_info = {0};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &swapchain->command_buffers[swapchain->image_index];
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    if (vkQueueSubmit(
        device->graphics_queue,
        1,
        &submit_info,
        swapchain->in_flight_fences[swapchain->current_frame]
    ) != VK_SUCCESS) {
        LOG_ERROR("Failed to submit draw command buffer!");
    }

    VkSwapchainKHR swapchains[] = {
        swapchain->handle
    };

    VkPresentInfoKHR present_info = {0};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphores;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swapchains;
    present_info.pImageIndices = &swapchain->image_index;

    res = vkQueuePresentKHR(swapchain->device->present_queue, &present_info);

    if (
        res == VK_ERROR_OUT_OF_DATE_KHR ||
        res == VK_SUBOPTIMAL_KHR ||
        device->window->resized
    ) {
        device->window->resized = false;
        recreate_swapchain(swapchain);
    } else if (res != VK_SUCCESS) {
        LOG_ERROR("Failed to present swapchain image!");
    }

    swapchain->current_frame = (swapchain->current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}
