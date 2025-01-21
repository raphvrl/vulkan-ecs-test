#include "pipeline_manager.h"
#include "mesh.h"

static void create_main_pipeline(pipeline_manager_t *manager)
{
    vk_pipeline_t *pipeline = vk_pipeline_create(
        manager->swapchain,
        (vk_pipeline_info_t){
            .vertex = "bin/default.vert.spv",
            .fragment = "bin/default.frag.spv",
            .input = {
                .binding = {
                    {
                        .binding = 0,
                        .stride = sizeof(vertex_t),
                        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
                    }
                },
                .binding_count = 1,
                .attribute = {
                    {
                        .location = 0,
                        .binding = 0,
                        .format = VK_FORMAT_R32G32B32_SFLOAT,
                        .offset = offsetof(vertex_t, pos)
                    },
                    {
                        .location = 1,
                        .binding = 0,
                        .format = VK_FORMAT_R32G32B32_SFLOAT,
                        .offset = offsetof(vertex_t, normal)
                    },
                    {
                        .location = 2,
                        .binding = 0,
                        .format = VK_FORMAT_R32G32_SFLOAT,
                        .offset = offsetof(vertex_t, uv)
                    }
                },
                .attribute_count = 3
            },
            .push_constant_count = 1,
            .push_constants = {
                {
                    .stage = VK_SHADER_STAGE_VERTEX_BIT,
                    .offset = 0,
                    .size = sizeof(mvp_pc_t)
                }
            }
        }
    );

    manager->pipelines[MAIN_PIPELINE] = pipeline;
}

pipeline_manager_t *pipeline_manager_create(vk_swapchain_t *swapchain)
{
    pipeline_manager_t *manager = malloc(sizeof(pipeline_manager_t));
    if (!manager) {
        return NULL;
    }

    manager->swapchain = swapchain;
    manager->pipelines = calloc(1, sizeof(vk_pipeline_t *));
    if (!manager->pipelines) {
        pipeline_manager_destroy(manager);
        return NULL;
    }

    create_main_pipeline(manager);

    return manager;
}

void pipeline_manager_destroy(pipeline_manager_t *manager)
{
    vk_pipeline_destroy(manager->pipelines[0]);

    free(manager->pipelines);
    free(manager);
}

void pipeline_manager_bind(pipeline_manager_t *manager, pipeline_type_e type)
{
    vk_pipeline_bind(manager->pipelines[type]);
}

void pipeline_manager_push_constant(
    pipeline_manager_t *manager,
    pipeline_type_e type,
    VkShaderStageFlags stage,
    u32 offset,
    u32 size,
    void *data
)
{
    vk_pipeline_push_constant(
        manager->pipelines[type],
        stage,
        offset,
        size,
        data
    );
}