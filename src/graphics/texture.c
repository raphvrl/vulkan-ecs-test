#include "texture.h"

texture_t *texture_create(vk_swapchain_t *swapchain, const char *path)
{
    texture_t *texture = malloc(sizeof(texture_t));
    if (!texture) {
        return NULL;
    }

    texture->swapchain = swapchain;

    int width, height, channels;
    unsigned char *pixel = stbi_load(
        path,
        &width,
        &height,
        &channels,
        STBI_rgb_alpha
    );

    if (!pixel) {
        LOG_ERROR("Failed to load texture!");
        return NULL;
    }

    VkDeviceSize image_size = width * height * 4;

    vk_device_t *device = swapchain->device;

    vk_buffer_t *staging = vk_buffer_create(
        device,
        image_size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
    );

    void *data;
    vkMapMemory(
        device->device,
        staging->memory,
        0,
        image_size,
        0,
        &data
    );

    memcpy(data, pixel, image_size);
    vkUnmapMemory(device->device, staging->memory);

    texture->image = vk_image_create(
        device,
        width,
        height,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    vk_image_transition_layout(
        texture->image,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
    );

    vk_image_copy_from_buffer(texture->image, staging);

    vk_image_transition_layout(
        texture->image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );

    VkSamplerCreateInfo sampler_info = {0};
    sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_info.magFilter = VK_FILTER_LINEAR;
    sampler_info.minFilter = VK_FILTER_LINEAR;
    sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    vkCreateSampler(
        device->device,
        &sampler_info,
        NULL,
        &texture->sampler
    );

    VkDescriptorSetAllocateInfo alloc_info = {0};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = device->descriptor_pool;
    alloc_info.descriptorSetCount = 1;

    vk_buffer_destroy(staging);
    stbi_image_free(pixel);

    return texture;
}

void texture_destroy(texture_t *texture)
{
    if (!texture) { return; }

    vk_device_t *device = texture->swapchain->device;

    vkDestroySampler(device->device, texture->sampler, NULL);
    vk_image_destroy(texture->image);

    free(texture);
}