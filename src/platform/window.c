#include "window.h"
#include "core/type.h"
#include "core/mem.h"
#include "core/log.h"

window_t *window_create(int width, int height, const char *title)
{
    window_t *window = malloc(sizeof(window_t));
    if (!window) {
        ERROR("Failed to allocate memory for window");
        return NULL;
    }

    window->width = width;
    window->height = height;
    window->title = title;

    if (!glfwInit()) {
        ERROR("Failed to initialize GLFW");
        free(window);
        return NULL;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window->handle = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window->handle) {
        ERROR("Failed to create window");
        free(window);
        return NULL;
    }

    LOG("Window created");

    return window;
}

void window_destroy(window_t *window)
{
    LOG("Destroying window");
    glfwDestroyWindow(window->handle);
    _free(window);
}

void window_terminate()
{
    glfwTerminate();
}

bool window_is_open(window_t *window)
{
    return !glfwWindowShouldClose(window->handle);
}

void window_poll_events()
{
    glfwPollEvents();
}