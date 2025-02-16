#include "map.h"

map_t *map_create(void)
{
    map_t *map = malloc(sizeof(map_t));
    if (!map) {
        return NULL;
    }

    map->keys = calloc(INITAL_CAPACITY, sizeof(char *));
    map->values = calloc(INITAL_CAPACITY, sizeof(void *));
    map->capacity = INITAL_CAPACITY;
    map->size = 0;

    return map;
}

void map_destroy(map_t *map)
{
    if (!map) { return; }

    for (usize i = 0; i < map->size; i++) {
        free(map->keys[i]);
    }

    free(map->keys);
    free(map->values);

    free(map);
}

void map_put(map_t *map, const char *key, void *value)
{
    if (!map || !key) { return; }

    for (usize i = 0; i < map->size; i++) {
        if (strcmp(map->keys[i], key) == 0) {
            map->values[i] = value;
            return;
        }
    }

    if (map->size >= map->capacity) {
        map->capacity *= GROWTH_FACTOR;
        map->keys = realloc(map->keys, map->capacity * sizeof(char *));
        map->values = realloc(map->values, map->capacity * sizeof(void *));
    }

    map->keys[map->size] = strdup(key);
    map->values[map->size] = value;
    map->size++;

    return;
}

void map_remove(map_t *map, const char *key)
{
    if (!map || !key) return;

    for (usize i = 0; i < map->size; i++) {
        if (strcmp(map->keys[i], key) == 0) {
            free(map->keys[i]);
            map->keys[i] = NULL;
            map->values[i] = NULL;
            map->size--;
            return;
        }
    }
}

void *map_get(map_t *map, const char *key)
{
    if (!map || !key) { return NULL; }

    for (usize i = 0; i < map->size; i++) {
        if (strcmp(map->keys[i], key) == 0) {
            return map->values[i];
        }
    }

    return NULL;
}

const char *map_get_key(map_t *map, usize i)
{
    if (i >= map->size) { return NULL; }

    return map->keys[i];
}
