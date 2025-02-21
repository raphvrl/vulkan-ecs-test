#include "ecs/ecs.h"

static void update_dynamic(c_transform_t* transform, c_dynamic_t* dynamic)
{
    const dReal* pos = dBodyGetPosition(dynamic->body);
    const dReal* rot = dBodyGetRotation(dynamic->body);

    transform->pos[0] = pos[0];
    transform->pos[1] = pos[1];
    transform->pos[2] = pos[2];

    dReal w = sqrt(1.0 + rot[0] + rot[5] + rot[10]) / 2.0;
    dReal w4 = (4.0 * w);
    transform->rotate[0] = (rot[6] - rot[9]) / w4;
    transform->rotate[1] = (rot[8] - rot[2]) / w4;
    transform->rotate[2] = (rot[1] - rot[4]) / w4;
}

static void create_dynamic(
    ecs_t *ecs,
    c_transform_t* transform,
    c_dynamic_t* dynamic
)
{
    if (!transform || !dynamic) { return; }

    dynamic->body = dBodyCreate(ecs->physics_world->world);

    f32 *positions = calloc(dynamic->mesh->vertex_count * 3, sizeof(f32));

    for (u32 i = 0; i < dynamic->mesh->vertex_count; i++) {
        vertex_t vertex = dynamic->mesh->vertices[i];
        positions[i * 3 + 0] = vertex.pos[0];
        positions[i * 3 + 1] = vertex.pos[1];
        positions[i * 3 + 2] = vertex.pos[2];
    }

    dTriMeshDataID data = dGeomTriMeshDataCreate();
    dGeomTriMeshDataBuildSingle(
        data,
        positions,
        sizeof(f32) * 3,
        dynamic->mesh->vertex_count,
        dynamic->mesh->indices,
        dynamic->mesh->index_count,
        sizeof(u32) * 3
    );

    dynamic->geom = dCreateTriMesh(
        ecs->physics_world->space,
        data,
        NULL,
        NULL,
        NULL
    );

    free(positions);

    dMass mass;
    dMassSetBoxTotal(&mass, dynamic->mass, 1.0, 1.0, 1.0);
    dBodySetMass(dynamic->body, &mass);

    dBodySetPosition(
        dynamic->body,
        transform->pos[0],
        transform->pos[1],
        transform->pos[2]
    );

    dGeomSetBody(dynamic->geom, dynamic->body);

    if (!dynamic->is_static) {
        dBodySetLinearDamping(dynamic->body, 0.01);
        dBodySetAngularDamping(dynamic->body, 0.01);
        dBodySetAutoDisableFlag(dynamic->body, 1);
        dBodySetGravityMode(dynamic->body, 1);
    } else {
        dBodyDisable(dynamic->body);
        dBodySetGravityMode(dynamic->body, 0);
    }
}

void s_physics_tick(ecs_t *ecs, f32 dt)
{
    physics_world_t *physics_world = ecs->physics_world;

    u32 count = ecs_get_count(ecs, C_VELOCITY);
    for (u32 i = 0; i < count; i++) {
        if (!ecs_is_active(ecs, i)) { continue; }

        if (ecs_has(ecs, i, C_VELOCITY) && ecs_has(ecs, i, C_TRANSFORM)) {
            c_velocity_t *velocity = ecs_get(ecs, i, C_VELOCITY);
            c_transform_t *transform = ecs_get(ecs, i, C_TRANSFORM);

            vec3 vel = {0};
            glm_vec3_scale(velocity->pos, dt, vel);

            glm_vec3_add(transform->pos, vel, transform->pos);
        }
    }

    count = ecs_get_count(ecs, C_DYNAMIC);
    for (u32 i = 0; i < count; i++) {
        if (!ecs_is_active(ecs, i)) { continue; }

        if (ecs_has(ecs, i, C_DYNAMIC) && ecs_has(ecs, i, C_TRANSFORM)) {
            c_dynamic_t *dynamic = ecs_get(ecs, i, C_DYNAMIC);
            c_transform_t *transform = ecs_get(ecs, i, C_TRANSFORM);

            if (!dynamic->body) {
                create_dynamic(ecs, transform, dynamic);
            }

            update_dynamic(transform, dynamic);
        }
    }

    physics_world_update(physics_world, dt);
}