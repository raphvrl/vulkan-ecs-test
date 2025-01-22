#pragma once

#include <vulkan/vulkan.h>

#include <stdlib.h>

#include "core/types.h"
#include "vk_swapchain.h"

typedef struct vk_pipeline {
    vk_swapchain_t *swapchain;

    VkPipelineLayout layout;
    VkPipeline handle;

    VkShaderModule vertex;
    VkShaderModule fragment;
} vk_pipeline_t;

typedef struct vk_pipeline_vertex_input_t {
    VkVertexInputBindingDescription binding[8];
    u32 binding_count;
    VkVertexInputAttributeDescription attribute[8];
    u32 attribute_count;
} vk_pipeline_vertex_input_t;

typedef struct vk_pipeline_push_constant_t {
    VkShaderStageFlags stage;
    u32 offset;
    u32 size;
} vk_pipeline_push_constant_t;

typedef struct vk_pipeline_info {
    char *vertex;
    char *fragment;
    vk_pipeline_vertex_input_t input;
    u32 push_constant_count;
    vk_pipeline_push_constant_t push_constants[8];
    VkDescriptorSetLayout descriptor_layouts;
} vk_pipeline_info_t;

// constructor and destructor
vk_pipeline_t *vk_pipeline_create(
    vk_swapchain_t *swapchain,
    vk_pipeline_info_t info
);

void vk_pipeline_destroy(vk_pipeline_t *pipeline);

// actions
void vk_pipeline_bind(vk_pipeline_t *pipeline);

void vk_pipeline_push_constant(
    vk_pipeline_t *pipeline,
    VkShaderStageFlags stage,
    u32 offset,
    u32 size,
    const void *data
);