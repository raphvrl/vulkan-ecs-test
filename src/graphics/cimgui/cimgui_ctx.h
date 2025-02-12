#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#define CIMGUI_USE_VULKAN
#define CIMGUI_USE_GLFW
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include "cimgui_impl.h"

#include "core/types.h"
#include "graphics/vulkan/vk_swapchain.h"

bool cimgui_ctx_create(vk_swapchain_t* swapchain);
void cimgui_ctx_destroy();