#pragma once

#include <ode/ode.h>

#include "core/types.h"
#include "graphics/mesh.h"

typedef struct c_dynamic {
    f32 mass;
    f32 friction;
    f32 bounce;
    bool is_static;
    mesh_t *mesh;
    dBodyID body;
    dGeomID geom;
    dTriMeshDataID data;
} c_dynamic_t;