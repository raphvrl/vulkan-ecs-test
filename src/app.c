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

    app->device = vk_device_create(app->window);
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
    app->ecs = ecs_create(
        &(ecs_info_t){
            .registry = app->registry,
            .component_manager = app->component_manager,
            .window = app->window,
            .pipeline_manager = app->pipeline_manager
        }
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

    ecs_destroy(app->ecs);
    component_manager_destroy(app->component_manager);
    registry_destroy(app->registry);

    vk_swapchain_destroy(app->swapchain);
    vk_device_destroy(app->device);
    window_destroy(app->window);
    free(app);
}

void app_run(app_t *app)
{
    u32 id = ecs_new(app->registry);
    ecs_add(app->ecs, id, C_TRANSFORM, &DEFAULT_TRANSFORM);
    ecs_add(app->ecs, id, C_MODEL, &(c_model_t){
        .mesh = app->mesh
    });

    id = ecs_new(app->registry);

    ecs_add(app->ecs, id, C_TRANSFORM, &(c_transform_t){
        .pos = {0.0f, 0.0f, 3.0f}
    });

    ecs_add(app->ecs, id, C_VELOCITY, &DEFAULT_TRANSFORM);
    ecs_add(app->ecs, id, C_CAMERA, &(c_camera_t){
        .pos = {0.0f, 0.0f, 2.0f},
        .front = {0.0f, 0.0f, -1.0f},
        .up = {0.0f, 1.0f, 0.0f},
        .right = {1.0f, 0.0f, 0.0f},
        .yaw = -90.0f,
        .pitch = 0.0f,
        .fov = 45.0f,
        .aspect = (f32)app->window->width / (f32)app->window->height,
        .near_plane = 0.1f,
        .far_plane = 100.0f
    });

    while (app->window->open) {
        window_update(app->window);

        ecs_update(
            app->ecs,
            app->window->dt
        );
    }
}

int main()
{
    setup_utf8();

    app_t *app = app_create(1280, 720, "Hello Vulkan!");
    if (!app) {
        return 1;
    }

    app_run(app);
    app_destroy(app);

    return 0;
}