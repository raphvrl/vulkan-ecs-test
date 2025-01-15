#include "window.h"

window_t *window_create(u32 w, u32 h, const char *title)
{
    window_t *win = malloc(sizeof(window_t));
    if (!win) {
        return NULL;
    }

    win->width = w;
    win->height = h;

    if (glfwInit() != GLFW_TRUE) {
        window_destroy(win);
        return NULL;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    win->handle = glfwCreateWindow(w, h, title, NULL, NULL);
    if (!win->handle) {
        window_destroy(win);
        return NULL;
    }

    return win;
}

void window_destroy(window_t *win)
{
    if (!win) { return; }

    glfwDestroyWindow(win->handle);
    glfwTerminate();

    free(win);

    win = NULL;
}

void window_update(window_t *win)
{
    if (!win) { return; }

    glfwPollEvents();

    win->open = !glfwWindowShouldClose(win->handle);
}