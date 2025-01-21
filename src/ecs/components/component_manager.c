#include "component_manager.h"

component_manager_t *component_manager_create()
{
    component_manager_t *manager = malloc(sizeof(component_manager_t));
    if (!manager) {
        return NULL;
    }

    _init_component(manager, c_transform_t, C_TRANSFORM);
    _init_component(manager, c_velocity_t, C_VELOCITY);
    _init_component(manager, c_model_t, C_MODEL);
    _init_component(manager, c_camera_t, C_CAMERA);

    return manager;
}

void component_manager_destroy(component_manager_t *manager)
{
    if (manager == NULL) { return; }

    for (usize i = 0; i < COMPONENT_LIST_NBR; i++) {
        free(manager->lists[i].data);
        manager->lists[i].data = NULL;
    }

    free(manager);
    manager = NULL;
}

void component_manager_add(
    component_manager_t *manager,
    u32 id,
    component_type_e component,
    void *data
)
{
    component_list_t *list = &manager->lists[component];
    char *dest = (char *)list->data + (list->size * id);
    memcpy(dest, data, list->size);

    if (id >= list->count) {
        list->count = id + 1;
    }
}

void *component_manager_get(
    component_manager_t *manager,
    u32 id,
    component_type_e component
)
{
    component_list_t *list = &manager->lists[component];
    return (char *)list->data + (list->size * id);
}