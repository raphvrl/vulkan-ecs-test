#include "ecs/ecs.h"

static void draw_mesh(ecs_t *ecs, pipeline_manager_t *pipeline_manager)
{
    static float rotation = 0.0f;
    rotation += 0.0005f;

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
            glm_rotate(model_matrix, rotation, (vec3){0.5f, 1.0f, 0.5f});

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

            mesh_bind(model->mesh);
            mesh_draw(model->mesh);
        }
    }
}

void s_render_tick(ecs_t *ecs, f32 dt)
{
    UNUSED(dt);

    pipeline_manager_t *pipeline_manager = ecs->pipeline_manager;
    vk_swapchain_t *swapchain = pipeline_manager->swapchain;

    begin_frame(swapchain);

    draw_mesh(ecs, pipeline_manager);

    end_frame(swapchain);
}