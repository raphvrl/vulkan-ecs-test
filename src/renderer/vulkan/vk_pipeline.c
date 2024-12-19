#include "vk_pipeline.h"
#include "vk_context.h"
#include <stdlib.h>
#include <stdio.h>
#include <core/log.h>

static char *read_file(const char *path)
{
    FILE *file = fopen(path, "r");
    if (!file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);

    return buffer;
}

static VkShaderModule create_shader_module(
    VkDevice device,
    const char *path
)
{
    char *code = read_file(path);
    if (!code) {
        LERROR("Failed to read shader file: %s", path);
        return VK_NULL_HANDLE;
    }

    VkShaderModuleCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = strlen(code),
        .pCode = (uint32_t *)code
    };

    VkShaderModule shader_module;
    VkResult res = vkCreateShaderModule(
        device,
        &create_info,
        NULL,
        &shader_module
    );

    if (res != VK_SUCCESS) {
        LERROR("Failed to create shader module: %d", res);
        return VK_NULL_HANDLE;
    }

    free(code);
    return shader_module;
}

vk_pipeline_t *vulkan_pipeline_create(
    vulkan_ctx_t *ctx,
    const char *vert_path,
    const char *frag_path
)
{
    VkShaderModule vert_module = create_shader_module(
        ctx->device.device,
        vert_path
    );

    VkShaderModule frag_module = create_shader_module(
        ctx->device.device,
        frag_path
    );

    VkPipelineShaderStageCreateInfo vert_stage_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vert_module,
        .pName = "main"
    };

    VkPipelineShaderStageCreateInfo frag_stage_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = frag_module,
        .pName = "main"
    };

    VkPipelineShaderStageCreateInfo shader_stages[] = {
        vert_stage_info,
        frag_stage_info
    };

    VkPipelineVertexInputStateCreateInfo vertex_input_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 0,
        .vertexAttributeDescriptionCount = 0
    };

    VkPipelineInputAssemblyStateCreateInfo input_assembly = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    };

    VkViewport viewport = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float)ctx->swapchain.extent.width,
        .height = (float)ctx->swapchain.extent.height,
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };

    VkRect2D scissor = {
        .offset = {0, 0},
        .extent = ctx->swapchain.extent
    };

    VkPipelineViewportStateCreateInfo viewport_state = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .pViewports = &viewport,
        .scissorCount = 1,
        .pScissors = &scissor
    };

    VkPipelineRasterizationStateCreateInfo rasterizer = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .lineWidth = 1.0f,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE
    };

    VkPipelineMultisampleStateCreateInfo multisampling = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .sampleShadingEnable = VK_FALSE,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT
    };

    VkPipelineColorBlendAttachmentState color_blend_attachment = {
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                            VK_COLOR_COMPONENT_G_BIT |
                            VK_COLOR_COMPONENT_B_BIT |
                            VK_COLOR_COMPONENT_A_BIT,

        .blendEnable = VK_FALSE
    };

    VkPipelineColorBlendStateCreateInfo color_blending = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = &color_blend_attachment
    };

    VkPipelineLayoutCreateInfo pipeline_layout_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 0,
        .pushConstantRangeCount = 0
    };

    VkPipelineLayout pipeline_layout;
    VkResult res = vkCreatePipelineLayout(
        ctx->device.device,
        &pipeline_layout_info,
        NULL,
        &pipeline_layout
    );

    if (res != VK_SUCCESS) {
        LERROR("Failed to create pipeline layout: %d", res);
        return NULL;
    }

    VkGraphicsPipelineCreateInfo pipeline_info = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = 2,
        .pStages = shader_stages,
        .pVertexInputState = &vertex_input_info,
        .pInputAssemblyState = &input_assembly,
        .pViewportState = &viewport_state,
        .pRasterizationState = &rasterizer,
        .pMultisampleState = &multisampling,
        .pColorBlendState = &color_blending,
        .layout = pipeline_layout,
        .renderPass = ctx->render_pass,
        .subpass = 0
    };

    VkPipeline pipeline;
    res = vkCreateGraphicsPipelines(
        ctx->device.device,
        VK_NULL_HANDLE,
        1,
        &pipeline_info,
        NULL,
        &pipeline
    );

    if (res != VK_SUCCESS) {
        LERROR("Failed to create graphics pipeline: %d", res);
        return NULL;
    }

    vkDestroyShaderModule(ctx->device.device, vert_module, NULL);
    vkDestroyShaderModule(ctx->device.device, frag_module, NULL);

    vk_pipeline_t *vk_pipeline = malloc(sizeof(vk_pipeline_t));
    vk_pipeline->handle = pipeline;
    vk_pipeline->layout = pipeline_layout;

    return vk_pipeline;
}

void vulkan_pipeline_destroy(
    vulkan_ctx_t *ctx,
    vk_pipeline_t *pipeline
)
{
    if (!pipeline) {
        return;
    }

    vkDestroyPipeline(ctx->device.device, pipeline->handle, NULL);
    vkDestroyPipelineLayout(ctx->device.device, pipeline->layout, NULL);

    free(pipeline);
}