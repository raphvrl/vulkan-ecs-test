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
    sys_physics_update(ecs, dt);
    sys_render_update(ecs, dt);
}
