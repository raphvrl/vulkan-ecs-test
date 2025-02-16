#pragma once

#include "core/types.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"

typedef struct c_model {
    mesh_t *mesh;
    texture_t *texture;
    u32 mesh_id;
    u32 texture_id;
} c_model_t;