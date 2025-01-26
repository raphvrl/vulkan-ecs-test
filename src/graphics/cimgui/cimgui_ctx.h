#pragma once

#include "core/types.h"
#include "graphics/vulkan/vk_swapchain.h"

typedef struct imgui_ctx {
    VkDescriptorPool descriptor_pool;
    bool initialized;
} imgui_ctx_t;

imgui_ctx_t *imgui_ctx_creatr(vk_swapchain_t *swapchain);
void imgui_ctx_destroy(imgui_ctx_t *ctx);