#include "cimgui_ctx.h"

bool cimgui_ctx_create(vk_swapchain_t* swapchain)
{
    vk_device_t *device = swapchain->device;

    igCreateContext(NULL);

    ImGuiIO *io = igGetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    if (!ImGui_ImplGlfw_InitForVulkan(device->window->handle, true)) {
        return false;
    }

    ImGui_ImplVulkan_InitInfo init_info = {
        .Instance = device->instance,
        .PhysicalDevice = device->physical_device,
        .Device = device->device,
        .QueueFamily = device->indices.graphics_family,
        .Queue = device->graphics_queue,
        .PipelineCache = VK_NULL_HANDLE,
        .DescriptorPool = device->descriptor_pool,
        .RenderPass = swapchain->render_pass,
        .Subpass = 0,
        .MinImageCount = swapchain->minimum_image_count,
        .ImageCount = swapchain->image_count,
        .MSAASamples = VK_SAMPLE_COUNT_1_BIT,
        .Allocator = NULL,
        .CheckVkResultFn = NULL,
    };

    return ImGui_ImplVulkan_Init(&init_info);
}

void cimgui_ctx_destroy()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(NULL);
}