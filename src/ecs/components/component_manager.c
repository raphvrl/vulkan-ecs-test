#include "component_manager.h"

component_manager_t *component_manager_create()
{
    component_manager_t *manager = malloc(sizeof(component_manager_t));
    if (!manager) {
        return NULL;
    }

    manager->transforms = calloc(MAX_ENTITIES, sizeof(cmp_transform_t));
    manager->velocities = calloc(MAX_ENTITIES, sizeof(cmp_velocity_t));
    manager->models = calloc(MAX_ENTITIES, sizeof(cmp_model_t));

    if (!manager->transforms || !manager->velocities) {
        component_manager_destroy(manager);
        return NULL;
    }

    return manager;
}

void component_manager_destroy(component_manager_t *manager)
{
    if (manager == NULL) { return; }

    free(manager->models);
    free(manager->transforms);
    free(manager->velocities);
    free(manager);

    manager = NULL;
}