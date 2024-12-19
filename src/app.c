#include "platform/window.h"
#include "renderer/vulkan/vk_context.h"
#include "core/log.h"
#include "utils/consol.h"

int main()
{
    set_save_log(false);

    window_t *window = window_create(800, 600, "Vulkan Test");
    if (!window) {
        return EXIT_FAILURE;
    }

    vulkan_ctx_t *vulkan_ctx = vulkan_ctx_create(window->handle);

    while (window_is_open(window)) {
        window_poll_events(window);
    }

    vulkan_ctx_destroy(vulkan_ctx);
    window_destroy(window);
    window_terminate();

    return EXIT_SUCCESS;
}