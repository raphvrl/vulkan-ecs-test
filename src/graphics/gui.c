#include "gui.h"
#include "ecs/ecs.h"

gui_t *gui_create(ecs_t *ecs, asset_manager_t *asset_manager)
{
    gui_t *gui = malloc(sizeof(gui_t));
    if (!gui) {
        return NULL;
    }

    gui->entity_list = (gui_entity_list_t){
        .active = false,
        .selected_id = -1
    };

    gui->ecs = ecs;
    gui->asset_manager = asset_manager;

    return gui;
}

void gui_destroy(gui_t *gui)
{
    free(gui);
}

static void draw_c_transform(c_transform_t *transform)
{
    if (!transform) { return; }

    if (igCollapsingHeader_TreeNodeFlags(
        "TRANSFORM",
        ImGuiTreeNodeFlags_DefaultOpen
    )) {
        igText("POSITION");
        igDragFloat3(
            "##position",
            (f32 *)&transform->pos,
            0.1f,
            -FLT_MAX,
            FLT_MAX,
            "%.2f",
            1.0f
        );

        igText("ROTATION");
        igDragFloat3(
            "##rotation",
            (f32 *)&transform->rotate,
            0.1f,
            -FLT_MAX,
            FLT_MAX,
            "%.2f",
            1.0f
        );

        igText("SCALE");
        igDragFloat3(
            "##scale",
            (f32 *)&transform->scale,
            0.1f,
            -FLT_MAX,
            FLT_MAX,
            "%.2f",
            1.0f
        );
    }
}

static void draw_c_model(c_model_t *model, asset_manager_t *asset_manager)
{
    if (!model) { return; }

    if (igCollapsingHeader_TreeNodeFlags(
        "MODEL",
        ImGuiTreeNodeFlags_DefaultOpen
    )) {
        igText("MESH");
        const char * const * meshes = map_keys(asset_manager->meshes);
        int selected_mesh = model->mesh_id;

        if (igCombo_Str_arr(
            "##mesh",
            &selected_mesh,
            meshes,
            asset_manager->meshes->size,
            -1
        )) {
            model->mesh = asset_manager_get_mesh(
                asset_manager,
                meshes[selected_mesh]
            );
            model->mesh_id = selected_mesh;
        }

        igText("TEXTURE");

        ImVec2 preview_size = {64, 64};
        igImage(
            (ImTextureID)(uint64_t)model->texture->descriptor,
            preview_size,
            (ImVec2){0, 0},
            (ImVec2){1, 1},
            (ImVec4){1, 1, 1, 1},
            (ImVec4){0.5f, 0.5f, 0.5f, 1.0f}
        );
        igSameLine(0, 10);

        const char * const * textures = map_keys(asset_manager->textures);
        int selected_texture = model->texture_id;

        if (igCombo_Str_arr(
            "##texture",
            &selected_texture,
            textures,
            asset_manager->textures->size,
            -1
        )) {
            model->texture = asset_manager_get_texture(
                asset_manager,
                textures[selected_texture]
            );
            model->texture_id = selected_texture;
        }
    }
}

static void draw_entity_detail(gui_t *gui)
{
    if (!gui->entity_list.active || gui->entity_list.selected_id == -1) {
        return;
    }

    ImVec2 size = {300, 400};
    ImVec2 pos = {400, 100};

    igSetNextWindowPos(pos, ImGuiCond_FirstUseEver, (ImVec2){0, 0});
    igSetNextWindowSize(size, ImGuiCond_FirstUseEver);

    ecs_t *ecs = gui->ecs;
    u32 id = gui->entity_list.selected_id;

    if (igBegin("ENTITY", NULL, ImGuiWindowFlags_None)) {
        igText("Entity : %d", id);
        igSeparator();

        if (ecs_has(ecs, id, C_TRANSFORM)) {
            c_transform_t *transform = ecs_get(ecs, id, C_TRANSFORM);
            draw_c_transform(transform);
        }

        if (ecs_has(ecs, id, C_MODEL)) {
            c_model_t *model = ecs_get(ecs, id, C_MODEL);
            draw_c_model(model, gui->asset_manager);
        }
    }

    igEnd();
}

static void draw_entity_list(gui_t *gui)
{
    if (!gui->entity_list.active) { return; }

    ecs_t *ecs = gui->ecs;
    ImVec2 size = {300, 400};
    ImVec2 pos = {100, 100};

    igSetNextWindowPos(pos, ImGuiCond_FirstUseEver, (ImVec2){0, 0});
    igSetNextWindowSize(size, ImGuiCond_FirstUseEver);

    if (igBegin("ENTITY LIST", NULL, ImGuiWindowFlags_None)) {
        u32 count = ecs->registry->count;
        for (u32 i = 0; i < count; i++) {
            if (!ecs_is_active(gui->ecs, i)) { continue; }

            char label[32];
            snprintf(label, 32, "%d", i);

            if (igSelectable_Bool(label, false, 0, (ImVec2){0, 0})) {
                gui->entity_list.selected_id = i;
            }
        }
    }

    igEnd();
}

void gui_render(gui_t *gui)
{
    draw_entity_list(gui);
    draw_entity_detail(gui);
}