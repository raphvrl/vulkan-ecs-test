#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string.h>

#include "core/types.h"

// main struct
typedef struct window {
    // handle
    GLFWwindow *handle;
    u32 w;
    u32 h;
    u32 x;
    u32 y;

    // open
    bool open;

    // time
    f64 last_time;
    f32 dt;

    // keyboard
    bool keys[GLFW_KEY_LAST];

    // mouse
    bool mouses[GLFW_MOUSE_BUTTON_LAST];

    // mouse pos
    f32 mx;
    f32 my;

    // mouse delta
    f32 rdx;
    f32 rdy;

    // resize
    bool resized;
} window_t;

// key enum
typedef enum key {
    K_UNKNOWN = GLFW_KEY_UNKNOWN,
    K_SPACE = GLFW_KEY_SPACE,
    K_APOSTROPHE = GLFW_KEY_APOSTROPHE,
    K_COMMA = GLFW_KEY_COMMA,
    K_MINUS = GLFW_KEY_MINUS,
    K_PERIOD = GLFW_KEY_PERIOD,
    K_SLASH = GLFW_KEY_SLASH,
    K_0 = GLFW_KEY_0,
    K_1 = GLFW_KEY_1,
    K_2 = GLFW_KEY_2,
    K_3 = GLFW_KEY_3,
    K_4 = GLFW_KEY_4,
    K_5 = GLFW_KEY_5,
    K_6 = GLFW_KEY_6,
    K_7 = GLFW_KEY_7,
    K_8 = GLFW_KEY_8,
    K_9 = GLFW_KEY_9,
    K_SEMICOLON = GLFW_KEY_SEMICOLON,
    K_EQUAL = GLFW_KEY_EQUAL,
    K_A = GLFW_KEY_A,
    K_B = GLFW_KEY_B,
    K_C = GLFW_KEY_C,
    K_D = GLFW_KEY_D,
    K_E = GLFW_KEY_E,
    K_F = GLFW_KEY_F,
    K_G = GLFW_KEY_G,
    K_H = GLFW_KEY_H,
    K_I = GLFW_KEY_I,
    K_J = GLFW_KEY_J,
    K_K = GLFW_KEY_K,
    K_L = GLFW_KEY_L,
    K_M = GLFW_KEY_M,
    K_N = GLFW_KEY_N,
    K_O = GLFW_KEY_O,
    K_P = GLFW_KEY_P,
    K_Q = GLFW_KEY_Q,
    K_R = GLFW_KEY_R,
    K_S = GLFW_KEY_S,
    K_T = GLFW_KEY_T,
    K_U = GLFW_KEY_U,
    K_V = GLFW_KEY_V,
    K_W = GLFW_KEY_W,
    K_X = GLFW_KEY_X,
    K_Y = GLFW_KEY_Y,
    K_Z = GLFW_KEY_Z,
    K_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET,
    K_BACKSLASH = GLFW_KEY_BACKSLASH,
    K_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET,
    K_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,
    K_WORLD_1 = GLFW_KEY_WORLD_1,
    K_WORLD_2 = GLFW_KEY_WORLD_2,
    K_ESCAPE = GLFW_KEY_ESCAPE,
    K_ENTER = GLFW_KEY_ENTER,
    K_TAB = GLFW_KEY_TAB,
    K_BACKSPACE = GLFW_KEY_BACKSPACE,
    K_INSERT = GLFW_KEY_INSERT,
    K_DELETE = GLFW_KEY_DELETE,
    K_RIGHT = GLFW_KEY_RIGHT,
    K_LEFT = GLFW_KEY_LEFT,
    K_DOWN = GLFW_KEY_DOWN,
    K_UP = GLFW_KEY_UP,
    K_PAGE_UP = GLFW_KEY_PAGE_UP,
    K_PAGE_DOWN = GLFW_KEY_PAGE_DOWN,
    K_HOME = GLFW_KEY_HOME,
    K_END = GLFW_KEY_END,
    K_CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
    K_SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
    K_NUM_LOCK = GLFW_KEY_NUM_LOCK,
    K_PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,
    K_PAUSE = GLFW_KEY_PAUSE,
    K_F1 = GLFW_KEY_F1,
    K_F2 = GLFW_KEY_F2,
    K_F3 = GLFW_KEY_F3,
    K_F4 = GLFW_KEY_F4,
    K_F5 = GLFW_KEY_F5,
    K_F6 = GLFW_KEY_F6,
    K_F7 = GLFW_KEY_F7,
    K_F8 = GLFW_KEY_F8,
    K_F9 = GLFW_KEY_F9,
    K_F10 = GLFW_KEY_F10,
    K_F11 = GLFW_KEY_F11,
    K_F12 = GLFW_KEY_F12,
    K_F13 = GLFW_KEY_F13,
    K_F14 = GLFW_KEY_F14,
    K_F15 = GLFW_KEY_F15,
    K_F16 = GLFW_KEY_F16,
    K_F17 = GLFW_KEY_F17,
    K_F18 = GLFW_KEY_F18,
    K_F19 = GLFW_KEY_F19,
    K_F20 = GLFW_KEY_F20,
    K_F21 = GLFW_KEY_F21,
    K_F22 = GLFW_KEY_F22,
    K_F23 = GLFW_KEY_F23,
    K_F24 = GLFW_KEY_F24,
    K_F25 = GLFW_KEY_F25,
    K_KP_0 = GLFW_KEY_KP_0,
    K_KP_1 = GLFW_KEY_KP_1,
    K_KP_2 = GLFW_KEY_KP_2,
    K_KP_3 = GLFW_KEY_KP_3,
    K_KP_4 = GLFW_KEY_KP_4,
    K_KP_5 = GLFW_KEY_KP_5,
    K_KP_6 = GLFW_KEY_KP_6,
    K_KP_7 = GLFW_KEY_KP_7,
    K_KP_8 = GLFW_KEY_KP_8,
    K_KP_9 = GLFW_KEY_KP_9,
    K_KP_DECIMAL = GLFW_KEY_KP_DECIMAL,
    K_KP_DIVIDE = GLFW_KEY_KP_DIVIDE,
    K_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
    K_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
    K_KP_ADD = GLFW_KEY_KP_ADD,
    K_KP_ENTER = GLFW_KEY_KP_ENTER,
    K_KP_EQUAL = GLFW_KEY_KP_EQUAL,
    K_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
    K_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,
    K_LEFT_ALT = GLFW_KEY_LEFT_ALT,
    K_LEFT_SUPER = GLFW_KEY_LEFT_SUPER,
    K_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,
    K_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,
    K_RIGHT_ALT = GLFW_KEY_RIGHT_ALT,
    K_RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER,
    K_MENU = GLFW_KEY_MENU
} key_e;

// mouse enum
typedef enum mouse {
    M_BUTTON_1 = GLFW_MOUSE_BUTTON_1,
    M_BUTTON_2 = GLFW_MOUSE_BUTTON_2,
    M_BUTTON_3 = GLFW_MOUSE_BUTTON_3,
    M_BUTTON_4 = GLFW_MOUSE_BUTTON_4,
    M_BUTTON_5 = GLFW_MOUSE_BUTTON_5,
    M_BUTTON_6 = GLFW_MOUSE_BUTTON_6,
    M_BUTTON_7 = GLFW_MOUSE_BUTTON_7,
    M_BUTTON_8 = GLFW_MOUSE_BUTTON_8
} mouse_e;

// mouse defines
#define M_BUTTON_LEFT M_BUTTON_1
#define M_BUTTON_RIGHT M_BUTTON_2
#define M_BUTTON_MIDDLE M_BUTTON_3

// constructor and destructor
window_t *window_create(u32 w, u32 h, const char *title);
void window_destroy(window_t *win);

// action
void window_update(window_t *win);
void window_close(window_t *win);

// hide
#define window_mouse_hide(win) \
    glfwSetInputMode((win)->handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED)


#define window_mouse_show(win) \
    glfwSetInputMode((win)->handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL)

// change screen status
void window_set_fullscreen(window_t *win, bool fullscreen);

