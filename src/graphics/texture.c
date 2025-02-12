#include "texture.h"

texture_t *texture_create(vk_swapchain_t *swapchain, const char *path)
{
    texture_t *texture = malloc(sizeof(texture_t));
    if (!texture) {
        return NULL;
    }

    texture->swapchain = swapchain;

    int w, height, channels;
    unsigned char *pixel = stbi_load(
        path,
        &w,
        &height,
        &channels,
        STBI_rgb_alpha
    );

    if (!pixel) {
        LOG_ERROR("Failed to load texture!");
        return NULL;
    }

    VkDeviceSize image_size = w * height * 4;

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
        w,
        height,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT
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
    alloc_info.pSetLayouts = &device->texture_layout;

    vkAllocateDescriptorSets(
        device->device,
        &alloc_info,
        &texture->descriptor
    );

    VkDescriptorImageInfo image_info = {0};
    image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    image_info.imageView = texture->image->view;
    image_info.sampler = texture->sampler;

    VkWriteDescriptorSet write = {0};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet = texture->descriptor;
    write.dstBinding = 0;
    write.dstArrayElement = 0;
    write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    write.descriptorCount = 1;
    write.pImageInfo = &image_info;

    vkUpdateDescriptorSets(device->device, 1, &write, 0, NULL);

    vk_buffer_destroy(staging);
    stbi_image_free(pixel);

    return texture;
}

void texture_destroy(texture_t *texture)
{
    if (!texture) { return; }

    vk_device_t *device = texture->swapchain->device;

    vkDeviceWaitIdle(device->device);

    vkDestroySampler(device->device, texture->sampler, NULL);
    vk_image_destroy(texture->image);

    free(texture);
}

void texture_bind(texture_t *texture, vk_pipeline_t *pipeline)
{
    if (!texture) { return; }

    VkCommandBuffer cmd = swapchain_get_buffer(texture->swapchain);

    vkCmdBindDescriptorSets(
        cmd,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipeline->layout,
        0,
        1,
        &texture->descriptor,
        0,
        NULL
    );
}