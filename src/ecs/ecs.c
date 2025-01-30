#include "ecs.h"

ecs_t *ecs_create(ecs_info_t *info)
{
    ecs_t *ecs = malloc(sizeof(ecs_t));
    if (ecs == NULL) {
        return NULL;
    }

    ecs->registry = info->registry;
    ecs->component_manager = info->component_manager;
    ecs->window = info->window;
    ecs->pipeline_manager = info->pipeline_manager;

    return ecs;
}

void ecs_destroy(ecs_t *ecs)
{
    if (ecs == NULL) { return; }

    free(ecs);
}

void ecs_update(ecs_t *ecs, f32 dt)
{
    s_control_tick(ecs, dt);
    s_physics_tick(ecs, dt);
    s_render_tick(ecs, dt);
    s_camera_tick(ecs, dt);
}

void ecs_add(ecs_t *ecs, u32 id, component_type_e component, void *data)
{
    registry_add_component(ecs->registry, id, component);
    component_manager_add(ecs->component_manager, id, component, data);
}
