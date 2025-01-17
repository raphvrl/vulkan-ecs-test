#pragma once

#include "core/types.h"
#include "graphics/mesh.h"

typedef struct cmp_model {
    mesh_t *mesh;
} cmp_model_t;

#define COMPONENT_MODEL (1 << 2)