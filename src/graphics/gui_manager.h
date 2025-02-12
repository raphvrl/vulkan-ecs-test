#pragma once

#include "core/types.h"
#include "graphics/asset_manager.h"
#include "graphics/cimgui/cimgui_ctx.h"

// forward declarations
typedef struct ecs ecs_t;

typedef struct gui_entity_creator {
    bool visible;
    v3 pos;
    v3 rot;
    v3 scale;
    bool has_model;
    int mesh_index;
    char mesh_name[256];
    int texture_index;
    char texture_name[256];
} gui_entity_creator_t;

// main struct
typedef struct gui_manager {
    ecs_t *ecs;
    asset_manager_t *asset_manager;

    gui_entity_creator_t entity_creator;
} gui_manager_t;

// constructor and destructor
gui_manager_t *gui_manager_create(ecs_t *ecs, asset_manager_t *asset_manager);
void gui_manager_destroy(gui_manager_t *gui_manager);

void gui_manager_update(gui_manager_t *gui_manager);
void gui_manager_render(gui_manager_t *gui_manager);