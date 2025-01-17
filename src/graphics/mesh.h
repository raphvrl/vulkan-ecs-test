#pragma once

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "vulkan/vk_swapchain.h"
#include "vulkan/vk_buffer.h"

typedef struct vertex {
    v3 pos;
    v3 normal;
    v2 uv;
} vertex_t;

// main struct
typedef struct mesh {
    vk_swapchain_t *swapchain;

    vk_buffer_t *vertex_buffer;
    vk_buffer_t *index_buffer;
    u32 vertex_count;
    u32 index_count;
    VkVertexInputBindingDescription binding_description;
    VkVertexInputAttributeDescription attribute_descriptions[3];
} mesh_t;

// constructor and destructor
mesh_t *mesh_create(
    vk_swapchain_t *swapchain,
    vertex_t *vertices,
    u32 vertex_count, 
    u32 *indices,
    u32 index_count
);

void mesh_destroy(mesh_t *mesh);

// actions
void mesh_bind(mesh_t *mesh);
void mesh_draw(mesh_t *mesh);

// helpers
VkVertexInputBindingDescription mesh_get_binding_description();
VkVertexInputAttributeDescription *mesh_get_attribute_descriptions();