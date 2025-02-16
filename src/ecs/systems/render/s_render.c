#include "ecs/ecs.h"
#include "graphics/cimgui/cimgui_ctx.h"

static void draw_mesh(ecs_t *ecs, pipeline_manager_t *pipeline_manager)
{
    m4 view = GLM_MAT4_IDENTITY_INIT;
    m4 proj = GLM_MAT4_IDENTITY_INIT;

    u32 count = ecs_get_count(ecs, C_CAMERA);
    for (u32 i = 0; i < count; i++) {
        if (!ecs_is_active(ecs, i)) { continue; }

        if (ecs_has(ecs, i, C_CAMERA)) {
            c_camera_t *camera = ecs_get(ecs, i, C_CAMERA);

            memcpy(view, camera->view, sizeof(m4));
            memcpy(proj, camera->proj, sizeof(m4));
        }
    }

    pipeline_manager_bind(pipeline_manager, MAIN_PIPELINE);

    count = ecs_get_count(ecs, C_MODEL);
    for (u32 i = 0; i < count; i++) {
        if (!ecs_is_active(ecs, i)) { continue; }

        if (ecs_has(ecs, i, C_MODEL)) {
            c_model_t *model = ecs_get(ecs, i, C_MODEL);
            c_transform_t *transform = ecs_get(ecs, i, C_TRANSFORM);

            m4 model_matrix = GLM_MAT4_IDENTITY_INIT;
            m4 mvp = GLM_MAT4_IDENTITY_INIT;

            glm_translate(model_matrix, transform->pos);
            glm_rotate(
                model_matrix,
                glm_rad(transform->rotate[0]),
                (v3){1.0f, 0.0f, 0.0f}
            );

            glm_rotate(
                model_matrix,
                glm_rad(transform->rotate[1]),
                (v3){0.0f, 1.0f, 0.0f}
            );

            glm_rotate(
                model_matrix,
                glm_rad(transform->rotate[2]),
                (v3){0.0f, 0.0f, 1.0f}
            );

            glm_scale(model_matrix, transform->scale);

            glm_mat4_mul(proj, view, mvp);
            glm_mat4_mul(mvp, model_matrix, mvp);

            mvp_pc_t data = {0};
            memcpy(data.mvp, mvp, sizeof(m4));

            pipeline_manager_push_constant(
                pipeline_manager,
                MAIN_PIPELINE,
                VK_SHADER_STAGE_VERTEX_BIT,
                0,
                sizeof(mvp_pc_t),
                &data
            );

            texture_bind(
                model->texture,
                pipeline_manager->pipelines[MAIN_PIPELINE]
            );

            mesh_bind(model->mesh);
            mesh_draw(model->mesh);
        }
    }
}

static void draw_gui(ecs_t *ecs, pipeline_manager_t *pipeline_manager)
{
    UNUSED(pipeline_manager);

    vk_swapchain_t *swapchain = pipeline_manager->swapchain;
    VkCommandBuffer command_buffer = swapchain_get_buffer(swapchain);

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();

    gui_t *gui = ecs->gui;
    gui_render(gui);

    igRender();
    ImGui_ImplVulkan_RenderDrawData(igGetDrawData(), command_buffer, VK_NULL_HANDLE);
}

void s_render_tick(ecs_t *ecs, f32 dt)
{
    UNUSED(dt);

    pipeline_manager_t *pipeline_manager = ecs->pipeline_manager;
    vk_swapchain_t *swapchain = pipeline_manager->swapchain;

    if (!begin_frame(swapchain)) { return; };

    draw_mesh(ecs, pipeline_manager);
    draw_gui(ecs, pipeline_manager);

    end_frame(swapchain);
}