#include "physics_world.h"

physics_world_t *physics_world_create()
{
    physics_world_t *world = malloc(sizeof(physics_world_t));
    if (world == NULL) {
        return NULL;
    }

    dInitODE2(0);

    world->world = dWorldCreate();
    world->space = dHashSpaceCreate(0);
    world->contacts = dJointGroupCreate(0);

    dWorldSetQuickStepNumIterations(world->world, 20);
    dWorldSetAutoDisableFlag(world->world, 1);
    dWorldSetContactMaxCorrectingVel(world->world, 0.1);
    dWorldSetContactSurfaceLayer(world->world, 0.001);
    dWorldSetGravity(world->world, 0, -9.81 * 3, 0);

    return world;
}

void physics_world_destroy(physics_world_t *world)
{
    dJointGroupDestroy(world->contacts);
    dSpaceDestroy(world->space);
    dWorldDestroy(world->world);

    free(world);
}

static void near_callback(void *data, dGeomID o1, dGeomID o2)
{
    physics_world_t *world = (physics_world_t *)data;
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);

    if (b1 && b2 && dAreConnectedExcluding(b1, b2, dJointTypeContact)) {
        return;
    }

    dContact contact[MAX_CONTACTS];
    int num_contacts = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact));

    for (int i = 0; i < num_contacts; i++) {
        contact[i].surface.mode = dContactBounce | dContactSoftCFM;
        contact[i].surface.mu = 0.5;
        contact[i].surface.bounce = 0.3;
        contact[i].surface.bounce_vel = 0.1;
        contact[i].surface.soft_cfm = 0.01;

        dJointID c = dJointCreateContact(world->world, world->contacts, &contact[i]);
        dJointAttach(c, b1, b2);
    }
}

void physics_world_update(physics_world_t *world, float dt)
{
    dSpaceCollide(world->space, world, &near_callback);
    dWorldStep(world->world, dt);
    dJointGroupEmpty(world->contacts);
}