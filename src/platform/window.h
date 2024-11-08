#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef struct {
    GLFWwindow *handle;
    int width;
    int height;
    const char *title;
} window_t;

window_t *window_create(int width, int height, const char *title);
void window_destroy(window_t *window);
void window_terminate();
bool window_is_open(window_t *window);
void window_poll_events();