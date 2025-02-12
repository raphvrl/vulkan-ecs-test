#include "gui_manager.h"
#include "ecs/ecs.h"

gui_manager_t *gui_manager_create(ecs_t *ecs, asset_manager_t *asset_manager)
{
    gui_manager_t *gui_manager = malloc(sizeof(gui_manager_t));
    if (!gui_manager) {
        return NULL;
    }

    gui_manager->ecs = ecs;
    gui_manager->asset_manager = asset_manager;

    gui_manager->entity_creator = (gui_entity_creator_t){
        .visible = true,
        .pos = {0.0f, 0.0f, 0.0f},
        .rot = {0.0f, 0.0f, 0.0f},
        .scale = {1.0f, 1.0f, 1.0f},
        .has_model = false,
        .mesh_name = "cube",
        .texture_name = "dog"
    };

    return gui_manager;
}

void gui_manager_destroy(gui_manager_t *gui_manager)
{
    if (!gui_manager) {
        return;
    }

    free(gui_manager);
}

static void draw_entiy_creator(gui_manager_t *gui_manager)
{
    gui_entity_creator_t *creator = &gui_manager->entity_creator;
    asset_manager_t *asset_manager = gui_manager->asset_manager;
    ecs_t *ecs = gui_manager->ecs;

    if (!creator->visible) {
        return;
    }

    if (igBegin("Entity Creator", &creator->visible, 0)) {
        igDragFloat3("position", creator->pos, 0.1f, -1000.0f, 1000.0f, "%.1f", 0.0f);
        igDragFloat3("rotation", creator->rot, 0.1f, -1000.0f, 1000.0f, "%.1f", 0.0f);
        igDragFloat3("scale", creator->scale, 0.1f, -1000.0f, 1000.0f, "%.1f", 0.0f);

        igCheckbox("Add Model", &creator->has_model);

        if (creator->has_model) {
            const char *meshes[] = {
                "cube",
                "monkey"
            };

            if (igCombo_Str_arr(
                "Mesh",
                &creator->mesh_index,
                meshes,
                2,
                -1
            )) {
                strcpy(creator->mesh_name, meshes[creator->mesh_index]);
                printf("Mesh: %s\n", creator->mesh_name);
            }

            const char *textures[] = {
                "dog",
                "gru"
            };

            if (igCombo_Str_arr(
                "Texture",
                &creator->texture_index,
                textures,
                2,
                -1
            )) {
                strcpy(creator->texture_name, textures[creator->texture_index]);
                printf("Texture: %s\n", creator->texture_name);
            }
        }

        if (igButton("Create Entity", (ImVec2){120, 30})) {
            u32 entity = ecs_new(ecs->registry);

            c_transform_t transform = {
                .pos = {creator->pos[0], creator->pos[1], creator->pos[2]},
                .rotate = {creator->rot[0], creator->rot[1], creator->rot[2]},
                .scale = {creator->scale[0], creator->scale[1], creator->scale[2]}
            };

            ecs_add(ecs, entity, C_TRANSFORM, &transform);

            if (creator->has_model) {
                mesh_t *mesh = asset_manager_get_mesh(
                    asset_manager,
                    creator->mesh_name
                );

                texture_t *texture = asset_manager_get_texture(
                    asset_manager,
                    creator->texture_name
                );

                c_model_t model = {
                    .mesh = mesh,
                    .texture = texture
                };

                ecs_add(ecs, entity, C_MODEL, &model);
            }

            glm_vec3_zero(creator->pos);
            glm_vec3_zero(creator->rot);
            glm_vec3_one(creator->scale);
            creator->has_model = false;
            strcpy(creator->mesh_name, "cube");
            strcpy(creator->texture_name, "dog");
        } 

        igEnd();
    }
}

void gui_manager_update(gui_manager_t *gui_manager)
{
    draw_entiy_creator(gui_manager);
}