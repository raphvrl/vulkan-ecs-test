#pragma once

#include <vulkan/vulkan.h>

#include "core/type.h"

typedef struct s_vulkan_ctx vulkan_ctx_t;

void vulkan_renderpass_create(vulkan_ctx_t *ctx);
void vulkan_renderpass_destroy(vulkan_ctx_t *ctx);