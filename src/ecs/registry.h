#pragma once

#include <stdlib.h>
#include <string.h>

#include "core/types.h"

// constants
#define MAX_ENTITIES 300
#define INVALID_ENTITY U32_MAX

// entity
typedef struct entity {
    u32 id;
    u32 mask;
    bool active;
} entity_t;

// main struct
typedef struct registry {
    entity_t *entities;
    u32 next_id;
    u32 count;
} registry_t;

// constructor and destructor
registry_t *registry_create();
void registry_destroy(registry_t *registry);

// actions
u32 registry_create_entity(registry_t *registry);
void registry_destroy_entity(registry_t *registry, u32 id);

void registry_add_component(registry_t *registry, u32 id, u32 component);
void registry_remove_component(registry_t *registry, u32 id, u32 component);