#pragma once

#include <stdbool.h>
#include <string.h>

#include "vulkan_context.h"

void vulkan_instance_create(vulkan_ctx_t *ctx);
void vulkan_instance_destroy(vulkan_ctx_t *ctx);