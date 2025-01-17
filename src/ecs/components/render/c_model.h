#pragma once

#include "core/types.h"
#include "graphics/mesh.h"

typedef struct c_model {
    mesh_t *mesh;
} c_model_t;

#define COMPONENT_MODEL (1 << 2)