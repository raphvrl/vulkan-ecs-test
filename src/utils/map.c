#include "map.h"

map_t *map_create(void)
{
    map_t *map = malloc(sizeof(map_t));
    if (!map) {
        return NULL;
    }

    map->size = 0;
    map->capacity = INITAL_CAPACITY;
    map->pairs = calloc(map->capacity, sizeof(pair_t));

    return map;
}

void map_destroy(map_t *map)
{
    if (!map) { return; }

    for (usize i = 0; i < map->size; i++) {
        free(map->pairs[i].key);
    }

    free(map->pairs);
    free(map);
}

void map_put(map_t *map, const char *key, void *value)
{
    if (map == NULL || key == NULL) { return; }

    for (usize i = 0; i < map->capacity; i++) {
        if (!map->pairs[i].used) {
            map->pairs[i].key = strdup(key);
            map->pairs[i].value = value;
            map->pairs[i].used = true;
            map->size++;
            return;
        }
    }

    map->capacity *= GROWTH_FACTOR;
    map->pairs = realloc(map->pairs, map->capacity * sizeof(pair_t));

    for (usize i = map->size; i < map->capacity; i++) {
        map->pairs[i].used = false;
    }

    map->pairs[map->size].key = strdup(key);
    map->pairs[map->size].value = value;
    map->pairs[map->size].used = true;
    map->size++;
}

void map_remove(map_t *map, const char *key)
{
    if (!map || !key) return;

    for (usize i = 0; i < map->capacity; i++) {
        if (map->pairs[i].used && strcmp(map->pairs[i].key, key) == 0) {
            free(map->pairs[i].key);
            map->pairs[i].key = NULL;
            map->pairs[i].value = NULL;
            map->pairs[i].used = false;
            map->size--;
            return;
        }
    }
}

void *map_get(map_t *map, const char *key)
{
    for (usize i = 0; i < map->size; i++) {
        if (map->pairs[i].used && strcmp(map->pairs[i].key, key) == 0) {
            return map->pairs[i].value;
        }
    }

    return NULL;
}

void map_print(map_t *map)
{
    for (usize i = 0; i < map->size; i++) {
        printf("%s: %p\n", map->pairs[i].key, map->pairs[i].value);
    }
}
