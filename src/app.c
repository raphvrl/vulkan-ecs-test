#include "app.h"
#include "core/logger/logger.h"

#ifdef _WIN32
#include <windows.h>
#endif

static void setup_utf8()
{

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#else
    setlocale(LC_ALL, "en_US.UTF-8");
#endif

}

app_t *app_create(u32 w, u32 h, const char *title)
{
    app_t *app = malloc(sizeof(app_t));
    if (!app) {
        return NULL;
    }

    app->window = window_create(w, h, title);
    if (!app->window) {
        app_destroy(app);
        return NULL;
    }

    app->device = vk_device_create(app->window->handle);
    if (!app->device) {
        app_destroy(app);
        return NULL;
    }

    app->swapchain = vk_swapchain_create(app->device);
    if (!app->swapchain) {
        app_destroy(app);
        return NULL;
    }

    app->pipeline = vk_pipeline_create(
        app->swapchain,
        (vk_pipeline_info_t){
            .vertex = "bin/default.vert.spv",
            .fragment = "bin/default.frag.spv",
            .input = {
                .binding = {
                    {
                        .binding = 0,
                        .stride = sizeof(vertex_t),
                        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
                    }
                },
                .binding_count = 1,
                .attribute = {
                    {
                        .location = 0,
                        .binding = 0,
                        .format = VK_FORMAT_R32G32B32_SFLOAT,
                        .offset = offsetof(vertex_t, pos)
                    },
                    {
                        .location = 1,
                        .binding = 0,
                        .format = VK_FORMAT_R32G32B32_SFLOAT,
                        .offset = offsetof(vertex_t, normal)
                    },
                    {
                        .location = 2,
                        .binding = 0,
                        .format = VK_FORMAT_R32G32_SFLOAT,
                        .offset = offsetof(vertex_t, uv)
                    }
                },
                .attribute_count = 3
            }
        }
    );

    // cube vertices
    vertex_t vertices[] = {
        // Front face
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        // Back face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        // Left face
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        // Right face
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        // Top face
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        // Bottom face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}
    };

    // cube indices
    u32 indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    app->mesh = mesh_create(
        app->swapchain,
        vertices,
        ARR_LEN(vertices),
        indices,
        ARR_LEN(indices)
    );

    app->registry = registry_create();
    app->component_manager = component_manager_create();
    app->system_manager = system_manager_create(
        app->registry,
        app->component_manager,
        app->window,
        app->device
    );

    return app;
}

void app_destroy(app_t *app)
{
    if (!app) {
        return;
    }

    mesh_destroy(app->mesh);

    vk_pipeline_destroy(app->pipeline);

    system_manager_destroy(app->system_manager);
    component_manager_destroy(app->component_manager);
    registry_destroy(app->registry);

    vk_swapchain_destroy(app->swapchain);
    vk_device_destroy(app->device);
    window_destroy(app->window);
    free(app);
}

void app_run(app_t *app)
{
    u32 id = registry_create_entity(app->registry);
    registry_add_component(app->registry, id, COMPONENT_TRANSFORM);
    registry_add_component(app->registry, id, COMPONENT_VELOCITY);

    while (app->window->open) {
        window_update(app->window);

        system_manager_update(
            app->system_manager,
            app->window->dt
        );

        begin_frame(app->swapchain);

        vk_pipeline_bind(app->pipeline);

        mesh_bind(app->mesh);
        mesh_draw(app->mesh);

        end_frame(app->swapchain);
    }
}

int main()
{
    setup_utf8();

    app_t *app = app_create(800, 600, "Hello Vulkan!");
    if (!app) {
        return 1;
    }

    app_run(app);
    app_destroy(app);

    return 0;
}