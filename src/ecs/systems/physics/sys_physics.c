#include "sys_physics.h"
#include "ecs/ecs.h"

void sys_physics_update(ecs_t *ecs, f32 dt)
{
    registry_t *registry = ecs->registry;
    component_manager_t *component_manager = ecs->component_manager;

    for (u32 i = 0; i < MAX_ENTITIES; i++) {
        if (!registry->entities[i].active) { continue; }

        if (registry->entities[i].mask & (COMPONENT_TRANSFORM | COMPONENT_VELOCITY)) {
            cmp_velocity_t *velocity = &component_manager->velocities[i];
            cmp_transform_t *transform = &component_manager->transforms[i];

            transform->position[0] += velocity->velocity[0] * dt;
            transform->position[1] += velocity->velocity[1] * dt;
            transform->position[2] += velocity->velocity[2] * dt;
        }
    }
}