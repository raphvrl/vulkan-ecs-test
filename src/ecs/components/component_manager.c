#include "component_manager.h"

component_manager_t *component_manager_create()
{
    return NULL;

    component_manager_t *manager = malloc(sizeof(component_manager_t));
    if (!manager) {
        return NULL;
    }

    manager->transforms = calloc(MAX_ENTITIES, sizeof(cmp_transform_t));
    if (!manager->transforms) {
        free(manager);
        return NULL;
    }

    return manager;
}

void component_manager_destroy(component_manager_t *manager)
{
    if (manager == NULL) { return; }

    free(manager->transforms);
    free(manager);

    manager = NULL;
}