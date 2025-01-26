#pragma once

#define CGLFT_IMPLEMENTATION
#include <cgltf.h>

#include "core/types.h"
#include "vulkan/vk_swapchain.h"
#include "texture.h"
#include "mesh.h"
#include "utils/map.h"

// main struct
typedef struct asset_manager {
    vk_swapchain_t *swapchain;

    map_t *textures;
    map_t *meshes;
} asset_manager_t;

// constructor and destructor
asset_manager_t *asset_manager_create(vk_swapchain_t *swapchain);
void asset_manager_destroy(asset_manager_t *asset_manager);

// action
void asset_manager_load_texture(
    asset_manager_t *asset_manager,
    const char *path,
    const char *name
);

void asset_manager_load_mesh(
    asset_manager_t *asset_manager,
    const char *path,
    const char *name
);

#define asset_manager_get_texture(asset_manager, name) \
    (texture_t*)map_get((asset_manager)->textures, name)

#define asset_manager_get_mesh(asset_manager, name) \
    (mesh_t*)map_get((asset_manager)->meshes, name)

