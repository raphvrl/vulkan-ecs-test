#include "window.h"

static void resize_callback(GLFWwindow *window, int width, int height)
{
    UNUSED(window);

    window_t *win = glfwGetWindowUserPointer(window);
    if (!win) { return; }

    win->w = width;
    win->h = height;
    win->resized = true;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    UNUSED(scancode);
    UNUSED(mods);

    window_t *win = glfwGetWindowUserPointer(window);
    if (!win) { return; }

    if (action == GLFW_PRESS) {
        win->keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        win->keys[key] = false;
    }
}

static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    UNUSED(mods);

    window_t *win = glfwGetWindowUserPointer(window);
    if (!win) { return; }

    if (action == GLFW_PRESS) {
        win->mouses[button] = true;
    } else if (action == GLFW_RELEASE) {
        win->mouses[button] = false;
    }
}

static void mouse_pos_callback(GLFWwindow *window, f64 x, f64 y)
{
    window_t *win = glfwGetWindowUserPointer(window);
    if (!win) { return; }

    win->rdx = x - win->mx;
    win->rdy = y - win->my;

    win->mx = x;
    win->my = y;
}

window_t *window_create(u32 w, u32 h, const char *title)
{
    window_t *win = malloc(sizeof(window_t));
    if (!win) {
        return NULL;
    }

    win->w = w;
    win->h = h;
    win->open = true;
    win->last_time = glfwGetTime();
    win->dt = 0.0f;

    memset(win->keys, 0, sizeof(win->keys));
    memset(win->mouses, 0, sizeof(win->mouses));

    win->mx = 0.0f;
    win->my = 0.0f;

    win->rdx = 0.0f;
    win->rdy = 0.0f;

    win->resized = false;

    if (glfwInit() != GLFW_TRUE) {
        window_destroy(win);
        return NULL;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    win->handle = glfwCreateWindow(w, h, title, NULL, NULL);
    if (!win->handle) {
        window_destroy(win);
        return NULL;
    }

    glfwSetWindowUserPointer(win->handle, win);

    glfwSetWindowSizeCallback(win->handle, resize_callback);
    glfwSetKeyCallback(win->handle, key_callback);
    glfwSetMouseButtonCallback(win->handle, mouse_button_callback);
    glfwSetCursorPosCallback(win->handle, mouse_pos_callback);

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

    memcpy(win->prev_keys, win->keys, sizeof(win->keys));

    win->rdx = 0.0f;
    win->rdy = 0.0f;

    glfwPollEvents();

    win->open = !glfwWindowShouldClose(win->handle);

    f64 now = glfwGetTime();
    win->dt = now - win->last_time;
    win->last_time = now;
}

void window_close(window_t *win)
{
    if (!win) { return; }

    glfwSetWindowShouldClose(win->handle, GLFW_TRUE);
}

void window_set_fullscreen(window_t *win, bool fullscreen)
{
    if (!win) { return; }

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    if (fullscreen) {
        glfwSetWindowMonitor(
            win->handle,
            monitor,
            0, 0,
            mode->width, mode->height,
            mode->refreshRate
        );
    } else {
        int x = (mode->width - win->w) / 2;
        int y = (mode->height - win->h) / 2;

        glfwSetWindowMonitor(
            win->handle,
            NULL,
            x, y,
            win->w, win->h,
            mode->refreshRate
        );
    }
}

bool window_is_fullscreen(window_t *win)
{
    if (!win) { return false; }

    return glfwGetWindowMonitor(win->handle) != NULL;
}