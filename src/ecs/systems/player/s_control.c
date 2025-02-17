#include "ecs/ecs.h"

#define CAMERA_SPEED 5.0f
#define SENSITIVITY 0.1f

#define WORLD_UP (v3){0.0f, 1.0f, 0.0f}

static void mouse_control(c_camera_t *camera, f32 mx, f32 my)
{
    camera->yaw += mx * SENSITIVITY;
    camera->pitch -= my * SENSITIVITY;

    if (camera->pitch > 89.0f) {
        camera->pitch = 89.0f;
    }

    if (camera->pitch < -89.0f) {
        camera->pitch = -89.0f;
    }
}

static void player_control(ecs_t *ecs, u32 i)
{
    if (!ecs_is_active(ecs, i)) { return; }

    if (ecs_has(ecs, i, C_CAMERA) && ecs_has(ecs, i, C_VELOCITY)) {
        c_camera_t *camera = ecs_get(ecs, i, C_CAMERA);
        c_camera_t *velocity = ecs_get(ecs, i, C_VELOCITY);

        glm_vec3_zero(velocity->pos);

        v3 front;
        front[0] = camera->front[0];
        front[1] = 0.0f;
        front[2] = camera->front[2];

        if (ecs->window->keys[K_W]) {
            glm_vec3_add(velocity->pos, front, velocity->pos);
        }

        if (ecs->window->keys[K_S]) {
            glm_vec3_sub(velocity->pos, front, velocity->pos);
        }

        if (ecs->window->keys[K_A]) {
            glm_vec3_sub(velocity->pos, camera->right, velocity->pos);
        }

        if (ecs->window->keys[K_D]) {
            glm_vec3_add(velocity->pos, camera->right, velocity->pos);
        }

        if (ecs->window->keys[K_SPACE]) {
            glm_vec3_add(velocity->pos, WORLD_UP, velocity->pos);
        }

        if (ecs->window->keys[K_LEFT_SHIFT]) {
            glm_vec3_sub(velocity->pos, WORLD_UP, velocity->pos);
        }

        glm_vec3_normalize(velocity->pos);
        glm_vec3_scale(velocity->pos, CAMERA_SPEED, velocity->pos);

        if (ecs->window->mouses[M_BUTTON_RIGHT]) {
            mouse_control(camera, ecs->window->rdx, ecs->window->rdy);
            window_mouse_hide(ecs->window);
        } else {
            window_mouse_show(ecs->window);
        }

        if (ecs->window->keys[K_ESCAPE]) {
            window_close(ecs->window);
        }
    }
}

static void editor_control(ecs_t *ecs)
{
    if (window_key_down(ecs->window, K_F1)) {
        SWITCH_BOOL(&ecs->gui->entity_list.active);
    }

    if (ecs->window->keys[K_LEFT_ALT] && window_key_down(ecs->window, K_ENTER)) {
        window_toggle_fullscreen(ecs->window);
    }
}

void s_control_tick(ecs_t *ecs, f32 dt)
{
    UNUSED(dt);
 
    u32 count = ecs_get_count(ecs, C_CAMERA);
    for (u32 i = 0; i < count; i++) {
        player_control(ecs, i);
    }

    editor_control(ecs);
}