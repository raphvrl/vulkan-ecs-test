#include "ecs/ecs.h"

void s_camera_tick(ecs_t *ecs, f32 dt)
{
    UNUSED(dt);

    registry_t *registry = ecs->registry;
    
    u32 count = ecs_get_count(ecs, C_CAMERA);
    for (u32 i = 0; i < count; i++) {
        if (!registry->entities[i].active) { continue; }

        if (
            ecs_has(ecs, i, C_CAMERA) &&
            ecs_has(ecs, i, C_TRANSFORM)
        ) {
            c_transform_t *transform = ecs_get(ecs, i, C_TRANSFORM);
            c_camera_t *camera = ecs_get(ecs, i, C_CAMERA);

            camera->pos[0] = transform->pos[0];
            camera->pos[1] = transform->pos[1];
            camera->pos[2] = transform->pos[2];
            
            camera->aspect = (f32)ecs->window->width / (f32)ecs->window->height;

            v3 front;
            front[0] = cosf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
            front[1] = sinf(glm_rad(camera->pitch));
            front[2] = sinf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
            glm_vec3_normalize_to(front, camera->front);

            v3 right;
            glm_vec3_crossn(camera->front, (v3){0.0f, 1.0f, 0.0f}, right);
            glm_vec3_normalize_to(right, camera->right);

            v3 up;
            glm_vec3_crossn(camera->right, camera->front, up);
            glm_vec3_normalize_to(up, camera->up);

            glm_look(camera->pos, camera->front, camera->up, camera->view);

            glm_perspective(
                glm_rad(camera->fov),
                camera->aspect,
                camera->near_plane,
                camera->far_plane,
                camera->proj
            );

            // flip the y axis for vulkan
            camera->proj[1][1] *= -1.0f;
        }
    }
}