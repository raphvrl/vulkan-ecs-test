#pragma once

#include <stb_image.h>

#include "core/types.h"
#include "vulkan/vk_buffer.h"
#include "vulkan/vk_image.h"
#include "vulkan/vk_swapchain.h"
#include "vulkan/vk_pipeline.h"

// main struct
typedef struct texture {
    vk_swapchain_t *swapchain;

    vk_image_t *image;
    VkSampler sampler;
    VkDescriptorSet descriptor;
} texture_t;

// constructor
texture_t *texture_create(vk_swapchain_t *swapchain, const char *path);
void texture_destroy(texture_t *texture);

void texture_bind(texture_t *texture, vk_pipeline_t *pipeline);