#include "physics_world.h"

physics_world_t *physics_world_create() {
    physics_world_t *world = malloc(sizeof(physics_world_t));
    if (!world) {
        return NULL;
    }

    world->universe = universeCreate();

    return world;
}

void physics_world_destroy(physics_world_t *world) {
    if (!world) {
        return;
    }

    universeDestroy(world->universe);
    free(world);
}

void physics_world_update(physics_world_t *world, float dt) {
    universeStep(world->universe, dt, 8);
}