#include "mesh.h"

static void create_vertex_buffer(
    mesh_t *mesh,
    vertex_t *vertices,
    u32 vertex_count
) {
    VkDeviceSize buffer_size = sizeof(vertex_t) * vertex_count;

    vk_buffer_t *staging = vk_buffer_create(
        mesh->swapchain->device,
        buffer_size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    void *data;
    vkMapMemory(
        mesh->swapchain->device->device,
        staging->memory,
        0,
        buffer_size,
        0,
        &data
    );
    memcpy(data, vertices, buffer_size);
    vkUnmapMemory(mesh->swapchain->device->device, staging->memory);

    mesh->vertex_buffer = vk_buffer_create(
        mesh->swapchain->device,
        buffer_size,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    vk_buffer_copy(
        staging,
        mesh->vertex_buffer,
        buffer_size
    );

    vk_buffer_destroy(staging);
}

static void create_index_buffer(
    mesh_t *mesh,
    u32 *indices,
    u32 index_count
) {
    VkDeviceSize buffer_size = sizeof(u32) * index_count;

    vk_buffer_t *staging = vk_buffer_create(
        mesh->swapchain->device,
        buffer_size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    void *data;
    vkMapMemory(
        mesh->swapchain->device->device,
        staging->memory,
        0,
        buffer_size,
        0,
        &data
    );
    memcpy(data, indices, buffer_size);
    vkUnmapMemory(mesh->swapchain->device->device, staging->memory);

    mesh->index_buffer = vk_buffer_create(
        mesh->swapchain->device,
        buffer_size,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    vk_buffer_copy(
        staging,
        mesh->index_buffer,
        buffer_size
    );

    vk_buffer_destroy(staging);
}

mesh_t *mesh_create(
    vk_swapchain_t *swapchain,
    vertex_t *vertices,
    u32 vertex_count,
    u32 *indices,
    u32 index_count
) {
    mesh_t *mesh = malloc(sizeof(mesh_t));
    if (!mesh) {
        return NULL;
    }

    mesh->swapchain = swapchain;
    mesh->vertex_count = vertex_count;
    mesh->index_count = index_count;

    mesh->binding_description = mesh_get_binding_description();
    memcpy(
        mesh->attribute_descriptions,
        mesh_get_attribute_descriptions(),
        sizeof(mesh->attribute_descriptions)
    );

    create_vertex_buffer(mesh, vertices, vertex_count);
    create_index_buffer(mesh, indices, index_count);

    return mesh;
}

void mesh_destroy(mesh_t *mesh)
{
    if (!mesh) {
        return;
    }

    vk_device_t *device = mesh->swapchain->device;

    vkDeviceWaitIdle(device->device);

    vk_buffer_destroy(mesh->vertex_buffer);
    vk_buffer_destroy(mesh->index_buffer);
    free(mesh);
}

void mesh_bind(mesh_t *mesh)
{
    VkBuffer vertex_buffers[] = {mesh->vertex_buffer->handle};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(
        mesh->swapchain->command_buffers[mesh->swapchain->image_index],
        0,
        1,
        vertex_buffers,
        offsets
    );

    vkCmdBindIndexBuffer(
        mesh->swapchain->command_buffers[mesh->swapchain->image_index],
        mesh->index_buffer->handle,
        0,
        VK_INDEX_TYPE_UINT32
    );
}

void mesh_draw(mesh_t *mesh)
{
    vk_swapchain_t *swapchain = mesh->swapchain;

    vkCmdDrawIndexed(
        swapchain->command_buffers[swapchain->image_index],
        mesh->index_count,
        1,
        0,
        0,
        0
    );
}

VkVertexInputBindingDescription mesh_get_binding_description()
{
    VkVertexInputBindingDescription binding_description = {0};
    binding_description.binding = 0;
    binding_description.stride = sizeof(vertex_t);
    binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return binding_description;
}

VkVertexInputAttributeDescription *mesh_get_attribute_descriptions()
{
    static VkVertexInputAttributeDescription attribute_descriptions[3] = {0};

    attribute_descriptions[0].binding = 0;
    attribute_descriptions[0].location = 0;
    attribute_descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attribute_descriptions[0].offset = offsetof(vertex_t, pos);

    attribute_descriptions[1].binding = 0;
    attribute_descriptions[1].location = 1;
    attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attribute_descriptions[1].offset = offsetof(vertex_t, normal);

    attribute_descriptions[2].binding = 0;
    attribute_descriptions[2].location = 2;
    attribute_descriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attribute_descriptions[2].offset = offsetof(vertex_t, uv);

    return attribute_descriptions;
}