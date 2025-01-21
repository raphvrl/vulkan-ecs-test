#include "ecs/ecs.h"

void s_physics_tick(ecs_t *ecs, f32 dt)
{
    u32 count = ecs_get_count(ecs, C_VELOCITY);
    for (u32 i = 0; i < count; i++) {
        if (!ecs_is_active(ecs, i)) { continue; }

        if (ecs_has(ecs, i, C_VELOCITY) && ecs_has(ecs, i, C_TRANSFORM)) {
            c_velocity_t *velocity = ecs_get(ecs, i, C_VELOCITY);
            c_transform_t *transform = ecs_get(ecs, i, C_TRANSFORM);

            vec3 vel = {0};
            glm_vec3_scale(velocity->pos, dt, vel);

            glm_vec3_add(transform->pos, vel, transform->pos);
        }
    }
}