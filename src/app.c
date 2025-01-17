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

    if (!app->mesh) {
        app_destroy(app);
        return NULL;
    }

    app->pipeline_manager = pipeline_manager_create(app->swapchain);

    app->registry = registry_create();
    app->component_manager = component_manager_create();
    app->system_manager = system_manager_create(
        app->registry,
        app->component_manager,
        app->window,
        app->pipeline_manager
    );

    return app;
}

void app_destroy(app_t *app)
{
    if (!app) {
        return;
    }

    pipeline_manager_destroy(app->pipeline_manager);
    mesh_destroy(app->mesh);

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
    registry_add_component(app->registry, id, COMPONENT_MODEL);

    cmp_transform_t *transform = &app->component_manager->transforms[id];
    transform->position[0] = 0.0f;
    transform->position[1] = 0.0f;
    transform->position[2] = 0.0f;

    transform->rotation[0] = 0.0f;
    transform->rotation[1] = 0.0f;
    transform->rotation[2] = 0.0f;

    transform->scale[0] = 1.0f;
    transform->scale[1] = 1.0f;
    transform->scale[2] = 1.0f;

    cmp_model_t *model = &app->component_manager->models[id];
    model->mesh = app->mesh;

    while (app->window->open) {
        window_update(app->window);

        system_manager_update(
            app->system_manager,
            app->window->dt
        );
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