#include "system_manager.h"

system_manager_t *system_manager_create(
    registry_t *registry,
    component_manager_t *component_manager,
    window_t *window,
    pipeline_manager_t *pipeline_manager
)
{
    system_manager_t *manager = malloc(sizeof(system_manager_t));
    if (manager == NULL) {
        return NULL;
    }

    manager->registry = registry;
    manager->component_manager = component_manager;
    manager->window = window;
    manager->pipeline_manager = pipeline_manager;

    return manager;
}

void system_manager_destroy(system_manager_t *manager)
{
    if (manager == NULL) { return; }

    free(manager);
}

void system_manager_update(system_manager_t *manager, f32 dt)
{
    sys_physics_update(manager, dt);
    sys_render_update(manager, dt);
}
