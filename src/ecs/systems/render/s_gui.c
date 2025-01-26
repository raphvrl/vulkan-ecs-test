#include "ecs/ecs.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

void s_gui_tick(ecs_t *ecs, f32 dt)
{
    UNUSED(dt);
    UNUSED(ecs);

    // igBegin("Debug Window", NULL, 0);
    // igText("FPS: %.1f", 1.0f / dt);
    // igEnd();
}