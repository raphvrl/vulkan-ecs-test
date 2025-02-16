#pragma once

#include "cimgui/cimgui_ctx.h"
#include "graphics/asset_manager.h"

typedef struct ecs ecs_t;

typedef struct gui_entity_list {
    bool active;
    u32 selected_id;
} gui_entity_list_t;

typedef struct gui {
    gui_entity_list_t entity_list;
    ecs_t *ecs;
    asset_manager_t *asset_manager;
} gui_t;

// constructor and destructor
gui_t *gui_create(ecs_t *ecs, asset_manager_t *asset_manager);
void gui_destroy(gui_t *gui);

// actions
void gui_render(gui_t *gui);