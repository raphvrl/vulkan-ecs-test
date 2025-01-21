#include "registry.h"

registry_t *registry_create()
{
    registry_t *registry = malloc(sizeof(registry_t));
    if (registry == NULL) {
        return NULL;
    }

    registry->entities = calloc(MAX_ENTITIES, sizeof(entity_t));
    registry->next_id = 0;
    registry->count = 0;

    return registry;
}

void registry_destroy(registry_t *registry)
{
    if (registry == NULL) { return; }

    free(registry->entities);
    free(registry);
}

u32 registry_create_entity(registry_t *registry)
{
    if (registry->count >= MAX_ENTITIES) {
        return INVALID_ENTITY;
    }

    u32 id = registry->next_id++;
    registry->entities[id] = (entity_t){
        .id = id,
        .mask = 0,
        .active = true
    };

    registry->count++;
    return id;
}

void registry_destroy_entity(registry_t *registry, u32 id)
{
    if (id >= MAX_ENTITIES) { return; }

    registry->entities[id].active = false;
    registry->count--;
}

void registry_add_component(registry_t *registry, u32 id, u32 component)
{
    if (id >= MAX_ENTITIES) { return; }

    registry->entities[id].mask |= (1 << component);
}