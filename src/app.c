#include "app.h"

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

    system_manager_destroy(app->system_manager);
    component_manager_destroy(app->component_manager);
    registry_destroy(app->registry);

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
    }
}

int main()
{
    app_t *app = app_create(800, 600, "Hello Vulkan!");
    if (!app) {
        return 1;
    }

    app_run(app);
    app_destroy(app);

    return 0;
}