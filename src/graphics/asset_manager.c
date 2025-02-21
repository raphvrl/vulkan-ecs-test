#include "asset_manager.h"

asset_manager_t *asset_manager_create(vk_swapchain_t *swapchain)
{
    asset_manager_t *asset_manager = malloc(sizeof(asset_manager_t));
    if (!asset_manager) {
        return NULL;
    }

    asset_manager->swapchain = swapchain;
    asset_manager->textures = map_create();
    asset_manager->meshes = map_create();

    return asset_manager;
}

void asset_manager_destroy(asset_manager_t *asset_manager)
{
    if (!asset_manager) { return; }

    for (usize i = 0; i < asset_manager->textures->size; i++) {
        texture_t *value = asset_manager->textures->values[i];
        texture_destroy(value);
    }

    for (usize i = 0; i < asset_manager->meshes->capacity; i++) {
        mesh_t *value = asset_manager->meshes->values[i];
        mesh_destroy(value);
    }

    map_destroy(asset_manager->textures);
    map_destroy(asset_manager->meshes);
    free(asset_manager);
}

void asset_manager_load_texture(
    asset_manager_t *asset_manager,
    const char *path,
    const char *name
)
{
    if (!asset_manager) { return; }

    texture_t *texture = texture_create(asset_manager->swapchain, path);
    if (!texture) {
        return;
    }

    map_put(asset_manager->textures, name, texture);
}

static mesh_t *load_gltf_mesh(asset_manager_t *asset_manager, const char *path)
{
    cgltf_options options = {0};
    cgltf_data *data = NULL;
    cgltf_result result = cgltf_parse_file(&options, path, &data);
    if (result != cgltf_result_success) {
        LOG_ERROR("Failed to parse gltf file");
        return NULL;
    }

    result = cgltf_load_buffers(&options, data, path);
    if (result != cgltf_result_success) {
        LOG_ERROR("Failed to load buffers");
        cgltf_free(data);
        return NULL;
    }

    if (data->meshes_count == 0 || data->meshes[0].primitives_count == 0) {
        LOG_ERROR("No meshes found in gltf file");
        cgltf_free(data);
        return NULL;
    }

    cgltf_primitive *prim = &data->meshes[0].primitives[0];

    usize vertex_count = 0;
    usize index_count = 0;

    for (usize i = 0; i < prim->attributes_count; i++) {
        if (prim->attributes[i].type == cgltf_attribute_type_position) {
            vertex_count = prim->attributes[i].data->count;
            break;
        }
    }

    if (prim->indices) {
        index_count = prim->indices->count;
    }

    vertex_t *vertices = calloc(vertex_count, sizeof(vertex_t));
    if (!vertices) {
        LOG_ERROR("Failed to allocate memory for vertices");
        return NULL;
    }
    
    for (usize i = 0; i < prim->attributes_count; i++) {
        cgltf_attribute *attr = &prim->attributes[i];
        f32 *buffer_data = (f32 *)attr->data->buffer_view->buffer->data + 
                            attr->data->buffer_view->offset / sizeof(f32);

        switch (attr->type) {
            case cgltf_attribute_type_position:
                for (usize v = 0; v < vertex_count; v++) {
                    vertices[v].pos[0] = buffer_data[v * 3 + 0];
                    vertices[v].pos[1] = buffer_data[v * 3 + 1];
                    vertices[v].pos[2] = buffer_data[v * 3 + 2];
                }
                break;

            case cgltf_attribute_type_normal:
                for (usize v = 0; v < vertex_count; v++) {
                    vertices[v].normal[0] = buffer_data[v * 3 + 0];
                    vertices[v].normal[1] = buffer_data[v * 3 + 1];
                    vertices[v].normal[2] = buffer_data[v * 3 + 2];
                }
                break;

            case cgltf_attribute_type_texcoord:
                for (usize v = 0; v < vertex_count; v++) {
                    vertices[v].uv[0] = buffer_data[v * 2 + 0];
                    vertices[v].uv[1] = buffer_data[v * 2 + 1];
                }
                break;

            default:
                break;
        }
    }

    u32 *indices = calloc(index_count, sizeof(u32));
    if (!indices) {
        LOG_ERROR("Failed to allocate memory for indices");
        return NULL;
    }

    if (prim->indices) {
        cgltf_accessor *accessor = prim->indices;
        void *buffer_data = (uint8_t*)accessor->buffer_view->buffer->data + 
                           accessor->buffer_view->offset;

        switch (accessor->component_type) {
            case cgltf_component_type_r_8:
            case cgltf_component_type_r_8u:
                for (usize i = 0; i < index_count; i++) {
                    indices[i] = ((u8*)buffer_data)[i];
                }
                break;
            case cgltf_component_type_r_16:
            case cgltf_component_type_r_16u:
                for (usize i = 0; i < index_count; i++) {
                    indices[i] = ((u16*)buffer_data)[i];
                }
                break;
            case cgltf_component_type_r_32u:
                for (usize i = 0; i < index_count; i++) {
                    indices[i] = ((u32*)buffer_data)[i];
                }
                break;
            default:
                LOG_ERROR("Unsupported index component type");
                return NULL;
        }
    }

    mesh_t *mesh = mesh_create(
        asset_manager->swapchain,
        vertices,
        vertex_count,
        indices,
        index_count
    );

    cgltf_free(data);
    return mesh;
}

void asset_manager_load_mesh(
    asset_manager_t *asset_manager,
    const char *path,
    const char *name
)
{
    if (!asset_manager) { return; }

    mesh_t *mesh = load_gltf_mesh(asset_manager, path);
    if (!mesh) {
        return;
    }

    map_put(asset_manager->meshes, name, mesh);
}