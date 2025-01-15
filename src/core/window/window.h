#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdlib.h>

#include "core/types.h"

// main struct
typedef struct window {
    GLFWwindow *handle;
    u32 width;
    u32 height;

    bool open;
} window_t;

// constructor and destructor
window_t *window_create(u32 w, u32 h, const char *title);
void window_destroy(window_t *win);

// action
void window_update(window_t *win);
