#include "sys_render.h"
#include "ecs/ecs.h"

void sys_render_update(ecs_t *ecs, f32 dt)
{
    UNUSED(dt);

    registry_t *registry = ecs->registry;
    component_manager_t *component_manager = ecs->component_manager;
    pipeline_manager_t *pipeline_manager = ecs->pipeline_manager;
    vk_swapchain_t *swapchain = pipeline_manager->swapchain;

    begin_frame(swapchain);

    for (u32 i = 0; i < MAX_ENTITIES; i++) {
        if (!registry->entities[i].active) { continue; }

        bool has_transform = registry->entities[i].mask & COMPONENT_TRANSFORM;
        bool has_model = registry->entities[i].mask & COMPONENT_MODEL;

        if (has_transform && has_model) {

            cmp_model_t *model = &component_manager->models[i];

            pipeline_manager_bind(pipeline_manager, MAIN_PIPELINE);

            mesh_bind(model->mesh);
            mesh_draw(model->mesh);
        }
    }

    end_frame(swapchain);
}