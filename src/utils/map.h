#pragma once

#include <string.h>

#include "core/types.h"

#define INITAL_CAPACITY 8
#define GROWTH_FACTOR 2

// main struct
typedef struct map {
    char **keys;
    void **values;
    usize capacity;
    usize size;
} map_t;

// constructor and destructor
map_t *map_create();
void map_destroy(map_t *map);

// action
void map_put(map_t *map, const char *key, void *value);
void map_remove(map_t *map, const char *key);
void *map_get(map_t *map, const char *key);
const char *map_get_key(map_t *map, usize i);

// macro
#define map_keys(map) (const char * const *)(map)->keys