#pragma once

#include "core/types.h"

typedef struct c_camera {
    v3 pos;
    v3 front;
    v3 up;
    v3 right;

    f32 yaw;
    f32 pitch;

    f32 fov;
    f32 aspect;
    f32 near_plane;
    f32 far_plane;

    m4 view;
    m4 proj;
} c_camera_t;