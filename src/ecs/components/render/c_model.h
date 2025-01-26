#pragma once

#include "core/types.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"

typedef struct c_model {
    mesh_t *mesh;
    texture_t *texture;
} c_model_t;