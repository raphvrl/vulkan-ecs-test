#include "app.h"
#include "core/logger/logger.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <locale.h>
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

    window_set_fullscreen(app->window, false);

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

    if (!cimgui_ctx_create(app->swapchain)) {
        app_destroy(app);
        return NULL;
    }

    app->pipeline_manager = pipeline_manager_create(app->swapchain);    
    app->asset_manager = asset_manager_create(app->swapchain);

    asset_manager_load_mesh(
        app->asset_manager,
        "assets/mesh/cube.glb",
        "cube"
    );

    asset_manager_load_mesh(
        app->asset_manager,
        "assets/mesh/monkey.glb",
        "monkey"
    );

    asset_manager_load_texture(
        app->asset_manager,
        "assets/texture/dog.jpg",
        "dog"
    );

    asset_manager_load_texture(
        app->asset_manager,
        "assets/texture/gru.png",
        "gru"
    );

    
    app->component_manager = component_manager_create();

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

    app->gui = gui_create(app->ecs, app->asset_manager);
    app->ecs->gui = app->gui;

    return app;
}

void app_destroy(app_t *app)
{
    if (!app) {
        return;
    }

    vkDeviceWaitIdle(app->device->device);

    gui_destroy(app->gui);

    asset_manager_destroy(app->asset_manager);
    pipeline_manager_destroy(app->pipeline_manager);

    ecs_destroy(app->ecs);
    component_manager_destroy(app->component_manager);
    registry_destroy(app->registry);

    cimgui_ctx_destroy();
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
        .mesh = asset_manager_get_mesh(app->asset_manager, "monkey"),
        .texture = asset_manager_get_texture(app->asset_manager, "gru"),
        .mesh_id = 1,
        .texture_id = 1
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
        .aspect = (f32)app->window->w / (f32)app->window->h,
        .near_plane = 0.1f,
        .far_plane = 100.0f
    });

    id = ecs_new(app->registry);

    ecs_add(app->ecs, id, C_TRANSFORM, &(c_transform_t){
        .pos = {0.0f, -2.0f, 0.0f},
        .scale = {1.0f, 1.0f, 1.0f}
    });

    ecs_add(app->ecs, id, C_MODEL, &(c_model_t){
        .mesh = asset_manager_get_mesh(app->asset_manager, "cube"),
        .texture = asset_manager_get_texture(app->asset_manager, "dog"),
        .mesh_id = 0,
        .texture_id = 0
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

    app_t *app = app_create(1600, 900, "Hello Vulkan!");
    if (!app) {
        return EXIT_FAILURE;
    }

    app_run(app);
    app_destroy(app);

    return EXIT_SUCCESS;
}