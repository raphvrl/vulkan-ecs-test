#include "vk_pipeline.h"

static char *read_file(const char *path, u32 *size)
{
    FILE *file = fopen(path, "rb");
    if (!file) {
        LOG_ERROR("Failed to open file!");
    }

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    rewind(file);

    char *buffer = malloc(*size);
    if (!buffer) {
        LOG_ERROR("Failed to allocate buffer!");
    }

    fread(buffer, 1, *size, file);
    fclose(file);

    return buffer;
}

static VkShaderModule create_shader_module(
    VkDevice device,
    const char *code,
    u32 size
)
{
    VkShaderModuleCreateInfo create_info = {0};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = size;
    create_info.pCode = (u32 *)code;

    VkShaderModule shader_module;
    VkResult res = vkCreateShaderModule(
        device,
        &create_info,
        NULL,
        &shader_module
    );

    if (res != VK_SUCCESS) {
        LOG_ERROR("Failed to create shader module!");
    }

    return shader_module;
}

vk_pipeline_t *vk_pipeline_create(
    vk_swapchain_t *swapchain,
    vk_pipeline_info_t info
)
{
    vk_pipeline_t *pipeline = malloc(sizeof(vk_pipeline_t));
    if (!pipeline) {
        return NULL;
    }

    pipeline->swapchain = swapchain;

    u32 vertex_size;
    char *vertex_code = read_file(info.vertex, &vertex_size);
    pipeline->vertex = create_shader_module(
        swapchain->device->device,
        vertex_code,
        vertex_size
    );

    u32 fragment_size;
    char *fragment_code = read_file(info.fragment, &fragment_size);
    pipeline->fragment = create_shader_module(
        swapchain->device->device,
        fragment_code,
        fragment_size
    );

    VkPipelineShaderStageCreateInfo shader_stages[2] = {0};

    shader_stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shader_stages[0].module = pipeline->vertex;
    shader_stages[0].pName = "main";

    shader_stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shader_stages[1].module = pipeline->fragment;
    shader_stages[1].pName = "main";

    VkDynamicState dynamic_states[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };

    VkPipelineDynamicStateCreateInfo dynamic_state = {0};
    dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state.dynamicStateCount = 2;
    dynamic_state.pDynamicStates = dynamic_states;

    VkPipelineVertexInputStateCreateInfo vertex_input_info = {0};
    vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_info.vertexBindingDescriptionCount = info.input.binding_count;
    vertex_input_info.pVertexBindingDescriptions = info.input.binding;
    vertex_input_info.vertexAttributeDescriptionCount = info.input.attribute_count;
    vertex_input_info.pVertexAttributeDescriptions = info.input.attribute;

    VkPipelineInputAssemblyStateCreateInfo input_assembly = {0};
    input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkViewport viewport = {0};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (f32)swapchain->extent.width;
    viewport.height = (f32)swapchain->extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {0};
    scissor.offset = (VkOffset2D){0, 0};
    scissor.extent = swapchain->extent;

    VkPipelineViewportStateCreateInfo viewport_state = {0};
    viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount = 1;
    viewport_state.pViewports = &viewport;
    viewport_state.scissorCount = 1;
    viewport_state.pScissors = &scissor;


    VkPipelineRasterizationStateCreateInfo rasterizer = {0};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling = {0};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState color_blend_attachment = {0};
    color_blend_attachment.colorWriteMask = 
        VK_COLOR_COMPONENT_R_BIT |
        VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo color_blending = {0};
    color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blending.logicOpEnable = VK_FALSE;
    color_blending.logicOp = VK_LOGIC_OP_COPY;
    color_blending.attachmentCount = 1;
    color_blending.pAttachments = &color_blend_attachment;

    VkPipelineLayoutCreateInfo pipeline_layout_info = {0};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    VkResult res = vkCreatePipelineLayout(
        swapchain->device->device,
        &pipeline_layout_info,
        NULL,
        &pipeline->layout
    );

    if (res != VK_SUCCESS) {
        LOG_ERROR("Failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipeline_info = {0};
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount = 2;
    pipeline_info.pStages = shader_stages;
    pipeline_info.pVertexInputState = &vertex_input_info;
    pipeline_info.pInputAssemblyState = &input_assembly;
    pipeline_info.pViewportState = &viewport_state;
    pipeline_info.pRasterizationState = &rasterizer;
    pipeline_info.pMultisampleState = &multisampling;
    pipeline_info.pColorBlendState = &color_blending;
    pipeline_info.layout = pipeline->layout;
    pipeline_info.renderPass = swapchain->render_pass;
    pipeline_info.pDynamicState = &dynamic_state;
    pipeline_info.subpass = 0;

    res = vkCreateGraphicsPipelines(
        swapchain->device->device,
        VK_NULL_HANDLE,
        1,
        &pipeline_info,
        NULL,
        &pipeline->handle
    );

    if (res != VK_SUCCESS) {
        LOG_ERROR("Failed to create graphics pipeline!");
    }

    free(vertex_code);
    free(fragment_code);

    return pipeline;
}

void vk_pipeline_destroy(vk_pipeline_t *pipeline)
{
    if (!pipeline) { return; }

    vk_device_t *device = pipeline->swapchain->device;

    vkDeviceWaitIdle(device->device);

    vkDestroyShaderModule(device->device, pipeline->vertex, NULL);
    vkDestroyShaderModule(device->device, pipeline->fragment, NULL);
    vkDestroyPipeline(device->device, pipeline->handle, NULL);
    vkDestroyPipelineLayout(device->device, pipeline->layout, NULL);
}

void vk_pipeline_bind(vk_pipeline_t *pipeline)
{
    vk_swapchain_t *swapchain = pipeline->swapchain;

    vkCmdBindPipeline(
        swapchain->command_buffers[swapchain->image_index],
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipeline->handle
    );
}